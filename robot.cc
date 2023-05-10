//NOM PRENOM SCIPER CONTRIBUTION(%)
//LAHLOU SAAD 361150 65%
//ASMOUKI ILYAS 356263 35%

#include <vector>
#include <iostream>
#include <array>
#include <fstream>
#include "robot.h"
#include "message.h"
#include "constantes.h"
#include "particule.h"

enum {SPATIAL=2, REPARATEUR, NEUTRALISEUR};

constexpr double NULL_DATA(0);
constexpr double max_delta_tr(vtran_max*delta_t);
constexpr double max_delta_rt(vrot_max*delta_t);

static std::vector<Robot*> tab_robot;

Robot::Robot(double x1=0, double y1=0) {
	forme.centre.x = x1;
	forme.centre.y = y1;
	forme.rayon = 0;
}

Robot::~Robot() {
}

Cercle Robot::getForme() const {
	return forme;
}

Spatial::Spatial(double x, double y, int nbUpdate, unsigned nbNr, unsigned nbNs,
	  unsigned nbNd, unsigned nbRr, unsigned nbRs, bool& file_success): Robot(x, y), 
	  nbUpdate(nbUpdate), nbNr(nbNr), nbNs(nbNs), nbNd(nbNd), nbRr(nbRr), nbRs(nbRs) {

	forme.rayon=r_spatial;
	error_outside(file_success);
	test_particle_robot_superposition(forme, file_success);
}

Spatial::~Spatial() {
}
	
void Spatial::error_outside(bool& file_success) {
	if (((abs(forme.centre.x)+r_spatial>dmax)or(abs(forme.centre.y)+r_spatial)>dmax)){
		std::cout<<message::spatial_robot_ouside(forme.centre.x, forme.centre.y);
			file_success = false;
	}
	return;
}

void Spatial::setnbUpdate(int value) {
	if (value > 0) {
		nbUpdate = value;
	}
	return;
}

Reparateur::Reparateur(double x, double y, bool& file_success): Robot(x, y) {
	forme.rayon=r_reparateur; 
	TestCollision(file_success);
	test_particle_robot_superposition(forme, file_success);
}
		
Reparateur::~Reparateur() {
}
		
Neutraliseur::Neutraliseur(double x, double y, double orientation, unsigned type,
						   std::string bool_panne, int nbUpdate, int k_update_panne, 
						   bool& file_success): Robot(x, y), orientation(orientation), 
						   type(type), k_update_panne(k_update_panne) {
	panne = ((bool_panne == "false") ? false : true);
	forme.rayon=r_neutraliseur;
	TestCollision(file_success);
	test_particle_robot_superposition(forme, file_success);
	error_k_update(nbUpdate, file_success);
}
	
Neutraliseur::~Neutraliseur() {
}

void Neutraliseur::error_k_update(int nbUpdate, bool& file_success) {
	if (k_update_panne > nbUpdate) {
		std::cout<<message::invalid_k_update(forme.centre.x, forme.centre.y, 
			k_update_panne, nbUpdate);
			file_success = false;
	}
	return;
}

Neutra_0::Neutra_0(double x, double y, double orientation, unsigned type,
					std::string bool_panne, int nbUpdate, int k_update_panne, 
					bool& file_success) : Neutraliseur(x, y, orientation, type, 
					bool_panne, nbUpdate, k_update_panne, file_success)
{
}

Neutra_1::Neutra_1(double x, double y, double orientation, unsigned type,
					std::string bool_panne, int nbUpdate, int k_update_panne, 
					bool& file_success) : Neutraliseur(x, y, orientation, type, 
					bool_panne, nbUpdate, k_update_panne, file_success)
{
}

Neutra_2::Neutra_2(double x, double y, double orientation, unsigned type,
					std::string bool_panne, int nbUpdate, int k_update_panne, 
					bool& file_success) : Neutraliseur(x, y, orientation, type, 
					bool_panne, nbUpdate, k_update_panne, file_success)
{
}
	
