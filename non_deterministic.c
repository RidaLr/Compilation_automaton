#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "non_deterministic.h"
#define C_SIZE 1024


void Desalloc_AFND(AFND *automate)
{
	etat* state;
	for(int i=0; i<automate->nbre_etats; i++)
	{
		for(int j=0; j<automate->Q[i].nbre_transitions; j++)
		{
			state = automate->Q[i].transitions[j].suiv;
			free(state);
		}
		free(automate->Q[i].transitions);
	}
	
	free(automate->Sigma);
	free(automate->s);
		
}

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
	automate->Sigma = (char*)malloc(sizeof(char));
	automate->Sigma = NULL;
	//automate->delta = NULL;
	
	return automate;
}

AFND *automate_Seul_Mot_vide()
{
	
	etat *init = (etat*)(malloc(sizeof(etat)));
	init->num = 0;
	init->type = ACCEPTEUR;


    //transition* transitions = (transition*)(malloc(sizeof(transition)));
    //transitions->c = NULL;

	//etat *fin = (etat*)(malloc(sizeof(etat)));
	//fin->num = 1;
	//fin->type = ACCEPTEUR;
    //fin->transitions = NULL;
    //fin->nbre_transitions = 0;
    //transitions->suiv = fin;
	
    init->transitions = NULL;
    init->nbre_transitions = 0;
	
    AFND *automate = (AFND*) (malloc(sizeof(AFND)));
	automate->s = init;
	automate->Q = (etat*)malloc(sizeof(etat) * 1);
	automate->Q[0] = *init;
   // automate->Q[1] = *fin;
    automate->nbre_etats = 1;
	//automate.F = NULL;
	automate->Sigma = (char*)malloc(sizeof(char));
	automate->Sigma = NULL;
	//automate->delta = NULL;
	
	return automate;
	
}

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
    //automate->delta = (transition*)malloc(sizeof(transition));
    //automate->delta = transitions;
	
	return automate;
	
}


AFND *reunion_automate(AFND* automate1, AFND* automate2)
{
	
	if( 1 == 0)
	{
		
		/*CODE*/
		
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


AFND *concatenation_automate(AFND *automate1, AFND *automate2)
{
	
	if( 1 == 0)
	{
		
		/*CODE*/
		
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
	
}	


AFND *automateTestPlein()
{

    etat *init = (etat *)(malloc(sizeof(etat)));
    init->num = 0;
    init->type = NON_ACCEPTEUR;

    etat *un = (etat *)(malloc(sizeof(etat)));
    un->num = 1;
    un->type = NON_ACCEPTEUR;

    etat *deux = (etat *)(malloc(sizeof(etat)));
    deux->num = 2;
    deux->type = NON_ACCEPTEUR;

    etat *trois = (etat *)(malloc(sizeof(etat)));
    trois->num = 3;
    trois->type = ACCEPTEUR;

    transition transitionsInit0;
    transitionsInit0.c = 'a';
    transitionsInit0.suiv = init;

    transition transitionsInit1;
    transitionsInit1.c = 'b';
    transitionsInit1.suiv = init;

    transition transitionsInit2;
    transitionsInit2.c = 'a';
    transitionsInit2.suiv = un;
    init->nbre_transitions = 3;
    init->transitions = (transition*)(malloc(sizeof(transition) * 3));
    init->transitions[0] = transitionsInit0;
    init->transitions[1] = transitionsInit1;
    init->transitions[2] = transitionsInit2;
    init->nbre_transitions = 3;

    //2
    transition transitionsInit3;
    transitionsInit3.c = 'a';
    transitionsInit3.suiv = deux;

    transition transitionsInit4;
    transitionsInit4.c = 'b';
    transitionsInit4.suiv = deux;
    un->transitions = (malloc(sizeof(transition) * 2));
    un->transitions[0] = transitionsInit3;
    un->transitions[1] = transitionsInit4;
    un->nbre_transitions = 2;
    //3
    transition transitionsInit5;
    transitionsInit5.c = 'a';
    transitionsInit5.suiv = trois;
    deux->transitions = (malloc(sizeof(transition)));
    deux->transitions[0] = transitionsInit5;
    deux->nbre_transitions = 1;

    AFND *automate = (AFND *)(malloc(sizeof(AFND)));
    automate->s = init;
    automate->Q = (etat *)malloc(sizeof(etat) * 4);
    automate->Q[0] = *init;
    automate->Q[1] = *un;
    automate->Q[2] = *deux;
    automate->Q[3] = *trois;
    automate->nbre_etats = 4;
    
    automate->Sigma = (char *)malloc(sizeof(char) * 2);
    automate->Sigma[0] = 'a';
    automate->Sigma[1] = 'b';
    automate->taille_alphabet = 2;

    return automate;
}

void Test_AFND()
{
	//Le test de l'automate standard reconnaissnat le langage vide
	AFND *aVide = automate_vide();
	Display(aVide);
	
	//Le test de l'automate standard reconnaissant le langage composÃ© du seul mot vide
	AFND *aSeulMot = automate_Seul_Mot_vide();
	Display(aSeulMot);
	
	//Le test de l'automate standard reconnaissant le langage composÃ© d'un mot d'un caractere passÃ© en parametre
	AFND *aStandard = automate_standard('a');
	Display(aStandard);
	
	//Automates de test 
	AFND *aTest1 = automateTestPlein();
	AFND *aTest2 = automateTestPlein();
	AFND *aTest3 = automateTestPlein();
	AFND *aTest4 = automateTestPlein();
	
	//Le test qui renvoi un automate standard reconnaissant la reunion des langages de deux automates  
	AFND *aReunion = reunion_automate(aTest1, aTest2);
	Display(aReunion);
	
	//Le test qui renvoi un automate standard reconnaissant la concatenation des langages de deux automates standard
	AFND *aConcat = concatenation_automate(aTest3, aTest4);
	Display(aConcat);
	
	//Le test qui renvoi un automate standard reconnaissant la fermeture iterative de Kleene de son langage 
	AFND *aFermTest = automateTestPlein();
	AFND *aFermeKleene = farmeture_automate(aFermTest);
	Display(aFermeKleene);
}
