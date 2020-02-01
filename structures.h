#ifndef STRUCTURES_H
#define STRUCTURES_H


typedef struct transition{
	
	char c;
	etat suiv;
	
}transition;


typedef struct etat{
	
	int num;
	transition* t;
	bool type;
	
}etat;

typedef struct AFND{
	
	int s;
	etat* Q;
	etat* F;
	etat* Sigma;
	transition* delta;
	
}AFND;


typedef struct AFD{
	
	int s;
	etat* Q;
	etat* F;
	etat* Sigma;
	transition* delta;
	
}AFD;


AFND automate_vide();
AFND automate_Seul_Mot();
AFND automate_standard(char c);

AFND reunion_automate(AFND a1, AFND a2);
AFND concatenation_automate(AFND a1, AFND a2);
AFND farmeture_automate(AFND automate);



#endif
