#ifndef NON8DETERMINISTIC_H
#define NON8DETERMINISTIC_H
#include "structures.h"

AFND automate_vide();
AFND automate_Seul_Mot_vide();
AFND automate_standard(char c);

AFND reunion_automate(AFND a1, AFND a2);
AFND concatenation_automate(AFND a1, AFND a2);
AFND farmeture_automate(AFND automate);
void Display(AFND automate);


#endif
