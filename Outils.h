#ifndef OUTILS_H
#define OUTILS_H


#include  "GestDsk.h"


void Hex( char Chaine[], int Valeur, int Digit );

int HexToDec( char * Valeur );

void SetBuffViewHexa( BYTE * src
                    , char * Hex
                    , char * Ascii
                    , USHORT Offset = 0
                    , BOOL AddOffset = FALSE
                    , int Longueur = 512
                    );

char * GetType( StAmsdos * Ams );

char * GetNomAmsdos( BYTE * AmsName );

char * GetUser( int );

char * GetTaille( int t );


#endif
