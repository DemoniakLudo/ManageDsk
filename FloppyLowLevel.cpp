// FloppyLowLevel.cpp : implementation file
//

#include  <afxwin.h>
#include  <afxcmn.h>

#include  <winerror.h>

#include  "Locale.h"
#include  "GestDsk.h"
#include  "FdRawCmd.h"
#include  "Resource.h"
#include  "FloppyLowLevel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CFloppyLowLevel dialog


CFloppyLowLevel::CFloppyLowLevel(int n, int t, int m, CWnd* pParent /*=NULL*/) : CDialog(CFloppyLowLevel::IDD, pParent) {
	//{{AFX_DATA_INIT(CFloppyLowLevel)
	//}}AFX_DATA_INIT
	nDSK = n;
	TypeCmd = t;
	MaxPiste = m;
	Result = FALSE;
	HandleDriver = INVALID_HANDLE_VALUE;
}


void CFloppyLowLevel::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloppyLowLevel)
	DDX_Control(pDX, IDC_ACCEPT, m_BpAccept);
	DDX_Control(pDX, IDC_DETAIL, m_BpDetail);
	DDX_Control(pDX, IDC_STOP_ERROR, m_BpStopError);
	DDX_Control(pDX, IDC_INFORMATIONS, m_Informations);
	DDX_Control(pDX, IDC_DETECT_MEDIA, m_BpDetectMedia);
	DDX_Control(pDX, IDC_DRIVEA, m_BpDriveA);
	DDX_Control(pDX, IDC_DRIVEB, m_BpDriveB);
	DDX_Control(pDX, IDC_DOUBLESTEP, m_BpDoubleStep);
	DDX_Control(pDX, IDC_FACE0, m_BpFace0);
	DDX_Control(pDX, IDC_FACE1, m_BpFace1);
	DDX_Control(pDX, IDCANCEL, m_BpCancel);
	DDX_Control(pDX, IDOK, m_BpOk);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_AVANCEE, m_Avancee);
	DDX_Control(pDX, IDC_CONFDRIVE, m_ConfDrive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFloppyLowLevel, CDialog)
	//{{AFX_MSG_MAP(CFloppyLowLevel)
	ON_BN_CLICKED(IDC_ACCEPT, OnAccept)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloppyLowLevel message handlers


void CFloppyLowLevel::InitInterface(void) {
	m_BpAccept.EnableWindow(FALSE);
	m_BpOk.EnableWindow(FALSE);
	m_BpFace0.EnableWindow(TRUE);
	m_BpFace1.EnableWindow(TRUE);
	m_BpDriveA.EnableWindow(TRUE);
	m_BpDriveB.EnableWindow(TRUE);
	m_BpDoubleStep.EnableWindow(TRUE);
	m_BpDetectMedia.EnableWindow(TRUE);
	m_BpStopError.EnableWindow(TRUE);
	m_BpDetail.EnableWindow(TRUE);
	m_Progress.SetPos(0);
	m_BpOk.SetWindowText(GetTexteLoc(93));
	m_BpCancel.SetWindowText(GetTexteLoc(94));
	m_BpFace0.SetWindowText(GetTexteLoc(98));
	m_BpFace1.SetWindowText(GetTexteLoc(99));
	m_BpDoubleStep.SetWindowText(GetTexteLoc(100));
	m_ConfDrive.SetWindowText(GetTexteLoc(104));
	m_Avancee.SetWindowText(GetTexteLoc(106));
	m_BpDriveA.SetWindowText(GetTexteLoc(108));
	m_BpDriveB.SetWindowText(GetTexteLoc(109));
	m_BpDetectMedia.SetWindowText(GetTexteLoc(116));
	m_BpStopError.SetWindowText(GetTexteLoc(117));
	m_BpDetail.SetWindowText(GetTexteLoc(118));
	m_BpAccept.SetWindowText(GetTexteLoc(120));
	DWORD Ver = GetDriverVersion();
	if (Ver) {
		if (HIWORD(Ver) == HIWORD(FDRAWCMD_VERSION))
			m_BpOk.EnableWindow(TRUE);
		else // version de fdrawcmd.sys incorrecte
			m_Informations.AddString(GetTexteLoc(89));
	}
	else // fdrawcmd.sys pas installé
		m_Informations.AddString(GetTexteLoc(90));
}


