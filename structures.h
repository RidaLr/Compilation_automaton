#ifndef STRUCTURES_H
#define STRUCTURES_H
#define EPSILON 'e'
typedef struct etat etat;
typedef struct transition transition;
typedef enum automate_type {ACCEPTEUR, NON_ACCEPTEUR} automate_type;
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
	int taille_alphabet;
	char* Sigma; //alphabet de l'automate
	//transition* delta; //ensemble des transitions de l'automate
	
}AFND;



#endif
