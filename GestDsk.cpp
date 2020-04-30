#include  <afxwin.h>

#include  "GestDsk.h"


static BYTE ImgDsk[ 2 ][ 0x100000 ];

static BYTE Bitmap[ 2 ][ 256 ];


//
// Retourne la position d'un secteur dans le fichier DSK
//
static int GetPosData( int nDSK, int track, int sect, BOOL SectPhysique )
{
    // Recherche position secteur
    int Pos = sizeof( CPCEMUEnt );
    for ( int t = 0; t <= track; t++ )
        {
        CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ Pos ];
        Pos += sizeof( CPCEMUTrack );
        for ( int s = 0; s < tr->NbSect; s++ )
            {
            if ( t == track )
                {
                if (  ( ( tr->Sect[ s ].R == sect ) && SectPhysique )
                   || ( ( s == sect ) && ! SectPhysique )
                   )
                    break;
                }
            if ( tr->Sect[ s ].SectSize )
                Pos += tr->Sect[ s ].SectSize;
            else
                Pos += ( 128 << tr->Sect[ s ].N );
            }
        }
    return( Pos );
}


//
// Recherche un bloc libre et le remplis
//
static int RechercheBlocLibre( int nDSK, int MaxBloc )
{
    for ( int i = 2; i < MaxBloc; i++ )
        if ( ! Bitmap[ nDSK ][ i ] )
            {
            Bitmap[ nDSK ][ i ] = 1;
            return( i );
            }
    return( 0 );
}


//
// Recherche une entrée de répertoire libre
//
static int RechercheDirLibre( int nDSK )
{
    for ( int i = 0; i < 64; i++ )
        {
        StDirEntry * Dir = GetInfoDirEntry( nDSK, i );
        if ( Dir->User == USER_DELETED )
            return( i );
        }
    return( -1 );
}


//
// Recherche le plus petit secteur d'une piste
//
int GetMinSect( int nDSK, BOOL ForceStd )
{
    int Sect = 0x100;
    CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ sizeof( CPCEMUEnt ) ];
    for ( int s = 0; s < tr->NbSect; s++ )
        {
        BYTE CurSect = tr->Sect[ s ].R;
        if ( ForceStd )
            if ( CurSect == 0x01 || CurSect == 0x41 || CurSect == 0xC1 )
                return( CurSect );

        if ( Sect > CurSect )
            Sect = CurSect;
        }
    return( Sect );
}


//
// Retourne les données "brutes" de l'image disquette
//
BYTE * GetRawData( int nDSK, int Pos )
{
    return( &ImgDsk[ nDSK ][ Pos ] );
}


//
// Ecriture de données "brutes" dans l'image disquette
//
void WriteRawData( int nDSK, int Pos, BYTE * Data, int Longueur )
{
    memcpy( &ImgDsk[ nDSK ][ Pos ], Data, Longueur );
}


//
// Copie d'un dsk vers l'autre
//
void CopyDsk( int nDSK_0, int nDSK_1 )
{
    memcpy( ImgDsk[ nDSK_1 ], ImgDsk[ nDSK_0 ], sizeof( ImgDsk[ nDSK_0 ] ) );
}

//
// Retourne la taille du fichier image
//
int GetTailleDsk( int nDSK )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    int NbTracks = Infos->NbTracks;
    int Pos = sizeof( CPCEMUEnt );
    for ( int t = 0; t < NbTracks; t++ )
        {
        CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ Pos ];
        Pos += sizeof( CPCEMUTrack );
        for ( int s = 0; s < tr->NbSect; s++ )
            {
            if ( tr->Sect[ s ].SectSize )
                Pos += tr->Sect[ s ].SectSize;
            else
                Pos += ( 128 << tr->Sect[ s ].N );
            }
        }
    return( Pos );
}


//
// Retourne le nombre de pistes de la disquette
//
int GetNbTracks( int nDSK )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    return( Infos->NbTracks );
}


//
// Positionne le nombre de pistes de la disquette
//
void SetNbTracks( int nDSK, int NbTracks )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    Infos->NbTracks = ( BYTE )NbTracks;
}


