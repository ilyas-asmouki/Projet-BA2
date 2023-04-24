//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef PARTICULE_H
#define PARTICULE_H

#include "shape.h"
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <random>

class Particule
{
	public: 
	 Particule(double x, double y, double cote, bool& file_success);
	 Carre getForme() const;
	 void detect_particle_outside(bool& file_success);
	 void detect_particle_too_small(bool& file_success);
	 void detect_particle_superposition(bool& file_success);
	 bool operator==(Particule const& prt2);
	private:
	 Carre forme;
};

void test_particle_robot_superposition(Cercle robot, bool& file_success);
void decodage_particule(std::istringstream& data, bool& file_success);
unsigned getnbP();
void desintegration(bool file_success, std::default_random_engine engine);
void draw_particles();
void destroy_tab_particule();
void sauvegarde_particules(std::ofstream& fichier);

#endif
