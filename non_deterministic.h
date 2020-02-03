#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct etat etat;
typedef struct transition transition;

struct transition{
	
	char c;
	etat* curr;
	etat* suiv;
	transition* tsuiv;
	
};


struct etat{
	
	int num;
	etat* suiv;
	int type; //0 non accepteur /1 accepteur
	
};

typedef struct AFND{
	
	int s; //Ã©tat initial
	etat* Q; // ensemble fini d'Ã©tat
	etat* F; //ensemble des Ã©tats accepteurs
	char* Sigma; //alphabet de l'automate
	transition* delta; //ensemble des transitions de l'automate
	
}AFND;


typedef struct AFD{
	
	int s;
	etat* Q;
	etat* F;
	char* Sigma;
	transition* delta;
	
}AFD;


AFND automate_vide();
AFND automate_Seul_Mot_vide();
AFND automate_standard(char c);

AFND reunion_automate(AFND a1, AFND a2);
AFND concatenation_automate(AFND a1, AFND a2);
AFND farmeture_automate(AFND automate);
void Display(AFND automate);


#endif
