#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include "shape.h"
#include "particule.h"
#include <sstream>

constexpr double r_spatial(16.);
constexpr double r_reparateur(2.);
constexpr double r_neutraliseur(4.);

class Robot {
	protected : 
	 Cercle forme;
	 void test_particle_robot_superposition();
	public :
	 Robot(double x, double y);
	 Cercle getforme();
	 void TestCollision();
};

class Spatial : public Robot {
	private :
	 unsigned nbUpdate;
	 unsigned nbNr;
	 unsigned nbNs;
	 unsigned nbNd;
	 unsigned nbRr;
	 unsigned nbRs;
	public:
	 Spatial(double x, double y, unsigned nbUpdate ,unsigned nbNr, unsigned nbNs,
	  unsigned nbNd, unsigned nbRr ,unsigned nbRs);
	 void error_outside();
};

class Reparateur : public Robot {
	private :
	public :
	 Reparateur(double x, double y);
};

class Neutraliseur : public Robot {
	private :
	 double  orientation;
	 unsigned type;
	 bool panne;
	 unsigned k_update_panne;
	public :
	 Neutraliseur(double x, double y, double a, unsigned b, bool c, unsigned nbUpdate, unsigned  d);
	 void error_k_update(unsigned nbUpdate);
};

void decodage_robot(std::istringstream& v, int n, int& compteur1, int& compteur2);

#endif
