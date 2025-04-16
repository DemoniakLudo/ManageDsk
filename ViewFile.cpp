// ViewFile.cpp : implementation file
//
#include  <afxwin.h>
#include  <afxdlgs.h>
#include  <math.h>

#include  "Dams.h"
#include  "Basic.h"
#include  "Outils.h"
#include  "Desass.h"
#include  "Locale.h"
#include  "GestDsk.h"
#include  "Resource.h"
#include  "ViewFile.h"
#include  "TypeFichier.h"
#include  "ChoixCouleur.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define     MAX_LIGNES_HEX  32

#define     MAX_LIGNES_SCR  200

#define     MAX_COLS_SCR    128


static char Listing[ MAX_FILESIZE ];


/////////////////////////////////////////////////////////////////////////////
// CViewFile dialog


CViewFile::CViewFile( char * Nom
                    , BYTE * b
                    , int n
                    , int t
                    , int d
                    , int l
                    , int dp
                    , CWnd * pParent /*=NULL*/ 
                    ) : CDialog(CViewFile::IDD, pParent)
{
    //{{AFX_DATA_INIT(CViewFile)
    //}}AFX_DATA_INIT
    BufFile = b;
    TailleFic = n;
    TypeModeExport = t;
    NbLignes = ( n + 15 ) >> 4;
    CurLigne = 0;
    NbCols = 80;
    NbL = MAX_LIGNES_SCR;
    Mode = 1;
    IsBasic = FALSE;
    NomFic = Nom;
    nDSK = d;
    Langue = l;
    BYTE Pal[ 16 ] = { 1, 24, 20, 6, 26, 0, 2, 8, 10, 12, 14, 16, 18, 22, 3, 4 };
    memcpy( Palette, Pal, sizeof( Palette ) );
    Deprotect = dp;
}


