//nom du fichier : graphic.h, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#ifndef GRAPHIC_H
#define GRAPHIC_H

enum Color { WHITE, BLACK, GREEN, BLUE, RED, GREY };

void graphic_draw_circle(double x, double y, double rayon, Color couleur);
void graphic_draw_segment(double x, double y, double x_final, double y_final, 
	Color couleur);
void graphic_draw_square(double x, double y, double cote, Color couleur);

#endif
