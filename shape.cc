//NOM PRENOM SCIPER CONTRIBUTION(%)
//LAHLOU SAAD 361150 20%
//ASMOUKI ILYAS 356263 80%

#include "shape.h"
#include "graphic.h"

double distance(S2d p1, S2d p2) {
    return sqrt((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y));
}

double norme(S2d vect) {
	return sqrt(pow(vect.x, 2) + pow(vect.y, 2));
}

double prod_scalaire(S2d vect1, S2d vect2)	{
	return vect1.x * vect2.x + vect1.y * vect2.y;
}

void add_scaled_vector(S2d& vect, S2d added_vect, double scalar)	{
	vect.x += scalar * added_vect.x;
	vect.y += scalar * added_vect.y;
}

int sign(double value) {
	return (( (value) < 0 ) ? -1 : ((value) > 0));
}

bool superposition_carres(const Carre s1, const Carre s2, MARGIN margin) {
	double d1 = s1.cote, d2 = s2.cote;
	return (abs(s2.centre.x - s1.centre.x) < d1/2 + d2/2 + shape::epsil_zero*margin)
	and (abs(s2.centre.y - s1.centre.y) < d1/2 + d2/2 + shape::epsil_zero*margin);
}

bool superposition_cercles(Cercle c1, Cercle  c2, MARGIN margin) {
    return ((c1.rayon+c2.rayon+shape::epsil_zero*margin) >
			distance(c1.centre, c2.centre));
}

bool superposition_cerclecarre(Carre c1, Cercle c2, MARGIN margin) {
	double x1 = c1.centre.x, y1 = c1.centre.y, d1 = c1.cote;
	double x2 = c2.centre.x, y2 = c2.centre.y, r2 = c2.rayon;
	S2d vect;
	vect.x =(abs(x2 - x1) - d1 / 2);
	vect.y = (abs(y2 - y1) - d1 / 2);
    double L = norme(vect);
    
    if ((abs(x2 - x1) > d1 / 2) and (abs(y2 - y1) > d1 / 2)
		and (L > r2 + shape::epsil_zero*margin)) {
			return false;
	} else if ((abs(x2 - x1) < d1 / 2 + r2 + shape::epsil_zero*margin)
		and (abs(y2 - y1) < d1 / 2 + r2 + shape::epsil_zero*margin)) {
			return true;
	}
	return false;
}

void dessin_cercle(Cercle cercle, std::string couleur) {
	draw_circle(cercle.centre.x, cercle.centre.y, cercle.rayon, couleur);
	return;
}

void dessin_carre(Carre carre) {
	draw_square(carre.centre.x, carre.centre.y, carre.cote);
	return;
}

void dessin_orientation(Cercle cercle, double orientation) {
	draw_orientation(cercle.centre.x, cercle.centre.y, cercle.rayon, orientation);
	return;
}