void CViewFile::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CViewFile)
    DDX_Control(pDX, IDC_SAVE, m_BpSave);
    DDX_Control(pDX, IDC_MODE0, m_Mode0);
    DDX_Control(pDX, IDC_MODE1, m_Mode1);
    DDX_Control(pDX, IDC_MODE2, m_Mode2);
    DDX_Control(pDX, IDC_STATIC_NBLIGNES, m_StaticNbLignes);
    DDX_Control(pDX, IDC_STATIC_OFFSET, m_StaticOffset);
    DDX_Control(pDX, IDC_STATIC_NBCOL, m_StaticNbCol);
    DDX_Control(pDX, IDC_CLEAR, m_Clear);
    DDX_Control(pDX, IDC_CRLF, m_CrLf);
    DDX_Control(pDX, IDC_HEXA, m_Hexa);
    DDX_Control(pDX, IDC_ECR, m_Ecran);
    DDX_Control(pDX, IDC_BASIC, m_Basic);
    DDX_Control(pDX, IDC_DAMS, m_Dams);
    DDX_Control(pDX, IDC_DESASS, m_Desass);
    DDX_Control(pDX, IDC_LIST_BASIC, m_ListBasic);
    DDX_Control(pDX, IDC_VIEW_HEX, m_ViewHex);
    DDX_Control(pDX, IDC_VIEW_ASCII, m_ViewAscii);
    DDX_Control(pDX, IDC_SCROLL, m_Scroll);
    DDX_Control(pDX, IDC_SCROLLSCR, m_ScrollScr);
    DDX_Control(pDX, IDC_SPIN_COL, m_SpinCol);
    DDX_Control(pDX, IDC_SPIN_LIGNES, m_SpinLignes);
    DDX_Control(pDX, IDC_COL00, m_Col[ 0 ]);
    DDX_Control(pDX, IDC_COL01, m_Col[ 1 ]);
    DDX_Control(pDX, IDC_COL02, m_Col[ 2 ]);
    DDX_Control(pDX, IDC_COL03, m_Col[ 3 ]);
    DDX_Control(pDX, IDC_COL04, m_Col[ 4 ]);
    DDX_Control(pDX, IDC_COL05, m_Col[ 5 ]);
    DDX_Control(pDX, IDC_COL06, m_Col[ 6 ]);
    DDX_Control(pDX, IDC_COL07, m_Col[ 7 ]);
    DDX_Control(pDX, IDC_COL08, m_Col[ 8 ]);
    DDX_Control(pDX, IDC_COL09, m_Col[ 9 ]);
    DDX_Control(pDX, IDC_COL10, m_Col[ 10 ]);
    DDX_Control(pDX, IDC_COL11, m_Col[ 11 ]);
    DDX_Control(pDX, IDC_COL12, m_Col[ 12 ]);
    DDX_Control(pDX, IDC_COL13, m_Col[ 13 ]);
    DDX_Control(pDX, IDC_COL14, m_Col[ 14 ]);
    DDX_Control(pDX, IDC_COL15, m_Col[ 15 ]);
    DDX_Control(pDX, IDC_START_ASM, m_StartAsm);
    DDX_Control(pDX, IDC_BP_START_ASM, m_BpStartAsm);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewFile, CDialog)
    //{{AFX_MSG_MAP(CViewFile)
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_BASIC, OnBasic)
    ON_BN_CLICKED(IDC_ECR, OnEcr)
    ON_BN_CLICKED(IDC_HEXA, OnHexa)
    ON_BN_CLICKED(IDC_CRLF, OnCrlf)
    ON_WM_PAINT()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COL, OnDeltaposSpinCol)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LIGNES, OnDeltaposSpinLignes)
    ON_BN_CLICKED(IDC_MODE0, OnMode0)
    ON_BN_CLICKED(IDC_MODE1, OnMode1)
    ON_BN_CLICKED(IDC_MODE2, OnMode2)
    ON_BN_CLICKED(IDC_SAVE, OnSave)
    ON_BN_CLICKED(IDC_DAMS, OnDams)
    ON_BN_CLICKED(IDC_DESASS, OnDesass)
    ON_BN_CLICKED(IDC_COL00, OnCol00)
    ON_BN_CLICKED(IDC_COL01, OnCol01)
    ON_BN_CLICKED(IDC_COL02, OnCol02)
    ON_BN_CLICKED(IDC_COL03, OnCol03)
    ON_BN_CLICKED(IDC_COL04, OnCol04)
    ON_BN_CLICKED(IDC_COL05, OnCol05)
    ON_BN_CLICKED(IDC_COL06, OnCol06)
    ON_BN_CLICKED(IDC_COL07, OnCol07)
    ON_BN_CLICKED(IDC_COL08, OnCol08)
    ON_BN_CLICKED(IDC_COL09, OnCol09)
    ON_BN_CLICKED(IDC_COL10, OnCol10)
    ON_BN_CLICKED(IDC_COL11, OnCol11)
    ON_BN_CLICKED(IDC_COL12, OnCol12)
    ON_BN_CLICKED(IDC_COL13, OnCol13)
    ON_BN_CLICKED(IDC_COL14, OnCol14)
    ON_BN_CLICKED(IDC_COL15, OnCol15)
    ON_BN_CLICKED(IDC_BP_START_ASM, OnBpStartAsm)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFile message handlers


