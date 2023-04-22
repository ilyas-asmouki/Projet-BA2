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
	 virtual unsigned getnbUpdate();
	 virtual unsigned getnbNr();
	 virtual unsigned getnbNs();
	 virtual unsigned getnbNd();
	 virtual unsigned getnbRr();
	 virtual unsigned getnbRs();
	 virtual double getorientation();
	 virtual unsigned gettype();
	 virtual bool getpanne();
	 virtual int getk_update();
	protected : 
	 Cercle forme;
};

class Spatial : public Robot
{
	public:
	 Spatial(double x, double y, int nbUpdate ,unsigned nbNr, unsigned nbNs,
	 unsigned nbNd, unsigned nbRr ,unsigned nbRs);
	 void error_outside();
	 virtual unsigned getnbUpdate();
	 virtual unsigned getnbNr();
	 virtual unsigned getnbNs();
	 virtual unsigned getnbNd();
	 virtual unsigned getnbRr();
	 virtual unsigned getnbRs();
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
	 Neutraliseur(double x, double y, double orientation, unsigned type,
	              std::string bool_panne, int nbUpdate, int k_update_panne);
	 void error_k_update(int nbUpdate);
	 virtual double getorientation();
	 virtual unsigned gettype();
	 virtual bool getpanne();
	 virtual int getk_update();
	private :
	 double  orientation;
	 unsigned type;
	 bool panne;
	 int k_update_panne;
	 bool en_collision;
};

void decodage_robot(std::istringstream& lig, int type, int& compteur1, int& compteur2);
unsigned spatial_getnbUpdate();
unsigned spatial_getnbNr();
unsigned spatial_getnbNs();
unsigned spatial_getnbNd();
unsigned spatial_getnbRr();
unsigned spatial_getnbRs();

double neutra_getorientation(unsigned i);
unsigned neutra_gettype(unsigned i);
bool neutra_getpanne(unsigned i);
int neutra_getk_update(unsigned i);

#endif
