#include "simulation.h"
#include <iostream>
using namespace std;

void erreur_fichier(int argc);

int main(int argc, char* argv[])
{	
	erreur_fichier(argc);
	Simulation Propre_en_Ordre;
	Propre_en_Ordre.lecture(argv[2]);
	return 0;
}

void erreur_fichier(int argc){
	if (argc == 1){
		exit(EXIT_FAILURE);
	}
	return;
}
