#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

AFND automate_vide()
{
	AFND automate;
	etat e;
	e.num = 0;
	e.t = NULL;
	e.type = 0;
	
	automate.s = 0;
	automate.Q = malloc(sizeof(etat));
	automate.Q->num = e.num;
	automate.F = NULL;
	automate.Sigma = NULL;
	automate.delta = NULL;
	
	return automate;
}

AFND automate_Seul_Mot_vide()
{
	
	AFND automate;
	etat e;
	e.num = 0;
	e.t = NULL;
	e.type = 1;
	
	automate.s = 0;
	automate.Q = malloc(sizeof(etat));
	automate.Q->num = e.num;
	automate.F = NULL;
	automate.Sigma = NULL;
	automate.delta = NULL;
	
	return automate;
	
}

AFND automate_standard(char c)
{
	
	AFND automate;
	etat *e0 = malloc(sizeof(etat));
	etat *e1 = malloc(sizeof(etat));
	transition *tr;
	
	e0->num = 0;
	e0->t = NULL;
	e0->type = 1;
	
	
	e1->num = 1;
	e1->t = NULL;
	e1->type = 1;
	
	tr->c = c;
	tr->curr = e0;
	tr->suiv = e1;
	
	e0->t = tr;
	
	automate.s = 0;
	automate.Q = malloc(sizeof(etat));
	automate.Q = e0;
	automate.Q = e1;
	
	automate.F = e1;
	automate.Sigma = &c;
	automate.delta = tr;
	
	return automate;
	
}


/*AFND reunion_automate(AFND a1, AFND a2)
{
	
}


AFND concatenation_automate(AFND a1, AFND a2)
{
	
}


AFND farmeture_automate(AFND automate)
{
	
	
}*/

void Display(AFND automate)
{
	
	printf(" \n ========== Affichage automate ===========\n\n");
	
	printf(" s = %d \n",automate.s);
	
	printf("Q = {");
	for(int i=0; i< sizeof(etat); i++)
	{
		printf("%d, ",automate.Q->num);
	}
	printf("}\n\n");
	
}
