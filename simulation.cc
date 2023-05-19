//NOM PRENOM SCIPER CONTRIBUTION(%)
//LAHLOU SAAD 361150 80%
//ASMOUKI ILYAS 356263 20%

#include <iostream>
#include <string>
#include <sstream>
#include "simulation.h"
#include "robot.h"
#include "particule.h"
#include "shape.h"
#include "message.h"
#include "constantes.h"

static std::default_random_engine engine;

void decodage_ligne(std::string line, bool& file_success) {
	engine.seed(1);
	enum ETAT{NBP, PARTICULE, SPATIAL, REPARATEUR, NEUTRALISEUR};
	static int etat(NBP);
	std::istringstream data(line);
	static int compteur1(0);
	static int compteur2(0);
	static int i(0);
	
	switch (etat) {
	case NBP : 
		data>>compteur1;
		if (compteur1==0) {
			etat = SPATIAL;
		} else {
			etat = PARTICULE;
		}
		break;
	case PARTICULE :
		decodage_particule(data, file_success);
		++i;
		if (compteur1==i) {
			etat = SPATIAL;
			sort_particle_vector();
		}
		break;
	case SPATIAL :
		decodage_robot(data, SPATIAL, compteur1, compteur2, file_success);
		if (compteur2 != 0) {
			etat = REPARATEUR;
		} else if (compteur2 == 0 and compteur1 != 0) {
			etat = NEUTRALISEUR;
		} else {
			etat = NBP;
		}	
		i=0;
		break;
	
	case REPARATEUR :
		decodage_robot(data, REPARATEUR, compteur1, compteur2, file_success);
		++i;
		if (compteur2 == i) {
			if (compteur1 != 0){
				etat = NEUTRALISEUR;
			} else {
				etat = NBP;
			}
			i=0;
		}
		break;
	
	case NEUTRALISEUR :
		decodage_robot(data, NEUTRALISEUR, compteur1, compteur2, file_success);
		++i;
		if (compteur1 == i) {
			etat = NBP;
			i=0;
		}
		break;
	}
	return;
}	

Simulation::Simulation(std::ifstream& fichier, bool success) : file_success(success){
	lecture(fichier);
}

Simulation::Simulation(char* file) : fichier(file) {
	lecture(fichier);
}

Simulation::~Simulation() {
}

bool Simulation::getfile_success() {
	return file_success;
}

void Simulation::lecture(std::ifstream& file) {
	std::string line;
		while (getline(file>>std::ws,line)) {
			if (line[0]=='#') {
				continue;
			}
		decodage_ligne(line, file_success);
	}
	if (file_success) {
		std::cout<<message::success();
	}	
	return;
}
	
unsigned Simulation::s_getnbUpdate() {
	return spatial_getnbUpdate();
}

unsigned Simulation::s_getnbNr() {
	return spatial_getnbNr();
}

unsigned Simulation::s_getnbNs() {
	return spatial_getnbNs();
}

unsigned Simulation::s_getnbNd() {
	return spatial_getnbNd();
}

unsigned Simulation::s_getnbRr() {
	return spatial_getnbRr();
}

unsigned Simulation::s_getnbRs() {
	return spatial_getnbRs();
}

unsigned Simulation::s_getnbNp() {
	return spatial_getnbNp();
}

unsigned Simulation::p_getnbP() {
	return getnbP();
}

void Simulation::s_setnbUpdate(int value) {
	spatial_setnbUpdate(value);
	return;
}

void Simulation::sauvegarde(std::string file) {
	std::ofstream fichier(file);
		sauvegarde_particules(fichier);
		sauvegarde_robots(fichier);

		return;
}

void draw_world() {
	draw_robots();
	draw_particles();
	return;
}
	
void Simulation::destroy_data() {
	destroy_tab_robots();
	destroy_tab_particule();
	return;
}

void Simulation::desintegration() {
	size_t vect_size = getnbP();
	double p = desintegration_rate;
	std::bernoulli_distribution b(p / getnbP());
	bool desintegration_occurred = false;	
	for (size_t i = 0; i < vect_size; ++i) {
		if (get_particle_shape(i).cote >= 2*(d_particule_min + 3*shape::epsil_zero)){
			if (b(engine)) {
				desintegration_occurred = true;
				for (size_t j = s_getnbRs()+1; j <= s_getnbRs() + s_getnbNs(); ++j) {
					if (in_desintegration_area(get_particle_shape(i), j))	{
						set_panne_robot(j);
					}	
				}
				new_particules(i,file_success);
			}
		}
	}
	
	if (desintegration_occurred) {
		sort_particle_vector();
	}
	return;
}

void Simulation::mise_a_jour(){
	desintegration();
	destroy_neutraliseurs();
	decision_creation_robot();
	decision_reparateur();
	std::vector<bool> tab_neutra(spatial_getnbNs(), true);
	for (size_t i(0); i < getnbP(); ++i) {
		decision_neutraliseur(get_particle_shape(i), tab_neutra);
	}
	decision_neutra_restant(tab_neutra);
	deplacement_robot();
	delete_robots();
	return;
}
	
bool simulation_over()	{
	return ((robots_left() == 1 and getnbP() == 0) ? true : false);
}
	
