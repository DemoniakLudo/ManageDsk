#if !defined(AFX_INFOFICHIER_H__E600E181_4A42_4F4E_AD6F_7F7F343417C6__INCLUDED_)
#define AFX_INFOFICHIER_H__E600E181_4A42_4F4E_AD6F_7F7F343417C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoFichier.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoFichier dialog

class CInfoFichier : public CDialog
{
// Construction
public:
    CInfoFichier( int n
                , StDirEntry * t
                , int i
                , BOOL r
                , CWnd * pParent = NULL
                );   // standard constructor
    BOOL IsValide( void ) { return( Valide ); }
    char * GetUser( void ) { return( User ); }
    char * GetNom( void ) { return( Nom ); }
    StAmsdos * GetAms( void ) { return( &Ams ); }
    BOOL IsReadOnly( void ) { return( ReadOnly ); }
    BOOL IsHidden( void ) { return( Hidden ); }

// Dialog Data
    //{{AFX_DATA(CInfoFichier)
    enum { IDD = IDD_INFOFICHIER };
    CComboBox   m_Type;
    CButton     m_ReadOnly;
    CButton     m_Hide;
    CButton     m_BpValider;
    CStatic     m_TxtUser;
    CStatic     m_TxtType;
    CStatic     m_TxtPointEntree;
    CStatic     m_TxtNomFic;
    CStatic     m_TxtLongueur;
    CStatic     m_TxtAdrDeb;
    CButton     m_BpRenomme;
    CEdit       m_User;
    CEdit       m_PointEntree;
    CEdit       m_NomFic;
    CEdit       m_Longueur;
    CEdit       m_AdresseDeb;
    CEdit       m_AdresseDeb2;
    CEdit       m_Longueur2;
    CEdit       m_PointEntree2;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CInfoFichier)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CInfoFichier)
    afx_msg void OnExport();
    afx_msg void OnSupprimer();
    virtual BOOL OnInitDialog();
    virtual void OnOK() {}
    afx_msg void OnValider();
    afx_msg void OnChangeAdresseDeb();
    afx_msg void OnChangeAdresseDeb2();
    afx_msg void OnChangePointEntree();
    afx_msg void OnChangePointEntree2();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    StDirEntry * TabDir;
    StAmsdos Ams;
    int Indice;
    int nDSK;
    char Msg[ 128 ];
    char User[ 4 ];
    char Nom[ 32 ];
    BOOL Renomme, Valide, LockModif, ReadOnly, Hidden;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOFICHIER_H__E600E181_4A42_4F4E_AD6F_7F7F343417C6__INCLUDED_)
