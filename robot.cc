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
#include <fstream>

enum {SPATIAL=2, REPARATEUR, NEUTRALISEUR};

static std::vector<Robot*> tab_robot;

Robot::Robot(double x1=0, double y1=0)
{
	forme.centre.x = x1;
	forme.centre.y = y1;
	forme.rayon = 0;
}

Robot::~Robot()
{
}

Cercle Robot::getForme() const
{
	return forme;
}

Spatial::Spatial(double x, double y, int nbUpdate ,unsigned nbNr, unsigned nbNs,
	  unsigned nbNd, unsigned nbRr ,unsigned nbRs, bool& file_success): Robot(x, y), nbUpdate(nbUpdate), 
	  nbNr(nbNr), nbNs(nbNs), nbNd(nbNd), nbRr(nbRr), nbRs(nbRs)
{
	forme.rayon=r_spatial;
	error_outside(file_success);
	test_particle_robot_superposition(forme, file_success);
}

Spatial::~Spatial()
{
}
	
void Spatial::error_outside(bool& file_success)
{
	if ((((abs(forme.centre.x)+r_spatial)>dmax)or(abs(forme.centre.y)+r_spatial)>dmax))
	{
		std::cout<<message::spatial_robot_ouside(forme.centre.x, forme.centre.y);
		file_success = false;
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

Reparateur::Reparateur(double x, double y, bool& file_success): Robot(x, y)
{
	forme.rayon=r_reparateur; 
	TestCollision(file_success);
	test_particle_robot_superposition(forme, file_success);
}
		
Reparateur::~Reparateur()
{
}
		
Neutraliseur::Neutraliseur(double x, double y, double orientation, unsigned type,
	std::string bool_panne, int nbUpdate, int k_update_panne, bool& file_success): Robot(x, y), 
	orientation(orientation), type(type), k_update_panne(k_update_panne) 
{
	panne = ((bool_panne == "false") ? false : true);
	forme.rayon=r_neutraliseur;
	TestCollision(file_success);
	test_particle_robot_superposition(forme, file_success);
	error_k_update(nbUpdate, file_success);
}
	
Neutraliseur::~Neutraliseur()
{
}

void Neutraliseur::error_k_update(int nbUpdate, bool& file_success)
{
	if (k_update_panne > nbUpdate)
	{
		std::cout<<message::invalid_k_update(forme.centre.x, forme.centre.y, 
		k_update_panne, nbUpdate);
		file_success = false;
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
	
void Robot::TestCollision(bool& file_success)
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
					file_success = false;
				} 
				else
				{ 
					std::cout << message::repairer_neutralizer_superposition(forme
					.centre.x, forme.centre.y, tab_robot[i]->getForme().centre.x,
					tab_robot[i]->getForme().centre.y);	
					file_success = false;
				}
			}
			else
			{
				if (forme.rayon == tab_robot[i]->getForme().rayon)
				{
					std::cout << message::neutralizers_superposition(forme.centre.x,
					forme.centre.y,	tab_robot[i]->getForme().centre.x,
					tab_robot[i]->getForme().centre.y);
					file_success = false;
				}
				else
				{ 
					std::cout<<message::repairer_neutralizer_superposition(tab_robot[i]
					->getForme().centre.x, tab_robot[i]->getForme().centre.y,
					forme.centre.x, forme.centre.y);
					file_success = false;	
				}	
			}
		}
	}	
}
		
int decodage_spatial(std::istringstream& data, int& compteur1, int& compteur2, bool& file_success)
{
	double x, y;
	int nbUpdate;
	unsigned nbNr, nbNs, nbNd, nbRr, nbRs;
	data >> x >> y >> nbUpdate >> nbNr >> nbNs >> nbNd >> nbRr >> nbRs;
	compteur1 = nbNs;
	compteur2 = nbRs;
	Spatial* pt = new Spatial(x,y,nbUpdate,nbNr,nbNs ,nbNd, nbRr,nbRs, file_success);
	tab_robot.push_back(pt);
	return nbUpdate;
}
	