BOOL CViewFile::OnInitDialog()
{
    char Tmp[ 128 ];
    CDialog::OnInitDialog();

    WinDC = GetDC();
    //
    // Initialisation interface
    //
    wsprintf( Tmp, GetTexteLoc( 74 ), NomFic );
    SetWindowText( Tmp );
    m_Hexa.SetWindowText( GetTexteLoc( 59 ) );
    m_Basic.SetWindowText( GetTexteLoc( 60 ) );
    m_Ecran.SetWindowText( GetTexteLoc( 61 ) );
    m_Dams.SetWindowText( GetTexteLoc( 69 ) );
    m_Desass.SetWindowText( GetTexteLoc( 70 ) );
    m_BpStartAsm.SetWindowText( GetTexteLoc( 32 ) );
    font8.CreatePointFont( 80, "Courier New", WinDC );
    m_ViewHex.SetFont( &font8 );
    m_ViewAscii.SetFont( &font8 );
    font9.CreatePointFont( 90, "Courier New", WinDC );
    m_ListBasic.SetFont( &font9 );
    m_BpSave.SetWindowText( GetTexteLoc( 7 ) );

    BOOL Ascii = TRUE;
    if ( CheckAmsdos( BufFile ) )
        {
        Ascii = FALSE;
        StAmsdos * Entete = ( StAmsdos * )BufFile;
        if ( Entete->FileType == 0 || Entete->FileType == 1 )
            IsBasic = TRUE;
        else
            m_Basic.EnableWindow( FALSE );

        TailleFic = Entete->LogicalLength ? Entete->LogicalLength : Entete->RealLength;
        StartAdr = Entete->Adress;

        if ( Entete->BigLength < 4 )
            TailleFic += ( Entete->BigLength << 16 );

        memcpy( BufFile, &BufFile[ sizeof( StAmsdos ) ], TailleFic );
        NbLignes = ( TailleFic + 15 ) >> 4;
        }
    sprintf( Tmp, "#%04X", StartAdr );
    m_StartAsm.SetWindowText( Tmp );

    if ( BmEcran.Create( WinDC, TAILLE_CPC_X, TAILLE_CPC_Y ) )
        {
        memset( EcrCPC.GetBitmapCPC(), 0, 0x4000 ); // Efface image
        EcrCPC.InitPalette( Palette, TRUE );
        EcrCPC.SetMode( 1 );
        m_Mode1.SetCheck( 1 );
        //
        // Choix du mode de visualisation à l'initialisation
        //
        if ( Ascii || IsBasic )
            {
            m_Basic.SetCheck( 1 );
            OnBasic();
            }
        else
            {
            char * p = &NomFic[ strlen( NomFic ) - 4 ];
            if ( ! strcmp( p, ".SCR" ) || ! strcmp( p, ".WIN" ) )
                {
                m_Ecran.SetCheck( 1 );
                OnEcr();
                }
            else
                {
                if ( ! strcmp( p, ".DAM" ) )
                    {
                    m_Dams.SetCheck( 1 );
                    OnDams();
                    }
                else
                    {
                    m_Hexa.SetCheck( 1 );
                    OnHexa();
                    }
                }
            }
        return( TRUE );
        }
    return( FALSE );
}


void CViewFile::OnPaint()
{
    if ( m_Ecran.GetCheck() )
       ViewBitmap();

    CDialog::OnPaint();
}


//
// Affichage secteur en cours
//
void CViewFile::ViewLine( void )
{
    static char Ascii[ 513 ];       // Buffer de représentation en ASCII
    static char HexSect[ 2048 ];    // Buffer de représentation en HEXA

    SetBuffViewHexa( &BufFile[ CurLigne << 4 ]
                   , HexSect
                   , Ascii
                   , ( USHORT )( CurLigne << 4 )
                   , TRUE
                   , min( 512, TailleFic - ( CurLigne << 4 ) )
                   );
    m_ViewHex.SetWindowText( HexSect );
    m_ViewAscii.SetWindowText( Ascii );
}


void CViewFile::ViewBasic( void )
{
    memset( Listing, 0, sizeof( Listing ) );
    Basic( BufFile, Listing, Langue, IsBasic, m_CrLf.GetCheck(), Deprotect, TailleFic );
    m_ListBasic.SetWindowText( Listing );
    m_ListBasic.SetSel( -1, -1 );
}


void CViewFile::ViewDams( void )
{
    memset( Listing, 0, sizeof( Listing ) );
    Dams( BufFile, TailleFic, Listing );
    m_ListBasic.SetWindowText( Listing );
    m_ListBasic.SetSel( -1, -1 );
}


