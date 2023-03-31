//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <sstream>
#include "shape.h"

class Robot
{
	public :
	 Robot(double x, double y);
	 Cercle getForme() const;
	 void TestCollision();
	protected : 
	 Cercle forme;
};

class Spatial : public Robot
{
	public:
	 Spatial(double x, double y, int nbUpdate ,unsigned nbNr, unsigned nbNs,
	 unsigned nbNd, unsigned nbRr ,unsigned nbRs);
	 void error_outside();
	private :
	 int nbUpdate;
	 unsigned nbNr;
	 unsigned nbNs;
	 unsigned nbNd;
	 unsigned nbRr;
	 unsigned nbRs;
};

class Reparateur : public Robot
{
	public :
	Reparateur(double x, double y);
};

class Neutraliseur : public Robot
{
	public :
	 Neutraliseur(double x, double y, double a, unsigned b, std::string c, 
	             int nbUpdate, int  d);
	 void error_k_update(int nbUpdate);
	private :
	 double  orientation;
	 unsigned type;
	 bool panne;
	 int k_update_panne;
};

void decodage_robot(std::istringstream& v, int n, int& compteur1, int& compteur2);

#endif