void Robot::TestCollision(bool& file_success)
{
	for (size_t i(1); i < tab_robot.size(); ++i) {
		if (superposition_cercles(forme, tab_robot[i]->getForme(),NO_MARGIN)) {
			if (forme.rayon == r_reparateur) {
				if (forme.rayon == tab_robot[i]->getForme().rayon) {
					std::cout << message::repairers_superposition(forme.centre.x,
						forme.centre.y, tab_robot[i]->getForme().centre.x,
						tab_robot[i]->getForme().centre.y);
						file_success = false;
				} else { 
					std::cout << message::repairer_neutralizer_superposition(
						forme.centre.x, forme.centre.y, 
						tab_robot[i]->getForme().centre.x,
						tab_robot[i]->getForme().centre.y);	
						file_success = false;
				}
			} else {
				if (forme.rayon == tab_robot[i]->getForme().rayon) {
					std::cout << message::neutralizers_superposition(forme.centre.x,
						forme.centre.y,	tab_robot[i]->getForme().centre.x,
						tab_robot[i]->getForme().centre.y);
						file_success = false;
				} else { 
					std::cout<<message::repairer_neutralizer_superposition(
						tab_robot[i]->getForme().centre.x, 
						tab_robot[i]->getForme().centre.y,forme.centre.x, 
						forme.centre.y);
						file_success = false;	
				}	
			}
		}
	}
	return;	
}
		
int decodage_spatial(std::istringstream& data, int& compteur1, int& compteur2, 
					bool& file_success) {
	double x, y;
	int nbUpdate;
	unsigned nbNr, nbNs, nbNd, nbRr, nbRs;
	data >> x >> y >> nbUpdate >> nbNr >> nbNs >> nbNd >> nbRr >> nbRs;
	compteur1 = nbNs;
	compteur2 = nbRs;
	Spatial* pt = new Spatial(x,y,nbUpdate,nbNr,nbNs ,nbNd, nbRr,nbRs, file_success);
	tab_robot.push_back(pt);
	return nbUpdate;
}
	
void decodage_neutraliseur(std::istringstream& data, int nbUpdate, bool& file_success){
	double x, y, orienta;
	unsigned type;
	std::string panne;
	int k_update_panne;
	data >> x >> y >> orienta >> type >> panne >> k_update_panne;
	Neutraliseur* pt = nullptr;
	if (type == 0)	{
		pt = new Neutra_0(x,y,orienta,type,panne,nbUpdate, k_update_panne,
		file_success);
	} else if (type == 1)	{
		pt = new Neutra_1(x,y,orienta,type,panne,nbUpdate, k_update_panne,
		file_success);
	} else if (type == 2)	{
		pt = new Neutra_2(x,y,orienta,type,panne,nbUpdate, k_update_panne,
		file_success);
	}

	tab_robot.push_back(pt);
}
	 
void decodage_reparateur(std::istringstream& data, bool& file_success) {
	double x, y;
	data >> x >> y;
	Reparateur* pt = new Reparateur(x,y, file_success);
	tab_robot.push_back(pt);
}

void decodage_robot(std::istringstream& data, int type, int& compteur1, int& compteur2,
						bool& file_success) {
	static int nbUpdate(0);
	switch(type) {
	case SPATIAL :
		nbUpdate = decodage_spatial(data, compteur1, compteur2, file_success);	
	break;
	case REPARATEUR :
		decodage_reparateur(data, file_success);
	break;
	case NEUTRALISEUR :
		decodage_neutraliseur(data, nbUpdate, file_success);
	break;
	}
}

Cercle r_getForme(unsigned i) {
	return tab_robot[i]->getForme();
}

unsigned spatial_getnbUpdate() {
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->get_data("nbUpdate"));
}

unsigned spatial_getnbNr() {
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->get_data("nbNr"));
}

