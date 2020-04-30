#include  <afxwin.h>

#include  "ViewFile.h"


#pragma warning(disable:4711)           // selected for automatic inline


//
// Indique si un label est un mot clé reservé ou non
//
static BOOL LabelReserve( char * Label )
{
    return(  ! _stricmp( Label, "END" )
          || ! _stricmp( Label, "READ" )
          || ! _stricmp( Label, "WRITE" )
          );
}


//
// Convertir le buffer en listing au format Dams
// Adaptation des sources de Thierry JOUIN ( Ramlaid )
//
void Dams( BYTE * BufFile, int TailleFic, char * Listing )
{
    static char * MotCleDams[ 0x80 ] = 
        {
        "LD","INC","DEC","ADD","ADC","SUB","SBC","AND","XOR","OR","CP",
        "PUSH","POP","BIT","RES","SET","RLC","RRC","RL","RR","SLA","SRA",
        "SRL","IN","OUT","RST","DJNZ","EX","IM","JR","CALL","RET","JP",
        "NOP","RLCA","RRCA","RLA","RRA","DAA","CPL","SCF","CCF","HALT",
        "EXX","DI","EI","NEG","RETN","RETI","RRD","RLD","LDI","CPI","INI",
        "OUTI","LDD","CPD","IND","OUTD","LDIR","CPIR","INIR","OTIR","LDDR",
        "CPDR","INDR","OTDR","DB","DW","DM","DS","EQU","ORG","ENT",
        "IF","ELSE","END"
        };
    char Tmp[ 64 ];
    char Label[ 64 ];
    int PosLabel = 0;
    int PosFile = 0;
    int PosDest = 0;
    BYTE c;

    * Listing = 0;
    c = BufFile[ PosFile++ ];
    while( c )
        {
        if ( c == 0xFF )
            {
            // Commentaire ligne
            Listing[ PosDest++ ] = ';';
            c = BufFile[ PosFile++ ];
            while( c != 0x0D && PosFile < TailleFic )
                {
                Listing[ PosDest++ ] = c;
                c = BufFile[ PosFile++ ];
                }
            Listing[ PosDest++ ] = '\r';
            Listing[ PosDest++ ] = '\n';
            }
        else
            {
            if ( c >= 0x80 && c != 0x0D )
                {
                // Mnemonique sans label
                // ENT
                if ( c == 0xC9 )
                    Listing[ PosDest++ ] = ';';

                sprintf( Tmp, "\t%s\t", MotCleDams[ c & 0x7F ] );
                int l = strlen( Tmp );
                memcpy( &Listing[ PosDest ], Tmp, l );
                PosDest += l;
                // DS ?,?
                if ( c == 0xC6 )
                    {
                    c = BufFile[ PosFile++ ];
                    // Fin de ligne
                    while( c != 0x0D && PosFile < TailleFic )
                        {
                        if ( c == ',' )
                            {
                            while( c != 0x0D && c != 0xFF && PosFile < TailleFic )
                                c = BufFile[ PosFile++ ];
                            }
                        if ( c != 0x0D )
                            {
                            if ( c == 0xFF )
                                Listing[ PosDest++ ] = '\t';
                            else          
                                Listing[ PosDest++ ] = c;

                            c = BufFile[ PosFile++ ];
                            }
                        }
                    }
                else
                    {
                    c = BufFile[ PosFile++ ];
                    // Fin de ligne
                    while( c != 0x0D && PosFile < TailleFic )
                        {
                        if ( c == 0xFF )
                            Listing[ PosDest++ ] = '\t';
                        else          
                                Listing[ PosDest++ ] = c;

                        c = BufFile[ PosFile++ ];
                        }
                    }
                Listing[ PosDest++ ] = '\r';
                Listing[ PosDest++ ] = '\n';
                }
            else
                {
                // Label
                * Label = 0;
                PosLabel = 0;
                while( c < 0x80 && c != 0x0D && PosFile < TailleFic && PosLabel < 32 )
                    {
                    Label[ PosLabel++ ] = c;
                    Listing[ PosDest++ ] = c;
                    c = BufFile[ PosFile++ ];
                    }
                Label[ PosLabel ] = 0;

                // Ajouter un '_' après un label réservé (V0.14d)
                if ( LabelReserve( Label ) )
                    Listing[ PosDest++ ] = '_';

                if ( c != 0x0D )
                    {
                    // Mnemonique apres label
                    // ENT
                    if ( c == 0xC9 )
                        Listing[ PosDest++ ] = ';';

                    if ( c != 0xFF )
                        {
                        sprintf( Tmp, "\t%s\t", MotCleDams[ c & 0x7F ] );
                        int l = strlen( Tmp );
                        memcpy( &Listing[ PosDest ], Tmp, l );
                        PosDest += l;
                        }
                    else
                        {
                        Listing[ PosDest++ ] = '\t';
                        Listing[ PosDest++ ] = '\t';
                        Listing[ PosDest++ ] = '\t';
                        }
                    // DS ?,?
                    if ( c == 0xC6 )
                        {
                        c = BufFile[ PosFile++ ];
                        // Fin de ligne
                        while( c != 0x0D && PosFile < TailleFic )
                            {
                            if ( c == ',' )
                                {
                                while( c != 0x0D && c != 0xFF && PosFile < TailleFic )
                                    c = BufFile[ PosFile++ ];
                                }
                            if ( c != 0x0D )
                                {
                                if ( c == 0xFF )
                                    {
                                    Listing[ PosDest++ ] = '\t';
                                    Listing[ PosDest++ ] = ';';
                                    }
                                else          
                                    Listing[ PosDest++ ] = c;

                                c = BufFile[ PosFile++ ];
                                }
                            }
                        }
                    else
                        {
                        c = BufFile[ PosFile++ ];
                        // Fin de ligne
                        while( c != 0x0D && PosFile < TailleFic )
                            {
                            if ( c == 0xFF )
                                {
                                Listing[ PosDest++ ] = '\t';
                                Listing[ PosDest++ ] = ';';
                                }
                            else          
                                Listing[ PosDest++ ] = c;

                            c = BufFile[ PosFile++ ];
                            }
                        }
                    Listing[ PosDest++ ] = '\r';
                    Listing[ PosDest++ ] = '\n';
                    }
                else
                    {
                    Listing[ PosDest++ ] = '\r';
                    Listing[ PosDest++ ] = '\n';
                    }
                }
            }
        c = BufFile[ PosFile++ ];
        if ( PosFile > TailleFic || PosDest >= MAX_FILESIZE - 16 )
            break;
        }
    Listing[ PosDest++ ] = 0;
}
