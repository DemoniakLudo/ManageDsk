#if !defined(AFX_VIEWFILE_H__6D27B016_F239_4A9F_A0EF_A2D90D9B38D6__INCLUDED_)
#define AFX_VIEWFILE_H__6D27B016_F239_4A9F_A0EF_A2D90D9B38D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFile.h : header file
//

#include  <afxcmn.h>

#include  "Bitmap.h"
#include  "BitmapCPC.h"
#include  "Resource.h"
#include "afxwin.h"


#define MAX_FILESIZE        0x100000



/////////////////////////////////////////////////////////////////////////////
// CViewFile dialog

class CViewFile : public CDialog
{
// Construction
public:
    CViewFile( char * Nom
             , BYTE * b
             , int n
             , int t
             , int d
             , int l
             , int dp
             , CWnd * pParent = NULL
             );

// Dialog Data
    //{{AFX_DATA(CViewFile)
    enum { IDD = IDD_EDIT_FICHIER };
    CButton     m_BpSave;
    CButton     m_Mode0;
    CButton     m_Mode1;
    CButton     m_Mode2;
    CStatic     m_StaticNbLignes;
    CStatic     m_StaticOffset;
    CStatic     m_StaticNbCol;
    CEdit       m_Clear;
    CButton     m_CrLf;
    CButton     m_Hexa;
    CButton     m_Ecran;
    CButton     m_Basic;
    CButton     m_Dams;
    CButton     m_Desass;
    CEdit       m_ListBasic;
    CEdit       m_ViewHex;
    CEdit       m_ViewAscii;
    CScrollBar  m_Scroll;
    CScrollBar  m_ScrollScr;
    CSpinButtonCtrl m_SpinCol;
    CSpinButtonCtrl m_SpinLignes;
    CStatic     m_Col[ 16 ];
    CEdit       m_StartAsm;
    CButton     m_BpStartAsm;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewFile)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CViewFile)
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    virtual BOOL OnInitDialog();
    afx_msg void OnBasic();
    afx_msg void OnEcr();
    afx_msg void OnHexa();
    afx_msg void OnCrlf();
    afx_msg void OnPaint();
    afx_msg void OnDeltaposSpinCol(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpinLignes(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMode0();
    afx_msg void OnMode1();
    afx_msg void OnMode2();
    afx_msg void OnSave();
    afx_msg void OnDams();
    afx_msg void OnDesass();
    afx_msg void OnCol00() { ClickCol( 0 ); }
    afx_msg void OnCol01() { ClickCol( 1 ); }
    afx_msg void OnCol02() { ClickCol( 2 ); }
    afx_msg void OnCol03() { ClickCol( 3 ); }
    afx_msg void OnCol04() { ClickCol( 4 ); }
    afx_msg void OnCol05() { ClickCol( 5 ); }
    afx_msg void OnCol06() { ClickCol( 6 ); }
    afx_msg void OnCol07() { ClickCol( 7 ); }
    afx_msg void OnCol08() { ClickCol( 8 ); }
    afx_msg void OnCol09() { ClickCol( 9 ); }
    afx_msg void OnCol10() { ClickCol( 10 ); }
    afx_msg void OnCol11() { ClickCol( 11 ); }
    afx_msg void OnCol12() { ClickCol( 12 ); }
    afx_msg void OnCol13() { ClickCol( 13 ); }
    afx_msg void OnCol14() { ClickCol( 14 ); }
    afx_msg void OnCol15() { ClickCol( 15 ); }
    afx_msg void OnBpStartAsm();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    void ViewLine( void );
    void ViewBitmap( BOOL InitPal = FALSE );
    void ViewBasic( void );
    void ViewDams( void );
    void ViewDesass( void );
    void SetResScroll( CScrollBar * pScrollBar );
    void ClickCol( int NumCol );
    void SaveWin( HANDLE f, char * Nom );

    CFont font8, font9;
    BYTE * BufFile;
    char * NomFic;
    int NbLignes, StartAdr, TailleFic, CurLigne, NbCols, NbL;
    BYTE Palette[ 16 ];
    int Mode;
    BOOL IsBasic;
    int TypeModeExport;
    int nDSK;
    int Langue;
    int Deprotect;
    CBitmapCPC EcrCPC;
    CDC * WinDC;
    MonBitmap BmEcran;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFILE_H__6D27B016_F239_4A9F_A0EF_A2D90D9B38D6__INCLUDED_)
