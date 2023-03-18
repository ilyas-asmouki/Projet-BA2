#include "particule.h"
#include <cmath>
#include <iostream>

//~ #ifndef MESSAGE_H_INCLUDED
//~ #define MESSAGE_H_INCLUDED
#include "message.h"
//~ #endif

#ifndef SHAPE_H
#define SHAPE_H
#include "shape.h"
#endif

using namespace std;

Particule::Particule(double x, double y, double cote)
{
	forme.centre.x = x;
	forme.centre.y = y;
	forme.cote = cote;
    detect_particle_outside();
    detect_particle_too_small();
    detect_particle_superposition();
    tab_particule.push_back(*this);
}

Carre Particule::getForme() const
{
	return forme;
}

void Particule::detect_particle_outside()
{
	if (abs(forme.centre.x) + forme.cote / 2 > dmax
	or abs(forme.centre.y) + forme.cote / 2 > dmax)
    {
        cout << message::particle_outside(forme.centre.x, forme.centre.y, forme.cote);
        exit(EXIT_FAILURE);
    }
}

void Particule::detect_particle_too_small()
{
	 if (forme.cote < d_particule_min)
    {
        cout << message::particle_too_small(forme.centre.x,forme.centre.y, forme.cote);
        exit(EXIT_FAILURE);
    }
}

void Particule::detect_particle_superposition()
{
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_carres(forme, tab_particule[i].getForme(), LECTURE))
		{
			cout << message::particle_superposition(forme.centre.x, forme.centre.y,
			tab_particule[i].getForme().centre.x,tab_particule[i].getForme().centre.y);
			exit(EXIT_FAILURE);
		}
	}
}

void decodage_particule(istringstream& data){
	double x, y, d;
	data >> x >> y >> d;
	Particule particule(x, y, d);
}


