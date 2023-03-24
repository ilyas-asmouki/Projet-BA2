#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <cmath>

enum MODE {LECTURE, SIMULATION};

namespace shape {
	constexpr double epsil_zero(0.125);
}
struct S2d
{
	double x;
	double y;
};

struct Cercle
{
	S2d centre;
	double rayon;
};

struct Carre
{
	S2d centre;
	double cote;
};

double distance(S2d p1, S2d p2);
double norme(S2d vect);
bool superposition_carres(const Carre s1, const Carre s2, MODE mode);
bool superposition_cercles(Cercle c1, Cercle  c2, MODE mode);
bool superposition_cerclecarre(Carre c1, Cercle c2, MODE mode);

#endif
