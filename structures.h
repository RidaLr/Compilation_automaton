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


#endif