//
// Retourne le nombre maxi de blocs pour un DSK
//
int GetMaxBlocs( int MaxTracks, int nDSK )
{
    if ( MaxTracks )
        return( 256 );

    int track = GetNbTracks( nDSK ); // ### A voir si possiblité récupération nb pistes DSK
    int MinSect = GetMinSect( nDSK, TRUE );
    if ( MinSect == 0x41 )
        track -= 2;
    else
        if ( MinSect == 0x01 )
            track--;

    return( max( 0, ( ( track * 9 ) >> 1 ) - 2 ) );
}


//
// Lecture d'un bloc AMSDOS (1 block = 2 secteurs)
//
BYTE * ReadBloc( int nDSK, int bloc )
{
    static BYTE BufBloc[ SECTSIZE * 2 ];

    int track = ( bloc << 1 ) / 9;
    int sect = ( bloc << 1 ) % 9;
    int MinSect = GetMinSect( nDSK, TRUE );
    if ( MinSect == 0x41 )
        track += 2;
    else
        if ( MinSect == 0x01 )
            track++;

    int Pos = GetPosData( nDSK, track, sect + MinSect, TRUE );
    memcpy( BufBloc, &ImgDsk[ nDSK ][ Pos ], SECTSIZE );
    if ( ++sect > 8 )
        {
        track++;
        sect = 0;
        }

    Pos = GetPosData( nDSK, track, sect + MinSect, TRUE );
    memcpy( &BufBloc[ SECTSIZE ], &ImgDsk[ nDSK ][ Pos ], SECTSIZE );
    return( BufBloc );
}


//
// Formater une piste
//
static void FormatTrack( int nDSK
                       , CPCEMUEnt * Infos
                       , int t
                       , int MinSect
                       , int NbSect
                       )
{
    CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ sizeof( CPCEMUEnt ) + t * Infos->TrackSize ];
    memset( &ImgDsk[ nDSK ][ sizeof( CPCEMUEnt )
                   + sizeof( CPCEMUTrack )
                   + ( t * Infos->TrackSize )
                   ]
          , 0xE5
          , SECTSIZE * NbSect
          );
    strcpy( tr->ID, "Track-Info\r\n" );
    tr->Track = ( BYTE )t;
    tr->Head = 0;
    tr->SectSize = 2;
    tr->NbSect = ( BYTE )NbSect;
    tr->Gap3 = 0x4E;
    tr->OctRemp = 0xE5;
    int ss = 0;
    //
    // Gestion "entrelacement" des secteurs
    //
    for ( int s = 0; s < NbSect; )
        {
        tr->Sect[ s ].C = ( BYTE )t;
        tr->Sect[ s ].H = 0;
        tr->Sect[ s ].R = ( BYTE )( ss + MinSect );
        tr->Sect[ s ].N = 2;
        tr->Sect[ s ].SectSize = SECTSIZE;
        ss++;
        if ( ++s < NbSect )
            {
            tr->Sect[ s ].C = ( BYTE )t;
            tr->Sect[ s ].H = 0;
            tr->Sect[ s ].R = ( BYTE )( ss + MinSect + 4 );
            tr->Sect[ s ].N = 2;
            tr->Sect[ s ].SectSize = SECTSIZE;
            s++;
            }
        }
}


//
// Indique si une piste a ses secteurs entrelacés ou non
//
BOOL IsTrackInterlaced( int nDSK, int t )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ sizeof( CPCEMUEnt ) + t * Infos->TrackSize ];
    int ss = 0;
    int MinSect = GetMinSect( nDSK, TRUE );
    //
    // Gestion "entrelacement" des secteurs
    //
    for ( int s = 0; s < tr->NbSect; )
        {
        if ( tr->Sect[ s ].R != ( BYTE )( ss + MinSect ) )
            return( FALSE );

        ss++;
        if ( ++s < tr->NbSect )
            {
            if ( tr->Sect[ s ].R != ( BYTE )( ss + MinSect + 4 ) )
                return( FALSE );

            s++;
            }
        }
    return( TRUE );
}


