#include "graphic.h"
#include "constantes.h"
#include <cmath>

static const Cairo::RefPtr<Cairo::Context>* ptcr = nullptr;

void set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	ptcr = &cr;
}

void set_world()
{
	(*ptcr)->set_source_rgb(1., 1., 1.);
	(*ptcr)->paint();
	(*ptcr)->set_line_width(1.0);
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5);
	(*ptcr)->rectangle(-dmax, -dmax, 2*dmax, 2*dmax);
	(*ptcr)->stroke();
}

void draw_circle(double xc, double yc, double radius, std::string color)
{
    if (color == "blue")
        (*ptcr)->set_source_rgb(0.67, 0.85, 0.9);
    else if (color == "purple")
        (*ptcr)->set_source_rgb(0.5, 0., 1.);
    else if (color == "orange")
        (*ptcr)->set_source_rgb(1., 0.4, 0.);
    else
        (*ptcr)->set_source_rgb(0., 0., 0.);
    (*ptcr)->arc(xc, yc, radius / 20., 0, 2*M_PI);
    (*ptcr)->stroke();
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->arc(xc, yc, radius, 0, 2*M_PI);
    (*ptcr)->stroke();
    if (color == "green")
    {
        (*ptcr)->set_source_rgb(0., 1., 0.);
        (*ptcr)->arc(xc, yc, radius, 0, 2*M_PI);
        (*ptcr)->fill();
    }
}

void draw_square(double xc, double yc, double length)
{
	(*ptcr)->set_source_rgb(0.75, 0.75, 0.75);
    (*ptcr)->rectangle(xc - length/2, yc - length/2, length, length);
    (*ptcr)->fill();
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5);
	(*ptcr)->set_line_width(1.0);
	(*ptcr)->set_source_rgb(1, 0., 0.);
    (*ptcr)->rectangle(xc - length/2, yc - length/2, length, length);
	(*ptcr)->stroke();
}

void draw_orientation(double xc, double yc, double radius, double angle)
{
	(*ptcr)->set_source_rgb(0., 1., 0.);
	(*ptcr)->set_line_width(1.0);
	(*ptcr)->move_to(xc, yc);
	(*ptcr)->line_to(xc + radius*cos(angle), yc + radius*sin(angle));
	(*ptcr)->stroke();
}


