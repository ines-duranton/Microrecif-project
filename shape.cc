//nom du fichier : shape.cc, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "shape.h"
using namespace std;

bool onSegment(S2d p, S2d q, S2d r, double marge){ 
	double s((q.x-p.x)*(r.x-p.x)+(q.y-p.y)*(r.y-p.y));
	double pr(sqrt(pow((p.x-r.x),2)+pow((p.y-r.y),2)));
	double x(s/pr);
	if(-marge<=x and x<=pr+marge){
		return true; 
	}
	return false; 
}

int orientation(S2d p, S2d q, S2d r, double marge){
	double pq(sqrt(pow((p.x-q.x),2)+pow((p.y-q.y),2)));
	double val = ((q.y - p.y)*(r.x - q.x)-(q.x-p.x)*(r.y-q.y))/pq; 
	// 0 : p, q et r colineaires, 1 : sens horaire, 2 : sens anti-horaire
	if (abs(val)<=marge) return 0;
	return (val > 0)? 1: 2; 
} 
  
bool doIntersect(Segment segment1, Segment segment2, double marge){ 
	S2d p1(segment1.get_base());
	S2d q1(segment1.point_final());
	S2d p2(segment2.get_base());
	S2d q2(segment2.point_final());
	int o1 = orientation(p1, q1, p2, marge); 
	int o2 = orientation(p1, q1, q2, marge); 
	int o3 = orientation(p2, q2, p1, marge); 
	int o4 = orientation(p2, q2, q1, marge); 
	
	if (o1 && o2 && o3 && o4 && o1 != o2 && o3 != o4) return true; 
	if (o1 == 0 && onSegment(p1, p2, q1, marge)) return true; 
	if (o2 == 0 && onSegment(p1, q2, q1, marge)) return true; 
	if (o3 == 0 && onSegment(p2, p1, q2, marge)) return true; 
	if (o4 == 0 && onSegment(p2, q1, q2, marge)) return true; 
	return false; 
}

void draw_circle(double x, double y, double rayon, Color couleur){
	graphic_draw_circle(x,y,rayon,couleur);
}

void draw_square(double x, double y, double cote, Color couleur){
	graphic_draw_square(x, y, cote, couleur);
}

Segment::Segment(S2d base_lue, double angle_lu, double longueur_lue) : base(base_lue),
	angle(angle_lu), longueur(longueur_lue){}

S2d Segment::get_base() const{
	return base;
}
		
double Segment::get_angle() const{
	return angle;
}

double Segment::get_longueur() const{
	return longueur;
}

void Segment::set_longueur(double l){
	longueur=l;
}
	
S2d Segment::point_final() const{
	S2d point_final;
	point_final.x=base.x+longueur*cos(angle);
	point_final.y=base.y+longueur*sin(angle);
	return point_final;
}	

double Segment :: ecart_angulaire(Segment bout){
	double angle_ecart(M_PI+get_angle()-bout.get_angle());
	if(angle_ecart>=M_PI) {
		angle_ecart = angle_ecart-2*M_PI;
	}
	return angle_ecart;
}

bool Segment :: superpos_pt_commun(Segment bout){
	if(ecart_angulaire(bout) == 0){
		return true;
	}
	return false;
}

void Segment::ecriture_segment(ofstream& flot){
	flot << angle << " " << longueur << endl;
}

void Segment::draw_segment(Color couleur){
	S2d point_fin(point_final());
	graphic_draw_segment(base.x, base.y, point_fin.x, point_fin.y, couleur);
}

void Segment::rotation(bool invtrigo, double angle_rot){
	if(!invtrigo) angle+=angle_rot;
	else angle-=angle_rot;
	if(angle > M_PI) angle-=2*M_PI;
	else if(angle < -M_PI) angle += 2*M_PI;
}

void Segment::allonge_seg(double l){
	longueur+=l;
}

void Segment::raccourcit_seg(double l){
	longueur -= l;
}
