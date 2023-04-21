#include "graphic.h"
#include <cmath>

static const Cairo::RefPtr<Cairo::Context>* ptcr = nullptr;

void set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	ptcr = &cr;
}

void draw_circle(double xc, double yc, double radius, std::string color)
{
	if (color == "blue")
		(*ptcr)->set_source_rgb(0., 0., 0.5);
	else if (color == "purple")
		(*ptcr)->set_source_rgb(0.5, 0., 1.);
	else if (color == "orange")
		(*ptcr)->set_source_rgb(1., 0.4, 0.);
	else
		(*ptcr)->set_source_rgb(0., 0., 0.);
	(*ptcr)->set_line_width(10.0);
	(*ptcr)->arc(xc, yc, radius, 0, 2*M_PI);
	(*ptcr)->stroke();
}

void draw_square(double xc, double yc, double length)
{
	(*ptcr)->set_source_rgb(1., 0., 0.);
	(*ptcr)->set_line_width(10.0);
	(*ptcr)->move_to(xc - length/2, yc - length/2);
	(*ptcr)->line_to(xc + length/2, yc - length/2);
	(*ptcr)->stroke();
	(*ptcr)->move_to(xc + length/2, yc - length/2);
	(*ptcr)->line_to(xc + length/2, yc + length/2);
	(*ptcr)->stroke();
	(*ptcr)->move_to(xc + length/2, yc + length/2);
	(*ptcr)->line_to(xc - length/2, yc + length/2);
	(*ptcr)->stroke();
	(*ptcr)->move_to(xc - length/2, yc + length/2);
	(*ptcr)->line_to(xc - length/2, yc - length/2);
	(*ptcr)->stroke();
}
