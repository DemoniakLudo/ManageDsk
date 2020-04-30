#if !defined(AFX_OUINON_H__C3B27701_0042_44DF_BCDD_31320015C2B8__INCLUDED_)
#define AFX_OUINON_H__C3B27701_0042_44DF_BCDD_31320015C2B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OuiNon.h : header file
//


//
// Valeurs de retour possibles de l'interface
//
enum    { RET_OUI, RET_OUITOUT, RET_NON };


/////////////////////////////////////////////////////////////////////////////
// COuiNon dialog

class COuiNon : public CDialog
{
// Construction
public:
    COuiNon( char * n, char * m, BOOL t, CWnd * pParent = NULL );   // standard constructor
    int GetRet( void ) { return( Ret ); }
    BOOL GetKill( void ) { return( Kill ); }

// Dialog Data
    //{{AFX_DATA(COuiNon)
    enum { IDD = IDD_OUINON };
    CButton    m_BpOuiTout;
    CButton    m_BpOui;
    CButton    m_BpNon;
    CButton     m_Kill;
    CStatic    m_MsgConfirm;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COuiNon)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(COuiNon)
    virtual void OnOK() {}
    afx_msg void OnNon();
    afx_msg void OnOui();
    afx_msg void OnOuitout();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    int Ret;
    BOOL Tout, Kill;
    char * Msg, * NomFic;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUINON_H__C3B27701_0042_44DF_BCDD_31320015C2B8__INCLUDED_)
