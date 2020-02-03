#ifndef DETERMINISTIC_H
#define DETERMINISTIC_H
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
typedef struct AFD{
	
	etat* s;
	etat* Q;
    int nbre_etats;
	//etat* F;
	//char* Sigma;
	//transition* delta;
	
}AFD;
#endif