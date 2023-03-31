//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "simulation.h"
#include "robot.h"
#include "particule.h"
#include "shape.h"
#include "message.h"

void decodage_ligne(std::string line)
{
	enum ETAT{NBP, PARTICULE, SPATIAL, REPARATEUR, NEUTRALISEUR};
	static int etat(NBP);
	std::istringstream data(line);
	static int compteur1(0);
	static int compteur2(0);
	static int i(0);
	
	switch (etat)
	{
	case NBP : 
	data>>compteur1;
	if (compteur1==0)
	{
		etat = SPATIAL;
	} 
	else 
	{
		etat = PARTICULE;
	}
	break;
	case PARTICULE :
	decodage_particule(data);
	++i;
	if (compteur1==i)
	{
		etat = SPATIAL;
	}
	break;
	
	case SPATIAL :
	decodage_robot(data, SPATIAL, compteur1, compteur2);
	etat = REPARATEUR;
	i=0;
	break;
	
	case REPARATEUR :
	decodage_robot(data, REPARATEUR, compteur1, compteur2);
	++i;
	if (compteur2 == i)
	{
		etat = NEUTRALISEUR;
		i=0;
	}
	break;
	
	case NEUTRALISEUR :
	decodage_robot(data, NEUTRALISEUR, compteur1, compteur2);
	++i;
	if (compteur1 == i)
	{
		std::cout<<message::success();
		exit(EXIT_SUCCESS);
	}
	}
	return;
}	

Simulation::Simulation(char* fichier) : file(fichier)
{
	lecture();
}

void Simulation::lecture()
{
	std::string line;
	if (!file.fail()){
		while (getline(file>>std::ws,line))
		{
			if (line[0]=='#')
			{
				continue;
			}
			decodage_ligne(line);
		}
	}
	else
	{
		exit(EXIT_FAILURE);
	}
	return;
}
