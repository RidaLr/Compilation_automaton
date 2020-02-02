
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"


int main()
{
	char c = 'a';
	AFND automate = automate_standard(c);
	Display(automate);
	
	return 0;
}
