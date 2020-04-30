// ListDir.cpp : implementation file
//
#include  <afxwin.h>
#include  <afxcmn.h>

#include  "Resource.h"
#include  "GestDsk.h"
#include  "ListDir.h"
#include  "Locale.h"
#include  "Outils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CListDir dialog


enum { COL_USER, COL_NOM, COL_NUMPAGE, COL_NBPAGES, COL_BLOCKS };



CListDir::CListDir( StDirEntry * t, CWnd * pParent /*=NULL*/ )
    : CDialog( CListDir::IDD, pParent )
{
    //{{AFX_DATA_INIT(CListDir)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    TabDir = t;
}


void CListDir::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CListDir)
    DDX_Control(pDX, IDOK, m_BpOk);
    DDX_Control(pDX, IDC_HEXA, m_BpHexa);
    DDX_Control(pDX, IDC_LISTE, m_Liste);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListDir, CDialog)
    //{{AFX_MSG_MAP(CListDir)
    ON_BN_CLICKED(IDC_HEXA, OnHexa)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDir message handlers


BOOL CListDir::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Liste.InsertColumn( COL_USER, GetTexteLoc( 11 ), LVCFMT_LEFT, 40 );
    m_Liste.InsertColumn( COL_NOM, GetTexteLoc( 10 ), LVCFMT_LEFT, 110 );
    m_Liste.InsertColumn( COL_NUMPAGE, GetTexteLoc( 113 ), LVCFMT_LEFT, 40 );
    m_Liste.InsertColumn( COL_NBPAGES, GetTexteLoc( 13 ), LVCFMT_LEFT, 40 );
    m_Liste.InsertColumn( COL_BLOCKS, GetTexteLoc( 114 ), LVCFMT_LEFT, 380 );
    m_BpOk.SetWindowText( GetTexteLoc( 94 ) );
    m_BpHexa.SetWindowText( GetTexteLoc( 115 ) );
    Affiche();
    return( TRUE );  // return TRUE unless you set the focus to a control
}


void CListDir::Affiche( void )
{
    m_Liste.DeleteAllItems();
    int NbItems = 0;
    for ( int i = 0; i < 64; i++ )
        {
        char ChaineTemp[ 128 ];
        char * Fmt = m_BpHexa.GetCheck() ? "#%02X" : "%d";
        char * Nom = GetNomAmsdos( ( BYTE * )TabDir[ i ].Nom );

        BOOL NomValide = FALSE;
        for ( int j = 0; j < 8; j++ )
            if ( TabDir[ i ].Nom[ j ] != 0xE5 )
                NomValide = TRUE;

        if ( NomValide )
            {
            sprintf( ChaineTemp, Fmt, TabDir[ i ].User );
            m_Liste.InsertItem( NbItems, ChaineTemp );
            m_Liste.SetItemText( NbItems, COL_NOM, Nom );

            sprintf( ChaineTemp, Fmt, TabDir[ i ].NumPage );
            m_Liste.SetItemText( NbItems, COL_NUMPAGE, ChaineTemp );

            int NbPages = TabDir[ i ].NbPages;
            sprintf( ChaineTemp, Fmt, NbPages );
            m_Liste.SetItemText( NbItems, COL_NBPAGES, ChaineTemp );

            NbPages = min( 16, ( NbPages + 7 ) >> 3 );
            * ChaineTemp = 0;
            for ( int j = 0; j < NbPages; j++ )
                {
                char Tmp[ 10 ];
                sprintf( Tmp, Fmt, TabDir[ i ].Blocks[ j ] );
                strcat( ChaineTemp, Tmp );
                if ( j < NbPages - 1 )
                    strcat( ChaineTemp, "," );
                }
            m_Liste.SetItemText( NbItems++, COL_BLOCKS, ChaineTemp );
            }
        }
}


void CListDir::OnHexa()
{
    Affiche();
}


void CListDir::OnOK()
{
    CDialog::OnOK();
}