BOOL CFloppyLowLevel::OnInitDialog() {
	CDialog::OnInitDialog();

	m_BpFace0.SetCheck(1);
	m_BpDriveA.SetCheck(1);
	switch (TypeCmd) {
	case CMD_WRITE_DIRECT:
		SetWindowText(GetTexteLoc(83));
		m_BpAccept.ShowWindow(FALSE);
		break;

	case CMD_READ_DIRECT:
		SetWindowText(GetTexteLoc(84));
		break;
	}
	InitInterface();
	return(TRUE);
}


//
// Affiche un message d'erreur Windows en fonction de la dernière erreur système
//
BOOL CFloppyLowLevel::Erreur(char* Msg) {
	static char sz[256];

	if (!Msg) {
		int i = GetLastError();
		switch (i) {
		case 19:
			strcpy(sz, GetTexteLoc(121));
			Break = TRUE;
			break;

		case 21:
			strcpy(sz, GetTexteLoc(88));
			break;

		case 23:
			strcpy(sz, "Erreur CRC");
			break;

		case 27:
			strcpy(sz, "Secteur non trouvé");
			break;

		default:
			wsprintf(sz, "Error message #%d", i);
			break;
		}
	}
	else
		strcpy(sz, Msg);

	if (m_BpStopError.GetCheck() && !ShowMessage) {
		ShowMessage = TRUE;
		MessageBox(sz, NULL, MB_ICONWARNING);
		Break = TRUE;
	}
	else {
		m_Informations.AddString(sz);
		m_Informations.SetCurSel(m_Informations.GetCount() - 1);
	}
	return(FALSE);
}


//
// Démarrer l'opération demandée
//
void CFloppyLowLevel::OnOK() {
	char Tmp[128];

	Break = FALSE;
	Retry = FALSE;
	DmdCancel = FALSE;
	ShowMessage = FALSE;
	Creation = FALSE;
	m_BpOk.EnableWindow(FALSE);
	m_BpFace0.EnableWindow(FALSE);
	m_BpFace1.EnableWindow(FALSE);
	m_BpDriveA.EnableWindow(FALSE);
	m_BpDriveB.EnableWindow(FALSE);
	m_BpDoubleStep.EnableWindow(FALSE);
	m_BpDetectMedia.EnableWindow(FALSE);
	m_BpStopError.EnableWindow(FALSE);
	m_BpDetail.EnableWindow(FALSE);
	int Drive = m_BpDriveA.GetCheck() ? 0 : 1;

	if (InitDriver(Drive, m_BpDetectMedia.GetCheck())) {
		m_BpAccept.EnableWindow(FALSE);
		m_Informations.ResetContent();
		m_Informations.UpdateWindow();
		Tete = (BYTE)m_BpFace1.GetCheck();
		DoubleStep = m_BpDoubleStep.GetCheck();
		Flags = FD_OPTION_MFM;
		switch (TypeCmd) {
		case CMD_WRITE_DIRECT:
			wsprintf(Tmp, GetTexteLoc(78), GetNbTracks(nDSK));
			m_Informations.AddString(Tmp);
			Result = DskToFloppy();
			break;

		case CMD_READ_DIRECT:
			Result = FloppyToDsk();
			break;
		}
		Retry = !Result;
		if (!Result)
			Creation = FALSE;

		MoveToTrack(0);
		CloseHandle(HandleDriver);
		HandleDriver = INVALID_HANDLE_VALUE;
	}
	if (DmdCancel) {
		CDialog::OnOK();
		Creation = FALSE;
	}
	else
		if (!Retry || !m_BpStopError.GetCheck() && !Break) {
			if (TypeCmd == CMD_WRITE_DIRECT)
				CDialog::OnOK();
			else
				m_BpAccept.EnableWindow(TRUE);
		}
		else
			InitInterface();
}


