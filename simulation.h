//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include "robot.h"
#include <string>
	
class Simulation 
{
	public :
	 Simulation(std::ifstream& fichier);
	 Simulation(char* file);
	 void lecture(std::ifstream& file);
	 unsigned s_getnbUpdate();
   	 unsigned s_getnbNr();
	 unsigned s_getnbNs();
	 unsigned s_getnbNd();
	 unsigned s_getnbRr();
	 unsigned s_getnbRs();
	 unsigned p_getnbP();
	 void sauvegarde(std::string file);
	 void destroy_data();
	private : 
	 std::ifstream fichier;
};

void draw_world();

#endif
