//nom du fichier : lifeform.cc, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n18

#include <iostream>
#include <cmath>
#include <random>
#include "lifeform.h"
#include "message.h"
using namespace std;

static default_random_engine e;

void reset_engine(){
	e.seed(1);
}

bool naissance_algue(){
	double p(alg_birth_rate);
	bernoulli_distribution b(p);
	return b(e);
}

S2d coord_nv_algue(){
	S2d centre_algue;
	uniform_int_distribution<unsigned> u(1,dmax-1);
	centre_algue.x=u(e);
	centre_algue.y=u(e);
	return centre_algue;
}

bool segment_valide(int id, Segment segment){
	if(segment.get_angle() > M_PI or segment.get_angle() < -M_PI){
		cout << message :: segment_angle_outside(id, segment.get_angle());
		return false;
	}
	if(segment.get_longueur() < l_repro-l_seg_interne 
		or segment.get_longueur() >= l_repro){
		cout << message :: segment_length_outside(id, segment.get_longueur());
		return false;
	}
	return true;
}

bool bout_hors_cadre(Segment segment, double marge){
	S2d bout(segment.point_final());
	if(bout.x<=marge or bout.x>=dmax-marge or bout.y<=marge or bout.y>=dmax-marge){
		return true;
	}
	return false;
}

// Lifeform

Lifeform :: Lifeform(double x_lu, double y_lu, int age_lu) 
	: position({x_lu, y_lu}), age(age_lu), valide(true){
	if (age<=0){
		cout << message :: lifeform_age(age);
		valide = false;
	}
	if ((position.x) < 1 or (position.x) > dmax-1 or (position.y) < 1 
		or (position.y) > dmax-1){
		cout << message :: lifeform_center_outside(position.x, position.y);
		valide = false;
	}
}

S2d Lifeform::get_position() const{
	return position;
}

bool Lifeform :: get_valide() const{
	return valide;
}

int Lifeform::get_age() const{
	return age;
}

// Algue

Algue :: Algue(double x_lu, double y_lu, int age_lu) 
	: Lifeform(x_lu, y_lu, age_lu){}

void Algue::ecriture_algue(std::ofstream& flot){
	flot << position.x << " " << position.y << " " << age << endl;
}

void Algue::draw_algue(){
	draw_circle(position.x,position.y,r_alg,GREEN);
}

void Algue::algue_maj(){
	age+=1;
}

// Corail

Corail::Corail(double x_lu, double y_lu, int age_lu, int id_lu, bool statut_cor_lu, 
	bool dir_rot_lu, bool statut_dev_lu):
	Lifeform(x_lu, y_lu, age_lu), id(id_lu), statut_cor(statut_cor_lu), 
	dir_rot(dir_rot_lu), statut_dev(statut_dev_lu), nb_segments(0), sca_attribue(false)
	{}

int Corail::get_id() const{
	return id;
}

bool Corail::get_dir_rot() const{
	return dir_rot;
}

bool Corail::get_statut_dev() const{
	return statut_dev;
}

vector<Segment> Corail::get_segment() const{
	return ens_segment;
}

bool Corail::get_statut_cor() const{
	return statut_cor;
}

bool Corail::get_sca_attribue() const{
	return sca_attribue;
}

int Corail::get_nb_segments() const{
	return nb_segments;
}

void Corail::set_statut_cor(bool statut_cor_lu){
	statut_cor=statut_cor_lu;
}
	
void Corail::set_sca_attribue(bool sca_attribue_lu){
	sca_attribue=sca_attribue_lu;
}

void Corail::add_segment(Segment segment){
	ens_segment.push_back(segment);
	nb_segments += 1;
}

void Corail::ecriture_corail(std::ofstream& flot){
	flot << position.x << " " << position.y << " " << to_string(age) << " " ;
	flot << to_string(id) << " " << statut_cor << " " ;
	flot << dir_rot << " " << statut_dev << " " << ens_segment.size() << endl;
	for(auto segment : ens_segment){
		segment.ecriture_segment(flot);
	}
}

void Corail::draw_corail(){
	Color couleur(statut_cor ? BLUE : BLACK);
	draw_square(position.x, position.y,d_cor,couleur);
	for(auto segment:ens_segment){
		segment.draw_segment(couleur);
	}
}

void Corail::corail_maj(){
	age+=1;
	if(age == max_life_cor) statut_cor = false;
}

void Corail::corail_rot(double angle){
	(ens_segment.back()).rotation(dir_rot, angle);
}

