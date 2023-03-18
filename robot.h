#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include "shape.h"
#include "particule.h"

constexpr double r_spatial(16.);
constexpr double r_reparateur(2.);
constexpr double r_neutraliseur(4.);

class Robot {
	protected : 
	 Cercle forme;
	public :
	 Robot(double x, double y);
	 Cercle getforme();
	 void TestCollision();
	 void test_particle_robot_superposition();
};

class Spatial : public Robot {
	private :
	 unsigned nbUpdate;
	 unsigned nbNr;
	 unsigned nbNd;
	 unsigned nbRr;
	 unsigned nbRs;
	public:
	 Spatial(double x, double y, unsigned nbUpdate ,unsigned nbNr, unsigned nbNd,
	  unsigned nbRr ,unsigned nbRs);
	 void error_outside();
};

class Reparateur : public Robot {
	private :
	public :
	 Reparateur(double x, double y);
};

class Neutraliseur : public Robot {
	private :
	 int orientation;
	 unsigned type;
	 bool panne;
	 unsigned k_update_panne = 0;
	public :
	 Neutraliseur(double x, double y, int a, unsigned b, bool c, unsigned d, unsigned nbUpdate);
	 void error_k_update(unsigned nbUpdate);
};

void decodage_robot(std::string v);

#endif
