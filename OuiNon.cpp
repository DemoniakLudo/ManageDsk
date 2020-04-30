// OuiNon.cpp : implementation file
//

#include  <afxwin.h>

#include  "Resource.h"
#include  "Locale.h"
#include  "OuiNon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COuiNon dialog


COuiNon::COuiNon( char * n, char * m, BOOL t, CWnd * pParent /*=NULL*/)
    : CDialog(COuiNon::IDD, pParent)
{
    //{{AFX_DATA_INIT(COuiNon)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    NomFic = n;
    Msg = m;
    Tout = t;
    Ret = RET_NON;
    Kill = FALSE;
}


void COuiNon::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COuiNon)
    DDX_Control(pDX, IDC_OUITOUT, m_BpOuiTout);
    DDX_Control(pDX, IDC_OUI, m_BpOui);
    DDX_Control(pDX, IDC_NON, m_BpNon);
    DDX_Control(pDX, IDC_KILL, m_Kill);
    DDX_Control(pDX, IDC_MSG_CONFIRMATION, m_MsgConfirm);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COuiNon, CDialog)
    //{{AFX_MSG_MAP(COuiNon)
    ON_BN_CLICKED(IDC_NON, OnNon)
    ON_BN_CLICKED(IDC_OUI, OnOui)
    ON_BN_CLICKED(IDC_OUITOUT, OnOuitout)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COuiNon message handlers


void COuiNon::OnNon()
{
    Ret = RET_NON;
    CDialog::OnOK();
}


void COuiNon::OnOui()
{
    Ret = RET_OUI;
    Kill = m_Kill.GetCheck();
    CDialog::OnOK();
}


void COuiNon::OnOuitout()
{
    Ret = RET_OUITOUT;
    Kill = m_Kill.GetCheck();
    CDialog::OnOK();
}

BOOL COuiNon::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetWindowText( NomFic );
    m_MsgConfirm.SetWindowText( Msg );
    m_BpOui.SetWindowText( GetTexteLoc( 101 ) );
    m_BpOuiTout.SetWindowText( GetTexteLoc( 102 ) );
    m_BpNon.SetWindowText( GetTexteLoc( 103 ) );
    m_Kill.SetWindowTextA( GetTexteLoc( 128 ) );
    m_BpOuiTout.EnableWindow( Tout );
    return( TRUE );
}
