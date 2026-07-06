//nom du fichier : lifeform.h, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n18

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "shape.h"
#include "constantes.h"

void reset_engine();
bool naissance_algue();
S2d coord_nv_algue();
bool segment_valide(int id, Segment segment);
bool bout_hors_cadre(Segment segment, double marge=0);


class Lifeform{
public :
	Lifeform(double x_lu, double y_lu, int age_lu);
	S2d get_position() const;
	bool get_valide() const;
	int get_age() const;
protected :
	S2d position; //dans 1, max-1
	int age; // strictement positif
	bool valide; //vérification de la validité à l'initialisation
private :
	
};

class Algue : public Lifeform{
public :
	Algue(double x_lu, double y_lu, int age_lu);
	void ecriture_algue(std::ofstream& flot);
	void draw_algue();
	void algue_maj();
private :
};

class Corail : public Lifeform{
public :
	Corail(double x_lu, double y_lu, int age_lu, int id_lu, bool statut_cor_lu, 
		bool dir_rot_lu, bool statut_dev_lu); 
	int get_id() const;
	bool get_dir_rot() const; 
	bool get_statut_dev() const;
	std::vector<Segment> get_segment() const;
	bool get_statut_cor() const ;
	bool get_sca_attribue() const;
	int get_nb_segments() const ;
	void set_statut_cor(bool statut_cor_lu);
	void set_sca_attribue(bool sca_attribue_lu);
	void add_segment(Segment segment);
	void ecriture_corail(std::ofstream& flot);
	void draw_corail();
	void corail_maj();
	void corail_rot(double angle);
	void change_dir_rot();
	void change_statut_dev();
	void annule_rot(double angle);
	bool rotation_sans_algue();
	void allongement();
	void raccourcissement();
	void corail_extend();
	void corail_repro();
	void affiche();
	void supr_dernier_seg();
	bool rotation_avec_algue();
private :
	int id;
	bool statut_cor; //0 : DEAD, 1 : ALIVE 
	bool dir_rot; //0: TRIGO, 1 : INVTRIGO
	bool statut_dev; //0 : EXTEND, 1 : REPRO
	int nb_segments; //strictement positif
	bool sca_attribue; 
	std::vector<Segment> ens_segment;
};

class Scavenger : public Lifeform{
public :
	Scavenger(double x_lu, double y_lu, int age_lu, int rayon_lu, 
		bool statut_sca_lu, int corail_id_cible_lu);
	bool get_statut_sca() const;
	bool get_corail_atteint() const;
	int get_corail_id_cible() const;
	void set_corail_id_cible(int corail_id_cible_lu);
	void set_statut_sca(bool statut_sca_lu);
	void set_corail_atteint(bool corail_atteint_lu);
	void ecriture_scavenger(std::ofstream& flot);
	void draw_sca();
	void scavenger_maj();
	void deplacement(S2d direction);
	bool sca_extend();
private :
	unsigned rayon;
	bool statut_sca; //0 : LIBRE, 1 : MANGE 
	int corail_id_cible; 
	bool corail_atteint;
};

#endif
