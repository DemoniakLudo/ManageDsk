#if !defined(AFX_TYPEFICHIER_H__7C85705D_5DFE_48D8_A0C0_AB865155BFBB__INCLUDED_)
#define AFX_TYPEFICHIER_H__7C85705D_5DFE_48D8_A0C0_AB865155BFBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeFichier.h : header file
//


enum { MODE_ASCII, MODE_BINAIRE, MODE_USER, MODE_AUTO };


/////////////////////////////////////////////////////////////////////////////
// CTypeFichier dialog

class CTypeFichier : public CDialog
{
// Construction
public:
    CTypeFichier( int t
                , StAmsdos * e
                , BOOL i
                , char * n
                , CWnd * pParent = NULL
                );   // standard constructor
    int GetTypeModeImport( void ) { return( TypeModeImport ); }

// Dialog Data
    //{{AFX_DATA(CTypeFichier)
    enum { IDD = IDD_IMPORT_FICHIER };
    CEdit       m_PointEntree2;
    CEdit       m_Longueur2;
    CStatic     m_IsAmsdos;
    CEdit       m_AdresseDeb2;
    CButton     m_TypeFic;
    CButton     m_InfoFic;
    CButton     m_TypeBin;
    CButton     m_BpValide;
    CButton     m_TypeAscii;
    CStatic     m_TxtPointEntree;
    CStatic     m_TxtLongueur;
    CStatic     m_TxtAdrDeb;
    CEdit       m_PointEntree;
    CEdit       m_Longueur;
    CEdit       m_AdresseDeb;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTypeFichier)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CTypeFichier)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnTypeAscii();
    afx_msg void OnTypeBin();
    afx_msg void OnChangeAdresseDeb();
    afx_msg void OnChangeAdresseDeb2();
    afx_msg void OnChangePointEntree();
    afx_msg void OnChangePointEntree2();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    int TypeModeImport;
    StAmsdos * Entete;
    char Tmp[ 128 ];
    char * Nom;
    USHORT Adress, EntryAdress;
    BOOL IsAmsdos, LockModif;
public:
    CEdit m_User;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPEFICHIER_H__7C85705D_5DFE_48D8_A0C0_AB865155BFBB__INCLUDED_)
