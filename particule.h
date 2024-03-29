//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef PARTICULE_H
#define PARTICULE_H

#include <vector>
#include <random>
#include <sstream>
#include <string>
#include "shape.h"

constexpr double infini(9999999);
constexpr S2d non_existent_particle({infini, infini});

class Particule {
public: 
	Particule(double x, double y, double cote, bool& file_success);
	Carre getForme() const;
	void detect_particle_outside(bool& file_success);
	void detect_particle_too_small(bool& file_success);
	void detect_particle_superposition(bool& file_success);
	bool operator==(Particule const& prt2) const;
	bool operator<(Particule const& prt2) const;
	
private:
	Carre forme;
};

void test_particle_robot_superposition(Cercle robot, bool& file_success);
void decodage_particule(std::istringstream& data, bool& file_success);
unsigned getnbP();
void desintegration(bool file_success, std::default_random_engine engine);
void draw_particles();
void destroy_tab_particule();
void sauvegarde_particules(std::ofstream& fichier);
void new_particules(unsigned i, bool file_success);
void sort_particle_vector();
Carre get_particle_shape(size_t i);
bool superposition_particle_robot_sim(Cercle robot);
S2d particle_to_destroy(S2d robot);
void destroy_particle(S2d particle);
Carre find_particule(S2d prt);

#endif