//
// Entrelace les secteurs d'une piste
//
void InterlaceTrack( int nDSK, int t )
{
    static BYTE RawSect[ MAX_SECTS * SECTSIZE ];
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ sizeof( CPCEMUEnt ) + t * Infos->TrackSize ];
    int MinSect = GetMinSect( nDSK, TRUE );
    int s;
    
    for ( s = 0; s < tr->NbSect; s++ )
        memcpy( &RawSect[ s * SECTSIZE ]
              , ReadSect( nDSK, t, s, FALSE )
              , SECTSIZE
              );

    //
    // Gestion "entrelacement" des secteurs
    //
    int ss = 0;
    for ( s = 0; s < tr->NbSect; )
        {
        tr->Sect[ s ].R = ( BYTE )( ss + MinSect );
        ss++;
        if ( ++s < tr->NbSect )
            {
            tr->Sect[ s ].R = ( BYTE )( ss + MinSect + 4 );
            s++;
            }
        }

    for ( s = 0; s < tr->NbSect; s++ )
        WriteSect( nDSK, t, s, &RawSect[ s * SECTSIZE ], TRUE );
}


void LowLevelFormatTrack( int nDSK
                        , int t
                        , int NumSect
                        , int NbSect
                        , BYTE C
                        , BYTE H
                        , BYTE R
                        , BYTE N
                        )
{
    CPCEMUTrack * tr = GetInfoTrack( nDSK, t );

    /*
    memset( &ImgDsk[ nDSK ][ sizeof( CPCEMUEnt )
                   + sizeof( CPCEMUTrack )
                   + ( t * Infos->TrackSize )
                   ]
          , 0xE5
          , SECTSIZE * NbSect
          );
    */
    strcpy( tr->ID, "Track-Info\r\n" );
    tr->Track = ( BYTE )t;
    tr->Head = 0;
    tr->SectSize = 2;
    tr->NbSect = ( BYTE )NbSect;
    tr->Gap3 = 0x4E;
    tr->OctRemp = 0xE5;
    tr->Sect[ NumSect ].C = C;
    tr->Sect[ NumSect ].H = H;
    tr->Sect[ NumSect ].R = R;
    tr->Sect[ NumSect ].N = N;
    tr->Sect[ NumSect ].SectSize = ( short )( 128 << N );
}


//
// Ecriture d'un bloc AMSDOS (1 block = 2 secteurs)
//
void WriteBloc( int nDSK, int bloc, BYTE BufBloc[ SECTSIZE * 2 ] )
{
    int track = ( bloc << 1 ) / 9;
    int sect = ( bloc << 1 ) % 9;
    int MinSect = GetMinSect( nDSK, TRUE );
    if ( MinSect == 0x41 )
        track += 2;
    else
        if ( MinSect == 0x01 )
            track++;

    //
    // Ajuste le nombre de pistes si dépassement capacité
    //
    CPCEMUEnt * Entete = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    if ( track > Entete->NbTracks - 1 )
        {
        Entete->NbTracks = ( BYTE )( track + 1 );
        FormatTrack( nDSK, Entete, track, MinSect, 9 );
        }

    int Pos = GetPosData( nDSK, track, sect + MinSect, TRUE );
    memcpy( &ImgDsk[ nDSK ][ Pos ], BufBloc, SECTSIZE );
    if ( ++sect > 8 )
        {
        track++;
        sect = 0;
        }
    Pos = GetPosData( nDSK, track, sect + MinSect, TRUE );
    memcpy( &ImgDsk[ nDSK ][ Pos ], &BufBloc[ SECTSIZE ], SECTSIZE );
}


//
// Ecriture d'un secteur
//
void WriteSect( int nDSK, int track, int sect, BYTE * Buff, BOOL AmsdosMode )
{
    int MinSect = AmsdosMode ? GetMinSect( nDSK, TRUE ) : 0;
    if ( ( MinSect == 0x41 ) && AmsdosMode )
        track += 2;
    else
        if ( ( MinSect == 0x01 ) && AmsdosMode )
            track++;

    int Pos = GetPosData( nDSK, track, sect + MinSect, AmsdosMode );
    memcpy( &ImgDsk[ nDSK ][ Pos ], Buff, SECTSIZE );
}


//
// Lecture d'un secteur
//
BYTE * ReadSect( int nDSK, int track, int sect, BOOL AmsdosMode )
{
    int MinSect = AmsdosMode ? GetMinSect( nDSK, TRUE ) : 0;
    if ( ( MinSect == 0x41 ) && AmsdosMode )
        track += 2;
    else
        if ( ( MinSect == 0x01 ) && AmsdosMode )
            track++;

    int Pos = GetPosData( nDSK, track, sect + MinSect, AmsdosMode );
    return( &ImgDsk[ nDSK ][ Pos ] );
}


