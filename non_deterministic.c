#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#define C_SIZE 1024

AFND automate_vide()
{
	AFND automate;
	etat e;
	e.num = 0;
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
	while(automate.Q)
	{
		printf("%d, ",automate.Q->num);
		automate.Q = automate.Q->suiv;
	}
	printf("}\n\n");
	
	size_t length = strlen(automate.Sigma);
	printf("Sigma = {");
	for(int i=0; i < length; i++)
	{
		printf("%c, ",automate.Sigma[i]);
	}
	printf("}\n\n");
	
	
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
	
	
}
