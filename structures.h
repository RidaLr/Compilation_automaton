#ifndef STRUCTURES_H
#define STRUCTURES_H
#define EPSILON 'e'
typedef struct etat etat;
typedef struct transition transition;
typedef enum automate_type {ACCEPTEUR, NON_ACCEPTEUR, MORT} automate_type;
struct transition{
	char c;
	etat* suiv;
};


struct etat{
	
	int num;
	automate_type type;
	int nbre_transitions;
	transition* transitions; // Tableau de transitions à partir de cet etat
	
};


typedef struct AFND{
	
	etat *s; //Ã©tat initial
	etat* Q; // ensemble fini d'Ã©tat
	int nbre_etats;
	//etat* F; //ensemble des Ã©tats accepteurs
	char* Sigma; //alphabet de l'automate
	int taille_alphabet;
	//transition* delta; //ensemble des transitions de l'automate
	
}AFND;

// ############ A F D ################//
typedef struct concatenated_etat concatenated_etat;
typedef struct AFD_transition AFD_transition;
struct concatenated_etat{
	
	int num;
	int* concatenatedNums;
	int concatenatedNumSize;
	automate_type type;
	int nbre_transitions;
	AFD_transition* transitions; // Tableau de transitions à partir de cet etat
	
};
struct AFD_transition{
	char c;
	concatenated_etat* suiv;
};
typedef struct AFD{
	
	concatenated_etat* s;
	concatenated_etat* Q;
    int nbre_etats;
	//etat* F;
	char* Sigma;
	int taille_alphabet;
	//transition* delta;
	
}AFD;
typedef struct transitionTemps
{
	int etatId;
	transition *Tabtransition;
	int taille;
}transitionTemps;


#endif
