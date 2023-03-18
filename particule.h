#ifndef PARTICULE_H
#define PARTICULE_H

#include "shape.h"
#include <vector>

constexpr double d_particule_min(8*epsil_zero);
constexpr double dmax(128.);

class Particule
{
    private:
    Carre forme;
    static std::vector<Particule> tab_particule;
    public:
    Particule(double x, double y, double cote);
    Carre getForme() const;
    void detect_particle_outside();
    void detect_particle_too_small();
    void detect_particle_superposition(); 
};

#endif
