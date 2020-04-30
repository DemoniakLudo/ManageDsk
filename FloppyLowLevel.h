#if !defined(AFX_INFOFLOPPY_H__29188673_EBAB_49B4_9E2A_33D865427B83__INCLUDED_)
#define AFX_INFOFLOPPY_H__29188673_EBAB_49B4_9E2A_33D865427B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloppyLowLevel.h : header file
//


#include  "fdrawcmd.h"


enum { CMD_WRITE_DIRECT, CMD_READ_DIRECT };


/////////////////////////////////////////////////////////////////////////////
// CFloppyLowLevel dialog

class CFloppyLowLevel : public CDialog
{
// Construction
public:
    CFloppyLowLevel( int n, int t, int m, CWnd * pParent = NULL);   // standard constructor
    BOOL GetResult( void ) { return( Result ); }
    BOOL GetCreation( void ) { return( Creation ); }

// Dialog Data
    //{{AFX_DATA(CFloppyLowLevel)
	enum { IDD = IDD_INFO_FLOPPY };
	CButton	m_BpAccept;
	CButton	m_BpDetail;
	CButton         m_BpStopError;
    CListBox        m_Informations;
    CButton         m_BpDetectMedia;
    CButton         m_BpDriveA;
    CButton         m_BpDriveB;
    CButton         m_BpDoubleStep;
    CButton         m_BpFace0;
    CButton         m_BpFace1;
    CButton         m_BpCancel;
    CButton         m_BpOk;
    CProgressCtrl   m_Progress;
    CStatic         m_Avancee;
    CStatic         m_ConfDrive;
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFloppyLowLevel)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CFloppyLowLevel)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
	afx_msg void OnAccept();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private :
    void InitInterface( void );
    DWORD GetDriverVersion( void );
    HANDLE InitDriver( int Drive, BOOL CheckMedia );
    BOOL GetFctInfo( FD_FDC_INFO * Info );
    BOOL MoveToTrack( BYTE Piste );
    BOOL ReadFloppyLayout( int Piste );
    BOOL ReadTrack( BYTE Piste, CPCEMUTrack * TrackInfos );
    BOOL WriteTrack( BYTE Piste, CPCEMUTrack * TrackInfos );
    BOOL FormatTrack( BYTE Piste, CPCEMUTrack * TrackInfos );
    BOOL DskToFloppy( void );
    BOOL FloppyToDsk( void );
    BOOL Erreur( char * Msg = NULL );
    BOOL GetGeometry( int Drive );

    char Msg[ 128 ];
    int nDSK, TypeCmd, MaxPiste;
    BYTE Flags, Tete;
    HANDLE HandleDriver;
    BOOL ShowMessage, Result, Creation, DoubleStep, Break, Retry, DmdCancel;
    DWORD rt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOFLOPPY_H__29188673_EBAB_49B4_9E2A_33D865427B83__INCLUDED_)
