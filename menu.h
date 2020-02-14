#ifndef MENU_H
#define MENU_H
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
int displayMenu();
void computeChoice(int choix);
AFND *constructUserDefinedAFND();
int stateAlreadyExists(AFND afnd, int stateId);
void tasksOnUserDefinedAFND();
void exucuteBuildInSamples();
int characterAlreadyExists(AFND afnd, char caracter);
#endif