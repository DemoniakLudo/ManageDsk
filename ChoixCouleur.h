#ifndef CHOIXCOULEUR_H
#define CHOIXCOULEUR_H


class CChoixCouleur : public CDialog
{
public:
    CChoixCouleur( int OldCol, CWnd * pParent = NULL );
    BOOL GetRetour( void ) { return( Retour ); }
    int GetCouleur( void ) { return( Couleur ); }
    CStatic m_NumCol;
    CStatic m_ColChoix;
    CStatic m_StaticTxtCol;
    CStatic m_Col[ 27 ];
    CButton m_BpOk, m_BpCancel;

protected:
    virtual void DoDataExchange( CDataExchange * pDX );
    void AffichePalette( int c );
    virtual BOOL OnInitDialog();
    afx_msg void OnCol00() { AffichePalette( 0 ); }
    afx_msg void OnCol01() { AffichePalette( 1 ); }
    afx_msg void OnCol02() { AffichePalette( 2 ); }
    afx_msg void OnCol03() { AffichePalette( 3 ); }
    afx_msg void OnCol04() { AffichePalette( 4 ); }
    afx_msg void OnCol05() { AffichePalette( 5 ); }
    afx_msg void OnCol06() { AffichePalette( 6 ); }
    afx_msg void OnCol07() { AffichePalette( 7 ); }
    afx_msg void OnCol08() { AffichePalette( 8 ); }
    afx_msg void OnCol09() { AffichePalette( 9 ); }
    afx_msg void OnCol10() { AffichePalette( 10 ); }
    afx_msg void OnCol11() { AffichePalette( 11 ); }
    afx_msg void OnCol12() { AffichePalette( 12 ); }
    afx_msg void OnCol13() { AffichePalette( 13 ); }
    afx_msg void OnCol14() { AffichePalette( 14 ); }
    afx_msg void OnCol15() { AffichePalette( 15 ); }
    afx_msg void OnCol16() { AffichePalette( 16 ); }
    afx_msg void OnCol17() { AffichePalette( 17 ); }
    afx_msg void OnCol18() { AffichePalette( 18 ); }
    afx_msg void OnCol19() { AffichePalette( 19 ); }
    afx_msg void OnCol20() { AffichePalette( 20 ); }
    afx_msg void OnCol21() { AffichePalette( 21 ); }
    afx_msg void OnCol22() { AffichePalette( 22 ); }
    afx_msg void OnCol23() { AffichePalette( 23 ); }
    afx_msg void OnCol24() { AffichePalette( 24 ); }
    afx_msg void OnCol25() { AffichePalette( 25 ); }
    afx_msg void OnCol26() { AffichePalette( 26 ); }
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

private :
    int Couleur;
    BOOL Retour;
};


#endif
