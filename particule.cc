#include <cmath>
#include <iostream>
#include "particule.h"
#include "message.h"
#include "shape.h"

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
        std::cout << message::particle_outside(forme.centre.x, forme.centre.y, 
        forme.cote);
        exit(EXIT_FAILURE);
    }
}

void Particule::detect_particle_too_small()
{
	 if (forme.cote < d_particule_min)
    {
        std::cout << message::particle_too_small(forme.centre.x,forme.centre.y,
        forme.cote);
        exit(EXIT_FAILURE);
    }
}

void Particule::detect_particle_superposition()
{
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_carres(forme, tab_particule[i].getForme(), NO_MARGIN))
		{
			std::cout << message::particle_superposition(forme.centre.x,forme.centre.y,
			tab_particule[i].getForme().centre.x,tab_particule[i].getForme().centre.y);
			exit(EXIT_FAILURE);
		}
	}
}

void decodage_particule(std::istringstream& data)
{
	double x, y, d;
	data >> x >> y >> d;
	Particule particule(x, y, d);
}

void test_particle_robot_superposition(Cercle robot)
{
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_cerclecarre(tab_particule[i].getForme(), robot, NO_MARGIN))
		{
			std::cout << message::particle_robot_superposition(tab_particule[i]
			.getForme().centre.x,tab_particule[i].getForme().centre.y,tab_particule[i]
			.getForme(). cote, robot.centre.x, robot.centre.y, robot.rayon);
			exit(EXIT_FAILURE);
		}
	}
}