//
// Retourne les informations d'une piste
//
CPCEMUTrack * GetInfoTrack( int nDSK, int track )
{
    int Pos = sizeof( CPCEMUEnt );
    for ( int t = 0; t < track; t++ )
        {
        CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ Pos ];
        Pos += sizeof( CPCEMUTrack );
        for ( int s = 0; s < tr->NbSect; s++ )
            {
            if ( tr->Sect[ s ].SectSize )
                Pos += tr->Sect[ s ].SectSize;
            else
                Pos += ( 128 << tr->Sect[ s ].N );
            }
        }
    return( ( CPCEMUTrack * )&ImgDsk[ nDSK ][ Pos ] );
}


//
// Remplit un "bitmap" pour savoir où il y a des fichiers sur la disquette
// Retourne également le nombre de Ko utilisés sur la disquette
//
int FillBitmap( int nDSK )
{
    int NbKo = 0;

    memset( Bitmap[ nDSK ], 0, sizeof( Bitmap[ nDSK ] ) );
    Bitmap[ nDSK ][ 0 ] = Bitmap[ nDSK ][ 1 ] = 1;
    for ( int i = 0; i < 64; i++ )
        {
        StDirEntry * Dir = GetInfoDirEntry( nDSK, i );
        if ( Dir->User != USER_DELETED )
            {
            for ( int j = 0; j < 16; j++ )
                {
                int b = Dir->Blocks[ j ];
                if ( b > 1 && ( ! Bitmap[ nDSK ][ b ] ) )
                    {
                    Bitmap[ nDSK ][ b ] = 1;
                    NbKo++;
                    }
                }
            }
        }
    return( NbKo );
}


//
// Positionne une entrée dans le répertoire
//
void SetInfoDirEntry( int nDSK, int NumDir, StDirEntry * Dir )
{
    int MinSect = GetMinSect( nDSK, TRUE );
    int s = ( NumDir >> 4 ) + MinSect;
    int t = ( MinSect == 0x41 ? 2 : 0 );
    if ( MinSect == 1 )
        t = 1;

    memcpy( &ImgDsk[ nDSK ][ ( ( NumDir & 15 ) << 5 ) + GetPosData( nDSK, t, s, TRUE ) ]
          , Dir
          , sizeof( StDirEntry )
          );
}


int CompareNomsAmsdos( char * n1, char * n2 )
{
    for ( int i = 0; i < 11; i++ )
        if ( ( n1[ i ] & 0x7F ) != ( n2[ i ] & 0x7F ) )
            return 1;

    return( 0 );
}

//
// Vérifie l'existente d'un fichier, retourne l'indice du fichier si existe,
// -1 sinon
//
int FileExist( int nDSK, char * Nom, int User )
{
    for ( int i = 0; i < 64; i++ )
        {
        StDirEntry * Dir = GetInfoDirEntry( nDSK, i );
        if ( Dir->User == User && ! CompareNomsAmsdos( Nom, ( char * )Dir->Nom ) )
            return( i );
        }
    return( -1 );
}


StDirEntry * GetNomDir( char * NomFic )
{
    static StDirEntry DirLoc;

    memset( &DirLoc, 0, sizeof( DirLoc ) );
    memset( DirLoc.Nom, ' ', 8 );
    memset( DirLoc.Ext, ' ', 3 );
    char * p = strchr( NomFic, '.' );
    if ( p )
        {
        p++;
        memcpy( DirLoc.Nom, NomFic, min( p - NomFic - 1, 8 ) );
        memcpy( DirLoc.Ext, p, min( strlen( p ), 3 ) );
        }
    else
        memcpy( DirLoc.Nom, NomFic, min( strlen( NomFic ), 8 ) );

    for ( int i = 0; i < 11; i++ )
        DirLoc.Nom[ i ] = ( BYTE )toupper( DirLoc.Nom[ i ] );

    return( &DirLoc );
}


