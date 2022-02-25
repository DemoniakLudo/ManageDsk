// ManageDsk.cpp : Defines the class behaviors for the application.
//

#include  <afxwin.h>
#include  <afxcmn.h>
#include  <afxext.h>

#include  "Locale.h"
#include  "Outils.h"
#include  "GestDsk.h"
#include  "Resource.h"
#include  "ManageDsk.h"
#include  "ManageDskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// Tableau de d�cryptage d'un programme en basic prot�g�
//
static BYTE AMSDOS_KEY[128] =
{
0xAB, 0x2C, 0xED, 0xEA, 0x6C, 0x37, 0x3F, 0xEC,
0x9B, 0xDF, 0x7A, 0x0C, 0x3B, 0xD4, 0x6D, 0xF5,
0x04, 0x44, 0x03, 0x11, 0xDF, 0x59, 0x8F, 0x21,
0x73, 0x7A, 0xCC, 0x83, 0xDD, 0x30, 0x6A, 0x30,
0xD3, 0x8F, 0x02, 0xF0, 0x60, 0x6B, 0x94, 0xE4,
0xB7, 0xF3, 0x03, 0xA8, 0x60, 0x88, 0xF0, 0x43,
0xE8, 0x8E, 0x43, 0xA0, 0xCA, 0x84, 0x31, 0x53,
0xF3, 0x1F, 0xC9, 0xE8, 0xAD, 0xC0, 0xBA, 0x6D,
0x93, 0x08, 0xD4, 0x6A, 0x2C, 0xB2, 0x07, 0x27,
0xC0, 0x99, 0xEE, 0x89, 0xAF, 0xC3, 0x53, 0xAB,
0x2B, 0x34, 0x5C, 0x2F, 0x13, 0xEE, 0xAA, 0x2C,
0xD9, 0xF4, 0xBC, 0x12, 0xB3, 0xC5, 0x1C, 0x68,
0x01, 0x20, 0x2C, 0xFA, 0x77, 0xA6, 0xB5, 0xA4,
0xFC, 0x9B, 0xF1, 0x32, 0x5B, 0xC3, 0x70, 0x77,
0x85, 0x36, 0xBE, 0x5B, 0x8C, 0xC8, 0xB5, 0xC2,
0xF0, 0x0B, 0x98, 0x0F, 0x36, 0x9D, 0xD8, 0x96
};


/********************************************************* !NAME! **************
* Nom : Retour
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Variables Globales
********************************************************** !1! *****************
*
* Fichier     : !./FPTH\/FLE!, ligne : !./LN!
*
* Description : Chaine de caract�res utilis�e pour stocker les arguments
*
********************************************************** !0! ****************/
static char Retour[ 256 ];


/////////////////////////////////////////////////////////////////////////////
// CManageDskApp

BEGIN_MESSAGE_MAP(CManageDskApp, CWinApp)
    //{{AFX_MSG_MAP(CManageDskApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageDskApp construction

CManageDskApp::CManageDskApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CManageDskApp object

CManageDskApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CManageDskApp initialization

BOOL CManageDskApp::InitInstance()
{
    // Standard initialization
    if ( ! m_lpCmdLine[ 0 ] )
        {
        CManageDskDlg dlg;
        m_pMainWnd = &dlg;
        dlg.DoModal();
        }
    else
        DecomposeArg( m_lpCmdLine );

    return( TRUE );
}


/********************************************************* !NAME! **************
* Nom : CManageDskApp::GetArg
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Fonctions
********************************************************** !1! *****************
*
* Description : Recherche le s�parateur d'arguments (-)
*
* R�sultat    : La position du premier argument trouv�
*
* Variables globales modifi�es : /
*
********************************************************** !0! ****************/
char * CManageDskApp::GetArg( char * argc )
{
    while( * argc && * argc != '-' )
        argc++;

    if ( * argc == '-' )
        return( ++argc );

    return( NULL );
}


