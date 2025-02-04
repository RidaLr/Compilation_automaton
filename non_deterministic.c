#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "non_deterministic.h"
#define C_SIZE 1024


void Desalloc_AFND(AFND *automate)
{
	//etat* state;
	for(int i=0; i<automate->nbre_etats; i++)
	{
		free(automate->Q[i].transitions);
	}
	
	free(automate->Sigma);
	free(automate->s);
		
}

//La fonction qui renvoie un automate standard reconnaissant le langage vide
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

	automate->Sigma = (char*)malloc(sizeof(char));
	automate->Sigma = NULL;

	
	return automate;
}


//La fonction qui renvoie un automate standard reconnaissant le langage composé du seul mot vide
AFND *automate_Seul_Mot_vide()
{
	
	etat *init = (etat*)(malloc(sizeof(etat)));
	init->num = 0;
	init->type = ACCEPTEUR;

	
    init->transitions = NULL;
    init->nbre_transitions = 0;
	
    AFND *automate = (AFND*) (malloc(sizeof(AFND)));
	automate->s = init;
	automate->Q = (etat*)malloc(sizeof(etat) * 1);
	automate->Q[0] = *init;
  
    automate->nbre_etats = 1;
	
	automate->Sigma = (char*)malloc(sizeof(char));
	automate->Sigma = NULL;
	
	return automate;
}

// La fonction qui renvoie un automate standard reconnaissant le langage composé d’un mot d’un caractère passé en paramètre
AFND *automate_standard(char c)
{
		
	etat *init = (etat*)(malloc(sizeof(etat)));
	init->num = 0;
	init->type = NON_ACCEPTEUR;


    transition transition0;
    transition0.c = c;

	etat *fin = (etat*)(malloc(sizeof(etat)));
	fin->num = 1;
	fin->type = ACCEPTEUR;
    fin->transitions = NULL;
    fin->nbre_transitions = 0;
    transition0.suiv = fin;
	
    init->transitions = (transition*)(malloc(sizeof(transition)));
    init->transitions[0] = transition0;
    init->nbre_transitions = 1;
	
    AFND *automate = (AFND*) (malloc(sizeof(AFND)));
	automate->s = init;
	automate->Q = (etat*)malloc(sizeof(etat) * 2);
	automate->Q[0] = *init;
    automate->Q[1] = *fin;
    automate->nbre_etats = 2;
    automate->Sigma = (char*)malloc(sizeof(char));
    automate->Sigma[0] = c;
	
	return automate;
}

/*La fonction qui prend deux automates standard en paramètre, et renvoie un automate standard reconnaissant 
 *la réunion de leurs langages*/
