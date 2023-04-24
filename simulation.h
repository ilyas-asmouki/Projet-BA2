//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include "robot.h"
#include <string>
#include <random>	

class Simulation 
{
	public :
	 Simulation(std::ifstream& fichier);
	 Simulation(char* file);
	 bool getfile_success();
	 virtual ~Simulation();
	 void lecture(std::ifstream& file);
	 unsigned s_getnbUpdate();
   	 unsigned s_getnbNr();
	 unsigned s_getnbNs();
	 unsigned s_getnbNd();
	 unsigned s_getnbRr();
	 unsigned s_getnbRs();
	 unsigned s_getnbNp();
	 unsigned p_getnbP();
	 void s_setnbUpdate(int value);
	 void sauvegarde(std::string file);
	 void destroy_data();
	 void desintegration_status();
	private : 
	 std::ifstream fichier;
	 bool file_success=true;
	 std::default_random_engine engine;
};

void draw_world();

#endif
