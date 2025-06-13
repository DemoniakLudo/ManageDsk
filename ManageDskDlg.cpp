// ManageDskDlg.cpp : implementation file
//

#include  <afxwin.h>
#include  <afxcmn.h>
#include  <afxdlgs.h>
#include  <afxext.h>
#include  <afxole.h>

#include  "Outils.h"
#include  "Locale.h"
#include  "GestDsk.h"
#include  "Resource.h"
#include  "ListDir.h"
#include  "OuiNon.h"
#include  "ViewFile.h"
#include  "ManageDsk.h"
#include  "TypeFichier.h"
#include  "InputTracks.h"
#include  "InfoFichier.h"
#include  "ManageDskDlg.h"
#include  "FloppyLowLevel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define     VERSION     "ManageDsk v0.23c "

#define     SIZE_X_1_WINDOWS    498
#define     SIZE_X_2_WINDOWS    990


static BYTE BufFicTmp[0x100000];


/////////////////////////////////////////////////////////////////////////////
// CManageDskDlg dialog

CManageDskDlg::CManageDskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageDskDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//{{AFX_DATA_INIT(CManageDskDlg)
	//}}AFX_DATA_INIT
	IsDsk[DSK_0] = IsDskValid[DSK_0] =
		IsDsk[DSK_1] = IsDskValid[DSK_1] = FALSE;
	IsDskSaved[DSK_0] = IsDskSaved[DSK_1] = TRUE;
}


void CManageDskDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManageDskDlg)
	DDX_Control(pDX, IDC_BP_EN, m_BpEn);
	DDX_Control(pDX, IDC_BP_FR, m_BpFr);
	DDX_Control(pDX, IDC_BP_SP, m_BpSp);
	DDX_Control(pDX, IDC_BP_SWAP, m_BpSwapDsk);
	DDX_Control(pDX, IDC_BP_COMPARE, m_BpCompare);
	DDX_Control(pDX, IDC_NETTOIE, m_Nettoie);
	DDX_Control(pDX, IDC_MAXPISTES, m_MaxPistes);
	DDX_Control(pDX, IDC_DBL_WINDOW, m_DblWindow);

	DDX_Control(pDX, IDC_BP_SEND, m_BpSend[DSK_0]);
	DDX_Control(pDX, IDC_EDIT_FIC, m_BpEditFic[DSK_0]);
	DDX_Control(pDX, IDC_STATIC_MODE_EXP, m_ModeExport[DSK_0]);
	DDX_Control(pDX, IDC_RADIO_EXP_BIN, m_RadioExpBin[DSK_0]);
	DDX_Control(pDX, IDC_RADIO_EXP_ASCII, m_RadioExpAscii[DSK_0]);
	DDX_Control(pDX, IDC_RADIO_EXP_AUTO, m_RadioExpAuto[DSK_0]);

	DDX_Control(pDX, IDC_EDIT_SECTEURS, m_Secteurs[DSK_0]);
	DDX_Control(pDX, IDC_SAUVE_DIRECT, m_BpSauveDirect[DSK_0]);
	DDX_Control(pDX, IDC_STATIC_MODE_AJ, m_ModeAjout[DSK_0]);
	DDX_Control(pDX, IDC_RADIO_FORCE_BIN, m_RadioForceBin[DSK_0]);
	DDX_Control(pDX, IDC_RADIO_FORCE_ASCII, m_RadioForceAscii[DSK_0]);
	DDX_Control(pDX, IDC_RADIO_CHOIX, m_RadioChoix[DSK_0]);
	DDX_Control(pDX, IDC_CONTENU_DSK, m_ContenuDsk[DSK_0]);
	DDX_Control(pDX, IDC_CREER, m_BpCreer[DSK_0]);
	DDX_Control(pDX, IDC_LIRE, m_BpLire[DSK_0]);
	DDX_Control(pDX, IDC_SUPPRIME, m_BpSupprime[DSK_0]);
	DDX_Control(pDX, IDC_SAUVE, m_BpSauve[DSK_0]);
	DDX_Control(pDX, IDC_EXPORT, m_BpExport[DSK_0]);
	DDX_Control(pDX, IDC_RENOMME, m_BpRenomme[DSK_0]);
	DDX_Control(pDX, IDC_AJOUTEFIC, m_BpAjoute[DSK_0]);
	DDX_Control(pDX, IDC_NOMDSK, m_NomDsk[DSK_0]);
	DDX_Control(pDX, IDC_LIST, m_Liste[DSK_0]);
	DDX_Control(pDX, IDC_READ_FLOPPY, m_ReadFloppy[DSK_0]);
	DDX_Control(pDX, IDC_FORMAT_FLOPPY, m_FormatFloppy[DSK_0]);
	DDX_Control(pDX, IDC_WRITE_FLOPPY, m_WriteFloppy[DSK_0]);
	DDX_Control(pDX, IDC_VISU_DIR, m_BpVisuDir[DSK_0]);

	DDX_Control(pDX, IDC_BP_SEND2, m_BpSend[DSK_1]);
	DDX_Control(pDX, IDC_EDIT_FIC2, m_BpEditFic[DSK_1]);
	DDX_Control(pDX, IDC_STATIC_MODE_EXP2, m_ModeExport[DSK_1]);
	DDX_Control(pDX, IDC_RADIO_EXP_BIN2, m_RadioExpBin[DSK_1]);
	DDX_Control(pDX, IDC_RADIO_EXP_ASCII2, m_RadioExpAscii[DSK_1]);
	DDX_Control(pDX, IDC_RADIO_EXP_AUTO2, m_RadioExpAuto[DSK_1]);

	DDX_Control(pDX, IDC_EDIT_SECTEURS2, m_Secteurs[DSK_1]);
	DDX_Control(pDX, IDC_SAUVE_DIRECT2, m_BpSauveDirect[DSK_1]);
	DDX_Control(pDX, IDC_STATIC_MODE_AJ2, m_ModeAjout[DSK_1]);
	DDX_Control(pDX, IDC_RADIO_FORCE_BIN2, m_RadioForceBin[DSK_1]);
	DDX_Control(pDX, IDC_RADIO_FORCE_ASCII2, m_RadioForceAscii[DSK_1]);
	DDX_Control(pDX, IDC_RADIO_CHOIX2, m_RadioChoix[DSK_1]);
	DDX_Control(pDX, IDC_CONTENU_DSK2, m_ContenuDsk[DSK_1]);
	DDX_Control(pDX, IDC_CREER2, m_BpCreer[DSK_1]);
	DDX_Control(pDX, IDC_LIRE2, m_BpLire[DSK_1]);
	DDX_Control(pDX, IDC_SUPPRIME2, m_BpSupprime[DSK_1]);
	DDX_Control(pDX, IDC_SAUVE2, m_BpSauve[DSK_1]);
	DDX_Control(pDX, IDC_EXPORT2, m_BpExport[DSK_1]);
	DDX_Control(pDX, IDC_RENOMME2, m_BpRenomme[DSK_1]);
	DDX_Control(pDX, IDC_AJOUTEFIC2, m_BpAjoute[DSK_1]);
	DDX_Control(pDX, IDC_NOMDSK2, m_NomDsk[DSK_1]);
	DDX_Control(pDX, IDC_LIST2, m_Liste[DSK_1]);
	DDX_Control(pDX, IDC_READ_FLOPPY2, m_ReadFloppy[DSK_1]);
	DDX_Control(pDX, IDC_FORMAT_FLOPPY2, m_FormatFloppy[DSK_1]);
	DDX_Control(pDX, IDC_WRITE_FLOPPY2, m_WriteFloppy[DSK_1]);
	DDX_Control(pDX, IDC_VISU_DIR2, m_BpVisuDir[DSK_1]);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CManageDskDlg, CDialog)
	//{{AFX_MSG_MAP(CManageDskDlg)
	ON_BN_CLICKED(IDC_BP_EN, OnBpEn)
	ON_BN_CLICKED(IDC_BP_FR, OnBpFr)
	ON_BN_CLICKED(IDC_BP_SP, OnBpSp)
	ON_BN_CLICKED(IDC_BP_SWAP, OnBpSwapDsk)
	ON_BN_CLICKED(IDC_BP_COMPARE, OnBpCompare)
	ON_BN_CLICKED(IDC_DBL_WINDOW, OnDblWindow)
	ON_BN_CLICKED(IDC_AJOUTEFIC, OnAjoutefic1)
	ON_BN_CLICKED(IDC_LIRE, OnLire1)
	ON_BN_CLICKED(IDC_SAUVE, OnSauve1)
	ON_BN_CLICKED(IDC_CREER, OnCreer1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList1)
	ON_BN_CLICKED(IDC_EXPORT, OnExport1)
	ON_BN_CLICKED(IDC_RENOMME, OnRenomme1)
	ON_BN_CLICKED(IDC_SUPPRIME, OnSupprime1)
	ON_BN_CLICKED(IDC_RADIO_FORCE_BIN, OnRadioForceBin1)
	ON_BN_CLICKED(IDC_RADIO_CHOIX, OnRadioChoix1)
	ON_BN_CLICKED(IDC_RADIO_FORCE_ASCII, OnRadioForceAscii1)
	ON_BN_CLICKED(IDC_SAUVE_DIRECT, OnSauveDirect1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList1)
	ON_BN_CLICKED(IDC_EDIT_SECTEURS, OnEditSecteurs1)
	ON_BN_CLICKED(IDC_RADIO_EXP_ASCII, OnRadioExpAscii1)
	ON_BN_CLICKED(IDC_RADIO_EXP_BIN, OnRadioExpBin1)
	ON_BN_CLICKED(IDC_RADIO_EXP_AUTO, OnRadioExpAuto1)
	ON_BN_CLICKED(IDC_EDIT_FIC, OnEditFic1)
	ON_BN_CLICKED(IDC_READ_FLOPPY, OnReadFloppy1)
	ON_BN_CLICKED(IDC_WRITE_FLOPPY, OnWriteFloppy1)
	ON_BN_CLICKED(IDC_FORMAT_FLOPPY, OnFormatFloppy1)
	ON_BN_CLICKED(IDC_BP_SEND, OnBpSend1)
	ON_BN_CLICKED(IDC_VISU_DIR, OnBpVisuDir1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnNMCustomdrawList1)

	ON_BN_CLICKED(IDC_AJOUTEFIC2, OnAjoutefic2)
	ON_BN_CLICKED(IDC_LIRE2, OnLire2)
	ON_BN_CLICKED(IDC_SAUVE2, OnSauve2)
	ON_BN_CLICKED(IDC_CREER2, OnCreer2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnRclickList2)
	ON_BN_CLICKED(IDC_EXPORT2, OnExport2)
	ON_BN_CLICKED(IDC_RENOMME2, OnRenomme2)
	ON_BN_CLICKED(IDC_SUPPRIME2, OnSupprime2)
	ON_BN_CLICKED(IDC_RADIO_FORCE_BIN2, OnRadioForceBin2)
	ON_BN_CLICKED(IDC_RADIO_CHOIX2, OnRadioChoix2)
	ON_BN_CLICKED(IDC_RADIO_FORCE_ASCII2, OnRadioForceAscii2)
	ON_BN_CLICKED(IDC_SAUVE_DIRECT2, OnSauveDirect2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDC_EDIT_SECTEURS2, OnEditSecteurs2)
	ON_BN_CLICKED(IDC_RADIO_EXP_ASCII2, OnRadioExpAscii2)
	ON_BN_CLICKED(IDC_RADIO_EXP_BIN2, OnRadioExpBin2)
	ON_BN_CLICKED(IDC_EDIT_FIC2, OnEditFic2)
	ON_BN_CLICKED(IDC_READ_FLOPPY2, OnReadFloppy2)
	ON_BN_CLICKED(IDC_WRITE_FLOPPY2, OnWriteFloppy2)
	ON_BN_CLICKED(IDC_FORMAT_FLOPPY2, OnFormatFloppy2)
	ON_BN_CLICKED(IDC_BP_SEND2, OnBpSend2)
	ON_BN_CLICKED(IDC_VISU_DIR2, OnBpVisuDir2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, OnNMCustomdrawList2)

	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CMyListe, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CManageDskDlg message handlers