void decodage_neutraliseur(std::istringstream& data, int nbUpdate, bool& file_success)
{
	double x, y, orienta;
	unsigned type;
	std::string panne;
	int k_update_panne;
	data >> x >> y >> orienta >> type >> panne >> k_update_panne;
	Neutraliseur* pt = new Neutraliseur(x,y,orienta,type,panne,nbUpdate, k_update_panne, file_success);
	tab_robot.push_back(pt);
}
	 
void decodage_reparateur(std::istringstream& data, bool& file_success)
{
	double x, y;
	data >> x >> y;
	Reparateur* pt = new Reparateur(x,y, file_success);
	tab_robot.push_back(pt);
}

void decodage_robot(std::istringstream& data, int type, int& compteur1, int& compteur2, bool& file_success)
{
	static int nbUpdate(0);
	switch(type)
	{
	case SPATIAL :
		nbUpdate = decodage_spatial(data, compteur1, compteur2, file_success);	
	break;
	case REPARATEUR :
		decodage_reparateur(data, file_success);
	break;
	case NEUTRALISEUR :
		decodage_neutraliseur(data, nbUpdate, file_success);
	break;
	}
}

Cercle r_getForme(unsigned i)
{
	return tab_robot[i]->getForme();
}

unsigned spatial_getnbUpdate()
{
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->getnbUpdate());
}

unsigned spatial_getnbNr()
{
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->getnbNr());
}

unsigned spatial_getnbNs()
{
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->getnbNs());
}

unsigned spatial_getnbNd()
{
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->getnbNd());
}

unsigned spatial_getnbRr()
{
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->getnbRr());
}

unsigned spatial_getnbRs()
{
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->getnbRs());
}

unsigned spatial_getnbNp()
{
	unsigned nbNp(0);
	for (size_t i(spatial_getnbRs()+1); i < tab_robot.size(); ++i)
	{
		if (tab_robot[i]->getpanne())
		{
			++nbNp;
		}
	}
	return nbNp;
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
            std::string color = (tab_robot[i]->getpanne() ? "orange" : "black");
            dessin_cercle(tab_robot[i]->getForme(), color);
            dessin_orientation(tab_robot[i]->getForme(), tab_robot[i]->getorientation());
        }
    }
}

void destroy_tab_robots()
{
	while (tab_robot.size() != 0)
	{
		delete tab_robot[tab_robot.size()-1];
		tab_robot.pop_back();
	}
	return;
}

void sauvegarde_robots(std::ofstream& fichier)
{
	fichier<<std::endl<<tab_robot[0]->getForme().centre.x<<" "<<tab_robot[0]->getForme().
		centre.y<<" "<<spatial_getnbUpdate()<<" "<<spatial_getnbNr()<<" "<<
		spatial_getnbNs()<<" "<<spatial_getnbNd()<<" "<<spatial_getnbRr()<<" "<<
		spatial_getnbRs()<<std::endl;
	
	for (unsigned i(1); i < spatial_getnbRs()+1 ; ++i)
	{
		fichier<<"\t"<<tab_robot[i]->getForme().centre.x<<" "<<tab_robot[i]->getForme().
			centre.y<<std::endl;
	}
	fichier<<std::endl;
	
	for (unsigned i(spatial_getnbRs()+1); i<spatial_getnbRs()+spatial_getnbNs()+1; ++i)
	{
		fichier<<"\t"<<tab_robot[i]->getForme().centre.x<<" "<<tab_robot[i]->getForme().
			centre.y<<" "<< tab_robot[i]->getorientation()<<" "<< 
			tab_robot[i]->gettype()<<" ";
		if (tab_robot[i]->getpanne() == 0)
		{
			fichier<<"false ";
		}
		else
		{
			fichier<<"true ";
		}
		fichier<<tab_robot[i]->getk_update()<<std::endl;
	}
	return;
}
