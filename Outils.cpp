#include  <afxwin.h>

#include  "Outils.h"
#include  "Locale.h"

//
// Initialise une chaine au format hexadécimal en fonction de la valeur d'entrée
//
void Hex( char Chaine[], int Valeur, int Digit )
{
    static char TabDigit[ 17 ] = "0123456789ABCDEF";

    while( Digit )
        * Chaine++ = TabDigit[ ( Valeur >> ( 4 * ( --Digit ) ) ) & 0x0F ];
}


//
// Conversion hexa->décimal
//
int HexToDec( char * Valeur )
{
    char * p = strchr( Valeur, 'X' );
    if ( p )
        Valeur = ++p;

    p = strchr( Valeur, 'x' );
    if ( p )
        Valeur = ++p;

    p = strchr( Valeur, '#' );
    if ( p )
        Valeur = ++p;

    p = strchr( Valeur, '$' );
    if ( p )
        Valeur = ++p;

    p = strchr( Valeur, '&' );
    if ( p )
        Valeur = ++p;

    int Ret = 0, i = 0;
    while( Valeur[ i ] )
        {
        Ret <<= 4;
        char c = Valeur[ i++ ];
        if ( c >= '0' && c <= '9' )
            Ret += c - '0';
        else
            Ret += ( c & 0x5F ) - 0x37;
        }
    return( Ret );
}


//
// Conversion d'un secteur (512 octets) en affichage Hexa et ASCII
//
void SetBuffViewHexa( BYTE * src
                    , char * Hex
                    , char * Ascii
                    , USHORT Offset
                    , BOOL AddOffset
                    , int Longueur
                    )
{
    static char * CodeHexa = "0123456789ABCDEF";
    int i, q = 0;

    //
    // Parcourir les X octets de la source et remplir les buffers
    //
    for ( i = 0; i < Longueur; i++ )
        {
        BYTE b = * src++;
        if ( b > 32 && b < 128 )
            Ascii[ i ] = b;
        else
            Ascii[ i ] = '.';

        if ( AddOffset && ( ! ( i & 0x0F ) ) )
            {
            Hex[ q++ ] = '#';
            Hex[ q++ ] = CodeHexa[ Offset >> 12 ];
            Hex[ q++ ] = CodeHexa[ ( Offset >> 8 ) & 0x0F ];
            Hex[ q++ ] = CodeHexa[ ( Offset >> 4 ) & 0x0F ];
            Hex[ q++ ] = CodeHexa[ Offset & 0x0F ];
            Hex[ q++ ] = ':';
            }
        Offset++;
        Hex[ q++ ] = CodeHexa[ b >> 4 ];
        Hex[ q++ ] = CodeHexa[ b & 0x0F ];
        Hex[ q++ ] = ' ';
        }
    Hex[ q ] = 0;
    Ascii[ i ] = 0;
}


//
// Retourne le numéro d'user sous forme de chaine
//
char * GetUser( int u )
{
    static char User[ 8 ];

    return( itoa( u, User, 10 ) );
}


//
// Retourne le type de fichier sous forme de chaine
//
char * GetType( StAmsdos * Ams )
{
    if ( CheckAmsdos( ( BYTE * )Ams ) )
        {
        switch( Ams->FileType )
            {
            case 0 :    // BASIC
                return( GetTexteLoc( 22 ) );

            case 1 :    // BASIC (P)
                return( GetTexteLoc( 23 ) );

            case 2 :    // BINAIRE
                return( GetTexteLoc( 24 ) );

            case 3 :    // BINAIRE (P)
                return( GetTexteLoc( 25 ) );

            default :
                return( GetTexteLoc( 26 ) );
            }
        }
    return( GetTexteLoc( 27 ) );
}


//
// Retourne la taille du fichier sous forme de chaine
//
char * GetTaille( int t )
{
    static char Taille[ 16 ];

    wsprintf( Taille, "%d Ko", t );
    return( Taille );
}


//
// Retourne le nom du fichier formaté amsdos (8+3)
//
char * GetNomAmsdos( BYTE * AmsName )
{
    static char NomAmsdos[ 16 ];
    int i;
    char * p = NomAmsdos;
    for ( i = 0; i < 8; i++ )
        if ( * AmsName > ' ' && * AmsName != '.' )
            * p++ = * AmsName++;

    while( * AmsName > ' ' && * AmsName != '.' )
        AmsName++;

    if ( * AmsName == '.' )
        AmsName++;

    * p = 0;
    strcat( NomAmsdos, "." );

    for ( i = 0; * AmsName && i < 3; i++ )
        *++p = * AmsName++;

    * ++p = 0;
    i = 0;
    while( NomAmsdos[ i ] )
        NomAmsdos[ i++ ] &= 0x7F;

    return( NomAmsdos );
}
