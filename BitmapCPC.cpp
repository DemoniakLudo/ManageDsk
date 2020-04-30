#include  <afxwin.h>

#include  "BitmapCPC.h"
#include  "GestDsk.h"


//
// Structure palette (comme OCP art studio)
//
typedef struct
    {
    BYTE    Mode;
    BYTE    AnimFlag;
    BYTE    AnimDelay;
    BYTE    Palette[ 16 ][ 12 ];
    BYTE    Border[ 12 ];
    BYTE    ExcludedInk[ 16 ];
    BYTE    ProtectedInk[ 16 ];
    } StPal;


static BYTE CpcVGA[ 28 ] = "TDU\\X]LEMVFW^@_NGORBSZY[ICK";
//                          0000 00000011111111112222222
//                          0123 45678901234567890123456


//
// Couleurs du CPC converties en composantes r, v, b
//
static StRVB RgbCPC[ 27 ] =
    {
        { 0x00, 0x00, 0x00, 0x00 },
        { 0x7F, 0x00, 0x00, 0x00 },
        { 0xFF, 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x7F, 0x00 },
        { 0x7F, 0x00, 0x7F, 0x00 },
        { 0xFF, 0x00, 0x7F, 0x00 },
        { 0x00, 0x00, 0xFF, 0x00 },
        { 0x7F, 0x00, 0xFF, 0x00 },
        { 0xFF, 0x00, 0xFF, 0x00 },
        { 0x00, 0x7F, 0x00, 0x00 },
        { 0x7F, 0x7F, 0x00, 0x00 },
        { 0xFF, 0x7F, 0x00, 0x00 },
        { 0x00, 0x7F, 0x7F, 0x00 },
        { 0x7F, 0x7F, 0x7F, 0x00 },
        { 0xFF, 0x7F, 0x7F, 0x00 },
        { 0x00, 0x7F, 0xFF, 0x00 },
        { 0x7F, 0x7F, 0xFF, 0x00 },
        { 0xFF, 0x7F, 0xFF, 0x00 },
        { 0x00, 0xFF, 0x00, 0x00 },
        { 0x7F, 0xFF, 0x00, 0x00 },
        { 0xFF, 0xFF, 0x00, 0x00 },
        { 0x00, 0xFF, 0x7F, 0x00 },
        { 0x7F, 0xFF, 0x7F, 0x00 },
        { 0xFF, 0xFF, 0x7F, 0x00 },
        { 0x00, 0xFF, 0xFF, 0x00 },
        { 0x7F, 0xFF, 0xFF, 0x00 },
        { 0xFF, 0xFF, 0xFF, 0x00 }
    };


int GetRgbCPC( int Coul )
{
    if ( Coul >= 0 && Coul < 27 )
        {
        StRVB i = RgbCPC[ Coul ];
        return( ( i.b << 16 ) + ( i.v << 8 ) + i.r );
        }
    return( -1 );
}


StRVB GetPalCPC( int Coul )
{
    if ( Coul >= 0 && Coul < 27 )
        return( RgbCPC[ Coul ] );
    
    return( RgbCPC[ 0 ] );
}


void CBitmapCPC::InitPalette( BYTE NewPal[ 16 ], BOOL SetNewPal )
{
    /*
    Si sauvegardé avec ConvImgCpc, alors la palette se trouve
    dans l'image...
    */
    if (  BitmapCPC[ 0x7D0 ] == 0x3A
       && BitmapCPC[ 0x7D1 ] == 0xD0
       && BitmapCPC[ 0x7D2 ] == 0xD7
       && BitmapCPC[ 0x7D3 ] == 0xCD
       )
        {
        Mode = BitmapCPC[ 0x17D0 ];
        for ( int i = 0; i < 16; i++ )
            Palette[ i ] = BitmapCPC[ 0x17D1 + i ];
        }
    if ( SetNewPal )
        for ( int i = 0; i < 16; i++ )
            Palette[ i ] = NewPal[ i ];
}


//
// Décompacter une image au format OCP
//
void CBitmapCPC::DepactOCP( void )
{
    static BYTE BufTmp[ 0x4000 ];
    int PosIn = 0, PosOut = 0;
    int LgOut, CntBlock = 0;

    memcpy( BufTmp, BitmapCPC, sizeof( BufTmp ) );
    memset( BitmapCPC, 0, sizeof( BitmapCPC ) );
    while( PosOut < 0x4000 )
        {
        if ( ! strncmp( ( char * )&BufTmp[ PosIn ], "MJH", 3 ) )
            {
            PosIn += 3;
            LgOut = BufTmp[ PosIn++ ];
            LgOut += ( BufTmp[ PosIn++ ] << 8 );
            CntBlock = 0;
            while( CntBlock < LgOut )
                {
                if ( ! strncmp( ( char * )&BufTmp[ PosIn ], "MJH", 3 ) )
                    break;

                BYTE a = BufTmp[ PosIn++ ];
                if ( a == MARKER_OCP )
                    {
                    int c = BufTmp[ PosIn++ ];
                    a = BufTmp[ PosIn++ ];
                    if ( ! c )
                        c = 0x100;

                    for ( int i = 0; i < c && CntBlock < LgOut; i++ )
                        {
                        BitmapCPC[ PosOut++ ] = a;
                        CntBlock++;
                        }
                    }
                else
                    {
                    BitmapCPC[ PosOut++ ] = a;
                    CntBlock++;
                    }
                }
            }
        else
            {
            LgDcmp = PosOut;
            PosOut = 0x4000;
            }
        }
}


