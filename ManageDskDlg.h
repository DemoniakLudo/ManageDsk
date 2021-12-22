// ManageDskDlg.h : header file
//

#if !defined(AFX_MANAGEDSKDLG_H__48E5BD50_726F_40CD_BA97_364027CC0183__INCLUDED_)
#define AFX_MANAGEDSKDLG_H__48E5BD50_726F_40CD_BA97_364027CC0183__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include  "TypeFichier.h"
#include  "EditSecteurs.h"


enum { DSK_0 = 0, DSK_1 };


/////////////////////////////////////////////////////////////////////////////
// CManageDskDlg dialog

class CManageDskDlg;


class CMyListe : public CListCtrl
{
public:
	void SetNumDsk(int n) { nDSK = n; }
	void SetParent(CManageDskDlg * p) { Parent = p; }

protected:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
private:
	int nDSK;
	CManageDskDlg * Parent;
};


class CManageDskDlg : public CDialog
{
	// Construction
public:
	CManageDskDlg(CWnd* pParent = NULL);    // standard constructor

	void AjouteUnFic(char * NomFic, int nDSK);
	void ReadNewDSK(char * NomFic, int nDSK);

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageDskDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

	void ReadDirDsk(int nDSK);

	// Implementation
	// Generated message map functions
	//{{AFX_MSG(CManageDskDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	afx_msg void OnBpEn() { SetInterface(LOC_ENGLISH); }
	afx_msg void OnBpFr() { SetInterface(LOC_FRANCAIS); }
	afx_msg void OnBpSp() { SetInterface(LOC_SPANISH); }
	afx_msg void OnAjoutefic1() { OnAjoutefic(DSK_0); }
	afx_msg void OnLire1() { OnLire(DSK_0); }
	afx_msg void OnSauve1() { OnSauve(DSK_0); }
	afx_msg void OnCreer1() { OnCreer(DSK_0); }
	afx_msg void OnRclickList1(NMHDR * pNMHDR, LRESULT * pResult)
	{
		OnRclickList(0, pNMHDR);
		*pResult = 0;
	}
	afx_msg void OnExport1() { OnExport(DSK_0); }
	afx_msg void OnRenomme1() { OnRenomme(DSK_0); }
	afx_msg void OnSupprime1() { OnSupprime(DSK_0); }
	afx_msg void OnRadioForceAscii1() { TypeModeImport[DSK_0] = MODE_ASCII; }
	afx_msg void OnRadioExpBin1() { TypeModeExport[DSK_0] = MODE_BINAIRE; }
	afx_msg void OnRadioExpAscii1() { TypeModeExport[DSK_0] = MODE_ASCII; }
	afx_msg void OnRadioExpAuto1() { TypeModeExport[DSK_0] = MODE_AUTO; }
	afx_msg void OnRadioForceBin1() { TypeModeImport[DSK_0] = MODE_BINAIRE; }
	afx_msg void OnRadioChoix1() { TypeModeImport[DSK_0] = MODE_USER; }
	afx_msg void OnSauveDirect1() { OnSauveDirect(DSK_0); }
	afx_msg void OnDblclkList1(NMHDR * pNMHDR, LRESULT * pResult)
	{
		OnDblclkList(DSK_0, pNMHDR);
		*pResult = 0;
	}
	afx_msg void OnEditSecteurs1() { CEditSecteurs(DSK_0).DoModal(); }
	afx_msg void OnEditFic1() { OnEditFic(DSK_0); }
	afx_msg void OnReadFloppy1() { OnReadFloppy(DSK_0); }
	afx_msg void OnWriteFloppy1() { OnWriteFloppy(DSK_0); }
	afx_msg void OnFormatFloppy1() { OnFormatFloppy(DSK_0); }
	afx_msg void OnBpSend1() { OnSend(DSK_0); }
	afx_msg void OnBpVisuDir1() { OnVisuDir(DSK_0); }
	afx_msg void OnNMCustomdrawList1(NMHDR * pNMHDR, LRESULT * pResult)
	{
		OnCustomDrawList(DSK_0, pNMHDR);
		*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYSUBITEMDRAW;
	}