unsigned spatial_getnbNs() {
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->get_data("nbNs"));
}

unsigned spatial_getnbNd() {
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->get_data("nbNd"));
}

unsigned spatial_getnbRr() {
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->get_data("nbRr"));
}

unsigned spatial_getnbRs() {
	return ((tab_robot.size()==0) ? 0 : tab_robot[0]->get_data("nbRs"));
}

unsigned spatial_getnbNp() {
	unsigned nbNp(0);
	for (size_t i((tab_robot[0]->get_data("nbRs"))+1); i < tab_robot.size(); ++i) {
		if (tab_robot[i]->get_data("panne")) {
			++nbNp;
		}
	}
	return nbNp;
}

void spatial_setnbUpdate(int value) {
	tab_robot[0]->setnbUpdate(value);
	return;
}

void draw_robots() {
    for (size_t i = 0; i < tab_robot.size(); ++i) {
        if (i == 0)
            dessin_cercle(tab_robot[i]->getForme(), "blue");
        else if ((i >= 1) and (i <= spatial_getnbRs()))
            dessin_cercle(tab_robot[i]->getForme(), "green");
        else {
            std::string color = (tab_robot[i]->get_data("panne") ? "orange" : "black");
				dessin_cercle(tab_robot[i]->getForme(), color);
				dessin_orientation(tab_robot[i]->getForme(), 
								   tab_robot[i]->get_data("orientation"));
        }
    }
}

void destroy_tab_robots() {
	while (tab_robot.size() != 0) {
		delete tab_robot[tab_robot.size()-1];
		tab_robot.pop_back();
	}
	return;
}

void sauvegarde_robots(std::ofstream& fichier) {
	fichier<<std::endl<<tab_robot[0]->getForme().centre.x<<" "<<
			tab_robot[0]->getForme().centre.y<<" "<<spatial_getnbUpdate()<<" "
			<<spatial_getnbNr()<<" "<< spatial_getnbNs()<<" "<<spatial_getnbNd()<<" "
			<<spatial_getnbRr()<<" "<< spatial_getnbRs()<<std::endl;
	
	for (unsigned i(1); i < spatial_getnbRs()+1 ; ++i) {
		fichier<<"\t"<<tab_robot[i]->getForme().centre.x<<" "<<
				tab_robot[i]->getForme().centre.y<<std::endl;
	}
	fichier<<std::endl;
	
	for (unsigned i(spatial_getnbRs()+1);i<spatial_getnbRs()+spatial_getnbNs()+1; ++i){
		fichier<<"\t"<<tab_robot[i]->getForme().centre.x<<" "<<
				tab_robot[i]->getForme().centre.y<<" "<<
				tab_robot[i]->get_data("orientation")<<" "<< 
				tab_robot[i]->get_data("type")<<" ";
		if (tab_robot[i]->get_data("panne") == 0) {
			fichier<<"false ";
		} else {
			fichier<<"true ";
		}
		fichier<<tab_robot[i]->get_data("k_update_panne")<<std::endl;
	}
	return;
}

double Spatial::get_data(std::string data_type) {
	if (data_type == "nbUpdate")
		return nbUpdate;
	else if (data_type == "nbNr")
		return nbNr;
	else if (data_type == "nbNs")
		return nbNs;
	else if (data_type == "nbNd")
		return nbNd;
	else if (data_type == "nbRr")
		return nbRr;
	else if (data_type == "nbRs")
		return nbRs;
		
	return NULL_DATA;
}

double Reparateur::get_data(std::string data_type) {
	return NULL_DATA;   
}

double Neutraliseur::get_data(std::string data_type) {
	if (data_type == "orientation") 
		return orientation;
	else if (data_type == "type")
		return type;
	else if (data_type == "panne")
		return panne;
	else if (data_type == "k_update_panne")
		return k_update_panne;
	
	return NULL_DATA;
}

