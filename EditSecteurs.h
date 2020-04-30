#if !defined(AFX_EDITSECTEURS_H__5C8B12E3_F96C_4855_B3A4_93B5DE643BBB__INCLUDED_)
#define AFX_EDITSECTEURS_H__5C8B12E3_F96C_4855_B3A4_93B5DE643BBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSecteurs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditSecteurs dialog

class CEditSecteurs : public CDialog
{
// Construction
public:
    CEditSecteurs( int n, CWnd * pParent = NULL );   // standard constructor

// Dialog Data
    //{{AFX_DATA(CEditSecteurs)
    enum { IDD = IDD_EDIT_SECTEURS };
    CStatic m_TxtTrack;
    CStatic m_TxtSect;
    CEdit   m_Track;
    CEdit   m_Sect;
    CEdit   m_ViewAscii;
    CEdit   m_ViewSect;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEditSecteurs)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEditSecteurs)
    virtual BOOL OnInitDialog();
    afx_msg void OnDeltaposSpinSect(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpinTrack(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    void CreeHexSect( void );

    CFont font;
    BYTE Secteur[ 512 ];
    int nDSK, Track, Sect;
    CPCEMUTrack * TrackData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSECTEURS_H__5C8B12E3_F96C_4855_B3A4_93B5DE643BBB__INCLUDED_)
