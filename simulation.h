//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include "robot.h"
	
class Simulation 
{
	public :
	 Simulation(char* fichier);	
	 void lecture();
	 unsigned s_getnbUpdate();
   	 unsigned s_getnbNr();
	 unsigned s_getnbNs();
	 unsigned s_getnbNd();
	 unsigned s_getnbRr();
	 unsigned s_getnbRs();
	 unsigned p_getnbP();
	 void save(std::ofstream fichier);
	private : 
	 std::ifstream file; 
};

#endif
