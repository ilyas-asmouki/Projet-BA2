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
		}
	}
	return;
}	

Simulation::Simulation(std::ifstream& fichier) 
{
	lecture(fichier);
}

Simulation::Simulation(char* file) : fichier(file)
{
	lecture(fichier);
}

void Simulation::lecture(std::ifstream& file)
{
	std::string line;
	if (!file.fail())
	{
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
	
unsigned Simulation::s_getnbUpdate()
{
	return spatial_getnbUpdate();
}

unsigned Simulation::s_getnbNr()
{
	return spatial_getnbNr();
}

unsigned Simulation::s_getnbNs()
{
	return spatial_getnbNs();
}

unsigned Simulation::s_getnbNd()
{
	return spatial_getnbNd();
}

unsigned Simulation::s_getnbRr()
{
	return spatial_getnbRr();
}

unsigned Simulation::s_getnbRs()
{
	return spatial_getnbRs();
}

unsigned Simulation::p_getnbP()
{
	return getnbP();
}

void sauvegarde(std::string file)
{
	std::ofstream fichier(file);
	fichier<<getnbP()<<std::endl;
	for (unsigned i(0); i < getnbP(); ++i)
	{
		fichier<<p_getforme(i).centre.x<<" "<<p_getforme(i).centre.y<<" "<<p_getforme(i).cote<<std::endl;
	}
	fichier<<std::endl<<r_getForme(0).centre.x<<" "<<r_getForme(0).centre.y<<" "<<r_getForme(0).rayon<<std::endl;
	
	for (unsigned i(1); i < spatial_getnbRs()+1 ; ++i)
	{
		fichier<<r_getForme(i).centre.x<<" "<<r_getForme(i).centre.y<<std::endl;
	}
	
	for (unsigned i(spatial_getnbRs()); i < spatial_getnbRs()+spatial_getnbNs()+1; ++i)
	{
		fichier<<r_getForme(i).centre.x<<" "<<r_getForme(i).centre.y<<
			neutra_getorientation(i)<<" "<< neutra_gettype(i)<<" ";
		if (neutra_getpanne(i) == 0)
		{
			fichier<<"false ";
		}
		else
		{
			fichier<<"true ";
		}
		fichier<<neutra_getk_update(i)<<std::endl;
	}
	return;
}

void draw_world()
{
	draw_robots();
	draw_particles();
	return;
}
	
void Simulation::destroy_data()
{
	destroy_tab_robots();
	destroy_tab_particule();
	return;
}
	
	
	
		
	

