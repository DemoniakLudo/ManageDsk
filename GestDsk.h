#ifndef GESTDSK_H
#define GESTDSK_H


#pragma warning(disable:4711)           // selected for automatic inline


#define     USER_DELETED    0xE5

#define     MAX_SECTS       29      // Nb max de secteurs/pistes pour formatage

#define     MAX_PISTE_STD   40

#define     MAX_TRACKS      255      // Nbre maxi de pistes/DSK


//
// Structure d'une entrée AMSDOS
//
typedef struct
    {
    BYTE    UserNumber;             // User
    BYTE    FileName[ 15 ];         // Nom + extension
    BYTE    BlockNum;               // Numéro du premier bloc (disquette)
    BYTE    LastBlock;              // Numéro du dernier bloc (disquette)
    BYTE    FileType;               // Type de fichier
    USHORT  Length;                 // Longueur
    USHORT  Adress;                 // Adresse
    BYTE    FirstBlock;             // Premier bloc de fichier (disquette)
    USHORT  LogicalLength;          // Longueur logique
    USHORT  EntryAdress;            // Point d'entrée
    BYTE    Unused[ 0x24 ];
    USHORT  RealLength;             // Longueur réelle
    BYTE    BigLength;              // Longueur réelle (3 octets)
    USHORT  CheckSum;               // CheckSum Amsdos
    BYTE    Unused2[ 0x3B ];
    } StAmsdos;


#define SECTSIZE   512


typedef struct
    {
    char  debut[ 0x30 ]; // "MV - CPCEMU Disk-File\r\nDisk-Info\r\n"
    BYTE NbTracks;
    BYTE NbHeads;
    SHORT TrackSize; // 0x1300 = 256 + ( 512 * nbsecteurs )
    BYTE TrackSizeTable[ 0xCC ]; // Si "EXTENDED CPC DSK File"
    } CPCEMUEnt;

typedef struct
    {
    BYTE C;                // track
    BYTE H;                // head
    BYTE R;                // sect
    BYTE N;                // size
    BYTE ST1;              // Valeur ST1
    BYTE ST2;              // Valeur ST2
    SHORT SectSize;         // Taille du secteur en octets
    } CPCEMUSect;

typedef struct
    {
    char        ID[ 0x10 ];   // "Track-Info\r\n"
    BYTE       Track;
    BYTE       Head;
    SHORT       Unused;
    BYTE       SectSize; // 2
    BYTE       NbSect;   // 9
    BYTE       Gap3;     // 0x4E
    BYTE       OctRemp;  // 0xE5
    CPCEMUSect  Sect[ MAX_SECTS ];
    } CPCEMUTrack;


typedef struct
    {
    BYTE        User;
    BYTE        Nom[ 8 ];
    BYTE        Ext[ 3 ];
    BYTE        NumPage;
    BYTE        Unused[ 2 ];
    BYTE        NbPages;
    BYTE        Blocks[ 16 ];
    } StDirEntry;


typedef struct
    {
    char        NomFic[ 256 ];
    DWORD       LongFic;
    BYTE        Image;
    CPCEMUEnt   Infos;
    CPCEMUTrack * Tracks[ MAX_TRACKS ][ 2 ];
    BYTE        * Data[ MAX_TRACKS ][ 2 ];
    BYTE        FlagWrite;
    } ImgDSK;


//extern ImgDSK Dsk[ MAX_DSK ];


enum { ERR_NO_ERR = 0, ERR_NO_DIRENTRY, ERR_NO_BLOCK, ERR_FILE_EXIST };


int GetMinSect( int nDSK, BOOL ForceStd = FALSE );

BYTE * GetRawData( int Pos );

void WriteRawData( int Pos, BYTE * Data, int Longueur );

void CopyDsk( int nDSK_0, int nDSK_1 );

int GetNbTracks( int nDSK );

void SetNbTracks( int nDSK, int NbTracks );

int GetMaxBlocs( int MaxTracks, int nDSK );

BYTE * ReadBloc( int nDSK, int bloc );

void WriteBloc( int nDSK, int bloc, BYTE * BufBloc );

void WriteSect( int nDSK
              , int Track
              , int Sect
              , BYTE * Buff
              , BOOL AmsdosMode
              );

BYTE * ReadSect( int nDSK, int Track, int Sect, BOOL AmsdosMode );

CPCEMUTrack * GetInfoTrack( int nDSK, int Track );

BOOL IsTrackInterlaced( int nDSK, int t );

void InterlaceTrack( int nDSK, int t );

void LowLevelFormatTrack( int nDSK
                        , int t
                        , int NumSect
                        , int NbSect
                        , BYTE C
                        , BYTE H
                        , BYTE R
                        , BYTE N
                        );

void InitEntete( int nDSK, int NbSect, int NbTrack );

void FormatDsk( int nDSK, int NbSect, int NbTrack );

StDirEntry * GetNomDir( char * Nom );

int CopieFichier( int nDSK
                , BYTE * BufFile
                , char * NomFic
                , int TailleFic
                , int MaxBloc
                , int User
                );

BOOL WriteDsk( int nDSK, char * NomDsk );

BOOL CheckAmsdos( BYTE * Buf );

BOOL ReadDsk( int nDSK, char * NomFic );

BOOL IsValidDsk( char * NomFic );

BOOL IsReadable( int nDSK );

BOOL CheckDsk( int nDSK );

int CompareNomsAmsdos( char * n1, char * n2 );

int FileExist( int nDSK, char * Nom, int User );

StDirEntry * GetInfoDirEntry( int nDSK, int NumDir );

void SetInfoDirEntry( int nDSK, int NumDir, StDirEntry * Dir );

int FillBitmap( int nDSK );

void ClearAmsdos( BYTE * Buf );

StAmsdos * CreeEnteteAmsdos( char * NomFic, int Length );

StAmsdos * CreeEnteteComplet( char * NomFic, int type, int st, int lg, int ex );

void SetChecksum( StAmsdos * pEntete );

void SwapDsk( void );


#endif
