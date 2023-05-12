//NOM PRENOM SCIPER CONTRIBUTION(%)
//LAHLOU SAAD 361150 65%
//ASMOUKI ILYAS 356263 35%

#include <iostream>
#include <array>
#include <fstream>
#include "robot.h"
#include "message.h"
#include "constantes.h"
#include "particule.h"

enum {SPATIAL=2, REPARATEUR, NEUTRALISEUR};

constexpr double NULL_DATA(0);
constexpr double default_orientation(0);
constexpr double default_k_update(0);
constexpr double max_delta_tr(vtran_max*delta_t);
constexpr double max_delta_rt(vrot_max*delta_t);
constexpr int NO_TARGET(-1);

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

S2d Robot::find_goal(Carre target) {
	return target.centre;
}

S2d Neutra_1::find_goal(Carre target) {
	double xr = forme.centre.x, yr = forme.centre.y;
	double xt = target.centre.x, yt = target.centre.y;
	double c = target.cote;
	double angle = atan2(yr - yt, xr - xt);
	Carre risk_zone = {{target.centre.x, target.centre.y}, target.cote * risk_factor}; 
	
	if (!superposition_cerclecarre(risk_zone, forme, WITH_MARGIN))
	{
		return find_goal_if_outside_desintegration_area(angle, xt, yt, xr, yr, c);
	}
	else {
		return find_goal_if_inside_desintegration_area(angle, xt, yt, xr, yr, c);
	}
		
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

Reparateur::Reparateur(double x, double y, bool& file_success): Robot(x, y) {
	forme.rayon=r_reparateur; 
	if (file_success){
		TestCollision(file_success);
		test_particle_robot_superposition(forme, file_success);
	}
}
		
Reparateur::~Reparateur() {
}
		
Neutraliseur::Neutraliseur(double x, double y, double orientation, unsigned type,
						   std::string bool_panne, int nbUpdate, int k_update_panne, 
						   bool& file_success): Robot(x, y), orientation(orientation), 
						   type(type), k_update_panne(k_update_panne) {
	panne = ((bool_panne == "false") ? false : true);
	forme.rayon=r_neutraliseur;
	if (file_success){
		TestCollision(file_success);
		test_particle_robot_superposition(forme, file_success);
		error_k_update(nbUpdate, file_success);
	}
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
	if (type == 1)	{
		pt = new Neutra_0(x,y,orienta,type,panne,nbUpdate, k_update_panne,
		file_success);
	} else if (type == 2)	{
		pt = new Neutra_1(x,y,orienta,type,panne,nbUpdate, k_update_panne,
		file_success);
	} else if (type == 3)	{
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
	tab_robot[0]->set_data("nbUpdate", value);
	return;
}

void draw_robots() {
	if (tab_robot.size() != 0){
		for (size_t i = 1; i < tab_robot.size(); ++i) {
			if (i <= spatial_getnbRs())
				dessin_cercle(tab_robot[i]->getForme(), "green");
			else {
				std::string color = (tab_robot[i]->get_data("panne") ? "orange" : "black");
					dessin_cercle(tab_robot[i]->getForme(), color);
					dessin_orientation(tab_robot[i]->getForme(), 
									   tab_robot[i]->get_data("orientation"));
			}
		}
		dessin_cercle(tab_robot[0]->getForme(), "blue");
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

void Spatial::set_data(std:: string data_type, double value) {
	if (data_type == "nbUpdate")
		nbUpdate = value;
	if (data_type == "nbNs")
		nbNs = value;
	if (data_type == "nbNr")
		nbNr = value;
	if (data_type == "nbRr")
		nbRr = value;
	if (data_type == "nbRs")
		nbRs = value;
	if (data_type == "nbNd")
		nbNd = value;
	return;
}

void Neutraliseur::set_data(std::string data_type, double value){
	if (data_type == "panne")
		panne = value;
	if (data_type == "k_update_panne")
		k_update_panne = value;
	return;
}

void Reparateur::set_data(std::string data_type, double value) {
	return;
}

void Robot::set_goal(S2d new_goal) {
	goal = new_goal;
}

S2d Robot::get_goal() {
	return goal;
}

bool in_desintegration_area(Carre particle, size_t i)
{
	return superposition_cerclecarre(particle, tab_robot[i]->getForme(), WITH_MARGIN);
}

void set_panne_robot(size_t i){
	tab_robot[i]->set_data("panne",true);
	tab_robot[i]->set_data("k_update_panne",spatial_getnbUpdate());
	tab_robot[i]->set_goal(tab_robot[i]->getForme().centre); 
}

void Reparateur::move(){
	S2d pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y} ;
	double norm(norme(pos_to_goal));
	
	if (norm <= max_delta_tr) {
		forme.centre = goal;
	} else {
		add_scaled_vector(forme.centre, pos_to_goal, max_delta_tr/norm);
	}
	return;
}

void Neutra_0::move(){
	S2d pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
	
	double goal_a(atan2(pos_to_goal.y ,pos_to_goal.x));
	double delta_a(goal_a - orientation);
	adjust_angle(delta_a);
	
	if((abs(delta_a) <= max_delta_rt) and (abs(delta_a) > 0)) {
		orientation = goal_a;
		goal = forme.centre;
	} else if (abs(delta_a) > max_delta_rt) {
		orientation += ((delta_a > 0) ?  1. : -1.)*max_delta_rt;
		goal = forme.centre;
	} else {
		S2d travel_dir = {cos(orientation), sin(orientation)}; 
		add_scaled_vector(forme.centre, travel_dir, max_delta_tr);
	}
}

void Neutra_1::move(){
}

void Neutra_2::move(){
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

void destroy_neutraliseurs(){
	for (size_t i(spatial_getnbRs()+1); i < spatial_getnbRs() + spatial_getnbNs() + 1 ;
		++i){
		if (tab_robot[i]->get_data("panne")){
			if (spatial_getnbUpdate() - tab_robot[i]->get_data("k_update_panne") >= 
				max_update){
				delete tab_robot[i];
				std::swap(tab_robot.back(), tab_robot[i]);
				tab_robot.pop_back();
				tab_robot[0]->set_data("nbNd", spatial_getnbNd()+1);
			}
		}
	}
	return;
}
			
void decision_reparateur(){
	std::vector <bool> tab_reparateur(spatial_getnbRs(), true);
	S2d vect;
	double dist;
	double dist_min;
	int k = NO_TARGET;
	for (size_t i(spatial_getnbRs()+1); i < tab_robot.size(); ++i){
		if (tab_robot[i]->get_data("panne")){
			for (size_t j(0); j < tab_reparateur.size() ; ++j){
				if (tab_reparateur[j]) {
					vect = {tab_robot[i]->getForme().centre.x - 
					tab_robot[j+1]->getForme().centre.x , tab_robot[i]->getForme().centre.y - 
					tab_robot[j+1]->getForme().centre.y };
					dist = norme(vect);
					if (dist <= max_update - (spatial_getnbUpdate() -
					tab_robot[i]->get_data("k_update_panne")) * vtran_max){
						dist_min = dist;
						k = j;
						break;
					}
				}
			 } 
			 for (size_t l(k+1) ; l < tab_reparateur.size(); ++l){
				 if (tab_reparateur[l]){
					vect = {tab_robot[i]->getForme().centre.x - 
						tab_robot[l+1]->getForme().centre.x , 
						tab_robot[l+1]->getForme().centre.y - 
						tab_robot[l+1]->getForme().centre.y };
					dist = norme(vect);
					if (dist < dist_min) {
						dist_min = dist;
						k=l;
					}
				}
			}	
		}		
		if (k == NO_TARGET){
			continue;
		} else {
			tab_reparateur[k] = false;
			tab_robot[k+1]->set_goal(tab_robot[i]->getForme().centre);
		}
	}
	for (size_t i(0); i < tab_reparateur.size() ; ++i){
		if (tab_reparateur[i])
			tab_robot[i+1]->set_goal(tab_robot[0]->getForme().centre);
	}
}	

void decision_neutraliseur(Carre prt, std::vector<bool>& tab_neutra) {
	S2d vect;
	double t_min, t, dist, d_angle;
	int k = NO_TARGET;
	for (size_t i(0); i < tab_neutra.size() ; ++i){
		if (tab_neutra[i] and (!tab_robot[spatial_getnbRs()+1+i]->get_data("panne"))){
			vect = {prt.centre.x-tab_robot[i+spatial_getnbRs()+1]->getForme().centre.x, 
				prt.centre.y - tab_robot[i+spatial_getnbRs()+1]->getForme().centre.y };
			dist = norme(vect);
			d_angle = atan2(vect.y, vect.x) - 
					  tab_robot[spatial_getnbRs()+1+i]->get_data("orientation");
			adjust_angle(d_angle);
			t_min = dist/vtran_max + d_angle/vrot_max;
			k=i;
			break;
		}
	}
	if (k == NO_TARGET){
		return;
	}
	for (size_t i(k+1); i < tab_neutra.size() ; ++i){
		if (tab_neutra[i] and (!tab_robot[spatial_getnbRs()+1+i]->get_data("panne"))){
			vect = {prt.centre.x-tab_robot[i+spatial_getnbRs()+1]->getForme().centre.x, 
				prt.centre.y - tab_robot[i+spatial_getnbRs()+1]->getForme().centre.y };
			dist = norme(vect);
			d_angle = atan2(vect.y, vect.x) - 
					  tab_robot[spatial_getnbRs()+1+i]->get_data("orientation");
			adjust_angle(d_angle);
			t = dist/vtran_max + d_angle/vrot_max;
			if (t < t_min) {
				t = t_min;
				k=i;
			}
		}
	}
	tab_neutra[k] = false;
	tab_robot[k+spatial_getnbRs()+1]->set_goal(tab_robot[k+spatial_getnbRs()+1]->find_goal(prt));
	return;
}

void decision_neutra_restant(std::vector<bool>& tab_neutra) {
	for (size_t i(0); i < tab_neutra.size(); ++i){
		if (!(tab_robot[spatial_getnbRs()+1+i]->get_data("panne"))){
			if (tab_neutra[i]){
				tab_robot[spatial_getnbRs()+1+i]->set_goal(tab_robot[0]->getForme().centre);
			}
		} else {
			tab_robot[spatial_getnbRs()+1+i]->set_goal(tab_robot[spatial_getnbRs()+1+i]->getForme().centre);
		}
	}
	return;
}
			
void decision_creation_robot(){
	bool p(false);
	if (spatial_getnbUpdate() % modulo_update ==0){
		if ((spatial_getnbNp() > spatial_getnbRs()) and (spatial_getnbRr() !=0)){
			Reparateur* pt = new Reparateur(tab_robot[0]->getForme().centre.x, 
											tab_robot[0]->getForme().centre.y, p);
			tab_robot.insert(tab_robot.begin()+1,pt);
			tab_robot[0]->set_data("nbRs", spatial_getnbRs()+1);
			tab_robot[0]->set_data("nbRr", spatial_getnbRr()-1);
		} 
		if (getnbP() > 3*spatial_getnbNs() and spatial_getnbNr() != 0){
			double type((spatial_getnbNd()+spatial_getnbNs())%3);
			Neutraliseur* pt = nullptr;
			bool p=false;
			S2d centre(tab_robot[0]->getForme().centre);
				if (type == 1)	{
					pt = new Neutra_0(centre.x, centre.y, default_orientation ,type ,
								"false", spatial_getnbUpdate(), default_k_update,p);
				} else if (type == 2) {
					pt = new Neutra_1(centre.x, centre.y, default_orientation ,type ,
								"false", spatial_getnbUpdate(), default_k_update,p);
				} else if (type == 3) {
					pt = new Neutra_2(centre.x, centre.y, default_orientation ,type ,
								"false", spatial_getnbUpdate(), default_k_update,p);
				}	
			tab_robot.push_back(pt);
			tab_robot[0]->set_data("nbNs", spatial_getnbNs()+1);
			tab_robot[0]->set_data("nbNr", spatial_getnbNr()-1);
		}
	}
	return;
}
			
void deplacement_robot(){
	for (size_t i(1); i < spatial_getnbRs()+spatial_getnbNs()+1 ; ++i){
		tab_robot[i]->move();
	}
}

S2d find_goal_if_outside_desintegration_area(double angle, double xt, double yt,
											 double xr, double yr, double c) {
	if ((angle > 0 and angle <= M_PI/4 and abs(yr - yt) <= c/2)
		 or (angle <= 0 and angle >= -M_PI/4 and abs(yr - yt) <= c/2))
		return {xt + (c/2)*risk_factor, yr};
	else if (angle > 0 and angle <= M_PI/4 and abs(yr - yt) > c/2)
		return {xt + (c/2)*risk_factor, yt + (c/2)};
	else if (angle > M_PI/4 and angle <= M_PI/2 and abs(xr - xt) > c/2)
		return {xt + (c/2), yt + (c/2)*risk_factor};
	else if ((angle > M_PI/4 and angle <= M_PI/2 and abs(xr - xt) <= c/2)
			  or (angle > M_PI/2 and angle <= 3*M_PI/4 and abs(xr - xt) <= c/2))
		return {xr, yt + (c/2)*risk_factor};
	else if (angle > M_PI/2 and angle <= 3*M_PI/4 and abs(xr - xt) > c/2)
		return {xt - (c/2), yt + (c/2)*risk_factor};
	else if (angle > 3*M_PI/4 and angle <= M_PI and abs(yr - yt) > c/2)
		return {xt - (c/2)*risk_factor, yt + (c/2)};
	else if ((angle > 3*M_PI/4 and angle <= M_PI and abs(yr - yt) <= c/2)
			  or (angle < -3*M_PI/4 and angle >= -M_PI and abs(yr - yt) <= c/2))
		return {xt - (c/2)*risk_factor, yr};
	else if (angle < -3*M_PI/4 and angle >= -M_PI and abs(yr - yt) > c/2)
		return {xt - (c/2)*risk_factor, yt - (c/2)};
	else if (angle < -M_PI/2 and angle >= -3*M_PI/4 and abs(xr - xt) > c/2)
		return {xt - (c/2), yt - (c/2)*risk_factor};
	else if ((angle < -M_PI/2 and angle >= -3*M_PI/4 and abs(xr - xt) <= c/2)
			  or (angle < -M_PI/4 and angle >= -M_PI/2 and abs(xr - xt) <= c/2))
		return {xr, yt - (c/2)*risk_factor};
	else if (angle < -M_PI/4 and angle >= -M_PI/2 and abs(xr - xt) > c/2)
		return {xt + (c/2), yt - (c/2)*risk_factor};
	else
		return {xt + (c/2)*risk_factor, yt - (c/2)};
	
}

S2d find_goal_if_inside_desintegration_area(double angle, double xt, double yt,
											 double xr, double yr, double c) {
	if (angle <= M_PI/4 and angle >= -M_PI/4 and abs(yr - yt) <= c/2)
		return {xt + (c/2), yr};
	else if (angle <= M_PI/4 and angle >= -M_PI/4 and abs(yr - yt) > c/2)
		return {xt + (c/2), yt + sign(yr - yt)*(c/2)};
	else if (angle <= 3*M_PI/4 and angle >= M_PI/4 and abs(xr - xt) <= c/2)
		return {xr, yt + (c/2)};
	else if (angle <= 3*M_PI/4 and angle >= M_PI/4 and abs(xr - xt) > c/2)
		return {xt + sign(xr - xt)*(c/2), yt + (c/2)};
	else if (abs(angle) >= 3*M_PI/4 and abs(yr - yt) <= c/2)
		return {xt - (c/2), yr};
	else if (abs(angle) >= 3*M_PI/4 and abs(yr - yt) > c/2)
		return {xt - (c/2), yt + sign(yr - yt)*(c/2)};
	else if (angle <= -M_PI/4 and angle >= -3*M_PI/4 and abs(xr - xt) <= c/2)
		return {xr, yt - (c/2)};
	else 
		return {xt + sign(xr - xt)*(c/2), yt - (c/2)};
}
	
	