enum { COL_NOM, COL_USER, COL_READONLY, COL_HIDDEN, COL_TYPE, COL_TAILLE };


//
// Initialisation de l'interface
//
BOOL CManageDskDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	BoutonsOff(DSK_0);
	BoutonsOff(DSK_1);

	m_BpFr.LoadBitmaps(IDB_FR);
	m_BpEn.LoadBitmaps(IDB_EN);
	m_BpSp.LoadBitmaps(IDB_SP);

	m_RadioChoix[DSK_0].SetCheck(1);
	OnRadioChoix1();
	m_RadioExpAuto[DSK_0].SetCheck(1);
	OnRadioExpAuto1();

	m_RadioChoix[DSK_1].SetCheck(1);
	OnRadioChoix2();
	m_RadioExpAuto[DSK_1].SetCheck(1);
	OnRadioExpAuto2();

	OnDblWindow();
	SetInterface(LOC_FRANCAIS);

	char Msg[128];
	wsprintf(Msg, "%s (Build %s %s)", VERSION, __DATE__, __TIME__);
	SetWindowText(Msg);

	DragAcceptFiles();
	m_Liste[0].DragAcceptFiles();
	m_Liste[0].SetNumDsk(DSK_0);
	m_Liste[0].SetParent(this);
	m_Liste[1].DragAcceptFiles();
	m_Liste[1].SetNumDsk(DSK_1);
	m_Liste[1].SetParent(this);
	return(TRUE);
}


void CManageDskDlg::BoutonsOn(int NumDsk) {
	m_BpSend[DSK_0].EnableWindow(IsDskValid[DSK_0]);
	m_BpSend[DSK_1].EnableWindow(IsDskValid[DSK_1]);
	if (IsDskValid[DSK_0] && IsDskValid[DSK_1]) {
		m_BpSwapDsk.EnableWindow(TRUE);
		m_BpCompare.EnableWindow(TRUE);
	}
	m_BpSauve[NumDsk].EnableWindow(TRUE);
	m_BpSauveDirect[NumDsk].EnableWindow(TRUE);
	m_Secteurs[NumDsk].EnableWindow(TRUE);
	m_WriteFloppy[NumDsk].EnableWindow(TRUE);
	m_FormatFloppy[NumDsk].EnableWindow(TRUE);
	m_BpAjoute[NumDsk].EnableWindow(IsReadable(NumDsk));
	m_BpExport[NumDsk].EnableWindow(IsReadable(NumDsk));
	m_BpRenomme[NumDsk].EnableWindow(IsReadable(NumDsk));
	m_BpSupprime[NumDsk].EnableWindow(IsReadable(NumDsk));
	m_BpEditFic[NumDsk].EnableWindow(IsReadable(NumDsk));
	m_Liste[NumDsk].EnableWindow(IsReadable(NumDsk));
	m_BpVisuDir[NumDsk].EnableWindow(IsReadable(NumDsk));
}


