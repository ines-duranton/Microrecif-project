//nom du fichier : simulation.h, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>
#include "shape.h"
#include "lifeform.h"
#include "constantes.h"

class Simulation{
public :
	Simulation (std::string nom_fichier_lu) : nom_fichier(nom_fichier_lu){};
	Simulation();
	void lecture();
	void ecriture(std::string nomdefichier);
	int get_nb_algues();
	int get_nb_coraux();
	int get_nb_sca();
	//execution et affichage
	void affiche();
	void execution(bool generation_algue);
private :
	std::string nom_fichier;
	//stockage des entités
	std::vector<Algue> liste_algues;
	std::vector<Corail> liste_coraux;
	std::vector<Scavenger> liste_scavengers;
	//informations lecture
	int etat;
	bool echec_lecture;
	void debut_lecture(); 
	bool decodage_ligne(std::string line);
	bool lecture_algue(double x, double y, int age);
	bool lecture_corail(double x, double y, double age, int id, bool statut_cor, 
		bool dir_rot, bool statut_dev, int nb_segments);
	bool lecture_segment(double x, double y, double angle, double longueur, 
		int compteur_segment, int id);
	bool lecture_scavenger(double x, double y, int age, double rayon, bool statut_sca, 
		int corail_id_cible);
	//tests de validité des entités
	bool duplicated_id(int id);
	bool segment_valide_lecture(Segment segment, int id, int compteur_segment);
	bool invalid_id(bool statut_sca,int corail_id_cible);
	bool lecture_success();
	//mise en mouvement
	bool segment_valide_simu(Corail& corail, size_t indice, double angle, 
		bool algue_mangee);
	void algue_plus_proche(Segment &segment, double &angle_ecart_algue,
		int &indice_algue, bool sens_rotation);
	void rotation_cor(Corail& corail, size_t indice);
	void corail_croissance(Corail& corail);
	int cor_plus_proche_sca(Scavenger sca, double& distance_sca);
	void attribution_sca(size_t place_corail);
	void mouvement_sca(Scavenger& sca);
	void vers_effecteur(Scavenger& sca, size_t place_corail);
	void sur_corail(Scavenger &sca, size_t place_corail);
	void create_sca(S2d pos, double angle);
	
};

#endif
