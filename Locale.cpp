#define WINDOWS_IGNORE_PACKING_MISMATCH

#include  <windows.h>

#include  "Locale.h"


static int Locale = LOC_FRANCAIS;   // Par d�faut langue en Fran�ais


//
// Ce tableau � deux dimensions contient les textes en fran�ais et
// les textes traduits en anglais.
// Il est possible d'ajouter d'autre langues.
//
static char * TabTextes[MAX_TEXTES][3] =
{
	{
	"Contenu du fichier DSK",                                   // 0
	"Inside DSK file",
	"Contenido del fichero DSK"
	},
	{
	"&Ajouter fichier(s)",                                      // 1
	"&Add file(s)",
	"&Adjuntar fichero"
	},
	{
	"&Exporter fichier(s)",                                     // 2
	"&Export file(s)",
	"&Exportar fichero"
	},
	{
	"&Renommer fichier(s)",                                     // 3
	"&Rename file(s)",
	"&Renombrar fichero"
	},
	{
	"&Supprimer fichier(s)",                                    // 4
	"&Delete file(s)",
	"&Borrar fichero"
	},
	{
	"Cr�er DSK",                                                // 5
	"Create DSK",
	"Crear DSK"
	},
	{
	"Lire DSK",                                                 // 6
	"Read DSK",
	"Leer DSK"
	},
	{
	"Sauver sous",                                              // 7
	"Save as",
	"Guardar como"
	},
	{
	"Contenu du fichier DSK (Ko utilis�s/libres : %d/%d)",      // 8
	"Inside DSK file (used Kb/free Kb : %d/%d)",
	"Contenido del fichero DSK (Kb usados/libres: %d/%d)"
	},
	{
	"Nettoyage automatique des en-t�tes au format AMSDOS",      // 9
	"Auto clean-up headers in AMSDOS format",
	"Auto limpiado de cabeceras en formato AMSDOS"
	},
	{
	"Nom Fichier",                                              // 10
	"File Name",
	"Nombre del fichero"
	},
	{
	"User",                                                     // 11
	"User",
	"Usuario"
	},
	{
	"Type",                                                     // 12
	"Type",
	"Tipo"
	},
	{
	"Taille",                                                   // 13
	"Size",
	"Tama�o"
	},
	{
	"Tous les fichiers (*.*)|*.*||",                            // 14
	"All files (*.*)|*.*||",
	"Todos los ficheros (*.*)|*.*||"
	},
	{
	"Taille sup�rieure � 64Ko. Continuer",                      // 15
	"size greather than 64Kb. Continue",
	"Tama�o mayor de 64 Kb. Continuar"
	},
	{
	"En-t�te Amsdos non trouv�e. Traiter comme fichier ASCII",  // 16
	"No Amsdos header found. Treat as ASCII file",
	"Cabecera AMSDOS no encontrada. Tratar como fichero ASCII"
	},
	{
	"Plus de place dans le r�pertoire !!!",                     // 17
	"No more entry in the directory !!!",
	"No hay m�s entradas en el directorio !!!"
	},
	{
	"Plus de place sur la disquette !!!",                       // 18
	"No more space on the floppy !!!",
	"No hay m�s espacio en el disco !!!"
	},
	{
	"Fichiers DSK (*.DSK)|*.DSK||",                             // 19
	"DSK Files (*.DSK)|*.DSK||",
	"Ficheros DSK (*.DSK)|*.DSK||"
	},
	{
	"Fichier non reconnu comme DSK standard (DATA ou VENDOR)",  // 20
	"File is not recognized as standard DSK (DATA or VENDOR)",
	"El fichero no est� reconocido como DSK standard (DATA o VENDOR)"
	},
	{
	"Voulez-vous vraiment supprimer ce fichier ?",              // 21
	"Do you really want to delete this file ?",
	"Est�s seguro que quieres eliminar este fichero"
	},
	{
	"BASIC",                                                    // 22
	"BASIC",
	"BASIC"
	},
	{
	"BASIC (P)",                                                // 23
	"BASIC (P)",
	"BASIC (P)"
	},
	{
	"BINAIRE",                                                  // 24
	"BINARY",
	"BINARIO"
	},
	{
	"BINAIRE (P)",                                              // 25
	"BINARY (P)",
	"BINARIO (P)"
	},
	{
	"INCONNU",                                                  // 26
	"UNKNOWN",
	"DESCONOCIDO"
	},
	{
	"ASCII",                                                    // 27
	"ASCII",
	"ASCII"
	},
	{
	"Informations sur un fichier",                              // 28
	"Informations about a file",
	"Informaci�n sobre un fichero"
	},
	{
	"Nom du fichier :",                                         // 29
	"Name of the file :",
	"Nombre del fichero :"
	},
	{
	"User :",                                                   // 30
	"User :",
	"Usuario :"
	},
	{
	"Type :",                                                   // 31
	"Type :",
	"Tipo :"
	},
	{
	"Adresse d�but :",                                          // 32
	"Start address :",
	"Direcci�n de comienzo :"
	},
	{
	"Longueur :",                                               // 33
	"Size :",
	"Tama�o :"
	},
	{
	"Point d'entr�e :",                                         // 34
	"Entry point :",
	"Punto de entrada :"
	},
	{
	"Valider",                                                  // 35
	"Validate",
	"Validar"
	},
	{
	"Importation du fichier %s",                                // 36
	"Import file %s",
	"Importar fichero %s"
	},
	{
	"En-t�te AMSDOS non trouv�e.",                              // 37
	"AMSDOS header not found.",
	"Cabecera AMSDOS no encontrada."
	},
	{
	"En-t�te AMSDOS trouv�e.",                                  // 38
	"AMSDOS header found.",
	"Cabecera AMSDOS encontrada."
	},
	{
	"Choisir le type de fichier",                               // 39
	"Choose file type",
	"Elija el tipo de fichero"
	},
	{
	"Fichier BINAIRE",                                          // 40
	"BINARY file",
	"Fichero BINARIO"
	},
	{
	"Fichier ASCII",                                            // 41
	"ASCII file",
	"Fichero ASCII"
	},
	{
	"Import :",                                                 // 42
	"Import :",
	"Importar :"
	},
	{
	"Forcer BINAIRE",                                           // 43
	"Force BINARY",
	"Forzar BINARIO"
	},
	{
	"Forcer ASCII",                                             // 44
	"Force ASCII",
	"Forzar ASCII"
	},
	{
	"Choix utilisateur",                                        // 45
	"User choice",
	"Escoger usuario"
	},
	{
	"Sauver DSK",                                               // 46
	"Save DSK",
	"Salvar DSK"
	},
	{
	"%s sauvegard�",                                            // 47
	"%s saved",
	"%s guardado"
	},
	{
	"Pas de DSK charg�.",                                       // 48
	"No DKS loaded.",
	"No se ha cargado ning�n DSK"
	},
	{
	"Lecture seule",                                            // 49
	"Read only",
	"S�lo lectura"
	},
	{
	"Cach�",                                                    // 50
	"Hidden",
	"Oculto"
	},
	{
	"Voir secteurs",                                            // 51
	"Show sectors",
	"Ver sectores"
	},
	{
	"Affichage des secteurs",                                   // 52
	"Showing sectors",
	"Viendo sectores"
	},
	{
	"Piste :",                                                  // 53
	"Track :",
	"Pista :"
	},
	{
	"Secteur :",                                                // 54
	"Sector :",
	"Sector :"
	},
	{
	"Export :",                                                 // 55
	"Export :",
	"Exportar :"
	},
	{
	"Avec en-t�te",                                             // 56
	"With header",
	"Con cabececera"
	},
	{
	"Sans en-t�te",                                             // 57
	"Without header",
	"Sin cabecera"
	},
	{
	"Voir fichier",                                             // 58
	"Show file",
	"Ver fichero"
	},
	{
	"Mode hexad�cimal",                                         // 59
	"Hexadecimal mode",
	"Modo hexadecimal"
	},
	{
	"Mode listing",                                             // 60
	"Listing mode",
	"Modo de escucha"
	},
	{
	"Mode m�moire vid�o",                                       // 61
	"Video memory mode",
	"Modo memoria de video"
	},
	{
	"Saut de lignes comme CPC mode 2",                          // 62
	"Line wrap like CPC mode 2",
	"Corte de l�nea como en el modo 2 de CPC"
	},
	{
	"Mode m�moire �cran � plat",                                // 63
	"Flat video memory mode",
	"Modo de memoria de video liso"
	},
	{
	"Nombre de colonnes : %d",                                  // 64
	"Number of columns : %d",
	"N�mero de columnas : %d"
	},
	{
	"Offset : #%04X",                                           // 65
	"Offset : #%04X",
	"Offset : #%04X"
	},
	{
	"Nombre de lignes : %d",                                    // 66
	"Number of rows : %d",
	"N�mero de filas: %d"
	},
	{
	"Fichiers WIN (*.WIN)|*.WIN||",                             // 67
	"WIN Files (*.WIN)|*.WIN||",
	"Ficheros WIN (*.WIN)|*.WIN||"
	},
	{
	"Le fichier %s existe d�j�. Le remplacer",                  // 68
	"File %s already exists. Replace",
	"El fichero %s ya existe. Reemplazar"
	},
	{
	"Mode Dams",                                                // 69
	"Dams mode",
	"Modo Dams"
	},
	{
	"Mode D�sassembl�",                                         // 70
	"Disassembly mode",
	"Modo Desensamblador"
	},
	{
	"Permettre plus de 42 pistes",                              // 71
	"Allows more than 42 tracks",
	"Permitir m�s de 42 pistas"
	},
	{
	"Disquette %s non sauvegard�e. Confirmer lecture",          // 72
	"Floppy %s not saved. Confirm read",
	"Floppy %s no salvado. Confirmar lectura"
	},
	{
	"Disquette %s non sauvegard�e. Confirmer sortie",           // 73
	"Floppy %s not saved. Confirm exit",
	"Floppy %s no salvado. Confirmar salida"
	},
	{
	"Voir fichier : %s",                                        // 74
	"Show file : %s",
	"Mostrar fichero : %s"
	},
	{
	"Impossible de lire le fichier %s",                         // 75
	"Unable to read the file %s",
	"Incapaz de leer el fichero %s"
	},
	{
	"Copier =>",                                                // 76
	"Copy =>",
	"Copiar =>"
	},
	{
	"Echanger",                                                 // 77
	"Swap",
	"Intercambiar"
	},
	{
	"Nombre de pistes : %d",                                    // 78
	"Number of tracks : %d",
	"N�mero de pistas : %d"
	},
	{
	"Ecriture secteur %d/%d #%02X,#%02X,#%02X,#%02X  ",         // 79
	"Writting sector %d/%d #%02X,#%02X,#%02X,#%02X  ",
	"Escribiendo sector %d/%d #%02X,#%02X,#%02X,#%02X  "
	},
	{
	"Formatage piste %02d",                                     // 80
	"Formating track %02d",
	"Formateando pista %02d"
	},
	{
	"Ecriture vers A:",                                         // 81
	"Write to A:",
	"Escribiendo en A:"
	},
	{
	"Lecture depuis A:",                                        // 82
	"Read from A:",
	"Lectura de A:"
	},
	{
	"Ecriture directe fichier DSK vers A:",                     // 83
	"Directly write DSK file to A:",
	"Escritura directa del fichero DSK en A:"
	},
	{
	"Lecture directe fichier DSK depuis A:",                    // 84
	"Directly read DSK file from A:",
	"Lectura directa del fichero DSK desde A:"
	},
	{
	"Utiliser deux fen�tres",                                   // 85
	"Use two windows",
	"Utilizar dos ventanas"
	},
	{
	"Erreur acc�s controleur disquette.",                       // 86
	"Error while accessing floppy disk controller.",
	"Error accediendo al controado del disco floppy"
	},
	{
	"Erreur ouverture driver.",                                 // 87
	"Driver open error.",
	"Error abriendo driver."
	},
	{
	"Erreur acc�s disquette.",                                  // 88
	"Floppy access error.",
	"Error accediendo al floppy."
	},
	{
	"Version driver fdrawcmd.sys incorrect.",                   // 89
	"Incorrect version of fdrawcmd.sys.",
	"Versi�n del driver fdrawcmd.sys incorrecta."
	},
	{
	"Driver fdrawcmd.sys non install�.",                        // 90
	"fdrawcmd.sys driver not installed.",
	"Driver fdrawcmd.sys no instalado."
	},
	{
	"Choix de la couleur",                                      // 91
	"Color selection",
	"Selecci�n de color"
	},
	{
	"Couleur",                                                  // 92
	"Color",
	"Color"
	},
	{
	"D�marrer",                                                 // 93
	"Start",
	"Empezar"
	},
	{
	"Annuler",                                                  // 94
	"Cancel",
	"Cancelar"
	},
	{
	"Lecture secteur %d/%d #%02X,#%02X,#%02X,#%02X  ",          // 95
	"Reading sector %d/%d #%02X,#%02X,#%02X,#%02X  ",
	"Leyendo sector %d/%d #%02X,#%02X,#%02X,#%02X  "
	},
	{
	"Analyse piste %02d",                                       // 96
	"Analysing track %02d",
	"Analizando pista %02d"
	},
	{
	"Format de disquette invalide.",                            // 97
	"Invalid floppy format.",
	"Formato de floppy inv�lido."
	},
	{
	"Utiliser la face 0/A",                                     // 98
	"Use side 0/A",
	"Utilice la cara 0/A"
	},
	{
	"Utiliser la face 1/B",                                     // 99
	"Use side 1/B",
	"Utilice la cara 1/B"
	},
	{
	"Utiliser le mode piste 'double step'",                     // 100
	"Use the 'double step' track mode",
	"Utiliar el modo de pista 'double step'"
	},
	{
	"&Oui",                                                     // 101
	"&Yes",
	"&S�"
	},
	{
	"Oui pour &tout",                                           // 102
	"Yes for &all",
	"S� a &todo"
	},
	{
	"&Non",                                                     // 103
	"&No",
	"&No"
	},
	{
	"Configuration lecteur de disquettes",                      // 104
	"Floppy drive configuration",
	"Configuraci�n de unidad floppy"
	},
	{
	"Comparer",                                                 // 105
	"Compare",
	"Comparar"
	},
	{
	"Options avanc�es",                                         // 106
	"Advanced options",
	"Opciones avanzadas"
	},
	{
	"Copier <=",                                                // 107
	"Copy <=",
	"Copiar <="
	},
	{
	"Lecteur A:",                                               // 108
	"Drive A:",
	"Unidad A:"
	},
	{
	"Lecteur B:",                                               // 109
	"Drive B:",
	"Unidad B:"
	},
	{
	"Secteurs non entrelac�s.\nForcer entrelacement ?",         // 110
	"Sectors not interlaced.\nForce interlacing ?",
	"Sectores no entrelazados.\nForzar entrelzadado ?"
	},
	{
	"Fichiers Textes (*.TXT)|*.TXT||",                          // 111
	"Text Files (*.TXT)|*.TXT||",
	"Ficheros de texto (*.TXT)|*.TXT||"
	},
	{
	"Voir Directory",                                           // 112
	"Show Directory",
	"Ver Directorio"
	},
	{
	"Page",                                                     // 113
	"Page",
	"P�gina"
	},
	{
	"Blocs",                                                    // 114
	"Blocks",
	"Bloques"
	},
	{
	"Affichage Hexad�cimal",                                    // 115
	"Hexadecimal display",
	"Mostrar Hexadecimal"
	},
	{
	"Detect media support",                                     // 116
	"Detect media support",
	"Detectar media support"
	},
	{
	"Arr�t sur erreur",                                         // 117
	"Stop on error",
	"Parar si hay error"
	},
	{
	"Informations d�taill�es",                                  // 118
	"Detailled informations",
	"Informaciones detalladas"
	},
	{
	"Erreur de lecture secteur #%02X",                          // 119
	"Error reading sector #%02X",
	"Error leyendo sector #%02X"
	},
	{
	"Utiliser comme fichier DSK",                               // 120
	"Use as DSK file",
	"Usar como fichero DSK"
	},
	{
	"Disquette prot�g�e en �criture",                           // 121
	"Write protected floppy",
	"Floppy protegido contra escritura"
	},
	{
	"Formater",                                                 // 122
	"Format",
	"Formatear"
	},
	{
	"Nombre de pistes :",                                       // 123
	"Number of tracks :",
	"N�mero de pistas :"
	},
	{
	"Fichiers %s et %s identiques",                             // 124
	"File %s and %s identical",
	"Ficheros %s y %s id�nticos"
	},
	{
	"Fichiers %s et %s diff�rents",                             // 125
	"File %s and %s different",
	"Ficheros %s y &s son diferentes"
	},
	{
	"Les DSKs sont identiques",                                 // 126
	"The DSKs are identical",
	"Los DSKs son id�nticos"
	},
	{
	"Les DSKs sont diff�rents",                                 // 127
	"The DSKs are different",
	"Los DSKs son distintos"
	},
	{
	"Suppression d�finitive",                                   // 128
	"Permanently delete",
	"Eliminar de forma permanente"
	},
	{
		"Nombre de secteurs :",									// 129
		"Number of sectors :",
		"N�mero de sectors :"
	},
	{
		"Erreur sauvegarde %s",									// 130
		"Error savind %s",
		"Error guardado %s"
	},
};


//
// Positionne la langue par d�faut
//
void SetLangue(int l)
{
	if (l == LOC_FRANCAIS || l == LOC_ENGLISH || l == LOC_SPANISH)
		Locale = l;
}


//
// Retourne le texte associ� au num�ro et � la langue choisie.
//
char * GetTexteLoc(int NumTexte)
{
	if ((NumTexte < MAX_TEXTES) && (Locale == LOC_FRANCAIS || Locale == LOC_ENGLISH || Locale == LOC_SPANISH))
		return(TabTextes[NumTexte][Locale]);

	// Texte ou langue inconnu
	return("???");
}

