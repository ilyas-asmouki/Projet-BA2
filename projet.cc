//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#include "simulation.h"
#include <iostream>

void erreur_fichier(int argc);

int main(int argc, char* argv[])
{	
	erreur_fichier(argc);
	Simulation Propre_en_Ordre(argv[1]);
	return 0;
}

void erreur_fichier(int argc)
{
	if (argc == 1)
	{
		exit(EXIT_FAILURE);
	}
}
