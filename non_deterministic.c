#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "non_deterministic.h"
#define C_SIZE 1024

AFND *automate_vide()
{
	AFND *automate = (AFND*) (malloc(sizeof(AFND)));
	etat *e = (etat*)(malloc(sizeof(etat)));
	e->num = 0;
	e->type = NON_ACCEPTEUR;
    e->transitions = NULL;
	
	automate->s = e;
	automate->Q = (etat*)malloc(sizeof(etat));
	automate->Q = e;
    automate->nbre_etats = 1;
	//automate.F = NULL;
	//automate.Sigma = NULL;
	//automate.delta = NULL;
	
	return automate;
}

AFND *automate_Seul_Mot_vide()
{
	
	etat *init = (etat*)(malloc(sizeof(etat)));
	init->num = 0;
	init->type = NON_ACCEPTEUR;


    transition* transitions = (transition*)(malloc(sizeof(transition)));
    transitions->c = EPSILON;

	etat *fin = (etat*)(malloc(sizeof(etat)));
	fin->num = 1;
	fin->type = ACCEPTEUR;
    fin->transitions = NULL;
    fin->nbre_transitions = 0;
    transitions->suiv = fin;
	
    init->transitions = transitions;
    init->nbre_transitions = 1;
	
    AFND *automate = (AFND*) (malloc(sizeof(AFND)));
	automate->s = init;
	automate->Q = (etat*)malloc(sizeof(etat) * 2);
	automate->Q[0] = *init;
    automate->Q[1] = *fin;
    automate->nbre_etats = 2;
	//automate.F = NULL;
	//automate.Sigma = NULL;
	//automate.delta = NULL;
	
	return automate;
	
}
/*
AFND automate_standard(char c)
{
	
	AFND automate;
	etat *e0 = malloc(sizeof(etat));
	etat *e1 = malloc(sizeof(etat));
	transition *tr;
	
	e0->num = 0;
	e0->type = 1;
	
	
	e1->num = 1;
	e1->type = 1;
	
	e0->suiv = e1;
	
	tr->c = c;
	tr->curr = e0;
	tr->suiv = e1;
	tr->tsuiv = NULL;
	
	automate.s = 0;
	automate.Q = malloc(sizeof(etat));
	automate.Q->num = 0;
	automate.Q->suiv = e1;
	
	automate.F = e1;
	automate.Sigma = (char*)malloc(sizeof(char)*C_SIZE);
	automate.Sigma[0] = c;
	automate.delta = tr;
	
	return automate;
	
}
*/

/*AFND reunion_automate(AFND a1, AFND a2)
{
	
}


AFND concatenation_automate(AFND a1, AFND a2)
{
	
}


AFND farmeture_automate(AFND automate)
{
	
	
}*/

void Display(AFND *automate)
{
	
	printf(" \n ========== Affichage automate ===========\n\n");
	
	printf("s = %d \n",automate->s->num);
	
	for (int i = 0; i < automate->nbre_etats; i++)
    {
		printf("+ ETAT: %d\n ",automate->Q[i].num);
        if(automate->Q[i].type == ACCEPTEUR)
            printf(" > TYPE: ACCEPTEUR\n ");
        else
            printf(" > TYPE: NON ACCEPTEUR\n ");
        
        printf(" TRANSITIONS = {");
        for (int j = 0; j < automate->Q[i].nbre_transitions; j++){
            printf("(%d, %c, %d),", automate->Q[i].num, automate->Q[i].transitions[j].c, automate->Q[i].transitions[j].suiv->num);
        }
        printf(" }\n\n");
	}
	/*
	size_t length = strlen(automate.Sigma);
	printf("Sigma = {");
	for(int i=0; i < length; i++)
	{
		printf("%c, ",automate.Sigma[i]);
	}
	printf("}\n\n");
	*/
	/*
	printf("F = {");
	while(automate.F)
	{
		printf("%d, ",automate.F->num);
		automate.F = automate.F->suiv;
	}
	printf("}\n\n");
	
	
	printf("Delta = {");
	while(automate.delta)
	{
		printf("(%d, %c, %d) ,",automate.delta->curr->num,automate.delta->c,automate.delta->suiv->num);
		automate.delta = automate.delta->tsuiv;
	}
	printf("}\n\n");
    */
	
	
}
