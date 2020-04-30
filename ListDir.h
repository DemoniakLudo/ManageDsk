#if !defined(AFX_LISTDIR_H__8B4010E6_0B69_42BF_B90A_42C4D874F485__INCLUDED_)
#define AFX_LISTDIR_H__8B4010E6_0B69_42BF_B90A_42C4D874F485__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListDir.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListDir dialog

class CListDir : public CDialog
{
// Construction
public:
    CListDir( StDirEntry * t, CWnd * pParent = NULL );   // standard constructor

// Dialog Data
    //{{AFX_DATA(CListDir)
    enum { IDD = IDD_LISTEDIR };
    CButton     m_BpOk;
    CButton     m_BpHexa;
    CListCtrl   m_Liste;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CListDir)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CListDir)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnHexa();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    void Affiche( void );

    StDirEntry * TabDir;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDIR_H__8B4010E6_0B69_42BF_B90A_42C4D874F485__INCLUDED_)
