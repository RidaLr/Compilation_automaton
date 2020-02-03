#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "non_deterministic.h"
#include "deterministic.h"

int main()
{
	AFND *automate = automate_Seul_Mot_vide();
	Display(automate);

	AFND *automate2 = automate_vide();
	Display(automate2);
	
	return 0;
}
