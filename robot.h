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
	 virtual ~Robot();
	 Cercle getForme() const;
	 void TestCollision();
	 virtual unsigned getnbUpdate(){return 0;}
	 virtual unsigned getnbNr(){return 0;}
	 virtual unsigned getnbNs(){return 0;}
	 virtual unsigned getnbNd(){return 0;}
	 virtual unsigned getnbRr(){return 0;}
	 virtual unsigned getnbRs(){return 0;}
	 virtual double getorientation(){return 0;}
	 virtual unsigned gettype(){return 0;}
	 virtual bool getpanne(){return 0;}
	 virtual int getk_update(){return 0;}
	protected : 
	 Cercle forme;
};

class Spatial : public Robot
{
	public:
	 Spatial(double x, double y, int nbUpdate ,unsigned nbNr, unsigned nbNs,
	 unsigned nbNd, unsigned nbRr ,unsigned nbRs);
	 ~Spatial();
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
	 ~Reparateur();
};

class Neutraliseur : public Robot
{
	public :
	 Neutraliseur(double x, double y, double orientation, unsigned type,
	              std::string bool_panne, int nbUpdate, int k_update_panne);
	 ~Neutraliseur();
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
Cercle r_getForme(unsigned i);
void draw_robots();
void destroy_tab_robots();

#endif