//
// Positionne l'état "WIN" ou non d'un bitmap
//
void CBitmapCPC::SetWin( BOOL i )
{
    IsWin = i;
    if ( ! strncmp( ( char * )BitmapCPC, "MJH", 3 ) )
        DepactOCP();

    if ( IsWin )
        {
        NbCol = ( BitmapCPC[ LgDcmp - 5 ] 
                + BitmapCPC[ LgDcmp - 4 ] 
                + ( BitmapCPC[ LgDcmp - 3 ] << 8 )
                + 7
                ) >> 3;
        NbLignes = BitmapCPC[ LgDcmp - 2 ];
        }
}


//
// Lecture d'une image (format AMSDOS) et affichage
//
BOOL CBitmapCPC::LireImage( char * Nom, StRVB * Bitmap )
{
    static BYTE Entete[ 0x80 ];
    BOOL Ret = FALSE;
    DWORD Nb;

    HANDLE hFile = CreateFile( Nom
                             , FILE_READ_DATA
                             , FILE_SHARE_READ
                             , NULL
                             , OPEN_EXISTING
                             , FILE_ATTRIBUTE_NORMAL
                             , NULL
                             );
    if ( hFile )
        {
        ReadFile( hFile, Entete, sizeof( Entete ), &Nb, NULL );
        if ( CheckAmsdos( Entete ) )
            {
            ReadFile( hFile, BitmapCPC, sizeof( BitmapCPC ), &Nb, NULL );
            if ( ! strncmp( ( char * )BitmapCPC, "MJH", 3 ) )
                DepactOCP();

            InitPalette( NULL, FALSE );
            Ret = TRUE;
            }
        CloseHandle( hFile );
        if ( Ret )
            Render( Bitmap );
        }
    return( Ret );

}


//
// Affiche l'image à l'écran
//
void CBitmapCPC::Render( StRVB * Bitmap, BOOL Flat )
{
    int AdrCPC = 0, i, p0, p1, p2, p3;
    int PosX = max( 0, 80 - NbCol ) >> 1;

    for ( int y = 0; y < NbLignes; y++ )
        {
        int AdrBitmap = TAILLE_CPC_X * ( y + ( ( 200 - NbLignes ) >> 1 ) )
                      + ( PosX << 3 );
        if ( Flat )
            AdrCPC = NbCol * y;
        else
            AdrCPC = ( y >> 3 ) * NbCol + ( y & 7 ) * 0x800;

        for ( int x = 0; x < NbCol; x++ )
            {
            BYTE Octet = BitmapCPC[ AdrCPC + x ];
            switch( Mode )
                {
                case 0 :
                case 3 : // Mode 3 = Mode 0
                    p0 = ( Octet >> 7 )
                       + ( ( Octet & 0x20 ) >> 3 )
                       + ( ( Octet & 0x08 ) >> 2 )
                       + ( ( Octet & 0x02 ) << 2 );
                    p1 = ( ( Octet & 0x40 ) >> 6 )
                       + ( ( Octet & 0x10 ) >> 2 )
                       + ( ( Octet & 0x04 ) >> 1 )
                       + ( ( Octet & 0x01 ) << 3 );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p0 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p0 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p0 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p0 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p1 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p1 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p1 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p1 ] );
                    break;

                case 1 :
                    p0 = ( ( Octet >> 7 ) & 1 ) + ( ( Octet >> 2 ) & 2 );
                    p1 = ( ( Octet >> 6 ) & 1 ) + ( ( Octet >> 1 ) & 2 );
                    p2 = ( ( Octet >> 5 ) & 1 ) + ( ( Octet >> 0 ) & 2 );
                    p3 = ( ( Octet >> 4 ) & 1 ) + ( ( Octet << 1 ) & 2 );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p0 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p0 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p1 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p1 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p2 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p2 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p3 ] );
                    Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ p3 ] );
                    break;

                case 2 :
                    for ( i = 8; i--; )
                        Bitmap[ AdrBitmap++ ] = GetPalCPC( Palette[ ( Octet >> i ) & 1 ] );
                    break;
                }
            }
        }
}


//
// affiche une image "brute"
//
void CBitmapCPC::SetBitmapCPC( BYTE * BitmapSource, int Lg, BOOL SetPalette )
{
    LgDcmp = Lg;
    memcpy( BitmapCPC, BitmapSource, min( Lg, sizeof( BitmapCPC ) ) );
    if ( Lg < sizeof( BitmapCPC ) )
        memset( &BitmapCPC[ Lg ], 0, sizeof( BitmapCPC ) - Lg );

    if ( ! strncmp( ( char * )BitmapCPC, "MJH", 3 ) )
        DepactOCP();

    if ( SetPalette )
        InitPalette( NULL, FALSE );
}


//
// Décode une palette de type OCP (.PAL)
//
void CBitmapCPC::DecodePalette( BYTE * PalOcp )
{
    StPal * Pal = ( StPal * )PalOcp;
    if ( Pal->Mode < 3 )
        {
        for ( int i = 0; i < 16; i++ )
            if ( Pal->Palette[ i ][ 0 ] == 'J' )
                Palette[ i ] = 24;
            else
                for ( int j = 0; j < 27; j++ )
                    if ( Pal->Palette[ i ][ 0 ] == CpcVGA[ j ] )
                        Palette[ i ] = ( BYTE )j;

        Mode = Pal->Mode;
        }
}
