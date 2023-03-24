#include "shape.h"

double distance(S2d p1, S2d p2)
{
    return sqrt((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y));
}

double norme(S2d vect)
{
	return sqrt(pow(vect.x, 2) + pow(vect.y, 2));
}

bool superposition_carres(const Carre s1, const Carre s2, MODE mode)
{
	double d1 = s1.cote, d2 = s2.cote;
	return (abs(s2.centre.x - s1.centre.x) < d1 / 2 + d2 / 2 + shape::epsil_zero*mode)
		and (abs(s2.centre.y - s1.centre.y) < d1 / 2 + d2 / 2 + shape::epsil_zero*mode);
}

bool superposition_cercles(Cercle c1, Cercle  c2, MODE mode)
{
    return ((c1.rayon+c2.rayon+shape::epsil_zero*mode)>distance(c1.centre, c2.centre));
}

bool superposition_cerclecarre(Carre c1, Cercle c2, MODE mode)
{
	double x1 = c1.centre.x, y1 = c1.centre.y, d1 = c1.cote;
	double x2 = c2.centre.x, y2 = c2.centre.y, r2 = c2.rayon;
	S2d vect;
	vect.x =(abs(x2 - x1) - d1 / 2);
	vect.y = (abs(y2 - y1) - d1 / 2);
    double L = norme(vect);
    
    if ((abs(x2 - x1) > d1 / 2) and (abs(y2 - y1) > d1 / 2)
    and (L > r2 + shape::epsil_zero*mode))
		return false;
	else if ((abs(x2 - x1) < d1 / 2 + r2 + shape::epsil_zero*mode)
	and (abs(y2 - y1) < d1 / 2 + r2 + shape::epsil_zero*mode))
		return true;
	return false;
}

