#include "particule.h"

//~ #ifndef MESSAGE_H_INCLUDED
//~ #define MESSAGE_H_INCLUDED
#include "message.h"
//~ #endif

#ifndef SHAPE_H
#define SHAPE_H
#include "shape.h"
#endif

using namespace std;

Particule::Particule(double x, double y, double cote) : forme({{x,y},cote})
{
    detect_particle_outside(x, y, cote);
    detect_particle_too_small(x, y, cote);
    tab_particule.push_back(*this);
}

Carre Particule::getForme() const
{
	return forme;
}

void detect_particle_outside(double x, double y, double s)
{
	if (abs(x) + s / 2 > dmax or abs(y) + s / 2 > dmax)
    {
        cout << message::particle_outside(x, y, s);
        exit(EXIT_FAILURE);
    }
}

void detect_particle_too_small(double x, double y, double s)
{
	 if (s < d_particule_min)
    {
        cout << message::particle_too_small(x, y, s);
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
