//nom du fichier : shape.h, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "graphic.h"

constexpr double epsil_zero(0.5);

struct S2d{ // coordonnées d'un point ou d'un vecteur
	double x;
	double y;
};

void draw_circle(double x, double y, double rayon, Color couleur);
void draw_square(double x, double y, double cote, Color couleur);

class Segment{
public :
	Segment(S2d base_lue = {0,0}, double angle_lu = 0, double longueur_lue = 0);
	S2d get_base() const;
	double get_angle() const;
	double get_longueur() const;
	void set_longueur(double l);
	S2d point_final()const;
	double ecart_angulaire(Segment bout);
	bool superpos_pt_commun(Segment bout);
	void ecriture_segment(std::ofstream& flot);
	void draw_segment(Color couleur);
	void rotation(bool invtrigo, double angle_rot);
	void allonge_seg(double l);
	void raccourcit_seg(double l);
private:
	S2d base;
	double angle;
	double longueur;
};

bool doIntersect(Segment segment1, Segment segment2, double marge);

#endif
