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

constexpr double INFINI(9999999);
constexpr S2d LIMIT({INFINI, INFINI});

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

S2d Neutraliseur::find_goal(Carre target) {
	if (!superposition_cerclecarre(target, forme, WITH_MARGIN)){
		color = "black";
		if (superposition_robots_sim()){
			color = "purple";
			return forme.centre;
		} else 
			return target.centre;
	} else {
		color = "purple";
		return forme.centre;
	}
}
		
S2d Neutra_1::find_goal(Carre target) {
	double xr = forme.centre.x, yr = forme.centre.y;
	double xt = target.centre.x, yt = target.centre.y;
	double c = target.cote;
	double angle = atan2(yr - yt, xr - xt);
	Carre risk_zone = {{target.centre.x, target.centre.y}, target.cote * risk_factor}; 
	double delta_angle = angle - orientation;
	adjust_angle(delta_angle);
	
	if (!superposition_cerclecarre(risk_zone, forme, WITH_MARGIN)){
		color = "black";
		return find_goal_if_outside_desintegration_area(angle, xt, yt, xr, yr, c);
	} else if ((superposition_cerclecarre(risk_zone, forme, WITH_MARGIN)) 
				and (delta_angle > epsil_alignement)){
		return forme.centre;
	} else {
		return find_goal_if_inside_desintegration_area(angle, xt, yt, xr, yr, c);
	}	
}	