void CFloppyLowLevel::OnCancel() {
	Break = TRUE;
	DmdCancel = TRUE;
	Creation = FALSE;
	if (HandleDriver == INVALID_HANDLE_VALUE)
		CDialog::OnCancel();
}


/**********************************
**                               **
**  Fonctions d'accès au driver  **
**                               **
**********************************/


//
// Retourne la version du driver
//
DWORD CFloppyLowLevel::GetDriverVersion(void) {
	DWORD v = 0;
	HANDLE h = CreateFile("\\\\.\\fdrawcmd", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		DeviceIoControl(h, IOCTL_FDRAWCMD_GET_VERSION, NULL, 0, &v, sizeof(v), &rt, NULL);
		CloseHandle(h);
	}
	return(v);
}


//
// Initialisation du driver
//
HANDLE CFloppyLowLevel::InitDriver(int Drive, BOOL CheckMedia) {
	char szDevice[32];
	int CodeErr = 0;

	if (CheckMedia)
		GetGeometry(Drive);

	wsprintf(szDevice, "\\\\.\\fdraw%u", Drive);
	HandleDriver = CreateFile(szDevice, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (HandleDriver != INVALID_HANDLE_VALUE) {
		BYTE b = 0; // Disable control disk presence
		if (!DeviceIoControl(HandleDriver, IOCTL_FD_SET_DISK_CHECK, &b, sizeof(b), NULL, 0, &rt, NULL))
			CodeErr = 86;
	}
	else
		CodeErr = 87;   // Erreur ouverture driver

	if (CodeErr)
		Erreur(GetTexteLoc(CodeErr));

	return(HandleDriver);
}


//
// Lecture géométrie disquette
//
BOOL CFloppyLowLevel::GetGeometry(int Drive) {
	char szDevice[32];
	DISK_GEOMETRY Dsk[32];

	wsprintf(szDevice, "\\\\.\\%c:", 'A' + Drive);
	HANDLE h = CreateFile(szDevice, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		if (!DeviceIoControl(h, IOCTL_DISK_GET_MEDIA_TYPES, NULL, 0, &Dsk, sizeof(Dsk), &rt, NULL))
			Erreur();

		CloseHandle(h);
	}
	return(TRUE);
}


//
// Lecture infos FDC
//
BOOL CFloppyLowLevel::GetFctInfo(FD_FDC_INFO* Info) {
	return(DeviceIoControl(HandleDriver, IOCTL_FD_GET_FDC_INFO, NULL, 0, Info, sizeof(*Info), &rt, NULL));
}


//
// Déplacement de la tête de lecture vers la piste demandée
//
BOOL CFloppyLowLevel::MoveToTrack(BYTE Piste) {
	return(DeviceIoControl(HandleDriver, IOCTL_FDCMD_SEEK, &Piste, sizeof(Piste), NULL, 0, &rt, NULL));
}


//
// Lecture de la structure d'une piste (format et nombre de secteurs)
//
BOOL CFloppyLowLevel::ReadFloppyLayout(int Piste) {
	BYTE buf[256];
	PFD_SCAN_RESULT psr = (PFD_SCAN_RESULT)buf;
	FD_SCAN_PARAMS sp;

	// Infos de la structure (flags et tête)
	sp.flags = Flags;
	sp.head = (BYTE)Tete;
	// Lecture infos piste
	if (DeviceIoControl(HandleDriver, IOCTL_FD_SCAN_TRACK, &sp, sizeof(sp), psr, sizeof(buf), &rt, NULL)) {
		int NbSects = psr->count;
		if (NbSects) {
			Creation = TRUE;
			if (!Piste)
				InitEntete(nDSK, NbSects, 0);

			SetNbTracks(nDSK, Piste + 1);
			for (int i = 0; i < NbSects; i++) {
				// Initialise fichier DSK en conséquence
				LowLevelFormatTrack(nDSK, Piste, i, NbSects, psr->Header[i].cyl, psr->Header[i].head, psr->Header[i].sector, psr->Header[i].size);
			}
			return(TRUE);
		}
	}
	return(FALSE);
}


//
// Lecture d'une piste (tous les secteurs)
//
BOOL CFloppyLowLevel::ReadTrack(BYTE Piste, CPCEMUTrack* Trk) {
	static FD_READ_WRITE_PARAMS rwp = { 0,	0,	0,	0,	0,	0,	0,	0x2A,       /* #### 0x0A #### */	0xFF };
	MSG msg;
	BOOL b = FALSE;

	rwp.flags = Flags;
	rwp.phead = Tete;
	for (int s = 0; s < Trk->NbSect && !Break; s++) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		// Paramètres de lecture/écriture d'un secteur
		rwp.cyl = Trk->Sect[s].C;
		rwp.head = Trk->Sect[s].H;
		rwp.sector = Trk->Sect[s].R;
		rwp.size = Trk->Sect[s].N;
		rwp.eot = (BYTE)(Trk->Sect[s].R + 1);

		BYTE* Datas = ReadSect(nDSK, Piste, s, FALSE);
		int SectSize = 128 << rwp.size;

		if (m_BpDetail.GetCheck()) {
			wsprintf(Msg, GetTexteLoc(95), s + 1, Trk->NbSect, Trk->Sect[s].C, Trk->Sect[s].H, Trk->Sect[s].R, Trk->Sect[s].N);
			m_Informations.AddString(Msg);
			m_Informations.SetCurSel(m_Informations.GetCount() - 1);
		}
		for (int retry = 0; retry < 5; retry++) {
			// Lecture secteur
			b = DeviceIoControl(HandleDriver, IOCTL_FDCMD_READ_DATA, &rwp, sizeof(rwp), Datas, SectSize, &rt, NULL);
			if (b)
				break;
		}
		if (!b) {
			wsprintf(Msg, GetTexteLoc(119), Trk->Sect[s].R);
			Erreur(Msg);
		}
	}
	return(b);
}


