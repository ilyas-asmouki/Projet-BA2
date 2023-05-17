//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include <string>
#include <random>	

class Simulation {
public :
	Simulation(std::ifstream& fichier, bool success);
	Simulation(char* file);
	virtual ~Simulation();
	bool getfile_success();
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
	void mise_a_jour();
	
private : 
	bool file_success=true;
	std::ifstream fichier;	
	void desintegration();
};

void draw_world();
bool simulation_over();

#endif
