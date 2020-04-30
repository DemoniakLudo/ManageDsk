// InputTracks.cpp : implementation file
//

#include  <afxwin.h>

#include  "Managedsk.h"
#include  "Locale.h"
#include  "InputTracks.h"
#include  "GestDsk.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputTracks dialog


CInputTracks::CInputTracks(CWnd* pParent /*=NULL*/)
	: CDialog(CInputTracks::IDD, pParent)
{
	Valide = FALSE;
	//{{AFX_DATA_INIT(CInputTracks)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInputTracks::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputTracks)
	DDX_Control(pDX, IDC_NB_TRACKS, m_TxtNbTracks);
	DDX_Control(pDX, IDC_NB_SECTS, m_TxtNbSects);
	DDX_Control(pDX, IDOK, m_BpOk);
	DDX_Control(pDX, IDCANCEL, m_BpCancel);
	DDX_Control(pDX, IDC_NBTRACKS, m_NbTracks);
	DDX_Control(pDX, IDC_NBSECTS, m_NbSects);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputTracks, CDialog)
	//{{AFX_MSG_MAP(CInputTracks)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, CInputTracks::OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputTracks message handlers


BOOL CInputTracks::OnInitDialog()
{
	char Tmp[16];

	NbTracks = MAX_PISTE_STD;
	NbSects = 9;
	Valide = FALSE;
	CDialog::OnInitDialog();
	SetWindowText(GetTexteLoc(122));
	m_BpOk.SetWindowText(GetTexteLoc(35));
	m_BpCancel.SetWindowText(GetTexteLoc(94));
	m_TxtNbTracks.SetWindowText(GetTexteLoc(123));
	m_TxtNbSects.SetWindowText(GetTexteLoc(129));
	itoa(NbTracks, Tmp, 10);
	m_NbTracks.SetWindowText(Tmp);
	itoa(NbSects, Tmp, 10);
	m_NbSects.SetWindowText(Tmp);
	return(TRUE);
}


void CInputTracks::OnOK()
{
	char Tmp[16];

	Valide = TRUE;
	m_NbTracks.GetWindowText(Tmp, sizeof(Tmp));
	NbTracks = min(MAX_TRACKS, max(atoi(Tmp), 0));
	m_NbSects.GetWindowText(Tmp, sizeof(Tmp));
	NbSects = min(MAX_SECTS, max(atoi(Tmp), 0));
	CDialog::OnOK();
}