AFND *reunion_automate(AFND* automate1, AFND* automate2)
{
	
	if( automate1->nbre_etats < 1)
		return automate2;
	if( automate2->nbre_etats < 1)
		return automate1;
	
	if(automate1->nbre_etats == 1){
		if(automate2->nbre_etats ==1 ){
			if(automate2->s->type == ACCEPTEUR || automate1->s->type == ACCEPTEUR){
				automate1->s->type = ACCEPTEUR;
			}
			for (int i = 0; i < automate2->s->nbre_transitions; i++)
			{
				int go=1;
				for (int j = 0; j < automate1->s->nbre_transitions; j++)
				{
					if(automate1->s->transitions[j].c == automate2->s->transitions[i].c)
					go = 0;
				}
				if(go){
				automate1->s->transitions = (transition*)realloc(automate1->s->transitions, sizeof(transition) * (automate1->s->nbre_transitions +1));
				automate1->s->transitions[automate1->s->nbre_transitions] = automate2->s->transitions[i];
				automate1->s->nbre_transitions ++;
				}
			}
			
			return automate1;
		}else
		{
			if(automate2->s->type == ACCEPTEUR || automate1->s->type == ACCEPTEUR){
				automate2->s->type = ACCEPTEUR;
			}
			for (int i = 0; i < automate1->s->nbre_transitions; i++)
			{
				int go=1;
				for (int j = 0; j < automate2->s->nbre_transitions; j++)
				{
					if(automate2->s->transitions[j].c == automate1->s->transitions[i].c)
					go = 0;
				}
				if(go){
				automate2->s->transitions = (transition*)realloc(automate2->s->transitions, sizeof(transition) * (automate2->s->nbre_transitions +1));
				automate2->s->transitions[automate2->s->nbre_transitions] = automate1->s->transitions[i];
				automate2->s->nbre_transitions ++;
				}
				
			}
			return automate2;
		}
		
	}else
	{
		if(automate2->nbre_etats ==1 ){
			if(automate2->s->type == ACCEPTEUR || automate1->s->type == ACCEPTEUR){
				automate1->s->type = ACCEPTEUR;
			}
			for (int i = 0; i < automate2->s->nbre_transitions; i++)
			{
				int go=1;
				for (int j = 0; j < automate1->s->nbre_transitions; j++)
				{
					if(automate1->s->transitions[j].c == automate2->s->transitions[i].c)
					go = 0;
				}
				if(go){
				automate1->s->transitions = (transition*)realloc(automate1->s->transitions, sizeof(transition) * (automate1->s->nbre_transitions +1));
				automate1->s->transitions[automate1->s->nbre_transitions] = automate2->s->transitions[i];
				automate1->s->nbre_transitions ++;
				}
			}
			return automate1;
		}
	}
	
	
		//Change all nums of the first automaton to avoid conflict between  nums
		int nextNum = 0;
		for(int i=1; i < automate2->nbre_etats; i++)
		{
			if(nextNum < automate2->Q[i].num+1)
			{
				nextNum = automate2->Q[i].num+1;	
			}
		}
		
		for(int i=1; i < automate1->nbre_etats; i++)
		{
			etat currentState = automate1->Q[i];
			//Find all transitions that point to me to change the suiv's num (rename)
			for(int j=0; j < automate1->nbre_etats; j++)
			{
				for(int k=0; k < automate1->Q[j].nbre_transitions; k++)
				{
					if(automate1->Q[j].transitions[k].suiv->num == currentState.num)
					{
						automate1->Q[j].transitions[k].suiv->num = nextNum;	
					}
				}
			}
			
			automate1->Q[i].num = nextNum;
			nextNum++;
		}
		
		for(int i=0; i < automate2->s->nbre_transitions; i++)
		{
			int prendre = 1;//Permet de ne pas gÃ©nÃ©rer des doublons sur letat initial rÃ©sulat lorsque les deux etats initiaux pointent vers eux memes et avec les memes lettres
			for(int j=0; j<automate1->s->nbre_transitions; j++)
			{
				if(automate1->s->transitions[j].c == automate2->s->transitions[i].c && automate1->s->transitions[j].suiv == automate1->s && automate2->s->transitions[i].suiv == automate2->s)
				{
					prendre = 0;
					break;
				}
			}
			
			if(prendre)
			{
				automate1->s->transitions = (transition*) realloc(automate1->s->transitions, sizeof(transition) * (automate1->s->nbre_transitions + automate2->s->nbre_transitions));
				transition trans;
				trans.c = automate2->s->transitions[i].c;
				trans.suiv = automate2->s->transitions[i].suiv;
				automate1->s->transitions[automate1->s->nbre_transitions] = trans;
				automate1->s->nbre_transitions++;
				automate1->Q[0] = *automate1->s; 
			}
		}	
	
	
	int oldNumber = automate1->nbre_etats;
	automate1->nbre_etats += automate2->nbre_etats -1;
	automate1->Q = (etat*)realloc(automate1->Q, (automate1->nbre_etats + automate2->nbre_etats -1) * sizeof(etat));
	
	for(int i=1; i < automate2->nbre_etats; i++)
	{
		automate1->Q[oldNumber] = automate2->Q[i];
		oldNumber++;
	}
	
	return automate1;
}

/* La fonction qui prend deux automates standard en paramètre, et renoie un automate standard reconnaissant 
 *la concaténation de leurs langages*/
AFND *concatenation_automate(AFND *automate1, AFND *automate2)
{
	if(	automate1->nbre_etats < 1)
	return automate2;
	if(automate2->nbre_etats < 1)
	return automate1;
	if( automate1->nbre_etats < 2){
		if(automate1->s->type == ACCEPTEUR){
			automate2->s->type = ACCEPTEUR;
		}
		return automate2;
	}
	if( automate2->nbre_etats < 2){
		if(automate2->s->type == ACCEPTEUR){
			automate1->s->type = ACCEPTEUR;
		}
		return automate1;
	}

	else
	{
		//Change all nums of the first automaton to avoid conflict between  nums
		int nextNum = 0;
		for(int i=1; i < automate2->nbre_etats; i++)
		{
			if(nextNum < automate2->Q[i].num+1)
			{
				nextNum = automate2->Q[i].num+1;	
			}
		}
		
		for(int i=1; i < automate1->nbre_etats; i++)
		{
			etat currentState = automate1->Q[i];
			//Find all transitions that point to me to change the suiv's num (rename)
			for(int j=0; j < automate1->nbre_etats; j++)
			{
				for(int k=0; k < automate1->Q[j].nbre_transitions; k++)
				{
					if(automate1->Q[j].transitions[k].suiv->num == currentState.num)
					{
						automate1->Q[j].transitions[k].suiv->num = nextNum;	
					}
				}
			}
			
			automate1->Q[i].num = nextNum;
			nextNum++;
		}
		
		
		for(int i =0;i<automate1->nbre_etats;i++)
		{
			
			if(automate1->Q[i].type==ACCEPTEUR)
			{
			
				automate1->Q[i].transitions = (transition*) realloc(automate1->Q[i].transitions, sizeof(transition) * (automate1->Q[i].nbre_transitions + automate2->s->nbre_transitions));
							
				for(int j =0;j<automate2->s->nbre_transitions;j++)
				{
					if(automate2->s->transitions[j].suiv->num != automate2->s->num)
					{
						transition trans;
						trans.c = automate2->s->transitions[j].c;
						trans.suiv = automate2->s->transitions[j].suiv;
						automate1->Q[i].transitions[automate1->Q[i].nbre_transitions] = trans;
						automate1->Q[i].nbre_transitions++;
					}
				}
			}
		}
			
	}
	
	int oldNumber = automate1->nbre_etats;
	automate1->nbre_etats += automate2->nbre_etats -1;
	automate1->Q = (etat*)realloc(automate1->Q, (automate1->nbre_etats + automate2->nbre_etats -1) * sizeof(etat));
	
	for(int i=1; i < automate2->nbre_etats; i++)
	{
		automate1->Q[oldNumber] = automate2->Q[i];
		oldNumber++;
	}
	
	return automate1;
}

