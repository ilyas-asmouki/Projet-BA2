//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef PARTICULE_H
#define PARTICULE_H

#include "shape.h"
#include <vector>
#include <sstream>

class Particule
{
	public: 
	 Particule(double x, double y, double cote);
	 Carre getForme() const;
	 void detect_particle_outside();
	 void detect_particle_too_small();
	 void detect_particle_superposition();
	private:
	 Carre forme;
};

void test_particle_robot_superposition(Cercle robot);
void decodage_particule(std::istringstream& data);
unsigned getnbP();
Carre p_getforme(unsigned i);
void desintegration();

#endif
