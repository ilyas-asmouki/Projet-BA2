#ifndef PARTICULE_H
#define PARTICULE_H

#include "shape.h"
#include <vector>
#include <sstream>
#include "constantes.h"

class Particule
{
    private:
    Carre forme;
    public:
    Particule(double x, double y, double cote);
    Carre getForme() const;
    void detect_particle_outside();
    void detect_particle_too_small();
    void detect_particle_superposition();
};

void test_particle_robot_superposition(Cercle robot);

static std::vector<Particule> tab_particule;

void decodage_particule(std::istringstream& data);

#endif