void Corail::change_dir_rot(){
	dir_rot=!(dir_rot);
}

void Corail::change_statut_dev(){
	statut_dev=!(statut_dev);
}

void Corail::annule_rot(double angle){
	(ens_segment.back()).rotation(!dir_rot, angle);
}

bool Corail::rotation_sans_algue(){
	if(nb_segments >= 2 and statut_cor==true){
		Segment segment(ens_segment.back());
		Segment segment_precedent(ens_segment[ens_segment.size()-2]);
		if (abs(segment_precedent.ecart_angulaire(segment)) > delta_rot){
			corail_rot(delta_rot);
			return true;
		}
		else if(segment_precedent.ecart_angulaire(segment) > 0 and dir_rot == true){
			corail_rot(delta_rot);
			return true;
		}
		else if(segment_precedent.ecart_angulaire(segment) < 0 and dir_rot == false){
			corail_rot(delta_rot);
			return true;
		}
		else change_dir_rot();
	}
	else if(nb_segments == 1 and statut_cor==true){
		corail_rot(delta_rot);
		return true;
	}
	return false;
}

void Corail::allongement(){
	(ens_segment.back()).allonge_seg(delta_l);
}

void Corail::raccourcissement(){
	(ens_segment.back()).raccourcit_seg(delta_l);
}

void Corail::corail_extend(){
	(ens_segment.back()).set_longueur(l_seg_interne);
	Segment segment((ens_segment.back()).point_final(), 
		(ens_segment.back()).get_angle(), l_repro-l_seg_interne);
	add_segment(segment);
	statut_dev=true;
}

void Corail::corail_repro(){
	(ens_segment.back()).set_longueur(l_repro/2);
}

void Corail::supr_dernier_seg(){
	if (ens_segment.size() > 0) {
		ens_segment.pop_back();
		nb_segments-=1;
	}
}

bool Corail::rotation_avec_algue(){
	if(nb_segments >= 2 and statut_cor==true){
		Segment segment(ens_segment.back());
		Segment segment_precedent(ens_segment[ens_segment.size()-2]);
		if(segment_precedent.ecart_angulaire(segment) > delta_rot or 
			segment_precedent.ecart_angulaire(segment) < -delta_rot){
			return true;
		}
		else if(segment_precedent.ecart_angulaire(segment) > 0 and dir_rot == true){
			return true;
		}
		else if(segment_precedent.ecart_angulaire(segment) < 0 and dir_rot == false){
			return true;
		}
	}
	else if(nb_segments == 1 and statut_cor==true){
		return true;
	}
	return false;
}

// Scavenger

Scavenger :: Scavenger(double x_lu, double y_lu, int age_lu, int rayon_lu, 
	bool statut_sca_lu, int corail_id_cible_lu) : Lifeform(x_lu, y_lu, age_lu), 
	rayon(rayon_lu), statut_sca(statut_sca_lu), corail_id_cible(corail_id_cible_lu), 
	corail_atteint(false){
	if(rayon<r_sca or rayon>= r_sca_repro){
		cout << message::scavenger_radius_outside(rayon);
		valide = false;	
	}
}

bool Scavenger::get_statut_sca() const{
	return statut_sca;
}

bool Scavenger::get_corail_atteint() const{
	return corail_atteint;
}

int Scavenger::get_corail_id_cible() const{
	return corail_id_cible;
}

void Scavenger::set_corail_id_cible(int corail_id_cible_lu){
	corail_id_cible=corail_id_cible_lu;
}

void Scavenger::set_statut_sca(bool statut_sca_lu){
	statut_sca=statut_sca_lu;
}

void Scavenger::set_corail_atteint(bool corail_atteint_lu){
	corail_atteint=corail_atteint_lu;
}

void Scavenger::ecriture_scavenger(std::ofstream& flot){
	flot << position.x << " " << position.y << " " << to_string(age) << " ";
	flot << rayon << " " << statut_sca << endl;
	if (corail_id_cible>=0) flot << corail_id_cible;
}

void Scavenger::draw_sca(){
	draw_circle(position.x,position.y,rayon,RED);
}

void Scavenger::scavenger_maj(){
	age += 1;
}

void Scavenger::deplacement(S2d direction){
	position = direction;
}

bool Scavenger::sca_extend(){
	rayon+=delta_r_sca;
	if(rayon >= r_sca_repro){
		rayon=r_sca;
		return true;
	}
	return false;
}

