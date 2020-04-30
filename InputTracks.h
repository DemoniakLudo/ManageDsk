#if !defined(AFX_INPUTTRACKS_H__1E408248_C7A3_40F0_A8B5_69D97E2438F8__INCLUDED_)
#define AFX_INPUTTRACKS_H__1E408248_C7A3_40F0_A8B5_69D97E2438F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputTracks.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputTracks dialog

class CInputTracks : public CDialog
{
	// Construction
public:
	CInputTracks(CWnd* pParent = NULL);   // standard constructor
	BOOL IsValide(void) { return(Valide); }
	int GetNbTracks(void) { return(NbTracks); }
	int GetNbSects(void) { return(NbSects); }

	// Dialog Data
		//{{AFX_DATA(CInputTracks)
	enum { IDD = IDD_INPUT_TRACKS };
	CStatic	m_TxtNbTracks;
	CStatic	m_TxtNbSects;
	CButton	m_BpOk;
	CButton	m_BpCancel;
	CEdit	m_NbTracks;
	CEdit	m_NbSects;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputTracks)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInputTracks)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL Valide;
	int NbTracks;
	int NbSects;
public:
	afx_msg void OnOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTTRACKS_H__1E408248_C7A3_40F0_A8B5_69D97E2438F8__INCLUDED_)
