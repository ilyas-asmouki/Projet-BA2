#include "robot.h"
#include <vector>
#include <iostream>

#ifndef PARTICULE_H
#define PARTICULE_H
#include "particule.h"
#endif


#include <string>
#include <sstream>

//~ #ifndef MESSAGE_H_INCLUDED
//~ #define MESSAGE_H_INCLUDED
#include "message.h"
//~ #endif

#ifndef SHAPE_H
#define SHAPE_H
#include "shape.h"
#endif

using namespace std;
static vector<Robot> tab_robot;
enum{SPATIAL=2, REPARATEUR, NEUTRALISEUR};

Robot::Robot(double x1=0, double y1=0)
{
	forme.centre.x = x1;
	forme.centre.y = y1;
	forme.rayon = 0;
}

Cercle Robot::getforme(){
	return forme;
}
 
Spatial::Spatial(double x, double y, unsigned nbUpdate ,unsigned nbNr, unsigned nbNs,
	  unsigned nbNd, unsigned nbRr ,unsigned nbRs): Robot(x, y), nbUpdate(nbUpdate), 
	  nbNr(nbNr), nbNs(nbNs), nbNd(nbNd), nbRr(nbRr), nbRs(nbRs)
{
	forme.rayon=r_spatial;
	error_outside();
}
	
void Spatial::error_outside() {
	if ((abs(forme.centre.x+r_spatial)>dmax)or(abs(forme.centre.y+r_spatial)>dmax)){
		cout<<message::spatial_robot_ouside(forme.centre.x, forme.centre.y);
		exit(EXIT_FAILURE);
	}
	return;
}

Reparateur::Reparateur(double x, double y): Robot(x, y)
{
	forme.rayon=r_reparateur; 
	TestCollision();
	test_particle_robot_superposition();
	tab_robot.push_back(*this);
}
		
Neutraliseur::Neutraliseur(double x, double y, double a, unsigned b, bool c, unsigned nbUpdate, unsigned d=0)
 : Robot(x, y), orientation(a), type(b), panne(c), k_update_panne(d) 
{
	forme.rayon=r_neutraliseur;
	error_k_update(nbUpdate);
	TestCollision();
	test_particle_robot_superposition();
	tab_robot.push_back(*this);
}
	
void Neutraliseur::error_k_update(unsigned nbUpdate){
	if (k_update_panne > nbUpdate){
		cout<<message::invalid_k_update(forme.centre.x, forme.centre.y, 
		                                k_update_panne, nbUpdate);
		exit(EXIT_FAILURE);
	}
	return;
}
	
	
void Robot::TestCollision() {
	for (size_t i(0); i < tab_robot.size(); ++i) {
		if (superposition_cercles(forme, tab_robot[i].getforme(),LECTURE)){
			if (forme.rayon == r_reparateur) {
				if (forme.rayon == tab_robot[i].getforme().rayon) {
					cout<<message::repairers_superposition(forme.centre.x, forme.centre.y ,	
					tab_robot[i].getforme().centre.x, tab_robot[i].getforme().centre.y);
				} else { 
					cout<<message::repairer_neutralizer_superposition(
					forme.centre.x, forme.centre.y, tab_robot[i].getforme().centre.x,
					tab_robot[i].getforme().centre.y);	
				}
			} else {
				if (forme.rayon == tab_robot[i].getforme().rayon) {
					cout<<message::neutralizers_superposition(forme.centre.x, forme.centre.y ,	
					tab_robot[i].getforme().centre.x, tab_robot[i].getforme().centre.y);
				} else { 
					cout<<message::repairer_neutralizer_superposition(tab_robot[i].
					getforme().centre.x, tab_robot[i].getforme().centre.y,
					forme.centre.x, forme.centre.y);	
				}	
			}
			exit(EXIT_FAILURE);
		}
	}	
	return;
}

void Robot::test_particle_robot_superposition() {
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_cerclecarre(tab_particule[i].getForme(), forme, LECTURE))
		{
			cout << message::particle_robot_superposition(tab_particule[i].getForme().
			centre.x,tab_particule[i].getForme().centre.y,tab_particule[i].getForme().
			cote, forme.centre.x, forme.centre.y, forme.rayon);
			exit(EXIT_FAILURE);
		}
	}
}
		
unsigned decodage_spatial(istringstream& data, int& compteur1, int& compteur2){
	double x;
	double y;
	unsigned nbUpdate;
	unsigned nbNr;
	unsigned nbNs;
	unsigned nbNd;
	unsigned nbRr;
	unsigned nbRs;
	data>>x>>y>>nbUpdate>>nbNr>>nbNs>>nbNd>>nbRr>>nbRs;
	compteur1 = nbNs;
	compteur2 = nbRs;
	Spatial robot_spatial(x,y,nbUpdate,nbNr,nbNd,nbNs, nbRr,nbRs);
	return nbUpdate;
}
	
void decodage_neutraliseur(istringstream& data, unsigned nbUpdate){
	double x;
	double y;
	double  orienta;
	unsigned type;
	bool panne;
	unsigned k_update_panne;
	data>>x>>y>>orienta>>type>>panne;
	data>>k_update_panne;
	cout <<x << y << orienta << type << panne <<  k_update_panne << endl;
	Neutraliseur robot_neutraliseur(x,y,orienta,type,panne,nbUpdate, k_update_panne);
	return;
}
	 
void decodage_reparateur(istringstream& data){
	double x;
	double y;
	data>>x>>y;
	Reparateur robot_reparateur(x,y);
	return;
}

void decodage_robot(istringstream& data, int n, int& compteur1, int& compteur2){
	static unsigned nbUpdate(0);
	switch(n)
	{
	case SPATIAL :
		nbUpdate=decodage_spatial(data, compteur1, compteur2);	
	break;
	case REPARATEUR :
		decodage_reparateur(data);
	break;
	case NEUTRALISEUR :
		decodage_neutraliseur(data, nbUpdate);
	break;
	}
	return;
}
