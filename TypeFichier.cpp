// TypeFichier.cpp : implementation file
//
#include  <afxwin.h>

#include  "Outils.h"
#include  "Locale.h"
#include  "GestDsk.h"
#include  "Resource.h"
#include  "TypeFichier.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeFichier dialog


CTypeFichier::CTypeFichier( int t
                          , StAmsdos * e
                          , BOOL i
                          , char * n
                          , CWnd* pParent /*=NULL*/
                          )
    : CDialog(CTypeFichier::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTypeFichier)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    TypeModeImport = t;
    Entete = e;
    Nom = n;
    IsAmsdos = i;
    LockModif = FALSE;
}


void CTypeFichier::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTypeFichier)
    DDX_Control(pDX, IDC_ISAMSDOS, m_IsAmsdos);
    DDX_Control(pDX, IDC_STATIC_TYPEFIC, m_TypeFic);
    DDX_Control(pDX, IDC_STATIC_INFOFIC, m_InfoFic);
    DDX_Control(pDX, IDC_TYPE_BIN, m_TypeBin);
    DDX_Control(pDX, IDOK, m_BpValide);
    DDX_Control(pDX, IDC_TYPE_ASCII, m_TypeAscii);
    DDX_Control(pDX, IDC_TXT_POINTENTREE, m_TxtPointEntree);
    DDX_Control(pDX, IDC_TXT_LONGUEUR, m_TxtLongueur);
    DDX_Control(pDX, IDC_TXT_ADRDEB, m_TxtAdrDeb);
    DDX_Control(pDX, IDC_ADRESSE_DEB, m_AdresseDeb);
    DDX_Control(pDX, IDC_ADRESSE_DEB2, m_AdresseDeb2);
    DDX_Control(pDX, IDC_LONGUEUR, m_Longueur);
    DDX_Control(pDX, IDC_LONGUEUR2, m_Longueur2);
    DDX_Control(pDX, IDC_POINT_ENTREE, m_PointEntree);
    DDX_Control(pDX, IDC_POINT_ENTREE2, m_PointEntree2);
    DDX_Control(pDX, IDC_USER, m_User);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTypeFichier, CDialog)
    //{{AFX_MSG_MAP(CTypeFichier)
    ON_BN_CLICKED(IDC_TYPE_ASCII, OnTypeAscii)
    ON_BN_CLICKED(IDC_TYPE_BIN, OnTypeBin)
    ON_EN_CHANGE(IDC_ADRESSE_DEB, OnChangeAdresseDeb)
    ON_EN_CHANGE(IDC_ADRESSE_DEB2, OnChangeAdresseDeb2)
    ON_EN_CHANGE(IDC_POINT_ENTREE, OnChangePointEntree)
    ON_EN_CHANGE(IDC_POINT_ENTREE2, OnChangePointEntree2)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeFichier message handlers


//
// Initialisation de l'interface
//
BOOL CTypeFichier::OnInitDialog() 
{
    CDialog::OnInitDialog();

    int Longueur = Entete->RealLength + ( Entete->BigLength << 16 );
    wsprintf( Tmp, GetTexteLoc( 36 ), Nom );
    SetWindowText( Tmp );
    wsprintf( Tmp, "0x%04X", Longueur );
    m_Longueur.SetWindowText( Tmp );
    m_Longueur2.SetWindowText( itoa( Longueur, Tmp, 10 ) );
    wsprintf( Tmp, "0x%04X", Entete->Adress );
    m_AdresseDeb.SetWindowText( Tmp );
    wsprintf( Tmp, "0x%04X", Entete->EntryAdress );
    m_PointEntree.SetWindowText( Tmp );
    wsprintf( Tmp, "%d", Entete->UserNumber );
    m_User.SetWindowText( Tmp );
    if ( TypeModeImport == MODE_USER )
        {
        //
        // Mode utilisateur
        //
        if ( IsAmsdos )
            m_TypeBin.SetCheck( 1 );
        else
            m_TypeAscii.SetCheck( 1 );
        }
    else
        {
        if ( TypeModeImport == MODE_BINAIRE )
            //
            // Modde binaire
            //
            m_TypeBin.SetCheck( 1 );
        else
            {
            //
            // Mode ascii
            //
            m_TypeAscii.SetCheck( 1 );
            OnTypeAscii();
            }
        m_TypeBin.EnableWindow( FALSE );
        m_TypeAscii.EnableWindow( FALSE );
        }
    if ( m_TypeAscii.GetCheck() )
        OnTypeAscii();

    m_IsAmsdos.SetWindowText( GetTexteLoc( 37 + IsAmsdos ) );
    m_TxtAdrDeb.SetWindowText( GetTexteLoc( 32 ) );
    m_TxtLongueur.SetWindowText( GetTexteLoc( 33 ) );
    m_TxtPointEntree.SetWindowText( GetTexteLoc( 34 ) );
    m_BpValide.SetWindowText( GetTexteLoc( 35 ) );
    m_InfoFic.SetWindowText( GetTexteLoc( 28 ) );
    m_TypeFic.SetWindowText( GetTexteLoc( 39 ) );
    m_TypeBin.SetWindowText( GetTexteLoc( 40 ) );
    m_TypeAscii.SetWindowText( GetTexteLoc( 41 ) );
    return( TRUE );
}