/********************************************************* !NAME! **************
* Nom : CManageDskApp::Argument
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Fonctions
********************************************************** !1! *****************
*
* Description : Isole un argument de la ligne de commande
*
* R�sultat    : Le premier argument trouv�
*
* Variables globales modifi�es : /
*
********************************************************** !0! ****************/
char * CManageDskApp::Argument( char * argc )
{

    char * p = Retour;

    while( * argc != ' ' )
        {
        if ( * argc == '"')
            {
            * p++ = * argc++;
            while( * argc != '"' )
                * p++ = * argc++;
            }
        * p++ = * argc++;
        }
    * p = 0;

     return( Retour );
}


void CManageDskApp::AjouteFichiers( int nDSK, char * Masque )
{
    WIN32_FIND_DATA FindData;
    static BYTE Buff[ 0x20000 ];
    char Path[ 256 ];
    char FullName[ 512 ];
    DWORD Lg;

    strcpy( Path, Masque );
    if ( * Path == '"' )
        {
        Path[ strlen( Path ) - 1 ] = 0;
        strcpy( Path, Path + 1 );
        }
    // Recherche le dernier caract�re '\'
    char * p = Path;
    char * last = p;
    do
        {
        last = strchr( p, '\\' );
        if ( last )
            p = ++last;
        }
    while( last );

    if ( p > Path )
        * --p = 0;
    else
        * p = 0;

    HANDLE Hfile = FindFirstFile( Masque, &FindData );
    while( Hfile != INVALID_HANDLE_VALUE )
        {
        if ( ! ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
            {
            if ( * Path )
                wsprintf( FullName, "%s\\%s", Path, FindData.cFileName );
            else
                strcpy( FullName, FindData.cFileName );

            HANDLE fp = CreateFile( FullName
                                 , GENERIC_READ
                                 , 0
                                 , NULL
                                 , OPEN_EXISTING
                                 , 0
                                 , NULL
                                 );
            if ( fp != INVALID_HANDLE_VALUE )
                {
                ReadFile( fp, Buff, sizeof( Buff ), &Lg, NULL );
                CloseHandle( fp );
                CopieFichier( nDSK, Buff, FindData.cFileName, Lg, 256, 0 );
                }
            else
                printf( "Erreur impossible de lire le fichier %s\n"
                      , FindData.cFileName
                      );
            }
        if ( ! FindNextFile( Hfile, &FindData ) )
            break; 
        }
    FindClose( Hfile );
}



void CManageDskApp::AjouteFichierEntete( int nDSK, char * FicInfo )
{
    WIN32_FIND_DATA FindData;
    static BYTE Buff[ 0x20000 ];
    char FullName[ 512 ];
    DWORD length;
    int start, exec = 0;
    

    // D�composer les param�tres
    char * amsFile = strchr( FicInfo, '/' );
    if ( amsFile )
        {
        * amsFile++ = 0;
        char * type = strchr( amsFile, '/' );
        if ( type )
            {
            * type++ = 0;
            strcpy( FullName, FicInfo );
            if ( * FullName == '"' )
                {
                FullName[ strlen( FullName ) - 1 ] = 0;
                strcpy( FullName, FullName + 1 );
                }
            HANDLE fp = CreateFile( FullName
                                 , GENERIC_READ
                                 , 0
                                 , NULL
                                 , OPEN_EXISTING
                                 , 0
                                 , NULL
                                 );
            if ( fp != INVALID_HANDLE_VALUE )
                {
                ReadFile( fp
                        , &Buff[ sizeof( StAmsdos ) ]
                        , sizeof( Buff ) - sizeof( StAmsdos )
                        , &length
                        , NULL
                        );
                CloseHandle( fp );

                char * NomAmsdos = GetNomAmsdos( ( BYTE * )amsFile );
                int t = 0;
                if (!(strncmp(type, "BIN", 3)))
                    t = 2;

                if ( strncmp( type, "ASC", 3 ) )
                    // Not ASCII
                    {
                    char * p = strchr( type, '/' );
                    if (p)
                    {

                        *p = 0;
                        if (strncmp(p+1, "P", 1)==0)
                        {
                            ++t; // Add protection bit

                            for (int t = 0; t < length; ++t)
                            {
                                Buff[sizeof(StAmsdos) + t] ^= AMSDOS_KEY[t % sizeof(AMSDOS_KEY)];
                            }

                            p = strchr(++p, '/');

                        }

                        if (p)
                        {
                            start = atoi(++p);
                            p = strchr(p, '/');
                            if (p)
                                exec = atoi(++p);
                        }

                        StAmsdos * Entete =  CreeEnteteComplet( NomAmsdos
                                                              , t
                                                              , start
                                                              , length
                                                              , exec
                                                              );
                        memcpy( Buff, Entete, sizeof( StAmsdos ) );
                        length += sizeof( StAmsdos );
                        }
                    }
                else
                    memcpy( Buff, Buff + sizeof( StAmsdos ), length );

                CopieFichier( nDSK
                            , Buff
                            , NomAmsdos
                            , length + sizeof( StAmsdos )
                            , 256
                            , 0
                            );
                }
            else
                printf( "Erreur impossible de lire le fichier %s\n"
                      , FindData.cFileName
                      );
            }
        }
}


/********************************************************* !NAME! **************
* Nom : CManageDskApp::DecomposeArg
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Fonctions
********************************************************** !1! *****************
*
* Description : D�compose les arguments pass�s en ligne de commande
*
* R�sultat    : /
*
* Variables globales modifi�es : /
*
********************************************************** !0! ****************/
void CManageDskApp::DecomposeArg( char * argc )
{
    char * p;
    BOOL IsDskLoc = FALSE;
    int nDSK = 0;

    do
        {
        argc = GetArg( argc );
        if ( argc )
            {
            switch( toupper( * argc++ ) )
                {
                case 'C' :  // Cr�er dsk
                    FormatDsk( nDSK, 9, 42 );
                    IsDskLoc = TRUE;
                    printf( "Cr�ation dsk ok.\n" );
                    break;

                case 'L' :  // Lire Dsk
                    p = Argument( argc );
                    if ( ReadDsk( nDSK, p ) )
                        {
                        printf( "Lecture fichier %s ok.\n", p );
                        IsDskLoc = TRUE;
                        }
                    else
                        {
                        printf( "Erreur lecture fichier %s\n", p );
                        IsDskLoc = FALSE;
                        }
                    break;

                case 'A' :  // Ajouter fichiers sur dsk
                    p = Argument( argc );
                    if ( IsDskLoc )
                        AjouteFichiers( nDSK, p );
                    else
                        printf( "Erreur pas de dsk charg�.\n" );
                    break;

                case 'I' : // Ajout d'un fichier avec en-t�te sur dsk
                    p = Argument( argc );
                    if ( IsDskLoc )
                        AjouteFichierEntete( nDSK, p );
                    else
                        printf( "Erreur pas de dsk charg�.\n" );
                    break;
                    
                case 'S' :  // Sauver dsk
                    p = Argument( argc );
                    if ( IsDskLoc )
                        {
                        if ( WriteDsk( nDSK, p ) )
                            printf( "Sauvegarde fichier %s ok.\n", p );
                        else
                            printf( "Erreur �criture fichier %s\n", p );
                        }
                    else
                        printf( "Erreur pas de dsk charg�.\n" );
                    break;

                case 'P' : // Affiche le contenu du dsk
                    {
                    for ( int i = 0; i < 64; i++ )
                        {
                        StDirEntry * t = GetInfoDirEntry( nDSK, i );
                        char * Nom = GetNomAmsdos( t->Nom );
                        BOOL NomValide = FALSE;
                        for ( int j = 0; j < 8; j++ )
                            if ( t->Nom[ j ] != 0xE5 )
                                NomValide = TRUE;

                        if ( NomValide )
                            printf( "User:%3d\t%s\n", t->User, Nom );
                        }
                    }
                default :
                    break;
                }
            }
        }
    while( argc );
}
