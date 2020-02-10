#ifndef NON_DETERMINISTIC_H
#define NON_DETERMINISTIC_H
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
AFND *automate_vide();
AFND *automate_Seul_Mot_vide();
AFND *automate_standard(char c);

AFND *reunion_automate(AFND *a1, AFND *a2);
AFND *concatenation_automate(AFND *a1, AFND *a2);
AFND *farmeture_automate(AFND *automate);
void Display(AFND *automate);
AFND *automateTestPlein();
#endif