//
// Copie un fichier sur le DSK
//
int CopieFichier( int nDSK, BYTE * BufFile, char * NomFic, int TailleFic, int MaxBloc, int User )
{
    int j, l, Bloc, PosFile, NbPages = 0, PosDir, TaillePage;

    FillBitmap( nDSK );
    StDirEntry * DirLoc = GetNomDir( NomFic );
    if ( FileExist( nDSK, ( char * )DirLoc->Nom, User ) > -1 )
        return( ERR_FILE_EXIST );

    for ( PosFile = 0; PosFile < TailleFic; )
        {
        PosDir = RechercheDirLibre( nDSK );
        if ( PosDir != -1 )
            {
            DirLoc->User = ( BYTE )User;
            DirLoc->NumPage = ( BYTE )NbPages++;
            TaillePage = ( ( TailleFic - PosFile ) + 127 ) >> 7;
            if ( TaillePage > 128 )
                TaillePage = 128;

            DirLoc->NbPages = ( BYTE )TaillePage;
            l = ( DirLoc->NbPages + 7 ) >> 3;
            memset( DirLoc->Blocks, 0, 16 );
            for ( j = 0; j < l; j++ )
                {
                Bloc = RechercheBlocLibre( nDSK, MaxBloc );
                if ( Bloc )
                    {
                    DirLoc->Blocks[ j ] = ( BYTE )Bloc;
                    WriteBloc( nDSK, Bloc, &BufFile[ PosFile ] );
                    PosFile += 1024;
                    }
                else
                    return( ERR_NO_BLOCK );
                }
            SetInfoDirEntry( nDSK, PosDir, DirLoc );
            }
        else
            return( ERR_NO_DIRENTRY );
        }
    return( ERR_NO_ERR );
}


//
// Retourne une entrée du répertoire
//
StDirEntry * GetInfoDirEntry( int nDSK, int NumDir )
{
    static StDirEntry Dir;
    int MinSect = GetMinSect( nDSK, TRUE );
    int s = ( NumDir >> 4 ) + MinSect;
    int t = ( MinSect == 0x41 ? 2 : 0 );
    if ( MinSect == 1 )
        t = 1;

    memcpy( &Dir
          , &ImgDsk[ nDSK ][ ( ( NumDir & 15 ) << 5 ) + GetPosData( nDSK, t, s, TRUE ) ]
          , sizeof( StDirEntry )
          );
    return( &Dir );
}


//
// Vérifier si DSK est "lisible" : contient un catalogue
//
BOOL IsReadable( int nDSK )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    if ( Infos->NbHeads == 1 )
        {
        int MinSectFirst = 0xFFFF;
        BOOL S2Ok = FALSE, S3Ok = FALSE, S4Ok = FALSE;
        int s;
        // Recherche position secteur
        int Pos = sizeof( CPCEMUEnt );
        CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ Pos ];

        // Recherche secteur début catalogue
        for ( s = 0; s < tr->NbSect; s++ )
            {
            BYTE CurSect = tr->Sect[ s ].R;
            if ( CurSect == 0x41 || CurSect == 0xC1 || CurSect == 0x01 )
                MinSectFirst = CurSect;
            }
        for ( s = 0; s < tr->NbSect; s++ )
            {
            BYTE CurSect = tr->Sect[ s ].R;
            if ( CurSect == MinSectFirst + 1 )
                S2Ok = TRUE;

            if ( CurSect == MinSectFirst + 2 )
                S3Ok = TRUE;

            if ( CurSect == MinSectFirst + 3 )
                S4Ok = TRUE;
            }
        if ( S2Ok && S3Ok && S4Ok ) 
            return( TRUE );
        }
    return( FALSE );
}