void CManageDskDlg::BoutonsOff(int NumDsk) {
	m_BpSend[NumDsk].EnableWindow(FALSE);
	m_BpSwapDsk.EnableWindow(FALSE);
	m_BpCompare.EnableWindow(FALSE);
	m_BpSauve[NumDsk].EnableWindow(FALSE);
	m_BpSauveDirect[NumDsk].EnableWindow(FALSE);
	m_Secteurs[NumDsk].EnableWindow(FALSE);
	m_BpAjoute[NumDsk].EnableWindow(FALSE);
	m_BpExport[NumDsk].EnableWindow(FALSE);
	m_BpRenomme[NumDsk].EnableWindow(FALSE);
	m_BpSupprime[NumDsk].EnableWindow(FALSE);
	m_BpEditFic[NumDsk].EnableWindow(FALSE);
	m_WriteFloppy[NumDsk].EnableWindow(FALSE);
	m_FormatFloppy[NumDsk].EnableWindow(FALSE);
	m_Liste[NumDsk].EnableWindow(FALSE);
	m_BpVisuDir[NumDsk].EnableWindow(FALSE);
}


BOOL CManageDskDlg::IsReadable(int nDSK) {
	return(::IsReadable(nDSK));
}


//
// Fonction de tri des fichiers
//
int __cdecl FctTri(const void* a1, const void* a2) {
	static char Nom1[12];
	static char Nom2[12];

	memset(Nom1, 0, sizeof(Nom1));
	memset(Nom2, 0, sizeof(Nom2));
	memcpy(Nom1, (char*)((StDirEntry*)a1)->Nom, 11);
	memcpy(Nom2, (char*)((StDirEntry*)a2)->Nom, 11);

	//BYTE User1 = ( ( StDirEntry * )a1 )->User;
	//BYTE User2 = ( ( StDirEntry * )a2 )->User;
	//if ( User1 != User2 )
	//    return( User1 - User2 );

	//
	// Masquer les bits 7 dans les noms (par exemples : bits d'attributs)
	//
	Nom1[11] = Nom2[11] = 0;
	for (int i = 0; i < 11; i++) {
		Nom1[i] &= 0x7F;
		Nom2[i] &= 0x7F;
	}

	int a = strcmp(Nom1, Nom2);
	if (!a)
		a = ((StDirEntry*)a1)->NumPage - ((StDirEntry*)a2)->NumPage;

	return(a);
}


//
// Lecture et affichage du contenu d'un fichier DSK
//
void CManageDskDlg::ReadDirDsk(int nDSK) {
	CListCtrl* Liste = &m_Liste[nDSK];
	int i;
	//
	// Tout remettre à zéro
	//
	m_Liste[nDSK].DeleteAllItems();
	memset(PosItem[nDSK], 0, sizeof(PosItem[nDSK]));

	if (IsReadable(nDSK)) // V0.16
	{
		int NbItems = 0;
		//
		// Récupère la table des répertoires
		//
		for (i = 0; i < 64; i++)
			memcpy(&TabDir[nDSK][i]
				, GetInfoDirEntry(nDSK, i)
				, sizeof(StDirEntry)
			);

		// Trier les fichiers
		qsort(TabDir[nDSK], 64, sizeof(TabDir[0][0]), FctTri);

		//
		// Affichage...
		//
		for (i = 0; i < 64; i++) {
			StDirEntry* Dir = &TabDir[nDSK][i];
			SetInfoDirEntry(nDSK, i, Dir);
			if (!Dir->NumPage) {
				char Nom[13];
				memcpy(Nom, Dir->Nom, 8);

				BOOL IsNomValid = FALSE;
				for (int c = 0; c < 8; c++)
					if (Nom[c])
						IsNomValid = TRUE;

				if (IsNomValid) {
					memcpy(&Nom[9], Dir->Ext, 3);
					Nom[8] = '.';
					Nom[12] = 0;
					//
					// Masquer les bits d'attributs
					//
					for (int j = 0; j < 12; j++)
						Nom[j] &= 0x7F;

					int User = Dir->User;
					Liste->InsertItem(NbItems, GetNomAmsdos((BYTE*)Nom));
					StAmsdos* Ams = (StAmsdos*)ReadBloc(nDSK, Dir->Blocks[0]);
					Liste->SetItemText(NbItems, COL_USER, GetUser(User));
					Liste->SetItemText(NbItems, COL_READONLY, Dir->Ext[0] & 0x80 ? "[X]" : "");
					Liste->SetItemText(NbItems, COL_HIDDEN, Dir->Ext[1] & 0x80 ? "[X]" : "");
					Liste->SetItemText(NbItems, COL_TYPE, GetType(Ams));

					//
					// Calcule la taille du fichier en fonction du nombre de blocs
					//
					int p = 0, t = 0;
					do {
						if (TabDir[nDSK][p + i].User == Dir->User && !CompareNomsAmsdos((char*)TabDir[nDSK][p + i].Nom, (char*)Dir->Nom))
							t += TabDir[nDSK][p + i].NbPages;

						p++;
					} while (TabDir[nDSK][p + i].NumPage && (p + i) < 64);
					Liste->SetItemText(NbItems, COL_TAILLE, GetTaille((t + 7) >> 3));
					PosItem[nDSK][NbItems++] = i;
				}
			}
		}
		//
		// Affichage taille utilisée par tous les fichiers
		//
		int KoUsed = FillBitmap(nDSK);
		sprintf(Msg
			, GetTexteLoc(8)
			, KoUsed
			, GetMaxBlocs(m_MaxPistes.GetCheck(), nDSK) - KoUsed
		);
		m_ContenuDsk[nDSK].SetWindowText(Msg);
	}
}


