#include "simulation.h"

//~ #ifndef ROBOT_H
//~ #define ROBOT_H
#include "robot.h"
//~ #endif

//~ #ifndef PARTICULE_H
//~ #define PARTICULE_H
#include "particule.h"
//~ #endif

#ifndef SHAPE_H
#define SHAPE_H
#include "shape.h"
#endif

#include <iostream>
#include <string>
#include <fstream>
#include "message.h"

using namespace std;

void decodage_ligne(std::string line){
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
	if (compteur1==0){
		etat = SPATIAL;
	} else {
		etat = PARTICULE;
	}
	case PARTICULE :
	decodage_particule(data);
	++i;
	if (compteur1==i){
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
	if (compteur2 == i){
		etat = NEUTRALISEUR;
		i=0;
	}
	break;
	
	case NEUTRALISEUR :
	decodage_robot(data, NEUTRALISEUR, compteur1, compteur2);
	++i;
	if (compteur1 == i){
		cout<<message::success();
		exit(EXIT_SUCCESS);
	}
	}
	return;
}	
		
void Simulation::lecture(char* monfichier){
	ifstream fichier(monfichier);
	string line;
	if (!fichier.fail()){
		while (getline(fichier>>ws,line)){
			if (line[0]=='#'){
				continue;
			}
			decodage_ligne(line);
		}
	} else {
		exit(EXIT_FAILURE);
	}
	return;
}