//
// Vérifier si DSK est "standard" (DATA ou VENDOR)
//
BOOL CheckDsk( int nDSK )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
    if ( Infos->NbHeads == 1 )
        {
        int MinSectFirst = GetMinSect( nDSK );
        if (  MinSectFirst != 0x41
           && MinSectFirst != 0xC1
           && MinSectFirst != 0x01
           )
            return( FALSE );

        for ( int track = 0; track < Infos->NbTracks; track++ )
            {
            // Recherche position secteur
            int Pos = sizeof( CPCEMUEnt ) + ( 0x1200 + sizeof( CPCEMUTrack ) ) * track;
            CPCEMUTrack * tr = ( CPCEMUTrack * )&ImgDsk[ nDSK ][ Pos ];
    
            int MinSect = 0xFF, MaxSect = 0;
            if ( tr->NbSect != 9 )
                {
                if ( track < 40 )
                    return( FALSE );
                else
                    {
                    Infos->NbTracks = ( BYTE )( track - 1 );
                    return( TRUE );
                    }
                }

            for ( int s = 0; s < tr->NbSect; s++ )
                {
                if ( MinSect > tr->Sect[ s ].R )
                    MinSect = tr->Sect[ s ].R;

                if ( MaxSect < tr->Sect[ s ].R )
                    MaxSect = tr->Sect[ s ].R;
                }
            if ( MaxSect - MinSect != 8 )
                return( FALSE );

            if ( MinSect != MinSectFirst )
                return( FALSE );
            }
        return( TRUE );
        }
    return( FALSE );
}


//
// Vérifie si un fichier est bien un fichier DSK
//
BOOL IsValidDsk( char * NomFic )
{
    BOOL Ret = FALSE;
    DWORD Lg;
    CPCEMUEnt Infos;

    if ( * NomFic == '"' )
        {
        NomFic[ strlen( NomFic ) - 1 ] = 0;
        strcpy( NomFic, NomFic + 1 );
        }
    HANDLE fp = CreateFile( NomFic
                         , GENERIC_READ
                         , 0
                         , NULL
                         , OPEN_EXISTING
                         , 0
                         , NULL
                         );
    if ( fp != INVALID_HANDLE_VALUE )
        {
        ReadFile( fp, &Infos, sizeof( Infos ), &Lg, NULL );
        if (  ! strncmp( Infos.debut, "MV -", 4 )
           || ! strncmp( Infos.debut, "EXTENDED CPC DSK", 16 )
           )
            Ret = TRUE;

        CloseHandle( fp );
        }
    return( Ret );
}


//
// Lire un fichier DSK
//
BOOL ReadDsk( int nDSK, char * NomFic )
{
    BOOL Ret = FALSE;
    DWORD Lg;
    CPCEMUEnt * Infos;

    if ( * NomFic == '"' )
        {
        NomFic[ strlen( NomFic ) - 1 ] = 0;
        strcpy( NomFic, NomFic + 1 );
        }
    HANDLE fp = CreateFile( NomFic
                         , GENERIC_READ
                         , 0
                         , NULL
                         , OPEN_EXISTING
                         , 0
                         , NULL
                         );
    if ( fp != INVALID_HANDLE_VALUE )
        {
        ReadFile( fp, ImgDsk[ nDSK ], sizeof( ImgDsk[ nDSK ] ), &Lg, NULL );
        Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];
        if (  ! strncmp( Infos->debut, "MV -", 4 )
           || ! strncmp( Infos->debut, "EXTENDED CPC DSK", 16 )
           )
            Ret = TRUE;

        CloseHandle( fp );
        }
    return( Ret );
}


//
// Initialisation de l'en-tête du DSK
//
void InitEntete( int nDSK, int NbSect, int NbTrack )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];

    strcpy( Infos->debut, "MV - CPCEMU Disk-File\r\nDisk-Info\r\n" );
    Infos->TrackSize = ( short )( sizeof( CPCEMUTrack ) + SECTSIZE * NbSect );
    Infos->NbTracks = ( BYTE )NbTrack;
    Infos->NbHeads = 1;
}


//
// Formater une disquette
//
void FormatDsk( int nDSK, int NbSect, int NbTrack )
{
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];

    InitEntete( nDSK, NbSect, NbTrack );
    for ( int t = 0; t < NbTrack; t++ )
        FormatTrack( nDSK, Infos, t, 0xC1, NbSect );

    FillBitmap( nDSK );
}