//
// Ajouter un fichier
//
void CManageDskDlg::AjouteUnFic(char* Nom, int nDSK) {
	static BYTE Buff[0x80000];
	char NomFic[16];
	DWORD Lg;
	int LgE = sizeof(StAmsdos);       // Taille En-tête

	if (IsDskValid[nDSK]) {
		char* p = Nom;
		char* q = p;
		char* r;
		//
			// Normalise le nom du fichier en cours
		//
		do {
			r = strchr(q, ':');
			if (!r)
				r = strchr(q, '\\');

			if (r)
				q = ++r;
		} while (r);
		strcpy(NomFic, GetNomAmsdos((BYTE*)q));

		HANDLE fp = CreateFile(p, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (fp != INVALID_HANDLE_VALUE) {
			memset(Buff, 0, sizeof(Buff));
			// Lecture fichier
			ReadFile(fp, &Buff, sizeof(Buff), &Lg, NULL);
			CloseHandle(fp);
			BOOL AjouteEntete = FALSE;
			StAmsdos* e = (StAmsdos*)Buff;
			// Attention : longueur > 64Ko !
			BOOL FicValid = TRUE;
			if (Lg > 0x10080)
				FicValid = (MessageBox(GetTexteLoc(15)
					, NomFic
					, MB_YESNO | MB_ICONQUESTION
				) == IDYES
					);
			if (FicValid) {
				//
				// Regarde si le fichier contient une en-tête ou non
				//
				BOOL IsAmsdos = CheckAmsdos(Buff);

				if (!IsAmsdos)
					// Créer une en-tête amsdos par défaut
					e = CreeEnteteAmsdos(NomFic, Lg);

				CTypeFichier DialType(TypeModeImport[nDSK], e, IsAmsdos, NomFic);
				//
				// En fonction du mode d'importation...
				//
				switch (TypeModeImport[nDSK]) {
				case MODE_ASCII:
					//
					// Importation en mode ASCII
					//
					if (IsAmsdos) {
						// Supprmier en-tête si elle existe
						memcpy(Buff, &Buff[LgE], Lg - LgE);
						Lg -= LgE;
						IsAmsdos = FALSE;
					}
					FicValid = TRUE;
					break;

				case MODE_BINAIRE:
					//
					// Importation en mode BINAIRE
					//
					DialType.DoModal();
					if (!IsAmsdos)
						//
						// Indique qu'il faudra ajouter une en-tête
						//
						AjouteEntete = TRUE;
					break;

				case MODE_USER:
					//
					// Importation en mode choix utilisateur
					//
					DialType.DoModal();
					if (DialType.GetTypeModeImport() == MODE_BINAIRE
						&& !IsAmsdos
						)
						//
						// Indique qu'il faudra ajouter une en-tête
						//
						AjouteEntete = TRUE;
					break;
				}
			}
			//
			// Si fichier ok pour être importé
			//
			if (FicValid) {
				if (m_Nettoie.GetCheck() /*&& IsAmsdos*/)
					ClearAmsdos(Buff);

				if (AjouteEntete) {
					// Ajoute l'en-tête amsdos si nécessaire
					memmove(&Buff[LgE], Buff, Lg);
					memcpy(Buff, e, LgE);
					Lg += LgE;
				}

				// Ajoute le fichier sur le DSK
				TryCopie(nDSK
					, Buff
					, NomFic
					, Lg
					, GetMaxBlocs(m_MaxPistes.GetCheck(), nDSK)
					, e->UserNumber
				);
			}
		}
	}
}


//
// Ajouter des fichiers
//
void CManageDskDlg::OnAjoutefic(int nDSK) {
	static char Noms[0x1000];
	static char LastDir[2][256];

	CFileDialog f(TRUE
		, NULL
		, NULL
		, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT
		, GetTexteLoc(14)
	);
	*Noms = 0;
	f.m_ofn.lpstrFile = Noms;
	f.m_ofn.nMaxFile = sizeof(Noms);
	f.m_ofn.lpstrInitialDir = LastDir[nDSK];
	if (f.DoModal() != IDCANCEL) {
		POSITION PosFic = f.GetStartPosition();
		while (PosFic) {
			//
			// Parcourir tous les fichiers sélectionnés
			//
			CString Path = f.GetNextPathName(PosFic);
			char* p = (char*)(const char*)Path;
			strcpy(LastDir[nDSK], p);
			AjouteUnFic(p, nDSK);
		}
	}
}


//
// Créer un DSK vierge
//
void CManageDskDlg::OnCreer(int nDSK) {
	IsDsk[nDSK] = IsDskValid[nDSK] = IsDskSaved[nDSK] = TRUE;
	FormatDsk(nDSK, 9, m_MaxPistes.GetCheck() ? MAX_TRACKS : MAX_PISTE_STD);
	ReadDirDsk(nDSK);
	//
	// Autoriser les boutons de l'interface
	//
	BoutonsOn(nDSK);

	// Nom par défaut du DSK
	strcpy(NomDsk[nDSK], "Disquette_Vide.DSK");
	m_NomDsk[nDSK].SetWindowText(NomDsk[nDSK]);
}


void CManageDskDlg::CheckInterlace(int nDSK) {
	BOOL Interlace = TRUE;
	int t, NbTracks = GetNbTracks(nDSK);

	for (t = 0; t < NbTracks; t++)
		if (!IsTrackInterlaced(nDSK, t)) {
			Interlace = FALSE;
			break;
		}

	if (!Interlace) {
		if (MessageBox(GetTexteLoc(110), NULL, MB_YESNO) == IDYES) {
			IsDskSaved[nDSK] = FALSE;
			for (t = 0; t < NbTracks; t++)
				InterlaceTrack(nDSK, t);
		}
	}
}


BOOL CManageDskDlg::CheckSaved(int nDSK) {
	if (!IsDskSaved[nDSK]) {
		sprintf(Msg, GetTexteLoc(72), NomDsk[nDSK]);
		if (MessageBox(Msg, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
			IsDskSaved[nDSK] = TRUE;
	}
	return(IsDskSaved[nDSK]);
}


void CManageDskDlg::ReadNewDSK(char* Nom, int nDSK) {
	if (CheckSaved(nDSK)) {
		strcpy(NomDsk[nDSK], Nom);
		m_NomDsk[nDSK].SetWindowText(Nom);

		// Lecture DSK
		if (ReadDsk(nDSK, Nom)) {
			IsDsk[nDSK] = TRUE;

			// Vérifier validité (format standard) du DSK
			IsDskValid[nDSK] = CheckDsk(nDSK);
			if (!IsDskValid[nDSK])
				MessageBox(GetTexteLoc(20));
			else
				if (IsReadable(nDSK)) {
					//
					// Récupère la table des répertoires initiale
					//
					for (int i = 0; i < 64; i++)
						memcpy(&TabDirInit[nDSK][i]
							, GetInfoDirEntry(nDSK, i)
							, sizeof(StDirEntry)
						);
					CheckInterlace(nDSK);
				}
		}
		else {
			wsprintf(Msg, GetTexteLoc(75), Nom);
			MessageBox(Msg, NULL, MB_ICONEXCLAMATION);
			IsDskValid[nDSK] = FALSE;
		}
		BoutonsOn(nDSK);
		ReadDirDsk(nDSK);
	}
}


//
// Lecture d'un fichier DSK
//
void CManageDskDlg::OnLire(int nDSK) {
	static char LastDir[2][256];

	if (CheckSaved(nDSK)) {
		CFileDialog f(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, GetTexteLoc(19));
		f.m_ofn.lpstrInitialDir = LastDir[nDSK];
		if (f.DoModal() == IDOK)
			ReadNewDSK((char*)(const char*)f.GetPathName(), nDSK);
	}
}


//
// Sauvegarde du fichier DSK en demandant le nom
//
void CManageDskDlg::OnSauve(int nDSK) {
	static char LastDir[2][256];
	char* Nom = NomDsk[nDSK];
	if (IsDsk[nDSK]) {
		CFileDialog f(FALSE
			, "*.DSK"
			, Nom
			, OFN_OVERWRITEPROMPT
			, GetTexteLoc(19)
		);
		f.m_ofn.lpstrInitialDir = LastDir[nDSK];
		if (f.DoModal() == IDOK) {
			IsDskSaved[nDSK] = TRUE;
			strcpy(Nom, f.GetPathName());
			strcpy(LastDir[nDSK], Nom);
			m_NomDsk[nDSK].SetWindowText(Nom);
			if (WriteDsk(nDSK, Nom))
				memcpy(TabDirInit[nDSK], TabDir[nDSK], sizeof(TabDir[0]));
			else {
				wsprintf(Msg, GetTexteLoc(130), Nom);
				MessageBox(Msg);
			}
		}
	}
}


//
// Sauvegarde "directe" du fichier DSK
//
void CManageDskDlg::OnSauveDirect(int nDSK) {
	char* Nom = NomDsk[nDSK];
	if (*Nom) {
		IsDskSaved[nDSK] = TRUE;
		if (WriteDsk(nDSK, Nom)) {
			memcpy(TabDirInit[nDSK], TabDir[nDSK], sizeof(TabDir[0]));
			wsprintf(Msg, GetTexteLoc(47), Nom);
			MessageBox(Msg);
		}
		else {
			wsprintf(Msg, GetTexteLoc(130), Nom);
			MessageBox(Msg);
		}
	}
	else
		MessageBox(GetTexteLoc(48));
}


//
// Gestion du "click droit" sur la liste des fichiers
//
void CManageDskDlg::OnRclickList(int nDSK, NMHDR* pNMHDR) {
	char Tmp[128];
	CListCtrl* Liste = &m_Liste[nDSK];
	POSITION pos = Liste->GetFirstSelectedItemPosition();
	if (pos) {
		int nItem = Liste->GetNextSelectedItem(pos);
		// Recherche le direntry correspondant
		int i = PosItem[nDSK][nItem];
		CInfoFichier Info(nDSK, TabDir[nDSK], i, FALSE);
		Info.DoModal();
		if (Info.IsValide()) {
			char NomFic[16];
			BOOL First = FALSE;
			char* p;

			strcpy(NomFic, GetNomAmsdos(TabDir[nDSK][i].Nom));
			int User = TabDir[nDSK][i].User;
			int NewUser = atoi(Info.GetUser());
			int ind = -1;
			if (User != NewUser) {
				ind = FileExist(nDSK, NomFic, NewUser);
				if (ind != -1) {
					sprintf(Tmp, GetTexteLoc(68), NomFic);
					if (MessageBox(Tmp, NULL, MB_YESNO) == IDYES) {
						Supprime(nDSK, ind, FALSE);
						ind = -1;
					}
				}
			}
			if (ind == -1) {
				IsDskSaved[nDSK] = FALSE; // Modifications effectuées sur la disquette
				do {
					//
					// Relecture du premier bloc du fichier
					//
					BYTE* b = ReadBloc(nDSK, TabDir[nDSK][i].Blocks[0]);
					BYTE* e = (BYTE*)Info.GetAms();
					//
					// Recopie l'en-tête amsdos modifiée par l'utilisateur
					//
					if (!First && CheckAmsdos(b)) {
						memcpy(b, e, sizeof(StAmsdos));
						First = TRUE;
					}

					//
					// Numéro User
					//
					TabDir[nDSK][i].User = (BYTE)NewUser;

					//
					// Positionne le flag "read only"
					//
					if (Info.IsReadOnly())
						TabDir[nDSK][i].Ext[0] |= 0x80;
					else
						TabDir[nDSK][i].Ext[0] &= ~0x80;

					//
					// Positionne le flag "système" (fichier caché)
					//
					if (Info.IsHidden())
						TabDir[nDSK][i].Ext[1] |= 0x80;
					else
						TabDir[nDSK][i].Ext[1] &= ~0x80;

					SetInfoDirEntry(nDSK, i, &TabDir[nDSK][i]);
					//
					// Réécriture du premier bloc du fichier
					//
					WriteBloc(nDSK, TabDir[nDSK][i].Blocks[0], b);
					p = GetNomAmsdos(TabDir[nDSK][++i].Nom);
				} while (!strncmp(NomFic, p, max(strlen(p), strlen(NomFic))) && User == TabDir[nDSK][i].User);
			}
			ReadDirDsk(nDSK);
		}
	}
}



int CManageDskDlg::GetFic(int nDSK, int Indice, int Ko, BYTE* BufOut, BOOL Strict) {
	int PosBufOut = 0;
	char NomFic[16];
	int i = Indice;
	char* p;
	int lMax = 0x1000000;
	BOOL FirstBlock = TRUE;
	int LgE = sizeof(StAmsdos);       // Taille En-Tête

	strcpy(NomFic, GetNomAmsdos(TabDir[nDSK][i].Nom));
	int User = TabDir[nDSK][i].User;
	do {
		// Longueur du fichier
		int l = (TabDir[nDSK][i].NbPages + 7) >> 3;
		for (int j = 0; j < l; j++) {
			int TailleBloc = 1024;
			BYTE* p = ReadBloc(nDSK, TabDir[nDSK][i].Blocks[j]);
			if (FirstBlock) {
				if (CheckAmsdos(p)) {
					StAmsdos* Ams = (StAmsdos*)p;
					if (Ams->Length >= Ams->LogicalLength)
						lMax = Ams->Length + LgE;
					else
						if (Ams->LogicalLength)
							lMax = Ams->LogicalLength + LgE;
						else
							if (Ams->RealLength)
								lMax = Ams->RealLength + LgE;

					lMax += (Ams->BigLength << 16);
					//
					// Supprime en-tête si mode ASCII
					//
					if (TypeModeExport[nDSK] == MODE_ASCII && !Strict) {
						TailleBloc -= LgE;
						lMax -= LgE;
						memcpy(p, &p[LgE], TailleBloc);
					}
				}
				else {
					if (TypeModeExport[nDSK] == MODE_BINAIRE && !Strict) {
						//
						// Ajoute en-tête amsdos
						//
						StAmsdos* e = CreeEnteteAmsdos(NomFic, (USHORT)(Ko << 10));
						memcpy(&BufOut[PosBufOut], e, LgE);
						PosBufOut += LgE;
					}
				}
				if (m_Nettoie.GetCheck() && TailleBloc == 1024)
					ClearAmsdos(p);

				FirstBlock = FALSE;
			}
			int NbOctets = min(lMax, TailleBloc);
			if (NbOctets > 0) {
				memcpy(&BufOut[PosBufOut], p, NbOctets);
				PosBufOut += NbOctets;
			}
			lMax -= NbOctets;
		}
		p = GetNomAmsdos(TabDir[nDSK][++i].Nom);
	} while (!strncmp(NomFic, p, max(strlen(p), strlen(NomFic))) && User == TabDir[nDSK][i].User);
	return(PosBufOut);
}


//
// Export de fichiers
//
void CManageDskDlg::OnExport(int nDSK) {
	static char LastDir[2][256];
	DWORD Lg;

	CListCtrl* Liste = &m_Liste[nDSK];
	POSITION pos = Liste->GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = Liste->GetNextSelectedItem(pos);
		Liste->GetItemText(nItem, COL_TAILLE, Msg, sizeof(Msg));
		int Ko = atoi(Msg);
		// Recherche le direntry correspondant
		int Indice = PosItem[nDSK][nItem];
		CFileDialog SelectFic(FALSE, NULL, GetNomAmsdos(TabDir[nDSK][Indice].Nom), OFN_OVERWRITEPROMPT, NULL);
		SelectFic.m_ofn.lpstrInitialDir = LastDir[nDSK];
		if (SelectFic.DoModal() == IDOK) {
			strcpy(LastDir[nDSK], SelectFic.GetPathName());
			HANDLE f = CreateFile(SelectFic.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (f != INVALID_HANDLE_VALUE) {
				int NbOctets = GetFic(nDSK, Indice, Ko, BufFicTmp);
				WriteFile(f, BufFicTmp, NbOctets, &Lg, NULL);
				CloseHandle(f);
			}
		}
	}
}


//
// Renommer des fichiers
//
void CManageDskDlg::OnRenomme(int nDSK) {
	char NomFic[32];
	char Tmp[128];
	BOOL Modif = FALSE;

	CListCtrl* Liste = &m_Liste[nDSK];
	POSITION pos = Liste->GetFirstSelectedItemPosition();
	while (pos) {
		//
		// Parcourir tous les fichiers sélectionnés
		//
		int nItem = Liste->GetNextSelectedItem(pos);
		// Recherche le direntry correspondant
		int i = PosItem[nDSK][nItem];
		CInfoFichier Info(nDSK, TabDir[nDSK], i, TRUE);
		Info.DoModal();
		//
		// Si appui sur bp "valider"
		//
		if (Info.IsValide()) {
			StDirEntry DirLoc;

			memset(DirLoc.Nom, ' ', 8);
			memset(DirLoc.Ext, ' ', 3);
			int User = atoi(Info.GetUser());
			strcpy(NomFic, Info.GetNom());
			char* p = strchr(NomFic, '.');
			if (p) {
				p++;
				memcpy(DirLoc.Nom, NomFic, p - NomFic - 1);
				memcpy(DirLoc.Ext, p, min(strlen(p), 3));
			}
			else
				memcpy(DirLoc.Nom, NomFic, min(strlen(NomFic), 8));

			int ind = FileExist(nDSK, (char*)DirLoc.Nom, User);
			if (ind != -1) {
				sprintf(Tmp, GetTexteLoc(68), NomFic);
				if (MessageBox(Tmp, NULL, MB_YESNO) == IDYES) {
					Supprime(nDSK, ind, FALSE);
					ind = -1;
				}
			}
			if (ind == -1) {
				strcpy(NomFic, GetNomAmsdos(TabDir[nDSK][i].Nom));
				DirLoc.User = (BYTE)User;
				do {
					TabDir[nDSK][i].User = (BYTE)User;
					memcpy(TabDir[nDSK][i].Nom, DirLoc.Nom, 11);
					SetInfoDirEntry(nDSK, i, &TabDir[nDSK][i]);
					p = GetNomAmsdos(TabDir[nDSK][++i].Nom);
				} while (!strncmp(NomFic, p, max(strlen(p), strlen(NomFic))) && User == TabDir[nDSK][i].User);
				Modif = TRUE;
				IsDskSaved[nDSK] = FALSE; // Modifications effectuées sur la disquette
			}
		}
	}
	//
	// Si au moins 1 fichier renomé, réafficher le contenu du DSK
	//
	if (Modif)
		ReadDirDsk(nDSK);
}


//
// Suppression d'un fichier
//
void CManageDskDlg::Supprime(int nDSK, int Indice, BOOL Kill) {
	static BYTE BufBloc[1024];
	char NomFic[16];
	int i = Indice;
	strcpy(NomFic, GetNomAmsdos(TabDir[nDSK][i].Nom));
	int User = TabDir[nDSK][i].User;
	char* p;
	do {
		TabDir[nDSK][i].User = USER_DELETED;
		if (Kill) {
			memset(BufBloc, USER_DELETED, sizeof(BufBloc));
			for (int b = 0; b < 16; b++) {
				int numB = TabDir[nDSK][i].Blocks[b];
				if (numB && numB != USER_DELETED)
					WriteBloc(nDSK, numB, BufBloc);
			}
			memset(&TabDir[nDSK][i], USER_DELETED, sizeof(StDirEntry));
		}
		SetInfoDirEntry(nDSK, i, &TabDir[nDSK][i]);
		p = GetNomAmsdos(TabDir[nDSK][++i].Nom);
	} while (!strncmp(NomFic
		, p
		, max(strlen(p), strlen(NomFic))
	)
		&& User == TabDir[nDSK][i].User
		);
}


//
// Supprime des fichiers
//
void CManageDskDlg::OnSupprime(int nDSK) {
	int Retour = RET_NON;
	BOOL Kill = FALSE;

	CListCtrl* Liste = &m_Liste[nDSK];
	POSITION pos = Liste->GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = Liste->GetNextSelectedItem(pos);
		// Recherche le direntry correspondant
		int Indice = PosItem[nDSK][nItem];
		if (Retour != RET_OUITOUT) {
			COuiNon d(GetNomAmsdos(TabDir[nDSK][Indice].Nom)
				, GetTexteLoc(21)
				, pos != NULL
			);
			d.DoModal();
			Retour = d.GetRet();
			Kill = d.GetKill();
		}
		if (Retour == RET_OUI || Retour == RET_OUITOUT) {
			IsDskSaved[nDSK] = FALSE; // Modifications effectuées sur la disquette
			Supprime(nDSK, Indice, Kill);
		}
	}
	ReadDirDsk(nDSK);
}


//
// Initialisation de l'interface (textes en fonction de la langue)
//
void CManageDskDlg::SetInterface(int l) {
	Langue = l;
	SetLangue(l);
	for (int nDSK = 0; nDSK < 2; nDSK++) {
		CListCtrl* Liste = &m_Liste[nDSK];
		int nColumnCount = Liste->GetHeaderCtrl()->GetItemCount();
		for (int i = 0; i < nColumnCount; i++)
			Liste->DeleteColumn(0);

		Liste->InsertColumn(COL_NOM, GetTexteLoc(10), LVCFMT_LEFT, 130);
		Liste->InsertColumn(COL_USER, GetTexteLoc(11), LVCFMT_LEFT, 50);
		Liste->InsertColumn(COL_READONLY, GetTexteLoc(49), LVCFMT_LEFT, 80);
		Liste->InsertColumn(COL_HIDDEN, GetTexteLoc(50), LVCFMT_LEFT, 50);
		Liste->InsertColumn(COL_TYPE, GetTexteLoc(12), LVCFMT_LEFT, 70);
		Liste->InsertColumn(COL_TAILLE, GetTexteLoc(13), LVCFMT_LEFT, 70);
		ReadDirDsk(nDSK);
		int KoUsed = FillBitmap(nDSK);
		sprintf(Msg
			, GetTexteLoc(8)
			, KoUsed
			, GetMaxBlocs(m_MaxPistes.GetCheck(), nDSK) - KoUsed
		);
		m_ContenuDsk[nDSK].SetWindowText(Msg);

		// Astuce pour enlever le '&' dans les boutons de la fenêtre DSK_1
		m_BpAjoute[nDSK].SetWindowText(GetTexteLoc(1) + nDSK);
		m_BpAjoute[nDSK].UpdateWindow();
		m_BpExport[nDSK].SetWindowText(GetTexteLoc(2) + nDSK);
		m_BpExport[nDSK].UpdateWindow();
		m_BpRenomme[nDSK].SetWindowText(GetTexteLoc(3) + nDSK);
		m_BpRenomme[nDSK].UpdateWindow();
		m_BpSupprime[nDSK].SetWindowText(GetTexteLoc(4) + nDSK);
		m_BpSupprime[nDSK].UpdateWindow();

		m_BpCreer[nDSK].SetWindowText(GetTexteLoc(5));
		m_BpLire[nDSK].SetWindowText(GetTexteLoc(6));
		m_BpSauve[nDSK].SetWindowText(GetTexteLoc(7));
		m_ModeAjout[nDSK].SetWindowText(GetTexteLoc(42));
		m_RadioForceBin[nDSK].SetWindowText(GetTexteLoc(43));
		m_RadioForceAscii[nDSK].SetWindowText(GetTexteLoc(44));
		m_RadioChoix[nDSK].SetWindowText(GetTexteLoc(45));
		m_BpSauveDirect[nDSK].SetWindowText(GetTexteLoc(46));
		m_Secteurs[nDSK].SetWindowText(GetTexteLoc(51));
		m_ModeExport[nDSK].SetWindowText(GetTexteLoc(55));
		m_RadioExpBin[nDSK].SetWindowText(GetTexteLoc(56));
		m_RadioExpAscii[nDSK].SetWindowText(GetTexteLoc(57));
		m_BpEditFic[nDSK].SetWindowText(GetTexteLoc(58));
		m_WriteFloppy[nDSK].SetWindowText(GetTexteLoc(81));
		m_ReadFloppy[nDSK].SetWindowText(GetTexteLoc(82));
		m_BpVisuDir[nDSK].SetWindowText(GetTexteLoc(112));
		m_FormatFloppy[nDSK].SetWindowText(GetTexteLoc(122));
	}

	m_BpSend[DSK_0].SetWindowText(GetTexteLoc(76));
	m_BpSend[DSK_1].SetWindowText(GetTexteLoc(107));
	m_Nettoie.SetWindowText(GetTexteLoc(9));
	m_MaxPistes.SetWindowText(GetTexteLoc(71));
	m_BpSwapDsk.SetWindowText(GetTexteLoc(77));
	m_DblWindow.SetWindowText(GetTexteLoc(85));
	m_BpCompare.SetWindowText(GetTexteLoc(105));
}


//
// Voir un fichier
//
void CManageDskDlg::OnEditFic(int nDSK) {
	static BYTE BufFile[MAX_FILESIZE];
	int LongFic = 0;
	int LgE = sizeof(StAmsdos);       // Taille En-Tête

	memset(BufFile, 0, sizeof(BufFile));
	CListCtrl* Liste = &m_Liste[nDSK];
	POSITION pos = Liste->GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = Liste->GetNextSelectedItem(pos);
		Liste->GetItemText(nItem, COL_TAILLE, Msg, sizeof(Msg));

		// Recherche le direntry correspondant
		int Indice = PosItem[nDSK][nItem];
		char NomFic[16];
		int i = Indice;
		BOOL FirstBlock = TRUE;
		StAmsdos* FirstEntry = (StAmsdos*)ReadBloc(nDSK, TabDir[nDSK][i].Blocks[0]);
		BOOL Type = FirstEntry->FileType;
		strcpy(NomFic, GetNomAmsdos(TabDir[nDSK][i].Nom));
		int User = TabDir[nDSK][i].User;
		char* p;
		int lMax = sizeof(BufFile);
		do {
			// Longueur du fichier
			int l = (TabDir[nDSK][i].NbPages + 7) >> 3;
			for (int j = 0; j < l; j++) {
				int TailleBloc = 1024;
				BYTE* p = ReadBloc(nDSK, TabDir[nDSK][i].Blocks[j]);
				if (FirstBlock) {
					if (CheckAmsdos(p)) {
						StAmsdos* Ams = (StAmsdos*)p;
						if (Ams->Length >= Ams->LogicalLength)
							lMax = Ams->Length + LgE;
						else
							if (Ams->LogicalLength)
								lMax = Ams->LogicalLength + LgE;
							else
								if (Ams->RealLength)
									lMax = Ams->RealLength + LgE;

						lMax += (Ams->BigLength << 16);
					}
					FirstBlock = FALSE;
				}
				int NbOctets = min(lMax, TailleBloc);
				if (NbOctets > 0) {
					memcpy(&BufFile[LongFic], p, NbOctets);
					LongFic += NbOctets;
				}
				lMax -= 1024;
			}
			p = GetNomAmsdos(TabDir[nDSK][++i].Nom);
		} while (!strncmp(NomFic, p, max(strlen(p), strlen(NomFic))) && User == TabDir[nDSK][i].User);
		CViewFile(NomFic, BufFile, LongFic, TypeModeExport[nDSK], nDSK, Langue, Type == 1).DoModal();
	}
}


void CManageDskDlg::TryCopie(int nDSK, BYTE* Buff, char* NomFic, int Lg, int MaxBlocs, int User) {
	char Tmp[128];
	int Retry = 0;
	char NomLoc[16];

	strcpy(NomLoc, NomFic);
	do {
		Retry = 0;
		// Ajoute le fichier sur le DSK
		int Res = CopieFichier(nDSK, Buff, NomLoc, Lg, MaxBlocs, User);
		if (Res == ERR_NO_ERR) {
			ReadDirDsk(nDSK);
			IsDskSaved[nDSK] = FALSE; // Modifications effectuées sur la disquette
		}
		else {
			// Erreur d'ajout du fichier...
			switch (Res) {
			case ERR_NO_DIRENTRY:
				MessageBox(GetTexteLoc(17));
				break;

			case ERR_NO_BLOCK:
				MessageBox(GetTexteLoc(18));
				break;

			case ERR_FILE_EXIST:
				sprintf(Tmp, GetTexteLoc(68), NomLoc);
				if (MessageBox(Tmp, NULL, MB_YESNO) == IDYES) {
					StDirEntry* d = GetNomDir(NomLoc);
					Supprime(nDSK, FileExist(nDSK, (char*)d->Nom, User), FALSE);
					Retry = 1;
				}
				break;
			}
		}
	} while (Retry);
}

//
// Copie de fichiers du DSK x vers le DSK y
//
void CManageDskDlg::OnSend(int nDSK_Src) {
	int nDSK_Dest = nDSK_Src == DSK_0 ? DSK_1 : DSK_0;

	if (IsDskValid[nDSK_Src]) {
		POSITION pos = m_Liste[nDSK_Src].GetFirstSelectedItemPosition();
		if (pos && IsDskValid[nDSK_Dest]) // Copie de fichiers
		{
			while (pos) {
				int nItem = m_Liste[nDSK_Src].GetNextSelectedItem(pos);
				m_Liste[nDSK_Src].GetItemText(nItem, COL_TAILLE, Msg, sizeof(Msg));
				int Ko = atoi(Msg);
				// Recherche le direntry correspondant
				int Indice = PosItem[nDSK_Src][nItem];
				TryCopie(nDSK_Dest
					, BufFicTmp
					, GetNomAmsdos(TabDir[nDSK_Src][Indice].Nom)
					, GetFic(nDSK_Src, Indice, Ko, BufFicTmp, TRUE)
					, GetMaxBlocs(m_MaxPistes.GetCheck(), nDSK_Dest)
					, TabDir[nDSK_Src][Indice].User
				);
			}
		}
		else // Copie du dsk complet
		{
			CopyDsk(nDSK_Src, nDSK_Dest);
			IsDsk[nDSK_Dest] = IsDskValid[nDSK_Dest] = IsDskSaved[nDSK_Dest] = TRUE;
			FillBitmap(nDSK_Dest);
			ReadDirDsk(nDSK_Dest);
			BoutonsOn(nDSK_Dest);
			strcpy(NomDsk[nDSK_Dest], "Disquette_Vide.DSK");
			m_NomDsk[nDSK_Dest].SetWindowText(NomDsk[nDSK_Dest]);
		}
	}
}


//
// Affichage en mode "directory"
//
void CManageDskDlg::OnVisuDir(int nDSK) {
	memcpy(TabDirInit[nDSK], TabDir[nDSK], sizeof(TabDir[0]));
	CListDir Liste(TabDirInit[nDSK]);
	Liste.DoModal();
}


//
// échange du DSK 0 et du DSK 1
//
void CManageDskDlg::OnBpSwapDsk() {
	if (IsDskValid[DSK_0] && IsDskValid[DSK_1]) {
		SwapDsk();
		ReadDirDsk(DSK_0);
		ReadDirDsk(DSK_1);
		char Nom1[256];
		char Nom2[256];
		m_NomDsk[DSK_0].GetWindowText(Nom1, sizeof(Nom1));
		m_NomDsk[DSK_1].GetWindowText(Nom2, sizeof(Nom2));
		m_NomDsk[DSK_0].SetWindowText(Nom2);
		m_NomDsk[DSK_1].SetWindowText(Nom1);
	}
}


//
// Comparer DSK 0 et DSK 1 ou fichiers si sélectionnés
//
void CManageDskDlg::OnBpCompare() {
	static BYTE BufFicTmp1[0x100000];

	if (IsDskValid[DSK_0] && IsDskValid[DSK_1]) {
		POSITION pos0 = m_Liste[DSK_0].GetFirstSelectedItemPosition();
		POSITION pos1 = m_Liste[DSK_1].GetFirstSelectedItemPosition();
		if (!pos0 || !pos1) {
			// Comparaison DSK
			int DernPiste = GetNbTracks(DSK_0);
			BOOL Egal = (DernPiste == GetNbTracks(DSK_1));
			if (Egal) {
				for (int Piste = 0; Piste < DernPiste; Piste++) {
					CPCEMUTrack* Trk0 = GetInfoTrack(DSK_0, Piste);
					CPCEMUTrack* Trk1 = GetInfoTrack(DSK_1, Piste);
					if (Trk0->NbSect == Trk1->NbSect && Trk0->SectSize == Trk1->SectSize) {
						for (int s = 0; s < Trk0->NbSect; s++) {
							BYTE* Datas0 = ReadSect(DSK_0, Piste, s, FALSE);
							BYTE* Datas1 = ReadSect(DSK_1, Piste, s, FALSE);
							for (int i = 0; i < Trk0->SectSize; i++)
								if (Datas0[i] != Datas1[i]) {
									Egal = FALSE;
									break;
								}
						}
					}
					else {
						Egal = FALSE;
						break;
					}
				}
			}
			MessageBox(GetTexteLoc(Egal ? 126 : 127));
		}
		else {
			// Comparaision fichiers
			while (pos0 && pos1) {
				int Indice0 = PosItem[DSK_0][m_Liste[DSK_0].GetNextSelectedItem(pos0)];
				int Indice1 = PosItem[DSK_1][m_Liste[DSK_1].GetNextSelectedItem(pos1)];
				int l0 = GetFic(DSK_0, Indice0, 0, BufFicTmp, TRUE);
				int l1 = GetFic(DSK_1, Indice1, 0, BufFicTmp1, TRUE);
				BOOL Egal = (l0 == l1);
				if (Egal) {
					for (int i = 0; i < l0; i++)
						if (BufFicTmp[i] != BufFicTmp1[i]) {
							Egal = FALSE;
							break;
						}
				}
				sprintf(Msg
					, GetTexteLoc(Egal ? 124 : 125)
					, GetNomAmsdos(TabDir[DSK_0][Indice0].Nom)
					, GetNomAmsdos(TabDir[DSK_1][Indice1].Nom)
				);
				MessageBox(Msg);
			}
		}
	}
}


void CManageDskDlg::OnWriteFloppy(int nDSK) {
	CFloppyLowLevel(nDSK
		, CMD_WRITE_DIRECT
		, m_MaxPistes.GetCheck() ? MAX_TRACKS : MAX_PISTE_STD
	).DoModal();
}


void CManageDskDlg::OnFormatFloppy(int nDSK) {
	CInputTracks c;
	c.DoModal();
	if (c.IsValide()) {
		int NbTracks = c.GetNbTracks(); // ###"
		int NbSects = c.GetNbSects();
		IsDsk[nDSK] = IsDskValid[nDSK] = IsDskSaved[nDSK] = TRUE;
		FormatDsk(nDSK, NbSects, NbTracks);
		ReadDirDsk(nDSK);
		//
		// Autoriser les boutons de l'interface
		//
		BoutonsOn(nDSK);
	}
}


void CManageDskDlg::OnReadFloppy(int nDSK) {
	CFloppyLowLevel d(nDSK
		, CMD_READ_DIRECT
		, m_MaxPistes.GetCheck() ? MAX_TRACKS : MAX_PISTE_STD
	);
	d.DoModal();
	if (d.GetCreation()) {
		IsDsk[nDSK] = IsDskSaved[nDSK] = TRUE;
		IsDskValid[nDSK] = CheckDsk(nDSK);
		if (!IsDskValid[nDSK])
			MessageBox(GetTexteLoc(20));

		BoutonsOn(nDSK);
		ReadDirDsk(nDSK);
		strcpy(NomDsk[nDSK], "Disquette_Vide.DSK");
		m_NomDsk[nDSK].SetWindowText(NomDsk[nDSK]);
	}
}

void CManageDskDlg::OnDblWindow(void) {
	WINDOWPLACEMENT plc;
	GetWindowPlacement(&plc);
	int TailleY = plc.rcNormalPosition.bottom - plc.rcNormalPosition.top;
	if (m_DblWindow.GetCheck()) {
		m_BpSwapDsk.ShowWindow(TRUE);
		m_BpCompare.ShowWindow(TRUE);
		m_BpSend[DSK_0].ShowWindow(TRUE);
		SetWindowPos(NULL, 0, 0, SIZE_X_2_WINDOWS, TailleY, SWP_NOMOVE);
	}
	else {
		m_BpSwapDsk.ShowWindow(FALSE);
		m_BpCompare.ShowWindow(FALSE);
		m_BpSend[DSK_0].ShowWindow(FALSE);
		SetWindowPos(NULL, 0, 0, SIZE_X_1_WINDOWS, TailleY, SWP_NOMOVE);
	}
	CenterWindow();
}


//
// Demande de sortie du programme
//
void CManageDskDlg::OnCancel() {
	BOOL Exit = TRUE;
	if (IsDsk[DSK_0] && !IsDskSaved[DSK_0]) {
		sprintf(Msg, GetTexteLoc(73), NomDsk[DSK_0]);
		Exit = (MessageBox(Msg, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES);
	}
	if (Exit && IsDsk[DSK_1] && !IsDskSaved[DSK_1]) {
		sprintf(Msg, GetTexteLoc(73), NomDsk[DSK_1]);
		Exit = (MessageBox(Msg, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES);
	}
	if (Exit)
		CDialog::OnCancel();
}


void CManageDskDlg::OnDropFiles(HDROP dropHandle) {
	static char NomFic[1024];
	POINT pt;

	DragQueryPoint(dropHandle, &pt);
	if (pt.y >= 16 && pt.y <= 290) {
		int numDsk = pt.x > SIZE_X_1_WINDOWS ? DSK_1 : DSK_0;
		for (unsigned int i = 0; i < DragQueryFile(dropHandle, 0xFFFFFFFF, NULL, 0); i++) {
			DragQueryFile(dropHandle, i, NomFic, sizeof(NomFic));
			int LastPt = -1;
			for (unsigned int j = 0; j < strlen(NomFic); j++) {
				NomFic[j] = (char)toupper(NomFic[j]);
				if (NomFic[j] == '.')
					LastPt = j;
			}
			if (LastPt > -1 && !strcmp(&NomFic[LastPt], ".DSK") && IsValidDsk(NomFic))
				ReadNewDSK(NomFic, numDsk);
			else
				AjouteUnFic(NomFic, numDsk);
		}
		DragFinish(dropHandle);
	}
	CDialog::OnDropFiles(dropHandle);
}


void CMyListe::OnDropFiles(HDROP dropHandle) {
	static char NomFic[1024];

	for (unsigned int i = 0; i < DragQueryFile(dropHandle, 0xFFFFFFFF, NULL, 0); i++) {
		DragQueryFile(dropHandle, i, NomFic, sizeof(NomFic));
		int LastPt = -1;
		for (unsigned int j = 0; j < strlen(NomFic); j++) {
			NomFic[j] = (char)toupper(NomFic[j]);
			if (NomFic[j] == '.')
				LastPt = j;
		}
		if (LastPt > -1 && !strcmp(&NomFic[LastPt], ".DSK") && IsValidDsk(NomFic))
			Parent->ReadNewDSK(NomFic, nDSK);
		else
			Parent->AjouteUnFic(NomFic, nDSK);
	}
	DragFinish(dropHandle);
	CListCtrl::OnDropFiles(dropHandle);
}


void CManageDskDlg::OnCustomDrawList(int nDSK, NMHDR* pNMHDR) {
	LPNMLVCUSTOMDRAW lpLVCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	if (lpLVCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
		CString s = m_Liste[nDSK].GetItemText(lpLVCustomDraw->nmcd.dwItemSpec, 1);
		lpLVCustomDraw->clrText = RGB(atoi(s) == USER_DELETED ? 255 : 0, 0, 0);
	}
}

