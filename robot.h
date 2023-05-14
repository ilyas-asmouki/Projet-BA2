//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <sstream>
#include <vector>
#include "shape.h"

enum SIDE {D=-1, R, U, L};

class Robot {
public :
	Robot(double x, double y);
	virtual ~Robot();
	Cercle getForme() const;
	S2d get_goal();
	void TestCollision(bool& file_success);
	bool superposition_robots_sim();
	
	virtual double get_data(std::string data_type)=0;
	virtual void move()=0;
	virtual void set_data(std:: string data_type, double value)=0;
	void set_goal(S2d new_goal);
	virtual S2d find_goal(Carre target);
	std::string getcolor();
	void set_color(std::string c);
	 
protected : 
	Cercle forme;
	std::string color;
	S2d goal;
	void cancel_movement(S2d temp);
};

class Spatial : public Robot {
public:
	Spatial(double x, double y, int nbUpdate ,unsigned nbNr, unsigned nbNs,
			unsigned nbNd, unsigned nbRr ,unsigned nbRs, bool& file_success);
	~Spatial();
	void error_outside(bool& file_success);
	virtual double get_data(std::string data_type);
	virtual void set_data(std:: string data_type, double value);
	void move(){}; 
	 
private :
	int nbUpdate=0;
	unsigned nbNr=0;
	unsigned nbNs=0;
	unsigned nbNd=0;
	unsigned nbRr=0;
	unsigned nbRs=0;
};

class Reparateur : public Robot {
public :
	Reparateur(double x, double y, bool& file_success);
	~Reparateur();
	virtual double get_data(std::string data_type);
	void move() override;
	virtual void set_data(std:: string data_type, double value);

private : 
	void repair_neutra(S2d goal);
};

class Neutraliseur : public Robot {
public :
	Neutraliseur(double x, double y, double orientation, unsigned type,
				 std::string bool_panne, int nbUpdate, int k_update_panne, 
				 bool& file_success);
	virtual ~Neutraliseur();
	void error_k_update(int nbUpdate, bool& file_success);
	virtual double get_data(std::string data_type);
	void move()=0;
	virtual void set_data(std:: string data_type, double value);
	SIDE find_side(S2d particle);
	void decontaminate();

protected :
	double  orientation;
	unsigned type;
	bool panne;
	int k_update_panne;
};

class Neutra_0 : public Neutraliseur {
public : 
	Neutra_0(double x, double y, double orientation, unsigned type,
			 std::string bool_panne, int nbUpdate, int k_update_panne, 
			 bool& file_success);
	~Neutra_0(){}
	void move() override;
};

class Neutra_1 : public Neutraliseur {
public : 
	Neutra_1(double x, double y, double orientation, unsigned type,
			 std::string bool_panne, int nbUpdate, int k_update_panne, 
			 bool& file_success);
	~Neutra_1(){}
	void move() override;
	virtual S2d find_goal(Carre target);
};

class Neutra_2 : public Neutraliseur {
public : 
	Neutra_2(double x, double y, double orientation, unsigned type,
			 std::string bool_panne, int nbUpdate, int k_update_panne, 
			 bool& file_success);
	~Neutra_2(){}
	void move() override;
};

void decodage_robot(std::istringstream& lig, int type, int& compteur1, int& compteur2,
						bool& file_success);
unsigned spatial_getnbUpdate();
unsigned spatial_getnbNr();
unsigned spatial_getnbNs();
unsigned spatial_getnbNd();
unsigned spatial_getnbRr();
unsigned spatial_getnbRs();
unsigned spatial_getnbNp();
void spatial_setnbUpdate(int value);
Cercle r_getForme(unsigned i);
void draw_robots();
void destroy_tab_robots();
void sauvegarde_robots(std::ofstream& fichier);
void decision_reparateur();
void decision_neutra_restant(std::vector<bool>& tab_neutra);
bool in_desintegration_area(Carre particle, size_t i);
void set_panne_robot(size_t i);
void decision_creation_robot();
void deplacement_robot();
void decision_neutraliseur(Carre prt, std::vector<bool>& tab_neutra);
void destroy_neutraliseurs();
S2d find_goal_if_outside_desintegration_area(double angle, double xt, double yt,
											 double xr, double yr, double c);
S2d find_goal_if_inside_desintegration_area(double angle, double xt, double yt,
											 double xr, double yr, double c);
void find_first_repairer(std::vector<double>& tab_distance,
						 std::vector<S2d>& tab_goal, int& k, size_t i, S2d& temp);
void  give_goal_repairer(std::vector<double>& tab_distance,
						 std::vector<S2d>& tab_goal);

#endif
