// EditSecteurs.cpp : implementation file
//
#include  <afxwin.h>

#include  "Outils.h"
#include  "Locale.h"
#include  "GestDsk.h"
#include  "Resource.h"
#include  "EditSecteurs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSecteurs dialog


CEditSecteurs::CEditSecteurs( int n, CWnd * pParent /*=NULL*/ )
    : CDialog(CEditSecteurs::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEditSecteurs)
    //}}AFX_DATA_INIT
    Track = Sect = 0;
    nDSK = n;
}


void CEditSecteurs::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEditSecteurs)
    DDX_Control(pDX, IDC_TRACK_STATIC, m_TxtTrack);
    DDX_Control(pDX, IDC_SECT_STATIC, m_TxtSect);
    DDX_Control(pDX, IDC_TRACK, m_Track);
    DDX_Control(pDX, IDC_SECT, m_Sect);
    DDX_Control(pDX, IDC_VIEW_ASCII, m_ViewAscii);
    DDX_Control(pDX, IDC_VIEW_SECT, m_ViewSect);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditSecteurs, CDialog)
    //{{AFX_MSG_MAP(CEditSecteurs)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SECT, OnDeltaposSpinSect)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TRACK, OnDeltaposSpinTrack)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//
// Affichage secteur en cours
//
void CEditSecteurs::CreeHexSect( void )
{
    static char Ascii[ 513 ];       // Buffer de représentation en ASCII
    static char HexSect[ 2048 ];    // Buffer de représentation en HEXA

    //
    // Lecture secteur depuis DSK
    //
    BYTE * p = ReadSect( nDSK, Track, Sect, FALSE );
    TrackData = GetInfoTrack( nDSK, Track );

    //
    // Affichage des buffers créés
    //
    SetBuffViewHexa( p, HexSect, Ascii );
    m_ViewSect.SetWindowText( HexSect );
    m_ViewAscii.SetWindowText( Ascii );
    wsprintf( Ascii, "%02X", TrackData->Sect[ Sect ].R );
    m_Sect.SetWindowText( Ascii );
    m_Track.SetWindowText( itoa( Track, Ascii, 10 ) );
}


/////////////////////////////////////////////////////////////////////////////
// CEditSecteurs message handlers


//
// Initialisation interface
//
BOOL CEditSecteurs::OnInitDialog()
{
    CDialog::OnInitDialog();

    font.CreatePointFont( 85, "Courier New", this->GetDC() );
    m_ViewSect.SetFont( &font );
    m_ViewAscii.SetFont( &font );
    SetWindowText( GetTexteLoc( 52 ) );
    m_TxtTrack.SetWindowText( GetTexteLoc( 53 ) );
    m_TxtSect.SetWindowText( GetTexteLoc( 54 ) );
    CreeHexSect();
    return( TRUE );
}


//
// Changement valeur curseur "secteurs"
//
void CEditSecteurs::OnDeltaposSpinSect( NMHDR * pNMHDR, LRESULT * pResult )
{
    NM_UPDOWN * pNMUpDown = ( NM_UPDOWN * )pNMHDR;

    //
    // Incrémenter secreur ?
    //
    if ( pNMUpDown->iDelta == -1 )
        {
        if ( Sect + 1 < TrackData->NbSect )
            Sect++;
        else
            if ( Track < GetNbTracks( nDSK ) )
                {
                Sect = 0;
                Track++;
                }
        CreeHexSect();
        }
    else
        //
        // Décrémenter secteur
        //
        if ( pNMUpDown->iDelta == 1 && Sect > 0 )
            {
            Sect--;
            CreeHexSect();
            }

    * pResult = 0;
}


//
// Changement valeur curseur "pistes"
//
void CEditSecteurs::OnDeltaposSpinTrack( NMHDR * pNMHDR, LRESULT * pResult )
{
    NM_UPDOWN * pNMUpDown = ( NM_UPDOWN * )pNMHDR;

    //
    // Incrémenter piste ?
    //
    if ( pNMUpDown->iDelta == -1 && Track + 1 < GetNbTracks( nDSK ) )
        {
        Track++;
        Sect = 0;
        CreeHexSect();
        }
    else
        //
        // Décrémenter piste
        //
        if ( pNMUpDown->iDelta == 1 && Track > 0 )
            {
            Track--;
            TrackData = GetInfoTrack( nDSK, Track );
            Sect = TrackData->NbSect - 1;
            CreeHexSect();
            }

    * pResult = 0;
}
