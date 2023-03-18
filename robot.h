#ifndef ROBOT_H
#define ROBOT_H

#include "shape.h"

constexpr double r_spatial(16.);
constexpr double r_reparateur(2.);
constexpr double r_neutraliseur(4.);

class Robot {
	protected : 
	 Cercle forme;
	public :
	 Robot(double x, double y);
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
	 Neutraliseur(double x, double y, int a, unsigned b, bool c, unsigned d);
};

#endif