/* La fonction qui prend un automate standard en paramètre, et renvoie un automate
 *standard reconnaissant la fermeture itérative de Kleene de son langage*/
AFND *farmeture_automate(AFND *automate)
{
	
	for(int i =0;i<automate->nbre_etats;i++)
		{
			
			if(automate->Q[i].type==ACCEPTEUR)
			{
			
				automate->Q[i].transitions = (transition*) realloc(automate->Q[i].transitions, sizeof(transition) * (automate->Q[i].nbre_transitions + automate->s->nbre_transitions));
							
				for(int j =0;j<automate->s->nbre_transitions;j++)
				{
				
					transition trans;
					trans.c = automate->s->transitions[j].c;
					trans.suiv = automate->s->transitions[j].suiv;
					automate->Q[i].transitions[automate->Q[i].nbre_transitions] = trans;
					automate->Q[i].nbre_transitions++;
				
				}
			}
		}
	
	return automate;
}

//La methode qui affiche un automate passe en parametres
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
        for (int j = 0; j < automate->Q[i].nbre_transitions; j++)
	{
            printf("(%d, %c, %d),", automate->Q[i].num, automate->Q[i].transitions[j].c, automate->Q[i].transitions[j].suiv->num);
        }
        printf(" }\n\n");
	}
	
}	

//Le methode qui test toutes les fonction implementes 
void Test_AFND()
{
	char buff[50];
	/*
	printf("##\tTest  de Réunion\n");
	scanf(" %s", buff);
	AFND *aVide = automate_vide();
	Display(aVide);
	
	//
	printf("##\n\tLe test de l'automate standard reconnaissant le langage composÃ© du seul mot vide, validez en appuyant sur une touche\n");
	scanf(" %s", buff);
	AFND *aSeulMot = automate_Seul_Mot_vide();
	Display(aSeulMot);
	
	//Le test de l'automate standard reconnaissant le langage composÃ© d'un mot d'un caractere passÃ© en parametre
	printf("##\n\tLe test de l'automate standard reconnaissant le langage composÃ© du seul mot vide, validez en appuyant sur une touche\n");
	scanf(" %s", buff);
	AFND *aStandard = automate_standard('a');
	Display(aStandard);
	*/
	//Automates de test 
	AFND *aTest1 = automateTestPlein();
	AFND *aTest2 = automateTestPlein();
	AFND *aTest3 = automateTestPlein();
	AFND *aTest4 = automateTestPlein();
	printf("\t####TEST DE AFND, validez une touche pour afficher l'automate de départ\n");
	scanf(" %s", buff);
	Display(aTest1);

	printf("\t####TEST DE REUNION, l'automate ci dessus sera concatené avec lui même,  validez une touche pour afficher la REUNION\n");
	scanf(" %s", buff);
	
	//Le test qui renvoi un automate standard reconnaissant la reunion des langages de deux automates  
	AFND *aReunion = reunion_automate(aTest1, aTest2);
	Display(aReunion);


	printf("\t####TEST DE CONCATENATION, l'automate de départ (celui sans réunion) sera concatené avec lui même,  validez une touche pour afficher la concatenation\n");
	scanf(" %s", buff);
	//Le test qui renvoi un automate standard reconnaissant la concatenation des langages de deux automates standard
	AFND *aConcat = concatenation_automate(aTest3, aTest4);
	Display(aConcat);
	
	printf("\t####TEST DE FERMETURE TRANSITIVE, sur l'automate (automate de départ sans réunion ni concatenation),  validez une touche pour afficher la FERMETURE\n");
	scanf(" %s", buff);
	//Le test qui renvoi un automate standard reconnaissant la fermeture iterative de Kleene de son langage 
	AFND *aFermTest = automateTestPlein();
	AFND *aFermeKleene = farmeture_automate(aFermTest);
	Display(aFermeKleene);
	/*
	Desalloc_AFND(aTest1);
	Desalloc_AFND(aTest2);
	Desalloc_AFND(aTest3);
	Desalloc_AFND(aTest4);
	Desalloc_AFND(aReunion);
	Desalloc_AFND(aConcat);
	Desalloc_AFND(aFermeKleene);*/
	
}