//
// Ecriture du fichier DSK
//
BOOL WriteDsk( int nDSK, char * NomDsk )
{
    DWORD Lg;
    CPCEMUEnt * Infos = ( CPCEMUEnt * )ImgDsk[ nDSK ];

    if ( * NomDsk == '"' )
        {
        NomDsk[ strlen( NomDsk ) - 1 ] = 0;
        strcpy( NomDsk, NomDsk + 1 );
        }
    HANDLE fp = CreateFile( NomDsk
                         , GENERIC_WRITE
                         , 0
                         , NULL
                         , CREATE_ALWAYS
                         , 0
                         , NULL
                         );
    if ( fp != INVALID_HANDLE_VALUE )
        {
        if ( ! Infos->TrackSize )
            Infos->TrackSize = 0x100 + SECTSIZE * 9;

        int Taille = Infos->NbTracks * Infos->TrackSize + sizeof( * Infos );
        WriteFile( fp, ImgDsk[ nDSK ], Taille, &Lg, NULL );
        CloseHandle( fp );
        return( TRUE );
        }
    return( FALSE );
}


//
// Vérifie si en-tête AMSDOS est valide
//
BOOL CheckAmsdos( BYTE * Buf )
{
    int i, Checksum = 0;
    BOOL ModeAmsdos = FALSE;
    StAmsdos * pEntete = ( StAmsdos * )Buf;

    for ( i = 0; i < 67; i++ )
        Checksum += Buf[ i ];

    if ( ( pEntete->CheckSum == ( USHORT )Checksum ) && Checksum )
        ModeAmsdos = TRUE;

    return( ModeAmsdos );
}


//
// Effectue un "nettoyage" de l'en-tête Amsdos :
// remet à zéro les octets inutilisés
//
void ClearAmsdos( BYTE * Buf )
{
    if ( CheckAmsdos( Buf ) )
        {
        StAmsdos * pEntete = ( StAmsdos * )Buf;
        memset( pEntete->Unused, 0, sizeof( pEntete->Unused ) );
        memset( pEntete->Unused2, 0, sizeof( pEntete->Unused2 ) );
        SetChecksum( pEntete );
        }
}


//
// Crée une en-tête AMSDOS par défaut
//
StAmsdos * CreeEnteteAmsdos( char * NomFic, int Longueur )
{
    static char NomReel[ 256 ];
    static StAmsdos Entete;
    static char Nom[ 12 ];
    int i;

    strcpy( NomReel, NomFic );
    memset( &Entete, 0, sizeof( Entete ) );
    memset( Nom, ' ', sizeof( Nom ) );
    char * p = NULL;
    do
        {
        p = strchr( NomReel, '\\' );
        if ( p )
            strcpy( NomReel, ++p );
        }
    while( p );
    p = strchr( NomReel, '.' );
    if ( p )
        * p++ = 0;

    int l = strlen( NomReel );
    if ( l > 8 )
        l = 8;

    for ( i = 0; i < l; i++ )
        Nom[ i ] = ( char )toupper( NomReel[ i ] );

    if ( p )
        for ( i = 0; i < 3; i++ )
            Nom[ i + 8 ] = ( char )toupper( p[ i ] );

    memcpy( Entete.FileName, Nom, 11 );
    Entete.Length = Entete.RealLength = Entete.LogicalLength = ( USHORT )Longueur;
    Entete.BigLength = ( BYTE )( Longueur >> 16 );
    Entete.FileType = 2;

    SetChecksum( &Entete );
    return( &Entete );
}


StAmsdos * CreeEnteteComplet( char * NomFic, int type, int st, int lg, int ex )
{
    StAmsdos * e = CreeEnteteAmsdos( NomFic, lg );
    e->Adress = ( short )st;
    e->EntryAdress = ( short )ex;
    e->FileType = ( char )type;
    SetChecksum( e );
    return( e );
}


//
// Calcule et positionne le checksum AMSDOS
//
void SetChecksum( StAmsdos * pEntete )
{
    int i, Checksum = 0;

    BYTE * p = ( BYTE * )pEntete;
    for ( i = 0; i < 67; i++ )
        Checksum += * p++;

    pEntete->CheckSum = ( USHORT )Checksum;
}


void SwapDsk( void )
{
    static BYTE ImgDskTmp[ 0x100000 ];

    memcpy( ImgDskTmp, ImgDsk[ 0 ], sizeof( ImgDskTmp ) );
    memcpy( ImgDsk[ 0 ], ImgDsk[ 1 ], sizeof( ImgDsk[ 0 ] ) );
    memcpy( ImgDsk[ 1 ], ImgDskTmp, sizeof( ImgDsk[ 1 ] ) );
}
