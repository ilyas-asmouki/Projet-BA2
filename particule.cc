//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#include <cmath>
#include <iostream>
#include "particule.h"
#include "message.h"
#include "shape.h"
#include "constantes.h"
#include <random>
#include <fstream>

static std::vector<Particule> tab_particule;

Particule::Particule(double x, double y, double cote, bool& file_success)
{
	forme.centre.x = x;
	forme.centre.y = y;
	forme.cote = cote;
    detect_particle_outside(file_success);
    detect_particle_too_small(file_success);
    detect_particle_superposition(file_success);
    tab_particule.push_back(*this);
}

Carre Particule::getForme() const
{
	return forme;
}

void Particule::detect_particle_outside(bool& file_success)
{
	if (abs(forme.centre.x) + forme.cote / 2 > dmax
	or abs(forme.centre.y) + forme.cote / 2 > dmax)
    {
        std::cout << message::particle_outside(forme.centre.x, forme.centre.y, 
        forme.cote);
        file_success = false;
    }
}

void Particule::detect_particle_too_small(bool& file_success)
{
	if (forme.cote < d_particule_min)
    {
		std::cout << message::particle_too_small(forme.centre.x,forme.centre.y,
			forme.cote);
		file_success = false;
    }
}

void Particule::detect_particle_superposition(bool& file_success)
{
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_carres(forme, tab_particule[i].getForme(), NO_MARGIN))
		{
			std::cout << message::particle_superposition(forme.centre.x,forme.centre.y,
			tab_particule[i].getForme().centre.x,tab_particule[i].getForme().centre.y);
			file_success = false;
		}
	}
}

void decodage_particule(std::istringstream& data, bool& file_success)
{
	double x, y, d;
	data >> x >> y >> d;
	Particule particule(x, y, d, file_success);
}

void test_particle_robot_superposition(Cercle robot, bool& file_success)
{
	for (size_t i = 0; i < tab_particule.size(); ++i)
	{
		if (superposition_cerclecarre(tab_particule[i].getForme(), robot, NO_MARGIN))
		{
			std::cout << message::particle_robot_superposition(tab_particule[i]
			.getForme().centre.x,tab_particule[i].getForme().centre.y,tab_particule[i]
			.getForme(). cote, robot.centre.x, robot.centre.y, robot.rayon);
			file_success = false;
		}
	}
}

unsigned getnbP()
{
	return tab_particule.size();
}

void draw_particles()
{
    for (size_t i = 0; i < tab_particule.size(); ++i)
        dessin_carre(tab_particule[i].getForme());
}

void destroy_tab_particule()
{
	while (tab_particule.size() != 0)
	{
		tab_particule.pop_back();
	}
	return;
}

void sauvegarde_particules(std::ofstream& fichier)
{
	fichier<<tab_particule.size()<<std::endl;
	for (unsigned i(0); i < getnbP(); ++i)
	{
		fichier<<"\t"<<tab_particule[i].getForme().centre.x<<" "<<tab_particule[i].
			getForme().centre.y<<" "<<tab_particule[i].getForme().cote<<std::endl;
	}
	return;
}

void desintegration(bool file_success, std::default_random_engine engine)
{
	size_t vect_size = tab_particule.size();
	double p = desintegration_rate;
	std::bernoulli_distribution b(p / tab_particule.size());
	std::vector<Particule> destroyed_particles;
	
	for (size_t i = 0; i < vect_size; ++i) 
	{
		if (b(engine) and tab_particule[i].getForme().cote > 2*d_particule_min)
		{
			destroyed_particles.push_back(tab_particule[i]);
		}
	}
	for (size_t j = 0; j < tab_particule.size(); ++j)
	{
		for (size_t k = 0; k < destroyed_particles.size(); ++k)
		{
			if (tab_particule[j] == destroyed_particles[k])
				tab_particule.erase(tab_particule.begin()+j);
		}
	}
	for (size_t l = 0; l < destroyed_particles.size(); ++l)
	{
		double d_particule = destroyed_particles[l].getForme().cote;
		Carre carre(destroyed_particles[l].getForme());
		Particule prt1(carre.centre.x-d_particule/4, carre.centre.y+d_particule/4,
					   d_particule/2 - 2*shape::epsil_zero, file_success);
		Particule prt2(carre.centre.x+d_particule/4, carre.centre.y+d_particule/4,
					   d_particule/2 - 2*shape::epsil_zero, file_success);
		Particule prt3(carre.centre.x-d_particule/4, carre.centre.y-d_particule/4,
					   d_particule/2 - 2*shape::epsil_zero, file_success);
		Particule prt4(carre.centre.x+d_particule/4, carre.centre.y-d_particule/4,
					   d_particule/2 - 2*shape::epsil_zero, file_success);
	}
}

bool Particule::operator==(Particule const& prt2)
{
	return (forme.centre.x == prt2.getForme().centre.x
		and forme.centre.y == prt2.getForme().centre.y
		and forme.cote == prt2.getForme().cote);
}
