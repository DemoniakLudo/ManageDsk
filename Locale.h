#ifndef LOCALE_H
#define LOCALE_H


enum { LOC_FRANCAIS = 0, LOC_ENGLISH, LOC_SPANISH };


#define MAX_TEXTES  132


void SetLangue( int l );

char * GetTexteLoc( int NumTexte );


#endif