//
// Ecriture d'une piste (tous les secteurs)
//
BOOL CFloppyLowLevel::WriteTrack(BYTE Piste, CPCEMUTrack* Trk) {
	static FD_READ_WRITE_PARAMS rwp = { 0,	0,	0,	0,	0,	0,	0,	0x2A,       /* #### 0x0A #### */	0xFF };
	MSG msg;

	rwp.flags = Flags;
	rwp.phead = Tete;
	for (int s = 0; s < Trk->NbSect && !Break; s++) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		// Paramètres de lecture/écriture d'un secteur
		rwp.cyl = Trk->Sect[s].C;
		rwp.head = Trk->Sect[s].H;
		rwp.sector = Trk->Sect[s].R;
		rwp.size = Trk->Sect[s].N;
		rwp.eot = (BYTE)(Trk->Sect[s].R + 1);

		BYTE* Datas = ReadSect(nDSK, Piste, s, FALSE);
		int SectSize = 128 << rwp.size;

		// Vérifier si il y a effectivement des données à écrire pour
		// le secteur en cours (au moins un octet != octet formatage)
		BOOL NoData = TRUE;

		for (int i = 0; i < SectSize; i++)
			if (Datas[i] != Trk->OctRemp) {
				NoData = FALSE;
				break;
			}

		BOOL b = TRUE;
		if (!NoData) {
			if (m_BpDetail.GetCheck()) {
				wsprintf(Msg, GetTexteLoc(79), s + 1, Trk->NbSect, Trk->Sect[s].C, Trk->Sect[s].H, Trk->Sect[s].R, Trk->Sect[s].N);
				m_Informations.AddString(Msg);
				m_Informations.SetCurSel(m_Informations.GetCount() - 1);
			}
			// Ecriture secteur
			b = DeviceIoControl(HandleDriver, IOCTL_FDCMD_WRITE_DATA, &rwp, sizeof(rwp), Datas, SectSize, &rt, NULL);
		}
		if (!b)
			return(FALSE);
	}
	return(TRUE);
}