//
// Choix du mode ascii
//
void CTypeFichier::OnTypeAscii()
{
    TypeModeImport = MODE_ASCII;
    m_PointEntree.EnableWindow( FALSE );
    m_AdresseDeb.EnableWindow( FALSE );
    m_Longueur.EnableWindow( FALSE );
    m_PointEntree2.EnableWindow( FALSE );
    m_AdresseDeb2.EnableWindow( FALSE );
    m_Longueur2.EnableWindow( FALSE );
}


//
// Choix du mode binaire
//
void CTypeFichier::OnTypeBin()
{
    TypeModeImport = MODE_BINAIRE;
    m_PointEntree.EnableWindow( TRUE );
    m_AdresseDeb.EnableWindow( TRUE );
    m_Longueur.EnableWindow( TRUE );
    m_PointEntree2.EnableWindow( TRUE );
    m_AdresseDeb2.EnableWindow( TRUE );
    m_Longueur2.EnableWindow( TRUE );
}


//
// Valide les modifications
//
void CTypeFichier::OnOK()
{
    if ( TypeModeImport >= MODE_BINAIRE )
        {
        m_PointEntree.GetWindowText( Tmp, sizeof( Tmp ) );
        Entete->EntryAdress = ( USHORT )HexToDec( Tmp );
        m_AdresseDeb.GetWindowText( Tmp, sizeof( Tmp ) );
        Entete->Adress = ( USHORT )HexToDec( Tmp );
        m_User.GetWindowText( Tmp, sizeof( Tmp ) );
        Entete->UserNumber = ( BYTE )atoi( Tmp );
        SetChecksum( Entete );
        }
    CDialog::OnOK();
}


//
// Changement adresse début (hexadécimal)
//
void CTypeFichier::OnChangeAdresseDeb()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_AdresseDeb.GetWindowText( Tmp, sizeof( Tmp ) );
        int i = HexToDec( Tmp );
        m_AdresseDeb2.SetWindowText( itoa( i, Tmp, 10 ) );
        LockModif = FALSE;
        }
}


//
// Changement adresse début (décimal)
//
void CTypeFichier::OnChangeAdresseDeb2()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_AdresseDeb2.GetWindowText( Tmp, sizeof( Tmp ) );
        int i = atoi( Tmp );
        wsprintf( Tmp, "0x%04X", i );
        m_AdresseDeb.SetWindowText( Tmp );
        LockModif = FALSE;
        }
}


//
// Changement point d'entrée (hexadécimal)
//
void CTypeFichier::OnChangePointEntree()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_PointEntree.GetWindowText( Tmp, sizeof( Tmp ) );
        int i = HexToDec( Tmp );
        m_PointEntree2.SetWindowText( itoa( i, Tmp, 10 ) );
        LockModif = FALSE;
        }
}


//
// Changement point d'entrée (décimal)
//
void CTypeFichier::OnChangePointEntree2()
{
    if ( ! LockModif )
        {
        LockModif = TRUE;
        m_PointEntree2.GetWindowText( Tmp, sizeof( Tmp ) );
        int i = atoi( Tmp );
        wsprintf( Tmp, "0x%04X", i );
        m_PointEntree.SetWindowText( Tmp );
        LockModif = FALSE;
        }
}
