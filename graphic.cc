//nom du fichier : graphic.cc, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "graphic_gui.h"
using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	ptcr = &cr;
}

void graphic_draw_circle(double x, double y, double rayon, Color couleur){
	(*ptcr)->set_line_width(1.0);
	switch(couleur){
		case GREEN : (*ptcr)->set_source_rgb(0.0, 1.0, 0.0); break;
		case RED : (*ptcr)->set_source_rgb(1.0, 0.0, 0.0); break;
		default : break;
	}
	(*ptcr)->arc(x, y, rayon, 0.0, 2*M_PI);
	(*ptcr)->stroke();
}

void graphic_draw_segment(double x, double y, double x_final, double y_final, 
	Color couleur){
	(*ptcr)->set_line_width(1.0);
	switch(couleur){
		case BLUE : (*ptcr)->set_source_rgb(0.0, 0.0, 1.0); break;
		case BLACK : (*ptcr)->set_source_rgb(0.0, 0.0, 0.0); break;
		default : break;
	}
	(*ptcr)->move_to(x,y);
	(*ptcr)->line_to(x_final, y_final);
	(*ptcr)->stroke();
}
	
void graphic_draw_square(double x, double y, double cote, Color couleur){
	(*ptcr)->set_line_width(1.0);
	switch(couleur){
		case BLUE : (*ptcr)->set_source_rgb(0.0, 0.0, 1.0); break;
		case BLACK : (*ptcr)->set_source_rgb(0.0, 0.0, 0.0); break;
		case GREY : (*ptcr)->set_source_rgb(0.66, 0.66, 0.66); break;
		default : break;
	}
	(*ptcr)->move_to(x+(cote/2),y+(cote/2));
	(*ptcr)->line_to(x-(cote/2),y+(cote/2));
	(*ptcr)->line_to(x-(cote/2),y-(cote/2));
	(*ptcr)->line_to(x+(cote/2),y-(cote/2));
	(*ptcr)->line_to(x+(cote/2),y+(cote/2)+0.5);
	(*ptcr)->stroke();
}
	
	
