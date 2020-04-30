#ifndef BITMAPCPC_H
#define BITMAPCPC_H


#define     TAILLE_CPC_X    1024

#define     TAILLE_CPC_Y    200

#define     TAILLE_PC_X    640

#define     TAILLE_PC_Y    400


#define     MARKER_OCP      1       // Marker pour compression RLE


typedef struct
    {
    BYTE b, v, r, a;
    } StRVB;


class CBitmapCPC
{
public :
    CBitmapCPC( void ) { NbCol = 80; NbLignes = 200; IsWin = FALSE; }
    BOOL LireImage( char * Nom, StRVB * Bitmap );
    void Render( StRVB * Bitmap, BOOL Flat = FALSE );
    void SetBitmapCPC( BYTE * BitmapSource, int Lg, BOOL SetPalette = FALSE );
    BYTE * GetBitmapCPC( void ) { return( BitmapCPC ); }
    void SetMode( int m ) { Mode = m; }
    void InitPalette( BYTE Pal[ 16 ], BOOL SetPal );
    BYTE * GetPalette( void ) { return( Palette ); }
    int GetMode( void ) { return( Mode ); }
    void SetNbCol( int n ) { NbCol = n; }
    void SetNbLignes( int n ) { NbLignes = n; }
    int GetNbCol( void ) { return( NbCol ); }
    int GetNbLignes( void ) { return( NbLignes ); }
    void DecodePalette( BYTE * Palette );
    void SetWin( BOOL i );

private :
    void DepactOCP( void );
    void LisseBitmap( StRVB * Bitmap );

    BYTE BitmapCPC[ 0x8000 ];
    BYTE Palette[ 16 ];
    int Mode, NbCol, NbLignes, LgDcmp;
    BOOL IsWin;
};


StRVB GetPalCPC( int Coul );

int GetRgbCPC( int Coul );


#endif
