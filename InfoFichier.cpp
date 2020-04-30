// InfoFichier.cpp : implementation file
//

#include  <afxwin.h>
#include  <afxdlgs.h>

#include  "Outils.h"
#include  "Locale.h"
#include  "GestDsk.h"
#include  "ManageDsk.h"
#include  "TypeFichier.h"
#include  "InfoFichier.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoFichier dialog


CInfoFichier::CInfoFichier( int n
                          , StDirEntry * t
                          , int i
                          , BOOL r
                          , CWnd * pParent /*=NULL*/)
    : CDialog(CInfoFichier::IDD, pParent)
{
    //{{AFX_DATA_INIT(CInfoFichier)
    //}}AFX_DATA_INIT
    nDSK = n;
    TabDir = t;
    Indice = i;
    Renomme = r;
    memcpy( ( BYTE * )&Ams
          , ReadBloc( nDSK, TabDir[ Indice ].Blocks[ 0 ] )
          , sizeof( StAmsdos )
          );
    LockModif = Valide = FALSE;
}


void CInfoFichier::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CInfoFichier)
    DDX_Control(pDX, IDC_TYPE, m_Type);
    DDX_Control(pDX, IDC_READOLNY, m_ReadOnly);
    DDX_Control(pDX, IDC_HIDE, m_Hide);
    DDX_Control(pDX, IDC_VALIDER, m_BpValider);
    DDX_Control(pDX, IDC_TXT_USER, m_TxtUser);
    DDX_Control(pDX, IDC_TXT_TYPE, m_TxtType);
    DDX_Control(pDX, IDC_TXT_POINTENTREE, m_TxtPointEntree);
    DDX_Control(pDX, IDC_TXT_NOMFIC, m_TxtNomFic);
    DDX_Control(pDX, IDC_TXT_LONGUEUR, m_TxtLongueur);
    DDX_Control(pDX, IDC_TXT_ADRDEB, m_TxtAdrDeb);
    DDX_Control(pDX, IDC_USER, m_User);
    DDX_Control(pDX, IDC_POINT_ENTREE, m_PointEntree);
    DDX_Control(pDX, IDC_NOM_FICHIER, m_NomFic);
    DDX_Control(pDX, IDC_LONGUEUR, m_Longueur);
    DDX_Control(pDX, IDC_ADRESSE_DEB, m_AdresseDeb);
    DDX_Control(pDX, IDC_ADRESSE_DEB2, m_AdresseDeb2);
    DDX_Control(pDX, IDC_LONGUEUR2, m_Longueur2);
    DDX_Control(pDX, IDC_POINT_ENTREE2, m_PointEntree2);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoFichier, CDialog)
    //{{AFX_MSG_MAP(CInfoFichier)
    ON_BN_CLICKED(IDC_VALIDER, OnValider)
    ON_EN_CHANGE(IDC_ADRESSE_DEB, OnChangeAdresseDeb)
    ON_EN_CHANGE(IDC_ADRESSE_DEB2, OnChangeAdresseDeb2)
    ON_EN_CHANGE(IDC_POINT_ENTREE, OnChangePointEntree)
    ON_EN_CHANGE(IDC_POINT_ENTREE2, OnChangePointEntree2)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoFichier message handlers


