#ifndef DETERMINISTIC_H
#define DETERMINISTIC_H
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

void executeDeterministicOnWord(char *word, AFD* afd);
int alreadyDeterministic(AFND afnd);
AFND *automateTestPlein();
concatenated_etat ** createNewStatesMAtrix(AFND afnd, int *resulting_AFD_nbreEtats);
AFD *create_AFD_fromStateMatrix(concatenated_etat **newStatesMAtrix, AFND afndFrom, int nbreEtats);
void displayDeterministic_AFD(AFD afd);
int bilanEqualsInit(int *tabInit, int *tabBilan, int taille);
void constructBilan(int **deterministicMatrix, AFD afd, int *tabInit, int *tabBilan, int *lastTypeNum, int *tabTypes);
AFD *constructBilan_AFD(AFD afd);
void updateDeterministicMatrix(int* tabBilan, int **deterministicMatrix, AFD afd);
AFND *automateTestPlein2();
void addDeathState(AFD *afd);
AFD *AFND_to_AFD_recopie(AFND afnd);
AFD *constructMinisedAFD_from_bilan(int **deterministicMatrix, AFD unMinimisedAFD, int *tabBilan);
void destroyAFDEtat(concatenated_etat *AFDetat);
void removeDeathState(AFD *afd);
void destroyAFD(AFD *finishedAFD);
#endif