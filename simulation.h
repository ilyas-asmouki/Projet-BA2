//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
	
class Simulation 
{
	public :
	 void lecture();
	 Simulation(char* fichier);
	private :
	 std::ifstream file;
};

#endif
