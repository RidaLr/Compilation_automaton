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
	
	AFND *automate3 = automateTestPlein();
	AFND *automate5 = automateTestPlein();
	
	Display(automate5);

	
	AFND *automate4 = reunion_automate(automate3, automate5);
	printf("\n UNION \n");
	Display(automate4);
	
	AFND *automate8 = automateTestPlein();
	AFND *automate9 = automateTestPlein();
	
	AFND *automate6 = concatenation_automate(automate8, automate9);
	printf("\n CONCATENATION \n");
	Display(automate6);
	
	//AFND *automate7 = farmeture_automate(automate3);
	
	return 0;
}