void Neutraliseur::set_panne(bool p) {
	panne = p;
}

bool in_desintegration_area(Carre particle, size_t i)
{
	return superposition_cerclecarre(particle, tab_robot[i]->getForme(), WITH_MARGIN);
}

void destroy_robot(size_t i)
{
	tab_robot[i]->set_panne(true);
}

void Reparateur::move_to(S2d goal){
	S2d pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y} ;
	double norm(norme(pos_to_goal));
	
	if (norm <= max_delta_tr) {
		forme.centre = goal;
	} else {
		add_scaled_vector(forme.centre, pos_to_goal, max_delta_tr/norm);
	}
	return;
}

void Neutra_0::move_to(S2d goal){
	S2d pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
	
	double goal_a(atan2(pos_to_goal.y ,pos_to_goal.x));
	double delta_a(goal_a - orientation);
	
	if((abs(delta_a) <= max_delta_rt) and (abs(delta_a) > 0)) {
		orientation = goal_a;
	} else if (abs(delta_a) > max_delta_rt) {
		orientation += ((delta_a > 0) ?  1. : -1.)*max_delta_rt;
	} else {
		S2d travel_dir = {cos(orientation), sin(orientation)}; 
		add_scaled_vector(forme.centre, travel_dir, max_delta_tr);
	}
}

void Neutra_1::move_to(S2d goal){
}

void Neutra_2::move_to(S2d goal){
	S2d init_pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
	S2d travel_dir    = {cos(orientation), sin(orientation)}; 
	double proj_goal= prod_scalaire(init_pos_to_goal, travel_dir);

    if(abs(proj_goal) > max_delta_tr){ 
		proj_goal = ((proj_goal > 0) ? 1 : -1)*max_delta_tr;
	}
	add_scaled_vector(forme.centre, travel_dir, proj_goal);
	S2d    updated_pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
	double goal_a(atan2(updated_pos_to_goal.y ,updated_pos_to_goal.x));
	double delta_a(goal_a - orientation);
	
	if(abs(delta_a) <= max_delta_rt) {
		orientation = goal_a;
	} else {
		orientation += ((delta_a > 0) ?  1. : -1.)*max_delta_rt;
	}
	return;
}

void Neutraliseur::destroy_neutraliseurs(){
	for (size_t i(spatial_getnbRs()+1); i < spatial_getnbRs() + spatial_getnbNs() + 1 ;
		++i){
		if (tab_robot[i]->get_data("panne")){
			if (spatial_getnbUpdate() - tab_robot[i]->get_data("k_update_panne") >= 
				max_update){
				delete tab_robot[i];
				std::swap(tab_robot.back(), tab_robot[i]);
				tab_robot.pop_back();
			}
		}
	}
	return;
}

void decision_reparateur(){
	int taken = 0;
	vector <array<double, 2>> reparateur_list;
	do {
		for (size_t i(spatial_getnbRs()+1); i < tab_robot.size() ; ++i){
			if (tab_robot[i]->get_data("panne")){
				S2d vect = {tab_robot[i]->getForme().centre.x - 
				tab_robot[1]->getForme().centre.x , tab_robot[i]->getForme().centre.y - 
				tab_robot[1]->getForme().centre.y };
				double dist_min(norme(vect));
				reparateur_list.push_back({1 , dist_min});
				tab_robot[1]->set_goal(tab_robot[i]->getForme().centre);
				
				for (size_t j(2); j < spatial_getnbRs()+1; ++j){
					dist = norme(tab_robot[i]->getForme().centre.x - 
						tab_robot[j]->getForme().centre.x , 
						tab_robot[i]->getForme().centre.y - 
						tab_robot[j]->getForme().centre.y);
						if (dist < dist_min){
							dist_min = dist;
							reparateur_list 
					
					
		} while (spatial_getnbRs() != taken);
					
				


void Robot::set_goal(S2d dest){
	goal = dest;
	return;
}
	

