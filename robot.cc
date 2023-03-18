#include "robot.h"
#include <vector>
#include <iostream>

#ifndef PARTICULE_H
#define PARTICULE_H
#include "particule.h"
#endif

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

Robot::Robot(double x = 0, double y = 0) : forme({{x, y}, 0})
{}

Cercle Robot::getforme(){
	return forme;
}
 
Spatial::Spatial(double x, double y, unsigned nbUpdate, unsigned nbNr, unsigned nbNd, 
unsigned nbRr, unsigned nbRs): Robot(x, y), nbUpdate(nbUpdate), nbNr(nbNr),
nbNd(nbNd), nbRr(nbRr), nbRs(nbRs)
{
	forme.rayon = r_spatial;
	error_outside();
}
	
void Spatial::error_outside() {
	if ((abs(forme.centre.x+r_spatial)>dmax)or(abs(forme.centre.y+r_spatial)>dmax))
	{
		cout << message::spatial_robot_ouside(forme.centre.x, forme.centre.y);
		exit(EXIT_FAILURE);
	}
	return;
}

Reparateur::Reparateur(double x, double y): Robot(x, y)
{
	forme.rayon = r_reparateur; 
	TestCollision();
	tab_robot.push_back(*this);
}
		
Neutraliseur::Neutraliseur(double x, double y, int orientation, unsigned type,
						   bool panne,  unsigned k_update, unsigned nbUpdate)
: Robot(x, y), orientation(orientation), type(type), panne(panne), 
k_update_panne(k_update) 
{
	forme.rayon = r_neutraliseur;
	error_k_update(nbUpdate);
	TestCollision();
	tab_robot.push_back(*this);
}
	
void Neutraliseur::error_k_update(unsigned nbUpdate){
	if (k_update_panne > nbUpdate)
	{
		cout << message::invalid_k_update(forme.centre.x, forme.centre.y, 
		                                k_update_panne, nbUpdate);
		exit(EXIT_FAILURE);
	}
	return;
}
	
	
void Robot::TestCollision() {
	for (size_t i = 0; i < tab_robot.size(); ++i) {
		if (superposition_cercles(forme, tab_robot[i].getforme(),LECTURE)){
			if (forme.rayon == tab_robot[i].getforme().rayon) {
				if (forme.rayon == r_reparateur) {
					cout << message::repairers_superposition(forme.centre.x , 	
					tab_robot[i].getforme().centre.x, forme.centre.y ,
					tab_robot[i].getforme().centre.y);
				} else { 
					cout << message::neutralizers_superposition(
					forme.centre.x, tab_robot[i].getforme().centre.x, 
					forme.centre.y , tab_robot[i].forme.centre.y);	
				}
			} else {
				cout << message::repairer_neutralizer_superposition(
				forme.centre.x , tab_robot[i].getforme().centre.x, 
				forme.centre.y ,tab_robot[i].getforme().centre.y);
			}
			exit(EXIT_FAILURE);
		}
	}
	return;
}	

void Robot::test_particle_robot_superposition()
{
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_cerclecarre(forme, tab_particule[i].getForme(), LECTURE))
		{
			cout << message::particle_robot_superposition(tab_particule[i].getForme().
			centre.x,tab_particule[i].getForme().centre.y,tab_particule[i].getForme().
			cote, forme.centre.x, forme.centre.y, forme.rayon);
			exit(EXIT_FAILURE);
		}
	}
}



