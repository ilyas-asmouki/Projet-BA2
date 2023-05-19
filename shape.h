//LAHLOU SAAD 361150
//ASMOUKI ILYAS 356263

#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <cmath>

enum Margin {NO_MARGIN, WITH_MARGIN};

namespace shape {
	constexpr double epsil_zero(0.125);
}

struct S2d {
	double x;
	double y;
};

struct Cercle {
	S2d centre;
	double rayon;
};

struct Carre {
	S2d centre;
	double cote;
};

double distance(S2d p1, S2d p2);
double norme(S2d vect);
double prod_scalaire(S2d vect1, S2d vect2);
void add_scaled_vector(S2d& vect, S2d added_vect, double scalar);
int sign(double value);
bool superposition_carres(const Carre s1, const Carre s2, Margin margin);
bool superposition_cercles(Cercle c1, Cercle  c2, Margin margin);
bool superposition_cerclecarre(Carre c1, Cercle c2, Margin margin);
void dessin_cercle(Cercle cercle, std::string couleur);
void dessin_carre(Carre carre);
void dessin_orientation(Cercle cercle, double orientation);
void adjust_angle(double& d_angle);
void mod_2pi(double& angle);

#endif
