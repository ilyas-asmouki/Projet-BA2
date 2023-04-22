//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "robot.h"
#include "message.h"
#include "constantes.h"
#include "shape.h"
#include "particule.h"

enum {SPATIAL=2, REPARATEUR, NEUTRALISEUR};

static std::vector<Robot*> tab_robot;

Robot::Robot(double x1=0, double y1=0)
{
	forme.centre.x = x1;
	forme.centre.y = y1;
	forme.rayon = 0;
}

Cercle Robot::getForme() const
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
	//~ dessin_cercle(forme);
}
	
void Spatial::error_outside()
{
	if ((((abs(forme.centre.x)+r_spatial)>dmax)or(abs(forme.centre.y)+r_spatial)>dmax))
	{
		std::cout<<message::spatial_robot_ouside(forme.centre.x, forme.centre.y);
		exit(EXIT_FAILURE);
	}
	return;
}

unsigned Spatial::getnbUpdate()
{
	return nbUpdate;
}

unsigned Spatial::getnbNr()
{
	return nbNr;
}

unsigned Spatial::getnbNs()
{
	return nbNs;
}

unsigned Spatial::getnbNd()
{
	return nbNd;
}

unsigned Spatial::getnbRr()
{
	return nbRr;
}

unsigned Spatial::getnbRs()
{
	return nbRs;
}

Reparateur::Reparateur(double x, double y): Robot(x, y)
{
	forme.rayon=r_reparateur; 
	TestCollision();
	test_particle_robot_superposition(forme);
	//~ dessin_cercle(forme);
}
		
Neutraliseur::Neutraliseur(double x, double y, double orientation, unsigned type,
	std::string bool_panne, int nbUpdate, int k_update_panne): Robot(x, y), 
	orientation(orientation), type(type), k_update_panne(k_update_panne) 
{
	panne = ((bool_panne == "false") ? false : true);
	forme.rayon=r_neutraliseur;
	error_k_update(nbUpdate);
	TestCollision();
	test_particle_robot_superposition(forme);
	std::string couleur = ( panne ? "orange" : "black");
	//~ dessin_cercle(forme);
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

double Neutraliseur::getorientation()
{
	return orientation;
}

unsigned Neutraliseur::gettype()
{
	return type;
}

bool Neutraliseur::getpanne()
{
	return panne;
}

int Neutraliseur::getk_update()
{
	return k_update_panne;
}
	
void Robot::TestCollision()
{
	for (size_t i(1); i < tab_robot.size(); ++i)
	{
		if (superposition_cercles(forme, tab_robot[i]->getForme(),NO_MARGIN))
		{
			if (forme.rayon == r_reparateur)
			{
				if (forme.rayon == tab_robot[i]->getForme().rayon)
				{
					std::cout << message::repairers_superposition(forme.centre.x,
					forme.centre.y, tab_robot[i]->getForme().centre.x,
					tab_robot[i]->getForme().centre.y);
				} 
				else
				{ 
					std::cout << message::repairer_neutralizer_superposition(forme
					.centre.x, forme.centre.y, tab_robot[i]->getForme().centre.x,
					tab_robot[i]->getForme().centre.y);	
				}
			}
			else
			{
				if (forme.rayon == tab_robot[i]->getForme().rayon)
				{
					std::cout << message::neutralizers_superposition(forme.centre.x,
					forme.centre.y,	tab_robot[i]->getForme().centre.x,
					tab_robot[i]->getForme().centre.y);
				}
				else
				{ 
					std::cout<<message::repairer_neutralizer_superposition(tab_robot[i]
					->getForme().centre.x, tab_robot[i]->getForme().centre.y,
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
	Spatial* pt = new Spatial(x,y,nbUpdate,nbNr,nbNd,nbNs, nbRr,nbRs);
	tab_robot.push_back(pt);
	return nbUpdate;
}
	
void decodage_neutraliseur(std::istringstream& data, int nbUpdate)
{
	double x, y, orienta;
	unsigned type;
	std::string panne;
	int k_update_panne;
	data >> x >> y >> orienta >> type >> panne >> k_update_panne;
	Neutraliseur* pt = new Neutraliseur(x,y,orienta,type,panne,nbUpdate, k_update_panne);
	tab_robot.push_back(pt);
}
	 
void decodage_reparateur(std::istringstream& data)
{
	double x, y;
	data >> x >> y;
	Reparateur* pt = new Reparateur(x,y);
	tab_robot.push_back(pt);
}

void decodage_robot(std::istringstream& data, int type, int& compteur1, int& compteur2)
{
	static int nbUpdate(0);
	switch(type)
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

Cercle r_getForme(unsigned i)
{
	return tab_robot[i]->getForme();
}

unsigned spatial_getnbUpdate()
{
	return tab_robot[0]->getnbUpdate();
}

unsigned spatial_getnbNr()
{
	return tab_robot[0]->getnbNr();
}

unsigned spatial_getnbNs()
{
	return tab_robot[0]->getnbNs();
}

unsigned spatial_getnbNd()
{
	return tab_robot[0]->getnbNd();
}

unsigned spatial_getnbRr()
{
	return tab_robot[0]->getnbRr();
}

unsigned spatial_getnbRs()
{
	return tab_robot[0]->getnbRs();
}

double neutra_getorientation(unsigned i)
{
	return tab_robot[i]->getorientation();
}

unsigned neutra_gettype(unsigned i)
{
	return tab_robot[i]->gettype();
}

bool neutra_getpanne(unsigned i)
{
	return tab_robot[i]->getpanne();
}

int neutra_getk_update(unsigned i)
{
	return tab_robot[i]->getk_update();
}

void draw_robots()
{
    for (size_t i = 0; i < tab_robot.size(); ++i)
    {
        if (i == 0)
            dessin_cercle(tab_robot[i]->getForme(), "blue");
        else if ((i >= 1) and (i <= spatial_getnbRs()))
            dessin_cercle(tab_robot[i]->getForme(), "green");
        else
        {
            std::string color = (neutra_getpanne(i) ? "orange" : "black");
            dessin_cercle(tab_robot[i]->getForme(), color);
        }
    }
}

void destroy_tab_robots()
{
	while (tab_robot.size() != 0)
	{
		tab_robot.pop_back();
	}
	return;
}