void CViewFile::ViewDesass( void )
{
    memset( Listing, 0, sizeof( Listing ) );
    Desass( BufFile, Listing, StartAdr, TailleFic );
    m_ListBasic.SetWindowText( Listing );
    m_ListBasic.SetSel( -1, -1 );
}


void CViewFile::SetResScroll( CScrollBar * pScrollBar )
{
    if ( m_Ecran.GetCheck() )
        {
        pScrollBar->SetScrollPos( CurLigne >> 2 );
        ViewBitmap();
        }
    else
        {
        pScrollBar->SetScrollPos( CurLigne );
        ViewLine();
        }
}


//
// Gestion du scrolling vertical
//
void CViewFile::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar * pScrollBar )
{
    int LigneM = MAX_LIGNES_HEX;
    int LigneF = NbLignes;

    if ( pScrollBar == &m_ScrollScr || pScrollBar == &m_Scroll )
        {
        if ( pScrollBar == &m_ScrollScr )
            {
            LigneM = NbCols;
            LigneF = TailleFic;
            }
        switch( nSBCode )
            {
            case SB_BOTTOM :
                CurLigne = LigneF - LigneM;
                if ( CurLigne < 0 )
                    CurLigne = 0;

                SetResScroll( pScrollBar );
                break;

            case SB_ENDSCROLL :
                SetResScroll( pScrollBar );
                break;

            case SB_LINEDOWN :
                if ( CurLigne + LigneM < LigneF )
                    CurLigne++;
                else
                    CurLigne = LigneF - LigneM;

                SetResScroll( pScrollBar );
                break;

            case SB_LINEUP :
                if ( CurLigne )
                    CurLigne--;
                else
                    CurLigne = 0;

                SetResScroll( pScrollBar );
                break;

            case SB_PAGEUP :
                CurLigne -= LigneM;
                if ( CurLigne < 0 )
                    CurLigne = 0;

                SetResScroll( pScrollBar );
                break;

            case SB_PAGEDOWN :
                CurLigne += LigneM;
                if ( CurLigne + LigneM >= LigneF )
                    {
                    CurLigne = LigneF - LigneM;
                    if ( CurLigne < 0 )
                        CurLigne = 0;
                    }
                SetResScroll( pScrollBar );
                break;

            case SB_THUMBPOSITION :
            case SB_THUMBTRACK :
                if ( pScrollBar == &m_ScrollScr )
                    CurLigne = nPos << 2;
                else
                    CurLigne = nPos;

                SetResScroll( pScrollBar );
                break;

            case SB_TOP :
                CurLigne = 0;
                SetResScroll( pScrollBar );
                break;
            }
        }
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


//
// Affichage au format Bitmap
//
void CViewFile::ViewBitmap( BOOL InitPal )
{
    char Tmp[ 128 ];
    int Start = CurLigne;
    EcrCPC.SetNbCol( NbCols );
    EcrCPC.SetNbLignes( NbL );
    EcrCPC.SetBitmapCPC( &BufFile[ Start ], TailleFic, InitPal );
    if ( InitPal )
        memcpy( Palette, EcrCPC.GetPalette(), sizeof( Palette ) );

    EcrCPC.Render( ( StRVB * )BmEcran.GetMemBitmap(), m_CrLf.GetCheck() );
    StretchBlt( WinDC->m_hDC, 4, 4, TAILLE_PC_X, TAILLE_PC_Y
              , BmEcran.GetDC(), 0, 0, TAILLE_PC_X, TAILLE_CPC_Y
              , SRCCOPY
              );
    wsprintf( Tmp, GetTexteLoc( 65 ), CurLigne );
    m_StaticOffset.SetWindowText( Tmp );
    wsprintf( Tmp, GetTexteLoc( 64 ), NbCols );
    m_StaticNbCol.SetWindowText( Tmp );
    wsprintf( Tmp, GetTexteLoc( 66 ), NbL );
    m_StaticNbLignes.SetWindowText( Tmp );

    // Ajustement offset
    if ( CurLigne + NbCols * NbL >= TailleFic )
        {
        CurLigne = TailleFic - NbL * NbCols;
        if ( CurLigne < 0 )
            CurLigne = 0;
        }

    // Calcul scrolling min-max
    int MaxScroll = TailleFic - NbCols * NbL;
    if ( MaxScroll <= 0 )
        {
        MaxScroll = 0;
        m_ScrollScr.ShowWindow( SW_HIDE );
        m_ScrollScr.EnableWindow( FALSE );
        }
    else
        {
        m_ScrollScr.SetScrollRange( 0, MaxScroll >> 2 );
        m_ScrollScr.ShowWindow( SW_SHOW );
        }

    // Affichage palette
    CRect rect;
    CDC * TempDC;
    int maxCol = EcrCPC.GetMode() == 2 ? 2 : EcrCPC.GetMode() == 1 ? 4 : 16;
    for ( int i = 0; i < 16; i++ )
        {
        m_Col[ i ].ShowWindow( i < maxCol  ? SW_SHOW : SW_HIDE );
        m_Col[ i ].GetClientRect( rect );
        TempDC = m_Col[ i ].GetDC();
        TempDC->FillSolidRect( rect, GetRgbCPC( Palette[ i ] ) );
        m_Col[ i ].ReleaseDC( TempDC );
        }
}


void CViewFile::OnBasic()
{
    for ( int i = 0; i < 16; i++ )
        m_Col[ i ].ShowWindow( SW_HIDE );

    m_Mode0.ShowWindow( SW_HIDE );
    m_Mode1.ShowWindow( SW_HIDE );
    m_Mode2.ShowWindow( SW_HIDE );
    m_ScrollScr.ShowWindow( SW_HIDE );
    m_BpSave.ShowWindow( SW_SHOW );
    m_Clear.ShowWindow( SW_SHOW );
    m_Clear.ShowWindow( SW_HIDE );
    m_ViewHex.ShowWindow( SW_HIDE );
    m_ViewAscii.ShowWindow( SW_HIDE );
    m_Scroll.ShowWindow( SW_HIDE );
    m_SpinCol.ShowWindow( SW_HIDE );
    m_SpinLignes.ShowWindow( SW_HIDE );
    m_StaticNbCol.SetWindowText( "" );
    m_StaticNbLignes.SetWindowText( "" );
    m_StaticOffset.SetWindowText( "" );
    m_ListBasic.ShowWindow( SW_SHOW );
    m_CrLf.ShowWindow( SW_SHOW );
    m_CrLf.SetWindowText( GetTexteLoc( 62 ) );
    m_CrLf.SetCheck( 0 );
    m_BpStartAsm.ShowWindow( SW_HIDE );
    m_StartAsm.ShowWindow( SW_HIDE );
    ViewBasic();
}


//
// Viewer en mode "écran"
//
void CViewFile::OnEcr()
{
    static char NomPal[ 16 ];

    for ( int i = 0; i < 16; i++ )
        m_Col[ i ].ShowWindow( SW_SHOW );

    CurLigne = 0;
    m_Mode0.ShowWindow( SW_SHOW );
    m_Mode1.ShowWindow( SW_SHOW );
    m_Mode2.ShowWindow( SW_SHOW );
    m_ScrollScr.ShowWindow( SW_SHOW );
    m_ViewHex.ShowWindow( SW_HIDE );
    m_ViewAscii.ShowWindow( SW_HIDE );
    m_Scroll.ShowWindow( SW_HIDE );
    m_ListBasic.ShowWindow( SW_HIDE );
    m_CrLf.ShowWindow( SW_SHOW );
    m_CrLf.SetWindowText( GetTexteLoc( 63 ) );
    m_CrLf.SetCheck( 0 );
    m_SpinCol.ShowWindow( SW_SHOW );
    m_SpinLignes.ShowWindow( SW_SHOW );
    m_BpSave.ShowWindow( SW_SHOW );
    m_BpStartAsm.ShowWindow( SW_HIDE );
    m_StartAsm.ShowWindow( SW_HIDE );

    //
    // Si le fichier est un .SCR, ou un .WIN, essaye de lire le .PAL correspondant
    //
    int l = strlen( NomFic ) - 4;
    if ( ! strcmp( &NomFic[ l ], ".SCR" ) || ! strcmp( &NomFic[ l ], ".WIN" ) )
        {
        BOOL Win = ! strcmp( &NomFic[ l ], ".WIN" );
        EcrCPC.SetBitmapCPC( BufFile, TailleFic, false );
        EcrCPC.SetWin( Win );
        m_CrLf.SetCheck( Win );
        if ( Win )
            {
            NbL =  EcrCPC.GetNbLignes();
            NbCols = EcrCPC.GetNbCol();
            }
        memcpy( NomPal, NomFic, l );
        memset( &NomPal[ l ], ' ', 9 - l );
        for ( int i = 0; i < 64; i++ )
            {
            StDirEntry * d = GetInfoDirEntry( nDSK, i );
            if (  ! strncmp( ( char * )d->Ext, "PAL", 3 )
               && ! strncmp( ( char * )d->Nom, NomPal, 8 )
               )
                {
                BYTE * ValPal = ReadBloc( nDSK, d->Blocks[ 0 ] );
                if ( CheckAmsdos( ValPal ) )
                    {
                    EcrCPC.DecodePalette( &ValPal[ sizeof( StAmsdos ) ] );
                    memcpy( Palette, EcrCPC.GetPalette(), sizeof( Palette ) );
                    EcrCPC.InitPalette( Palette, TRUE );
                    Mode = EcrCPC.GetMode();
                    switch( Mode )
                        {
                        case 0 :
                            m_Mode0.SetCheck( 1 );
                            m_Mode1.SetCheck( 0 );
                            m_Mode2.SetCheck( 0 );
                            break;

                        case 1 :
                            m_Mode0.SetCheck( 0 );
                            m_Mode1.SetCheck( 1 );
                            m_Mode2.SetCheck( 0 );
                            break;

                        case 2 :
                            m_Mode0.SetCheck( 0 );
                            m_Mode1.SetCheck( 0 );
                            m_Mode2.SetCheck( 1 );
                            break;
                        }
                    }
                break;
                }
            }
        }
    ViewBitmap( TRUE );
}


//
// Viewer Hexadécimal
//
void CViewFile::OnHexa()
{
    for ( int i = 0; i < 16; i++ )
        m_Col[ i ].ShowWindow( SW_HIDE );

    CurLigne = 0;
    m_Mode0.ShowWindow( SW_HIDE );
    m_Mode1.ShowWindow( SW_HIDE );
    m_Mode2.ShowWindow( SW_HIDE );
    m_ScrollScr.ShowWindow( SW_HIDE );
    m_BpSave.ShowWindow( SW_HIDE );
    m_Clear.ShowWindow( SW_SHOW );
    m_Clear.ShowWindow( SW_HIDE );
    m_ViewHex.ShowWindow( SW_SHOW );
    m_ViewAscii.ShowWindow( SW_SHOW );
    m_ListBasic.ShowWindow( SW_HIDE );
    m_CrLf.ShowWindow( SW_HIDE );
    m_SpinCol.ShowWindow( SW_HIDE );
    m_SpinLignes.ShowWindow( SW_HIDE );
    m_StaticNbCol.SetWindowText( "" );
    m_StaticNbLignes.SetWindowText( "" );
    m_StaticOffset.SetWindowText( "" );
    m_BpStartAsm.ShowWindow( SW_HIDE );
    m_StartAsm.ShowWindow( SW_HIDE );
    int MaxScroll = NbLignes - MAX_LIGNES_HEX;
    if ( MaxScroll < 0 )
        {
        MaxScroll = 0;
        m_Scroll.ShowWindow( SW_HIDE );
        m_Scroll.EnableWindow( FALSE );
        }
    else
        {
        m_Scroll.SetScrollRange( 0, MaxScroll );
        m_Scroll.ShowWindow( SW_SHOW );
        }
    ViewLine();
}


//
// Viewer de source DAMS
//
void CViewFile::OnDams()
{
    for ( int i = 0; i < 16; i++ )
        m_Col[ i ].ShowWindow( SW_HIDE );

    m_Mode0.ShowWindow( SW_HIDE );
    m_Mode1.ShowWindow( SW_HIDE );
    m_Mode2.ShowWindow( SW_HIDE );
    m_ScrollScr.ShowWindow( SW_HIDE );
    m_BpSave.ShowWindow( SW_SHOW /*SW_HIDE*/ );
    m_Clear.ShowWindow( SW_SHOW );
    m_Clear.ShowWindow( SW_HIDE );
    m_ViewHex.ShowWindow( SW_HIDE );
    m_ViewAscii.ShowWindow( SW_HIDE );
    m_Scroll.ShowWindow( SW_HIDE );
    m_SpinCol.ShowWindow( SW_HIDE );
    m_SpinLignes.ShowWindow( SW_HIDE );
    m_StaticNbCol.SetWindowText( "" );
    m_StaticNbLignes.SetWindowText( "" );
    m_StaticOffset.SetWindowText( "" );
    m_ListBasic.ShowWindow( SW_SHOW );
    m_CrLf.ShowWindow( SW_HIDE );
    m_BpStartAsm.ShowWindow( SW_HIDE );
    m_StartAsm.ShowWindow( SW_HIDE );
    ViewDams();
}


//
// Viewer désassembleur
//
void CViewFile::OnDesass()
{
    for ( int i = 0; i < 16; i++ )
        m_Col[ i ].ShowWindow( SW_HIDE );

    m_Mode0.ShowWindow( SW_HIDE );
    m_Mode1.ShowWindow( SW_HIDE );
    m_Mode2.ShowWindow( SW_HIDE );
    m_ScrollScr.ShowWindow( SW_HIDE );
    m_BpSave.ShowWindow( SW_HIDE );
    m_Clear.ShowWindow( SW_SHOW );
    m_Clear.ShowWindow( SW_HIDE );
    m_ViewHex.ShowWindow( SW_HIDE );
    m_ViewAscii.ShowWindow( SW_HIDE );
    m_Scroll.ShowWindow( SW_HIDE );
    m_SpinCol.ShowWindow( SW_HIDE );
    m_SpinLignes.ShowWindow( SW_HIDE );
    m_StaticNbCol.SetWindowText( "" );
    m_StaticNbLignes.SetWindowText( "" );
    m_StaticOffset.SetWindowText( "" );
    m_ListBasic.ShowWindow( SW_SHOW );
    m_CrLf.ShowWindow( SW_HIDE );
    m_BpStartAsm.ShowWindow( SW_SHOW );
    m_StartAsm.ShowWindow( SW_SHOW );
    ViewDesass();
}


void CViewFile::OnCrlf()
{
    if ( m_Basic.GetCheck() )
        {
        m_BpSave.EnableWindow( ! m_CrLf.GetCheck() );
        ViewBasic();
        }

    if ( m_Ecran.GetCheck() )
        {
        CurLigne = 0;
        ViewBitmap();
        }
}


void CViewFile::OnDeltaposSpinCol( NMHDR * pNMHDR, LRESULT * pResult )
{
    if ( m_Ecran.GetCheck() )
        {
        NM_UPDOWN * pNMUpDown = ( NM_UPDOWN * )pNMHDR;
        NbCols -= pNMUpDown->iDelta;

        if ( NbCols > MAX_COLS_SCR )
            NbCols = MAX_COLS_SCR;

        if ( NbCols < 1 )
            NbCols = 1;

        ViewBitmap();
        }
    * pResult = 0;
}


void CViewFile::OnDeltaposSpinLignes( NMHDR * pNMHDR, LRESULT * pResult)
{
    if ( m_Ecran.GetCheck() )
        {
        NM_UPDOWN * pNMUpDown = ( NM_UPDOWN * )pNMHDR;
        NbL -= pNMUpDown->iDelta;

        if ( NbL > MAX_LIGNES_SCR )
            NbL = MAX_LIGNES_SCR;

        if ( NbL < 1 )
            NbL = 1;

        ViewBitmap();
        }
    * pResult = 0;
}


void CViewFile::OnMode0()
{
    EcrCPC.SetMode( 0 );
    ViewBitmap();
}


void CViewFile::OnMode1()
{
    EcrCPC.SetMode( 1 );
    ViewBitmap();
}


void CViewFile::OnMode2()
{
    EcrCPC.SetMode( 2 );
    ViewBitmap();
}


//
// Sauvegarde au format .WIN d'OCP Art Studio
//
void CViewFile::SaveWin( HANDLE f, char * nomFic )
{
    DWORD Lg;
    BYTE EndWin[ 5 ];
    int Longueur = NbL * NbCols;

    if ( TypeModeExport == MODE_BINAIRE )
        {
        //
        // Ajoute en-tête amsdos
        //
        StAmsdos * e = CreeEnteteAmsdos( nomFic, ( USHORT )( Longueur + 5 ) );
        WriteFile( f, e, sizeof( StAmsdos ), &Lg, NULL );
        }
    int Start = CurLigne;
    WriteFile( f, &BufFile[ Start ], Longueur, &Lg, NULL );
    //
    // 5 octets de fin définissant la taille de la fenêtre
    //
    int Largeur = NbCols << 3;
    EndWin[ 0 ] = 0;
    EndWin[ 1 ] = ( BYTE )( Largeur & 0xFF );
    EndWin[ 2 ] = ( BYTE )( Largeur >> 8 );
    EndWin[ 3 ] = ( BYTE )NbL;
    EndWin[ 4 ] = 0;
    WriteFile( f, EndWin, sizeof( EndWin ), &Lg, NULL );
}


//
// Appui sur le bouton sauvegarde
//
void CViewFile::OnSave()
{
    static char LastDir[ 256 ];
    DWORD Lg;
    char Nom[ 128 ];

    * Nom = 0;
    CFileDialog SelectFic( FALSE
                         , m_Ecran.GetCheck() ? "*.WIN" : "*.TXT"
                         , Nom
                         , OFN_OVERWRITEPROMPT
                         , GetTexteLoc( m_Ecran.GetCheck() ? 67 : 111 )
                         );
    SelectFic.m_ofn.lpstrInitialDir = LastDir;
    if ( SelectFic.DoModal() == IDOK )
        {
        strcpy( LastDir, SelectFic.GetPathName() );
        HANDLE f = CreateFile( LastDir
                             , GENERIC_WRITE
                             , 0
                             , NULL
                             , CREATE_ALWAYS
                             , 0
                             , NULL
                             );
        if ( f != INVALID_HANDLE_VALUE )
            {
            if ( m_Ecran.GetCheck() )
                SaveWin( f, LastDir );
            else
                WriteFile( f, Listing, strlen( Listing ), &Lg, NULL );

            CloseHandle( f );
            }
        }
}


void CViewFile::ClickCol( int NumCol )
{
    CChoixCouleur c( Palette[ NumCol ], this );
    c.DoModal();
    if ( c.GetRetour() )
        Palette[ NumCol ] = ( BYTE )c.GetCouleur(); // Changement de couleur

    EcrCPC.InitPalette( Palette, TRUE );
    ViewBitmap();
}


void CViewFile::OnBpStartAsm()
{
    char Msg[ 8 ];

    m_StartAsm.GetWindowText( Msg, sizeof( Msg ) );
    StartAdr = ( USHORT )HexToDec( Msg );
    ViewDesass();
}
