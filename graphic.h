#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <gtkmm/drawingarea.h>
#include <string>

void set_context(const Cairo::RefPtr<Cairo::Context>& cr);
void draw_circle(double xc, double yc, double radius, std::string color);
void draw_square(double xc, double yc, double length, std::string color);

#endif
