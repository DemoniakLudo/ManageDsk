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


/********************************************************* !NAME! **************
* Nom : Retour
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Variables Globales
********************************************************** !1! *****************
*
* Fichier     : !./FPTH\/FLE!, ligne : !./LN!
*
* Description : Chaine de caractères utilisée pour stocker les arguments
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
* Description : Recherche le séparateur d'arguments (-)
*
* Résultat    : La position du premier argument trouvé
*
* Variables globales modifiées : /
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
* Résultat    : Le premier argument trouvé
*
* Variables globales modifiées : /
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
    // Recherche le dernier caractère '\'
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
    

    // Décomposer les paramètres
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
                if ( ! ( strncmp( type, "BIN", 3 ) ) )
                    t = 2;

                if ( strncmp( type, "ASC", 3 ) )
                    {
                    char * p = strchr( type, '/' );
                    if ( p )
                        {
                        *p++ = 0; // Isoler le type
                        start = atoi( p );
                        p = strchr( p, '/' );
                        if ( p )
                            exec = atoi( ++p );

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
* Description : Décompose les arguments passés en ligne de commande
*
* Résultat    : /
*
* Variables globales modifiées : /
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
                case 'C' :  // Créer dsk
                    FormatDsk( nDSK, 9, 42 );
                    IsDskLoc = TRUE;
                    printf( "Création dsk ok.\n" );
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
                        printf( "Erreur pas de dsk chargé.\n" );
                    break;

                case 'I' : // Ajout d'un fichier avec en-tête sur dsk
                    p = Argument( argc );
                    if ( IsDskLoc )
                        AjouteFichierEntete( nDSK, p );
                    else
                        printf( "Erreur pas de dsk chargé.\n" );
                    break;
                    
                case 'S' :  // Sauver dsk
                    p = Argument( argc );
                    if ( IsDskLoc )
                        {
                        if ( WriteDsk( nDSK, p ) )
                            printf( "Sauvegarde fichier %s ok.\n", p );
                        else
                            printf( "Erreur écriture fichier %s\n", p );
                        }
                    else
                        printf( "Erreur pas de dsk chargé.\n" );
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
