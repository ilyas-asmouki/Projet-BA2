#include "robot.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "message.h"
#include "shape.h"

static std::vector<Robot> tab_robot;
enum{SPATIAL=2, REPARATEUR, NEUTRALISEUR};

Robot::Robot(double x1=0, double y1=0)
{
	forme.centre.x = x1;
	forme.centre.y = y1;
	forme.rayon = 0;
}

Cercle Robot::getforme()
{
	return forme;
}
 
Spatial::Spatial(double x, double y, int nbUpdate ,unsigned nbNr, unsigned nbNs,
	  unsigned nbNd, unsigned nbRr ,unsigned nbRs): Robot(x, y), nbUpdate(nbUpdate), 
	  nbNr(nbNr), nbNs(nbNs), nbNd(nbNd), nbRr(nbRr), nbRs(nbRs)
{
	forme.rayon=r_spatial;
	error_outside();
	test_particle_robot_superposition(forme);
}
	
void Spatial::error_outside()
{
	if ((abs(forme.centre.x+r_spatial)>dmax)or(abs(forme.centre.y+r_spatial)>dmax))
	{
		std::cout<<message::spatial_robot_ouside(forme.centre.x, forme.centre.y);
		exit(EXIT_FAILURE);
	}
	return;
}

Reparateur::Reparateur(double x, double y): Robot(x, y)
{
	forme.rayon=r_reparateur; 
	TestCollision();
	test_particle_robot_superposition(forme);
	tab_robot.push_back(*this);
}
		
Neutraliseur::Neutraliseur(double x, double y, double orientation, unsigned type,
std::string bool_panne, int nbUpdate, int k_update_panne)
 : Robot(x, y), orientation(orientation), type(type), k_update_panne(k_update_panne) 
{
	panne = ((bool_panne == "false") ? false : true);
	forme.rayon=r_neutraliseur;
	error_k_update(nbUpdate);
	TestCollision();
	test_particle_robot_superposition(forme);
	tab_robot.push_back(*this);
}
	
void Neutraliseur::error_k_update(int nbUpdate)
{
	if (k_update_panne > nbUpdate)
	{
		std::cout<<message::invalid_k_update(forme.centre.x, forme.centre.y, 
		k_update_panne, nbUpdate);
		exit(EXIT_FAILURE);
	}
}
	
	
void Robot::TestCollision()
{
	for (size_t i(0); i < tab_robot.size(); ++i)
	{
		if (superposition_cercles(forme, tab_robot[i].getforme(),LECTURE))
		{
			if (forme.rayon == r_reparateur)
			{
				if (forme.rayon == tab_robot[i].getforme().rayon)
				{
					std::cout << message::repairers_superposition(forme.centre.x,
					forme.centre.y, tab_robot[i].getforme().centre.x,
					tab_robot[i].getforme().centre.y);
				} else
				{ 
					std::cout << message::repairer_neutralizer_superposition(forme
					.centre.x, forme.centre.y, tab_robot[i].getforme().centre.x,
					tab_robot[i].getforme().centre.y);	
				}
			} else
			{
				if (forme.rayon == tab_robot[i].getforme().rayon)
				{
					std::cout << message::neutralizers_superposition(forme.centre.x,
					forme.centre.y,	tab_robot[i].getforme().centre.x,
					tab_robot[i].getforme().centre.y);
				} else
				{ 
					std::cout<<message::repairer_neutralizer_superposition(tab_robot[i]
					.getforme().centre.x, tab_robot[i].getforme().centre.y,
					forme.centre.x, forme.centre.y);	
				}	
			}
			exit(EXIT_FAILURE);
		}
	}	
}
		
int decodage_spatial(std::istringstream& data, int& compteur1, int& compteur2)
{
	double x, y;
	int nbUpdate;
	unsigned nbNr, nbNs, nbNd, nbRr, nbRs;
	data >> x >> y >> nbUpdate >> nbNr >> nbNs >> nbNd >> nbRr >> nbRs;
	compteur1 = nbNs;
	compteur2 = nbRs;
	Spatial robot_spatial(x,y,nbUpdate,nbNr,nbNd,nbNs, nbRr,nbRs);
	return nbUpdate;
}
	
void decodage_neutraliseur(std::istringstream& data, int nbUpdate)
{
	double x, y, orienta;
	unsigned type;
	std::string panne;
	int k_update_panne;
	data >> x >> y >> orienta >> type >> panne >> k_update_panne;
	Neutraliseur robot_neutraliseur(x,y,orienta,type,panne,nbUpdate, k_update_panne);
}
	 
void decodage_reparateur(std::istringstream& data)
{
	double x, y;
	data >> x >> y;
	Reparateur robot_reparateur(x,y);
}

void decodage_robot(std::istringstream& data, int n, int& compteur1, int& compteur2)
{
	static int nbUpdate(0);
	switch(n)
	{
	case SPATIAL :
		nbUpdate = decodage_spatial(data, compteur1, compteur2);	
	break;
	case REPARATEUR :
		decodage_reparateur(data);
	break;
	case NEUTRALISEUR :
		decodage_neutraliseur(data, nbUpdate);
	break;
	}
}