Spatial::Spatial(double x, double y, int nbUpdate, unsigned nbNr, unsigned nbNs,
	  unsigned nbNd, unsigned nbRr, unsigned nbRs, bool& file_success): Robot(x, y), 
	  nbUpdate(nbUpdate), nbNr(nbNr), nbNs(nbNs), nbNd(nbNd), nbRr(nbRr), nbRs(nbRs){

	color = "blue";
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

Reparateur::Reparateur(double x, double y, bool& file_success): Robot(x,y) {
	color = "green";
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
	color = ((panne) ? "orange" : "black");
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
	
void Robot::TestCollision(bool& file_success){
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
	tab_robot[0]->set_data("nbUpdate", value);
	return;
}

void draw_robots() {
	if (tab_robot.size() != 0){
		for (size_t i = 1; i < tab_robot.size(); ++i) {
			if (i <= spatial_getnbRs())
				dessin_cercle(tab_robot[i]->getForme(), "green");
			else {
				dessin_cercle(tab_robot[i]->getForme(), tab_robot[i]->getcolor());
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

void Robot::set_color(std::string c) {
	color = c;
}

S2d Robot::get_goal() {
	return goal;
}

bool in_desintegration_area(Carre particle, size_t i)
{
	return superposition_cerclecarre({{particle.centre.x, particle.centre.y}, 
			particle.cote*risk_factor}, tab_robot[i]->getForme(), WITH_MARGIN);
}

void set_panne_robot(size_t i){
	tab_robot[i]->set_data("panne",true);
	tab_robot[i]->set_data("k_update_panne",spatial_getnbUpdate());
	tab_robot[i]->set_goal(tab_robot[i]->getForme().centre); 
}

void Reparateur::move(){
	S2d pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y} ;
	double norm(norme(pos_to_goal));
	
	S2d temp = forme.centre;
	if (norm <= max_delta_tr) {
		forme.centre = goal;
	} else {
		add_scaled_vector(forme.centre, pos_to_goal, max_delta_tr/norm);
	}
	if (superposition_particle_robot_sim(forme)){
		forme.centre = temp;	
	}
	if (superposition_robots_sim()){
		forme.centre = temp;
		repair_neutra(goal);
	}
	return;
}

void Reparateur::repair_neutra(S2d goal){
	for (size_t i(1 + spatial_getnbRs()); i < tab_robot.size(); ++i){
		if (tab_robot[i]->get_data("panne") 
		and goal.x == tab_robot[i]->getForme().centre.x 
		and goal.y == tab_robot[i]->getForme().centre.y){
			tab_robot[i]->set_data("panne", false);
			tab_robot[i]->set_color("black");
		}
	}
}
				
void Neutraliseur::move(){
	//~ if (forme.centre.x == goal.x and forme.centre.y == goal.y) 
		//~ return;
	S2d pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
	double goal_a(atan2(pos_to_goal.y ,pos_to_goal.x));
	double delta_a(goal_a - orientation);
	adjust_angle(delta_a);
	if((abs(delta_a) <= max_delta_rt)) {
		orientation = goal_a;
	} else if (abs(delta_a) > max_delta_rt) {
		orientation += ((delta_a > 0) ?  1. : -1.)*max_delta_rt;
	} else {
		S2d temp=forme.centre;
		S2d travel_dir = {cos(orientation), sin(orientation)}; 
		add_scaled_vector(forme.centre, travel_dir, max_delta_tr);
		if  (superposition_robots_sim()){
			forme.centre = temp;
			color = "purple";	
		}
		if (superposition_particle_robot_sim(forme)){
			forme.centre = temp;
			color = "purple";
			decontaminate();
		}
	}
	return;
}

void Neutraliseur::decontaminate() {
	if (not panne)	{
		//~ SIDE side = find_side(particle_to_destroy(forme.centre));
		SIDE side = find_side(particle_to_destroy(goal));
		double angle = M_PI - side*M_PI/2;
		//~ std::cout<<side<<","<<angle<<std::endl;
		if (abs(orientation - angle) <= epsil_alignement){
			//~ std::cout<<"zzzzzz"<<std::endl;
			//~ destroy_particle(particle_to_destroy(forme.centre));
			destroy_particle(goal);
		}
	}
}

//~ void Neutra_1::move(){
//~ }

void Neutra_2::move(){
	//~ if (forme.centre.x == goal.x and forme.centre.y == goal.y) 
		//~ return;
	S2d init_pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
	S2d travel_dir    = {cos(orientation), sin(orientation)}; 
	double proj_goal= prod_scalaire(init_pos_to_goal, travel_dir);
	double delta_a;
	SIDE side=find_side(goal);
	delta_a = side*M_PI/2 - orientation;
	adjust_angle(delta_a);	
	if (abs(delta_a) >= M_PI/3){
		//~ if (abs(goal_a - orientation) <= max_delta_rt){
			//~ orientation = goal_a;
			//~ return;
		//~ } else {
			orientation += ((delta_a > 0) ?  1. : -1.)*max_delta_rt;
			return;
	} else if (abs(delta_a) < epsil_alignement) {
		if(abs(proj_goal) > max_delta_tr){ 
			proj_goal = ((proj_goal > 0) ? 1 : -1)*max_delta_tr;
		}
		add_scaled_vector(forme.centre, travel_dir, proj_goal);
		//~ S2d    updated_pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
		//~ double goal_a(atan2(updated_pos_to_goal.y ,updated_pos_to_goal.x));
		//~ double alpha(goal_a - orientation);
		
		//~ if(abs(alpha) <= max_delta_rt) {
			//~ orientation = goal_a;
		//~ } else {
			//~ orientation += ((alpha > 0) ?  1. : -1.)*max_delta_rt;
		//~ }
	} else {
		double v_tran = abs(proj_goal)*vrot_max/abs(delta_a);
		if(abs(proj_goal) > v_tran*delta_t){ 
			proj_goal = ((proj_goal > 0) ? 1 : -1)*v_tran*delta_t;
		}
		S2d temp = forme.centre;
		add_scaled_vector(forme.centre, travel_dir, proj_goal);
		if  (superposition_robots_sim()){
			forme.centre = temp;
			color = "purple";	
		}
		if (superposition_particle_robot_sim(forme)){
			forme.centre = temp;
			color = "purple";
			decontaminate();
		}
		//~ S2d updated_pos_to_goal = {goal.x - forme.centre.x, goal.y - forme.centre.y};
		//~ double goal_a(atan2(updated_pos_to_goal.y ,updated_pos_to_goal.x));
		if(abs(delta_a) <= max_delta_rt) {
			orientation = side*M_PI/2 + M_PI - delta_a;
			adjust_angle(orientation);
		} else {
			orientation += ((delta_a > 0) ?  1. : -1.)*max_delta_rt;
		}
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
				tab_robot.erase(tab_robot.begin() + i);
				tab_robot[0]->set_data("nbNd", spatial_getnbNd()+1);
				tab_robot[0]->set_data("nbNs", spatial_getnbNs()-1);
			}
		}
	}
	return;
}

unsigned find_indice(S2d temp){
	for (size_t i(1+spatial_getnbRs()); i < tab_robot.size(); ++i){
		if (tab_robot[i]->getForme().centre.x == temp.x and 
		tab_robot[i]->getForme().centre.y == temp.y) {
			return i;
		}
	}
	return NULL_DATA;
}

void decision_reparateur(){
	std::vector <double> tab_distance(spatial_getnbRs(), INFINI);
	std::vector <S2d> tab_goal(spatial_getnbRs(), LIMIT);
	S2d vect;
	double dist;
	int k = NO_TARGET;
	for (size_t i(spatial_getnbRs()+1); i < tab_robot.size(); ++i){
		if (tab_robot[i]->get_data("panne")){
			S2d temp = LIMIT;
			find_first_repairer(tab_distance, tab_goal, k, i, temp); 
			if (temp.x != INFINI and temp.y != INFINI) {
				i = find_indice(temp);
				continue;
			} 
			if (k != NO_TARGET){
				for (size_t l(k+1) ; l < tab_distance.size(); ++l){
					vect = {tab_robot[i]->getForme().centre.x - tab_robot[l+1]->
							getForme().centre.x , tab_robot[l+1]->getForme().centre.y - 
							tab_robot[l+1]->getForme().centre.y };
					dist = norme(vect);
					if (dist < tab_distance[l]) {
						k=l;
						temp = tab_goal[l];
						tab_distance[l] = dist;
						tab_goal[l] = tab_robot[i]->getForme().centre;
					}
					if (temp.x != INFINI and temp.y != INFINI) {
						i = find_indice(temp);
						continue;
					}
				}	
			} else
				continue;
		}
	}
	give_goal_repairer(tab_distance, tab_goal);
}

void give_goal_repairer(std::vector<double>& tab_distance,std::vector<S2d>& tab_goal){
	for (size_t i(0); i < tab_distance.size() ; ++i){	
		if (tab_distance[i] == INFINI)
			tab_robot[i+1]->set_goal(tab_robot[0]->getForme().centre);
		else 
			tab_robot[i+1]->set_goal(tab_goal[i]);
	}
}
	

void find_first_repairer(std::vector<double>& tab_distance, 
						 std::vector<S2d>& tab_goal,int& k,size_t i, S2d& temp){
	S2d vect;
	double dist; 
	for (size_t j(0); j < tab_distance.size() ; ++j){
		vect = {tab_robot[i]->getForme().centre.x - 
				tab_robot[j+1]->getForme().centre.x , tab_robot[i]->getForme().centre.y - 
				tab_robot[j+1]->getForme().centre.y };
		dist = norme(vect);
		if ((dist < tab_distance[j]) and  dist < ((max_update - 
		(spatial_getnbUpdate() - 
		tab_robot[i]->get_data("k_update_panne"))) * vtran_max)){
			tab_distance[j] = dist;
			temp = tab_goal[j];
			tab_goal[j] = tab_robot[i]->getForme().centre;	
			k = j;
			return;
		}
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
		if ((getnbP() > 3*spatial_getnbNs()) and (spatial_getnbNr() != 0)){
			double type((spatial_getnbNd()+spatial_getnbNs())%3);
			Neutraliseur* pt = nullptr;
			bool p=false;
			S2d centre(tab_robot[0]->getForme().centre);
				if (type == 0)	{
					pt = new Neutra_0(centre.x, centre.y, default_orientation ,type ,
								"false", spatial_getnbUpdate(), default_k_update,p);
				} else if (type == 1) {
					pt = new Neutra_1(centre.x, centre.y, default_orientation ,type ,
								"false", spatial_getnbUpdate(), default_k_update,p);
				} else if (type == 2) {
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
	
bool Robot::superposition_robots_sim() {
	bool p(false);
	for (size_t i(1); i < tab_robot.size(); ++i){
		if (superposition_cercles(forme, tab_robot[i]->forme, WITH_MARGIN) and
		 (forme.centre.x != tab_robot[i]->forme.centre.x or
		 forme.centre.y != tab_robot[i]->forme.centre.y)){
			p = true;
		}
	}
	return p;
}

double set_orientation(S2d robot, Carre target) {
    double xr = robot.x, yr = robot.y;
    double xt = target.centre.x, yt = target.centre.y, c = target.cote;
    if (abs(yt - yr) <= c/2)
        return ((xr < xt) ? 0 : M_PI);
    else if (abs(xt - xr) <= c/2)
        return sign(yt - yr)*M_PI/2;
    else
        return atan2(yt - yr + sign(yr - yt + sign(yt - yr)*(c/2))*(c/2),
                     xt - xr + sign(xr - xt + sign(xt - xr)*(c/2))*(c/2));
}

SIDE Neutraliseur::find_side(S2d particle){
	double angle(atan2(forme.centre.y-particle.y, forme.centre.x-particle.x));
	if (angle >= -M_PI/4 and angle < M_PI/4)
		return R;
	else if (angle >= M_PI/4 and angle < 3*M_PI/4)
		return U;
	else if (angle >= 3*M_PI/4 or angle < -3*M_PI/4)
		return L;
	else 
		return D;
}
	
std::string Robot::getcolor(){
	return color;
}
