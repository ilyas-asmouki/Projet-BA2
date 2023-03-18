#include "robot.h"
#include <vector>

#ifndef PARTICULE_H
#define PARTICULE_H
#include "particule.h"
#endif

#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED
#include "message.h"
#endif

#ifndef SHAPE_H
#define SHAPE_H
#include "shape.h"
#endif

using namespace std;
static vector<Robot> tab_robot;

Robot::Robot(double x1=0, double y1=0) : forme({{x1,y1}, 0})

{}
 
Spatial::Spatial(double x, double y, unsigned a ,unsigned b, unsigned c, 
 unsigned d ,unsigned e): Robot(x, y), nbUpdate(a), nbNr(b), nbNd(c), nbRr(d), nbRs(e)
{
	forme.rayon=r_spatial;
	tab_robot.push_back(*this);
}
	
Reparateur::Reparateur(double x, double y): Robot(x, y)
{
	forme.rayon=r_reparateur; 
	tab_robot.push_back(*this);
}
		
Neutraliseur::Neutraliseur(double x, double y, int a, unsigned b, bool c, unsigned d)
 : Robot(x, y), orientation(a), type(b), panne(c), k_update_panne(d) 
{
	forme.rayon=r_neutraliseur;
	tab_robot.push_back(*this);
}
		
