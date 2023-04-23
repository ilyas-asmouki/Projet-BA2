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

void decodage_ligne(std::string line, bool& file_success)
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
		decodage_particule(data, file_success);
		++i;
		if (compteur1==i)
		{
			etat = SPATIAL;
		}
		break;
	case SPATIAL :
		decodage_robot(data, SPATIAL, compteur1, compteur2, file_success);
		etat = REPARATEUR;
		i=0;
		break;
	
	case REPARATEUR :
		decodage_robot(data, REPARATEUR, compteur1, compteur2, file_success);
		++i;
		if (compteur2 == i)
		{
			etat = NEUTRALISEUR;
			i=0;
		}
		break;
	
	case NEUTRALISEUR :
		decodage_robot(data, NEUTRALISEUR, compteur1, compteur2, file_success);
		++i;
		if (compteur1 == i)
		{
			etat = NBP;
			i=0;
			if (file_success)
			{
				std::cout<<message::success();
			}
		}
		break;
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

Simulation::~Simulation()
{
}

bool Simulation::getfile_success()
{
	return file_success;
}

void Simulation::lecture(std::ifstream& file)
{
	std::string line;
	while (getline(file>>std::ws,line))
	{
		if (line[0]=='#')
		{
			continue;
		}
		decodage_ligne(line, file_success);
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

void Simulation::sauvegarde(std::string file)
{
	std::ofstream fichier(file);
	fichier<<getnbP()<<std::endl;
	for (unsigned i(0); i < getnbP(); ++i)
	{
		fichier<<"\t"<<p_getforme(i).centre.x<<" "<<p_getforme(i).centre.y<<" "<<p_getforme(i).cote<<std::endl;
	}
	fichier<<std::endl<<r_getForme(0).centre.x<<" "<<r_getForme(0).centre.y<<" "<<spatial_getnbUpdate()<<
		" "<<spatial_getnbNr()<<" "<<spatial_getnbNs()<<" "<<spatial_getnbNd()<<" "<<spatial_getnbRr()<<" "<<
		spatial_getnbRs()<<std::endl;
	
	for (unsigned i(1); i < spatial_getnbRs()+1 ; ++i)
	{
		fichier<<"\t"<<r_getForme(i).centre.x<<" "<<r_getForme(i).centre.y<<std::endl;
	}
	
	fichier<<std::endl;
	
	for (unsigned i(spatial_getnbRs()+1); i < spatial_getnbRs()+spatial_getnbNs()+1; ++i)
	{
		fichier<<"\t"<<r_getForme(i).centre.x<<" "<<r_getForme(i).centre.y<<" "<<
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
	
void Simulation::desintegration_status()
{
	desintegration(file_success, engine);
}
	
		
	