//
// Formatage d'une piste
//
BOOL CFloppyLowLevel::FormatTrack(CPCEMUTrack* Trk) {
	BYTE abFormat[sizeof(FD_FORMAT_PARAMS) + sizeof(FD_ID_HEADER) * MAX_SECTS];
	PFD_FORMAT_PARAMS pfp = (PFD_FORMAT_PARAMS)abFormat;

	// Déterminer taille moyenne secteurs et taille piste
	int TaillePiste = 0;
	for (int i = 0; i < Trk->NbSect; i++)
		TaillePiste += Trk->Sect[i].N;

	int TailleGap = (Trk->NbSect + 5997 - TaillePiste * 287) / Trk->NbSect;
	Trk->Gap3 = (BYTE)min(0x4E, TailleGap);

	// Initialisation structure pour formatage
	pfp->flags = Flags;
	pfp->phead = Tete;
	pfp->size = (BYTE)((TaillePiste + Trk->NbSect - 1) / Trk->NbSect);
	pfp->sectors = Trk->NbSect;
	pfp->gap = Trk->Gap3;
	pfp->fill = Trk->OctRemp;

	PFD_ID_HEADER ph = pfp->Header;

	// Initialisation infos de chaque secteurs
	for (BYTE s = 0; s < Trk->NbSect; s++, ph++) {
		ph->cyl = Trk->Sect[s].C;
		ph->head = Trk->Sect[s].H;
		ph->sector = Trk->Sect[s].R;
		ph->size = Trk->Sect[s].N;
	}

	return(DeviceIoControl(HandleDriver, IOCTL_FDCMD_FORMAT_TRACK, pfp, (PBYTE)ph - abFormat, NULL, 0, &rt, NULL));
}


//
// Copie un fichier DSK vers la disquette
//
BOOL CFloppyLowLevel::DskToFloppy(void) {
	BOOL Ret = TRUE;

	int DernPiste = GetNbTracks(nDSK);
	m_Progress.SetRange(0, (short)(DernPiste - 1));

	//
	// Parcourir de 0 à MaxPiste-1 sur la disquette
	//
	for (BYTE Piste = 0; !Break && Piste < DernPiste; Piste++) {
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		m_Progress.SetPos(Piste);

		wsprintf(Msg, GetTexteLoc(80), Piste);
		m_Informations.AddString(Msg);
		m_Informations.SetCurSel(m_Informations.GetCount() - 1);

		// Déplacement de la tête
		if (MoveToTrack(DoubleStep ? (BYTE)(Piste * 2) : Piste)) {
			CPCEMUTrack* t = GetInfoTrack(nDSK, Piste);
			// Formater la piste
			if (FormatTrack(t)) {
				// Ecrire tous les secteurs de la piste
				Ret = !WriteTrack(Piste, t);
			}
			else
				Ret = Erreur();
		}
		else
			Ret = Erreur();
	}
	return(Ret);
}


//
// Lecture de la disquette dans un fichier DSK
//
BOOL CFloppyLowLevel::FloppyToDsk(void) {
	BOOL Ret = FALSE;

	m_Progress.SetRange(0, (short)(MaxPiste - 1));

	//
	// Parcourir de 0 à MaxPiste-1 sur la disquette
	//
	for (BYTE Piste = 0; !Break && Piste < MaxPiste; Piste++) {
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		m_Progress.SetPos(Piste);

		wsprintf(Msg, GetTexteLoc(96), Piste);
		m_Informations.AddString(Msg);
		m_Informations.SetCurSel(m_Informations.GetCount() - 1);

		// Déplacement de la tête
		if (MoveToTrack(DoubleStep ? (BYTE)(Piste * 2) : Piste)) {
			// Lecture strucure des secteurs
			if (ReadFloppyLayout(Piste)) {
				// Lecture de tous les secteurs la piste
				if (ReadTrack(Piste, GetInfoTrack(nDSK, Piste)))
					Ret = TRUE;
				else
					Erreur();
			}
			else
				Erreur(GetTexteLoc(97));
		}
		else
			Erreur();
	}
	return(Ret);
}


void CFloppyLowLevel::OnAccept() {
	CDialog::OnOK();
}
