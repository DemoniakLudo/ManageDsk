// ManageDsk.cpp : Defines the class behaviors for the application.
//

#include  <afxwin.h>
#include  <afxcmn.h>
#include  <afxext.h>
#include  <iostream>

#include  "Locale.h"
#include  "Outils.h"
#include  "GestDsk.h"
#include  "Resource.h"
#include  "ManageDsk.h"
#include  "ManageDskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BYTE BufFicTmp[0x100000];

//
// Tableau de décryptage d'un programme en basic protégé
//
static BYTE AMSDOS_KEY[128] =
{
0xAB, 0x2C, 0xED, 0xEA, 0x6C, 0x37, 0x3F, 0xEC,
0x9B, 0xDF, 0x7A, 0x0C, 0x3B, 0xD4, 0x6D, 0xF5,
0x04, 0x44, 0x03, 0x11, 0xDF, 0x59, 0x8F, 0x21,
0x73, 0x7A, 0xCC, 0x83, 0xDD, 0x30, 0x6A, 0x30,
0xD3, 0x8F, 0x02, 0xF0, 0x60, 0x6B, 0x94, 0xE4,
0xB7, 0xF3, 0x03, 0xA8, 0x60, 0x88, 0xF0, 0x43,
0xE8, 0x8E, 0x43, 0xA0, 0xCA, 0x84, 0x31, 0x53,
0xF3, 0x1F, 0xC9, 0xE8, 0xAD, 0xC0, 0xBA, 0x6D,
0x93, 0x08, 0xD4, 0x6A, 0x2C, 0xB2, 0x07, 0x27,
0xC0, 0x99, 0xEE, 0x89, 0xAF, 0xC3, 0x53, 0xAB,
0x2B, 0x34, 0x5C, 0x2F, 0x13, 0xEE, 0xAA, 0x2C,
0xD9, 0xF4, 0xBC, 0x12, 0xB3, 0xC5, 0x1C, 0x68,
0x01, 0x20, 0x2C, 0xFA, 0x77, 0xA6, 0xB5, 0xA4,
0xFC, 0x9B, 0xF1, 0x32, 0x5B, 0xC3, 0x70, 0x77,
0x85, 0x36, 0xBE, 0x5B, 0x8C, 0xC8, 0xB5, 0xC2,
0xF0, 0x0B, 0x98, 0x0F, 0x36, 0x9D, 0xD8, 0x96
};


/********************************************************* !NAME! **************
* Nom : Retour
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Variables Globales
********************************************************** !1! *****************
*
* Fichier     : !./FPTH\/FLE!, ligne : !./LN!
*
* Description : Chaine de caractères utilisée pour stocker les arguments
*
********************************************************** !0! ****************/
static char Retour[256];


/////////////////////////////////////////////////////////////////////////////
// CManageDskApp

BEGIN_MESSAGE_MAP(CManageDskApp, CWinApp)
	//{{AFX_MSG_MAP(CManageDskApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageDskApp construction

