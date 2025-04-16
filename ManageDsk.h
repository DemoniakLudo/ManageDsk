// ManageDsk.h : main header file for the MANAGEDSK application
//

#if !defined(AFX_MANAGEDSK_H__00D54A00_D266_470D_A295_892B61B5A347__INCLUDED_)
#define AFX_MANAGEDSK_H__00D54A00_D266_470D_A295_892B61B5A347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols
#include "GestDsk.h"

/////////////////////////////////////////////////////////////////////////////
// CManageDskApp:
// See ManageDsk.cpp for the implementation of this class
//

class CManageDskApp : public CWinApp {
public:
	CManageDskApp();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CManageDskApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
// Implementation

	//{{AFX_MSG(CManageDskApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	char* GetArg(char* argc);
	char* Argument(char* argc);
	void DecomposeArg(char* argc);
	void AjouteFichiers(int nDSK, char* masque);
	void AjouteFichierEntete(int nDSK, char* param);
	void ExportFichier(int nDSK, char* masque, bool withEntete);
	int CManageDskApp::GetFic(int nDSK, StDirEntry* Dir, int Ko, BYTE* BufOut);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEDSK_H__00D54A00_D266_470D_A295_892B61B5A347__INCLUDED_)
