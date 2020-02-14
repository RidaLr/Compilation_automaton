#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "non_deterministic.h"
#include "deterministic.h"
#include "menu.h"

int main()
{
	while (1)
	{
		//system("clear");
		computeChoice(displayMenu());
	}

	return 0;
}