CManageDskApp::CManageDskApp() {
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CManageDskApp object

CManageDskApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CManageDskApp initialization

BOOL CManageDskApp::InitInstance() {
	// Standard initialization
	if (!m_lpCmdLine[0]) {
		CManageDskDlg dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
	}
	else
		DecomposeArg(m_lpCmdLine);

	return(TRUE);
}


/********************************************************* !NAME! **************
* Nom : CManageDskApp::GetArg
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Fonctions
********************************************************** !1! *****************
*
* Description : Recherche le séparateur d'arguments (-)
*
* Résultat    : La position du premier argument trouvé
*
* Variables globales modifiées : /
*
********************************************************** !0! ****************/
char* CManageDskApp::GetArg(char* argc) {
	while (*argc && *argc != '-')
		argc++;

	if (*argc == '-')
		return(++argc);

	return(NULL);
}


/********************************************************* !NAME! **************
* Nom : CManageDskApp::Argument
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Fonctions
********************************************************** !1! *****************
*
* Description : Isole un argument de la ligne de commande
*
* Résultat    : Le premier argument trouvé
*
* Variables globales modifiées : /
*
********************************************************** !0! ****************/
char* CManageDskApp::Argument(char* argc) {

	char* p = Retour;

	while (*argc != ' ') {
		if (*argc == '"') {
			*p++ = *argc++;
			while (*argc != '"')
				*p++ = *argc++;
		}
		*p++ = *argc++;
	}
	*p = 0;

	return(Retour);
}


void CManageDskApp::AjouteFichiers(int nDSK, char* Masque) {
	WIN32_FIND_DATA FindData;
	static BYTE Buff[0x20000];
	char Path[256];
	char FullName[512];
	DWORD Lg;

	strcpy(Path, Masque);
	if (*Path == '"') {
		Path[strlen(Path) - 1] = 0;
		strcpy(Path, Path + 1);
	}
	// Recherche le dernier caractère '\'
	char* p = Path;
	char* last = p;
	do {
		last = strchr(p, '\\');
		if (last)
			p = ++last;
	}
	while (last);

	if (p > Path)
		*--p = 0;
	else
		*p = 0;

	HANDLE Hfile = FindFirstFile(Masque, &FindData);
	while (Hfile != INVALID_HANDLE_VALUE) {
		if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			if (*Path)
				wsprintf(FullName, "%s\\%s", Path, FindData.cFileName);
			else
				strcpy(FullName, FindData.cFileName);

			HANDLE fp = CreateFile(FullName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (fp != INVALID_HANDLE_VALUE) {
				ReadFile(fp, Buff, sizeof(Buff), &Lg, NULL);
				CloseHandle(fp);
				CopieFichier(nDSK, Buff, FindData.cFileName, Lg, 256, 0);
			}
			else
				std::cout << "Erreur impossible de lire le fichier " << FindData.cFileName;
		}
		if (!FindNextFile(Hfile, &FindData))
			break;
	}
	FindClose(Hfile);
}



void CManageDskApp::AjouteFichierEntete(int nDSK, char* FicInfo) {
	WIN32_FIND_DATA FindData;
	static BYTE Buff[0x20000];
	char FullName[512];
	DWORD length;
	int start = 0, exec = 0;

	// Décomposer les paramètres
	char* amsFile = strchr(FicInfo, '/');
	if (amsFile) {
		*amsFile++ = 0;
		char* type = strchr(amsFile, '/');
		if (type) {
			*type++ = 0;
			strcpy(FullName, FicInfo);
			if (*FullName == '"') {
				FullName[strlen(FullName) - 1] = 0;
				strcpy(FullName, FullName + 1);
			}
			HANDLE fp = CreateFile(FullName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (fp != INVALID_HANDLE_VALUE) {
				ReadFile(fp, &Buff[sizeof(StAmsdos)], sizeof(Buff) - sizeof(StAmsdos), &length, NULL);
				CloseHandle(fp);

				char* NomAmsdos = GetNomAmsdos((BYTE*)amsFile);
				int t = 0;
				if (!(strncmp(type, "BIN", 3)))
					t = 2;

				if (strncmp(type, "ASC", 3)) {
					// Not ASCII
					char* p = strchr(type, '/');
					if (p) {
						*p = 0;
						if (strncmp(p + 1, "P", 1) == 0) {
							++t; // Add protection bit
							for (int t = 0; t < length; ++t) {
								Buff[sizeof(StAmsdos) + t] ^= AMSDOS_KEY[t % sizeof(AMSDOS_KEY)];
							}
							p = strchr(++p, '/');
						}

						if (p) {
							start = atoi(++p);
							p = strchr(p, '/');
							if (p)
								exec = atoi(++p);
						}

						StAmsdos* Entete = CreeEnteteComplet(NomAmsdos, t, start, length, exec);
						memcpy(Buff, Entete, sizeof(StAmsdos));
						length += sizeof(StAmsdos);
					}
				}
				else
					memcpy(Buff, Buff + sizeof(StAmsdos), length);

				CopieFichier(nDSK, Buff, NomAmsdos, length + sizeof(StAmsdos), 256, 0);
			}
			else
				std::cout << "Erreur impossible de lire le fichier " << FindData.cFileName;
		}
	}
}


int CManageDskApp::GetFic(int nDSK, StDirEntry* Dir, int Ko, BYTE* BufOut) {
	int PosBufOut = 0;
	char NomFic[16];
	int lMax = 0x1000000;
	BOOL FirstBlock = TRUE;
	int LgE = sizeof(StAmsdos);       // Taille En-Tête

	strcpy(NomFic, GetNomAmsdos(Dir->Nom));

	// Longueur du fichier
	int l = (Dir->NbPages + 7) >> 3;
	for (int j = 0; j < l; j++) {
		int TailleBloc = 1024;
		BYTE* p = ReadBloc(nDSK, Dir->Blocks[j]);
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
			else {
				StAmsdos* e = CreeEnteteAmsdos(NomFic, (USHORT)(Ko << 10));
				memcpy(&BufOut[PosBufOut], e, LgE);
				PosBufOut += LgE;
			}
			FirstBlock = FALSE;
		}
		int NbOctets = min(lMax, TailleBloc);
		if (NbOctets > 0) {
			memcpy(&BufOut[PosBufOut], p, NbOctets);
			PosBufOut += NbOctets;
		}
		lMax -= NbOctets;
	}
	return(PosBufOut);
}


void CManageDskApp::ExportFichier(int nDSK, char* nomFic, bool withEntete) {
	int indice = FileExist(nDSK, nomFic, 0);
	DWORD Lg;

	StDirEntry* Dir = GetInfoDirEntry(nDSK, indice);
	HANDLE f = CreateFile(nomFic, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (f != INVALID_HANDLE_VALUE) {
		int NbOctets = GetFic(nDSK, Dir, indice, BufFicTmp), start = withEntete || !CheckAmsdos(BufFicTmp) ? 0 : 128;
		WriteFile(f, &BufFicTmp[start], NbOctets - start, &Lg, NULL);
		CloseHandle(f);
	}
}

/********************************************************* !NAME! **************
* Nom : CManageDskApp::DecomposeArg
********************************************************** !PATHS! *************
* !./V1!\!./V2!\!./V3!\!./V4!\Fonctions
********************************************************** !1! *****************
*
* Description : Décompose les arguments passés en ligne de commande
*
* Résultat    : /
*
* Variables globales modifiées : /
*
********************************************************** !0! ****************/
void CManageDskApp::DecomposeArg(char* argc) {
	char* p;
	BOOL IsDskLoc = FALSE;
	int nDSK = 0;

	AllocConsole();
	std::cout << "-- Mode Ligne de commandes --\n";
	do {
		argc = GetArg(argc);
		if (argc) {
			switch (toupper(*argc++)) {
			case 'C':  // Créer dsk
				FormatDsk(nDSK, 9, 42);
				IsDskLoc = TRUE;
				std::cout << "Création dsk ok.\n";
				break;

			case 'L':  // Lire Dsk
				p = Argument(argc);
				if (ReadDsk(nDSK, p)) {
					std::cout << "Lecture fichier " << p << " ok.\n";
					IsDskLoc = TRUE;
				}
				else {
					std::cout << "Erreur lecture fichier " << p;
					IsDskLoc = FALSE;
				}
				break;

			case 'A':  // Ajouter fichiers sur dsk
				p = Argument(argc);
				if (IsDskLoc)
					AjouteFichiers(nDSK, p);
				else
					std::cout << "Erreur pas de dsk chargé.\n";
				break;

			case '-': // Extraine un fichier du dsk en supprimant l'en-tête AMSDOS
				if (*argc++ == 'E') {
					p = Argument(argc);
					if (IsDskLoc) {
						if (FileExist(nDSK, p, 0) != -1)
							ExportFichier(nDSK, p, false);
						else
							std::cout << "Erreur fichier " << p << " non trouvé.\n";
					}
					else
						std::cout << "Erreur pas de dsk chargé.\n";
				}
				break;

			case 'E': // Etraire un fichier du dsk
				p = Argument(argc);
				if (IsDskLoc) {
					if (FileExist(nDSK, p, 0) != -1)
						ExportFichier(nDSK, p, true);
					else
						std::cout << "Erreur fichier " << p << " non trouvé.\n";
				}
				else
					std::cout << "Erreur pas de dsk chargé.\n";
				break;

			case 'I': // Ajout d'un fichier avec en-tête sur dsk
				p = Argument(argc);
				if (IsDskLoc)
					AjouteFichierEntete(nDSK, p);
				else
					std::cout << "Erreur pas de dsk chargé.\n";
				break;

			case 'S':  // Sauver dsk
				p = Argument(argc);
				if (IsDskLoc) {
					if (WriteDsk(nDSK, p))
						std::cout << "Sauvegarde fichier " << p << " ok.\n";
					else
						std::cout << "Erreur écriture fichier " << p;
				}
				else
					std::cout << "Erreur pas de dsk chargé.\n";
				break;

			case 'P': // Affiche le contenu du dsk
			{
				for (int i = 0; i < 64; i++) {
					StDirEntry* t = GetInfoDirEntry(nDSK, i);
					char* Nom = GetNomAmsdos(t->Nom);
					BOOL NomValide = FALSE;
					for (int j = 0; j < 8; j++)
						if (t->Nom[j] != 0xE5)
							NomValide = TRUE;

					if (NomValide)
						std::cout << "User:" << t->User << ":" << Nom << "\n";
				}
			}
			default:
				break;
			}
		}
	}
	while (argc);
	std::getchar();
	FreeConsole();
}