	afx_msg void OnAjoutefic2() { OnAjoutefic(DSK_1); }
	afx_msg void OnLire2() { OnLire(DSK_1); }
	afx_msg void OnSauve2() { OnSauve(DSK_1); }
	afx_msg void OnCreer2() { OnCreer(DSK_1); }
	afx_msg void OnRclickList2(NMHDR * pNMHDR, LRESULT * pResult)
	{
		OnRclickList(DSK_1, pNMHDR);
		*pResult = 0;
	}
	afx_msg void OnExport2() { OnExport(DSK_1); }
	afx_msg void OnRenomme2() { OnRenomme(DSK_1); }
	afx_msg void OnSupprime2() { OnSupprime(DSK_1); }
	afx_msg void OnRadioForceAscii2() { TypeModeImport[DSK_1] = MODE_ASCII; }
	afx_msg void OnRadioExpBin2() { TypeModeExport[DSK_1] = MODE_BINAIRE; }
	afx_msg void OnRadioExpAuto2() { TypeModeExport[DSK_1] = MODE_AUTO; }
	afx_msg void OnRadioExpAscii2() { TypeModeExport[DSK_1] = MODE_ASCII; }
	afx_msg void OnRadioForceBin2() { TypeModeImport[DSK_1] = MODE_BINAIRE; }
	afx_msg void OnRadioChoix2() { TypeModeImport[DSK_1] = MODE_USER; }
	afx_msg void OnSauveDirect2() { OnSauveDirect(DSK_1); }
	afx_msg void OnDblclkList2(NMHDR * pNMHDR, LRESULT * pResult)
	{
		OnDblclkList(DSK_1, pNMHDR);
		*pResult = 0;
	}
	afx_msg void OnEditSecteurs2() { CEditSecteurs(DSK_1).DoModal(); }
	afx_msg void OnEditFic2() { OnEditFic(DSK_1); }
	afx_msg void OnReadFloppy2() { OnReadFloppy(DSK_1); }
	afx_msg void OnWriteFloppy2() { OnWriteFloppy(DSK_1); }
	afx_msg void OnFormatFloppy2() { OnFormatFloppy(DSK_1); }
	afx_msg void OnBpSend2() { OnSend(DSK_1); }
	afx_msg void OnBpVisuDir2() { OnVisuDir(DSK_1); }
	afx_msg void OnNMCustomdrawList2(NMHDR * pNMHDR, LRESULT * pResult)
	{
		OnCustomDrawList(DSK_1, pNMHDR);
		*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYSUBITEMDRAW;
	}

	afx_msg void OnBpSwapDsk(void);
	afx_msg void OnBpCompare(void);
	afx_msg void OnDblWindow(void);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void OnLire(int nDSK);
	void OnCreer(int nDSK);
	void OnSauve(int nDSK);
	BOOL CheckSaved(int nDSK);
	void OnSauveDirect(int nDSK);
	void OnAjoutefic(int nDSK);
	void OnRclickList(int nDSK, NMHDR * pNMHDR);
	void OnDblclkList(int nDSK, NMHDR * pNMHDR) { OnRclickList(nDSK, pNMHDR); }
	void OnCustomDrawList(int nDSK, NMHDR * pNMHDR);
	void OnExport(int nDSK);
	void OnRenomme(int nDSK);
	void OnSupprime(int nDSK);
	void OnEditFic(int nDSK);
	void OnReadFloppy(int nDSK);
	void OnWriteFloppy(int nDSK);
	void OnFormatFloppy(int nDSK);
	void OnSend(int nDSK);
	void OnVisuDir(int nDSK);
	void Supprime(int nDSK, int Indice, BOOL Kill);
	void SetInterface(int Langue);
	void BoutonsOn(int NumDsk);
	void BoutonsOff(int NumDsk);
	int GetFic(int nDSK, int Indice, int Ko, BYTE * BufOut, BOOL Strict = FALSE);
	void TryCopie(int nDSK, BYTE * Buff, char * NomFic, int Lg, int MaxBlocs, int User);
	void CheckInterlace(int nDSK);
	BOOL IsReadable(int nDSK);

	enum { IDD = IDD_MANAGEDSK_DIALOG };
	CButton         m_MaxPistes;
	CBitmapButton   m_BpEn;
	CBitmapButton   m_BpFr;
	CBitmapButton   m_BpSp;
	CButton         m_BpSend[2];
	CButton         m_BpSwapDsk;
	CButton         m_BpCompare;
	CButton         m_Nettoie;
	CButton         m_DblWindow;
	CButton         m_BpEditFic[2];
	CStatic         m_ModeExport[2];
	CButton         m_RadioExpBin[2];
	CButton         m_RadioExpAscii[2];
	CButton         m_RadioExpAuto[2];
	CButton         m_Secteurs[2];
	CButton         m_BpSauveDirect[2];
	CStatic         m_ModeAjout[2];
	CButton         m_RadioForceBin[2];
	CButton         m_RadioForceAscii[2];
	CButton         m_RadioChoix[2];
	CButton         m_ContenuDsk[2];
	CButton         m_BpCreer[2];
	CButton         m_BpLire[2];
	CButton         m_BpSupprime[2];
	CButton         m_BpSauve[2];
	CButton         m_BpExport[2];
	CButton         m_BpRenomme[2];
	CButton         m_BpAjoute[2];
	CEdit           m_NomDsk[2];
	CMyListe        m_Liste[2];
	CButton         m_WriteFloppy[2];
	CButton         m_ReadFloppy[2];
	CButton         m_BpVisuDir[2];
	CButton         m_FormatFloppy[2];

	HICON m_hIcon;
	char NomDsk[2][256];
	char Msg[128];
	StDirEntry TabDirInit[2][64];
	StDirEntry TabDir[2][64];
	int PosItem[2][64];
	BOOL IsDsk[2], IsDskValid[2], IsDskSaved[2];
	int TypeModeImport[2], TypeModeExport[2];
	int Langue;
public:
	afx_msg void OnBnClickedRadioExpAuto();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEDSKDLG_H__48E5BD50_726F_40CD_BA97_364027CC0183__INCLUDED_)