//
// Initlalisation de l'interface
//
BOOL CInfoFichier::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_Type.SetWindowText( GetType( &Ams ) );
    m_Type.AddString( GetTexteLoc( 22 ) );
    m_Type.AddString( GetTexteLoc( 23 ) );
    m_Type.AddString( GetTexteLoc( 24 ) );
    m_Type.AddString( GetTexteLoc( 25 ) );
    m_Type.AddString( GetTexteLoc( 26 ) );
    if ( Ams.FileType <= 3 )
        m_Type.SetCurSel( Ams.FileType );
    else
        m_Type.SetCurSel( 4 );

    m_NomFic.SetWindowText( GetNomAmsdos( TabDir[ Indice ].Nom ) );
    m_User.SetWindowText( ::GetUser( TabDir[ Indice ].User ) );
    if ( Renomme )
        {
        //
        // Interface en mode "RENOMMER"
        //
        m_NomFic.SetReadOnly( FALSE );
//        m_User.SetReadOnly( FALSE );
        m_AdresseDeb.SetReadOnly( TRUE );
        m_PointEntree.SetReadOnly( TRUE );
        m_AdresseDeb2.SetReadOnly( TRUE );
        m_PointEntree2.SetReadOnly( TRUE );
        m_ReadOnly.EnableWindow( FALSE );
        m_Hide.EnableWindow( FALSE );
        m_Type.EnableWindow( FALSE );
        }
    else
        {
        //
        // Interface en mode "modification paramètres amsdos"
        //
        m_NomFic.SetReadOnly( TRUE );
//        m_User.SetReadOnly( TRUE );
        if ( CheckAmsdos( ( BYTE * )&Ams ) )
            {
            //
            // En-tête présente
            //
            int l = Ams.RealLength + ( Ams.BigLength << 16 );
            wsprintf( Msg, "0x%04X", Ams.Adress );
            m_AdresseDeb.SetWindowText( Msg );
            wsprintf( Msg, "0x%04X", l );
            m_Longueur.SetWindowText( Msg );
            m_Longueur2.SetWindowText( itoa( l, Msg, 10 ) );
            wsprintf( Msg, "0x%04X", Ams.EntryAdress );
            m_PointEntree.SetWindowText( Msg );
            m_ReadOnly.SetCheck( TabDir[ Indice ].Ext[ 0 ] & 0x80 );
            m_Hide.SetCheck( TabDir[ Indice ].Ext[ 1 ] & 0x80 );
            m_AdresseDeb.SetReadOnly( FALSE );
            m_PointEntree.SetReadOnly( FALSE );
            m_AdresseDeb2.SetReadOnly( FALSE );
            m_PointEntree2.SetReadOnly( FALSE );
            }
        else
            {
            //
            // Pas d'en-tête amsdos
            //
            m_AdresseDeb.SetReadOnly( TRUE );
            m_PointEntree.SetReadOnly( TRUE );
            m_AdresseDeb2.SetReadOnly( TRUE );
            m_PointEntree2.SetReadOnly( TRUE );
            }
        }
    SetWindowText( GetTexteLoc( 28 ) );
    m_TxtNomFic.SetWindowText( GetTexteLoc( 29 ) );
    m_TxtUser.SetWindowText( GetTexteLoc( 30 ) );
    m_TxtType.SetWindowText( GetTexteLoc( 31 ) );
    m_TxtAdrDeb.SetWindowText( GetTexteLoc( 32 ) );
    m_TxtLongueur.SetWindowText( GetTexteLoc( 33 ) );
    m_TxtPointEntree.SetWindowText( GetTexteLoc( 34 ) );
    m_BpValider.SetWindowText( GetTexteLoc( 35 ) );
    m_ReadOnly.SetWindowText( GetTexteLoc( 49 ) );
    m_Hide.SetWindowText( GetTexteLoc( 50 ) );
    return( TRUE );
}


//
// Changement adresse début (hexadécimal)
//
void CInfoFichier::OnChangeAdresseDeb()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_AdresseDeb.GetWindowText( Msg, sizeof( Msg ) );
        int i = HexToDec( Msg );
        m_AdresseDeb2.SetWindowText( itoa( i, Msg, 10 ) );
        LockModif = FALSE;
        }
}


//
// Changement adresse début (décimal)
//
void CInfoFichier::OnChangeAdresseDeb2()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_AdresseDeb2.GetWindowText( Msg, sizeof( Msg ) );
        int i = atoi( Msg );
        wsprintf( Msg, "0x%04X", i );
        m_AdresseDeb.SetWindowText( Msg );
        LockModif = FALSE;
        }
}


//
// Changement point d'entrée (hexadécimal)
//
void CInfoFichier::OnChangePointEntree()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_PointEntree.GetWindowText( Msg, sizeof( Msg ) );
        int i = HexToDec( Msg );
        m_PointEntree2.SetWindowText( itoa( i, Msg, 10 ) );
        LockModif = FALSE;
        }
}


//
// Changement point d'entrée (décimal)
//
void CInfoFichier::OnChangePointEntree2()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_PointEntree2.GetWindowText( Msg, sizeof( Msg ) );
        int i = atoi( Msg );
        wsprintf( Msg, "0x%04X", i );
        m_PointEntree.SetWindowText( Msg );
        LockModif = FALSE;
        }
}


//
// Valide les modifications
//
void CInfoFichier::OnValider()
{
    Valide = TRUE;
    m_User.GetWindowText( User, sizeof( User ) );
    Ams.UserNumber = ( BYTE )atoi( User );
    m_NomFic.GetWindowText( Nom, sizeof( Nom ) );
    m_PointEntree.GetWindowText( Msg, sizeof( Msg ) );
    Ams.EntryAdress = ( USHORT )HexToDec( Msg );
    m_AdresseDeb.GetWindowText( Msg, sizeof( Msg ) );
    Ams.Adress = ( USHORT )HexToDec( Msg );
    Ams.FileType = ( BYTE )m_Type.GetCurSel();
    ReadOnly = m_ReadOnly.GetCheck();
    Hidden = m_Hide.GetCheck();
    SetChecksum( &Ams );
    OnCancel();
}

