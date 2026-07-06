//nom du fichier : simulation.cc, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "simulation.h"
#include "message.h"
using namespace std;

enum Etat_lecture {NB_ALG,ALG,NB_COR,COR,SEGMENT, NB_SCA, SCA, SUCCESS};

Simulation::Simulation(){}

void Simulation::lecture(){
	string ligne;
	ifstream fichier(nom_fichier); 
	debut_lecture();
	if(!fichier.fail()){
		echec_lecture = false;
		while(getline(fichier >> ws,ligne ) and echec_lecture == false){
			if(ligne[0]=='#' ) continue;  
			echec_lecture = !(decodage_ligne(ligne));
		}
	}
	if(echec_lecture == true) debut_lecture();
}

void Simulation :: ecriture(string nomdefichier){
	ofstream fichier(nomdefichier);
	fichier << liste_algues.size() << endl;
	for(auto algue : liste_algues){
		algue.ecriture_algue(fichier);
	}
	fichier << liste_coraux.size() << endl;
	for(auto corail : liste_coraux){
		corail.ecriture_corail(fichier);
	}
	fichier << liste_scavengers.size() << endl;
	for(auto scavenger : liste_scavengers){
		scavenger.ecriture_scavenger(fichier);
	}
}

int Simulation::get_nb_algues(){
	return liste_algues.size();
}

int Simulation::get_nb_coraux(){
	return liste_coraux.size();
}

int Simulation::get_nb_sca(){
	return liste_scavengers.size();
}

//affichage et exécution

void Simulation::affiche(){
	for(auto algue : liste_algues){
		algue.draw_algue();
	}
	for(auto scavenger : liste_scavengers){
		scavenger.draw_sca();
	}
	for(auto corail: liste_coraux){
		corail.draw_corail();
	}
}

void Simulation::execution(bool generation_algue){
	//traitement des algues
	int algue_a_eliminer(0);
	if(liste_algues.size()>0){
		for(size_t i(0); i<liste_algues.size(); ++i){
			(liste_algues[i]).algue_maj();
			if((liste_algues[i]).get_age()==max_life_alg){
				algue_a_eliminer+=1;
				if(i!=0){
					swap(liste_algues[i],liste_algues[algue_a_eliminer-1]);
				}
			}
		}
	}
	liste_algues.erase(liste_algues.begin(), liste_algues.begin()+algue_a_eliminer);
	if(generation_algue and naissance_algue()){
		S2d base_algue(coord_nv_algue());
		Algue algue(base_algue.x,base_algue.y,1);
		liste_algues.push_back(algue);
	}
	//traitement des coraux
	size_t size(liste_coraux.size());
	if(size>0){
		for(size_t i(0); i<size; ++i){
			(liste_coraux[i]).corail_maj();
			rotation_cor(liste_coraux[i], i);
			attribution_sca(i);
		}
	}
	//traitement des scavengers
	int sca_a_eliminer(0);
	if(liste_scavengers.size()>0){
		for(size_t i(0); i<liste_scavengers.size(); ++i){
			(liste_scavengers[i]).scavenger_maj();
			if((liste_scavengers[i]).get_age()==max_life_sca){
				sca_a_eliminer+=1;
				if(i!=0){
					swap(liste_scavengers[i],liste_scavengers[sca_a_eliminer-1]);
				}
			}
		}
		liste_scavengers.erase(liste_scavengers.begin(), 
			liste_scavengers.begin()+sca_a_eliminer);
		size_t size(liste_scavengers.size());
		for(size_t i(0); i<size; ++i){
			if(liste_scavengers[i].get_statut_sca()==true){
				mouvement_sca(liste_scavengers[i]);
			}
		}
	}
}

//lecture 

void Simulation :: debut_lecture(){
	etat = NB_ALG;
	liste_algues.clear();
	liste_coraux.clear();
	liste_scavengers.clear();
	reset_engine();
}

bool Simulation :: decodage_ligne(string ligne){
	istringstream data(ligne);

	static int nb_alg, nb_cor, nb_segments, nb_sca, compteur_segment, id;
	static double x, y;
	int age;
	bool statut_cor, dir_rot, statut_dev, statut_sca;
	double angle;
	int rayon, longueur;
	int corail_id_cible;

	switch(etat){
	case NB_ALG: 
		data >> nb_alg; 
		if(nb_alg==0) etat=NB_COR; 
		else etat=ALG ; 
		break;
	case ALG:
		data >> x >> y >> age;
		nb_alg -= 1;
		if(lecture_algue(x, y, age) == false) return false;
		if(nb_alg == 0) etat=NB_COR ;
		break;   
	case NB_COR:
		data >> nb_cor; 
		if(nb_cor==0) etat=NB_SCA; 
		else etat=COR ; 
		break; 
	case COR:
		data >> x >> y >> age >> id >> statut_cor >> dir_rot;
		data >> statut_dev >> nb_segments;
		if(lecture_corail(x, y, age, id, statut_cor, dir_rot, statut_dev, 
			nb_segments) == false) return false;
		nb_cor -= 1;
		compteur_segment = 0; 
		if(nb_segments != 0) etat = SEGMENT;
		else if(nb_cor == 0) etat = NB_SCA;
		break;
	case SEGMENT:
		data >> angle >> longueur;
		if(lecture_segment(x, y, angle, longueur, compteur_segment, 
			id) == false) return false;
		compteur_segment += 1;
		if(compteur_segment == nb_segments and nb_cor == 0) etat = NB_SCA;
		else if(compteur_segment == nb_segments) etat = COR;
		else etat = SEGMENT;
		break;	
	case NB_SCA:
		data >> nb_sca;
		if(nb_sca==0) return lecture_success();
		else etat=SCA ; 
		break;
	case SCA:
		data >> x >> y >> age >> rayon >> statut_sca;
		if(statut_sca == 1) data >> corail_id_cible;
		//choix arbitraire pour distinguer les scavengers qui n'ont pas de cible
		else corail_id_cible = -1; 
		if(lecture_scavenger(x, y, age, rayon, statut_sca, 
			corail_id_cible) == false) return false;
		nb_sca-=1;
		if(nb_sca==0) return lecture_success();
		break;
	}
	return 1;
}

bool Simulation::lecture_algue(double x, double y, int age){
	Algue algue(x, y, age);
	if(algue.get_valide() == false) return false;
	liste_algues.push_back(algue);
	return true;
}

bool Simulation::lecture_corail(double x, double y, double age, int id, 
	bool statut_cor, bool dir_rot, bool statut_dev, int nb_segments){
	Corail corail(x, y, age, id, statut_cor, dir_rot, statut_dev);
	if(corail.get_valide() == false) return false;
	if(duplicated_id(id) == true) return false;
	liste_coraux.push_back(corail);
	return true;
}

bool Simulation::lecture_segment(double x, double y, double angle, double longueur, 
	int compteur_segment, int id){
	S2d base_segment;
	if(compteur_segment == 0) base_segment = {x,y};
	else base_segment = ((((liste_coraux.back()).get_segment()).back()).point_final());
	Segment segment(base_segment, angle, longueur);
	if(segment_valide(id, segment) == false) return false;
	if(segment_valide_lecture(segment,id,compteur_segment) == false) return false;
	(liste_coraux.back()).add_segment(segment);
	return true;
}

bool Simulation :: lecture_scavenger(double x, double y, int age, double rayon, 
	bool statut_sca, int corail_id_cible){
	Scavenger scavenger(x,y,age,rayon,statut_sca,corail_id_cible);
	if(scavenger.get_valide() == false) return false;
	if(invalid_id(statut_sca,corail_id_cible) == true) return false;
	liste_scavengers.push_back(scavenger);
	return true;
}

//test de validité des entités

bool Simulation :: duplicated_id(int id){
	for(size_t i(0); i<liste_coraux.size(); ++i){
		if(id==(liste_coraux[i]).get_id()){
			cout << message :: lifeform_duplicated_id(id);
			return true;
		}
	}
	return false;
}

bool Simulation::segment_valide_lecture(Segment segment, int id, int compteur_segment){
	if(bout_hors_cadre(segment)){
		cout << message::lifeform_computed_outside(id, (segment.point_final()).x, 
		(segment.point_final()).y); 
		return false;
	}
	if(compteur_segment != 0){
		Segment segment_precedent(((liste_coraux.back()).get_segment()).back());
		if(segment.superpos_pt_commun(segment_precedent) == true){
			cout << message::segment_superposition(id, compteur_segment-1, 
				compteur_segment);
			return false;
		}
	}
	for(size_t i(0); i < liste_coraux.size(); ++i){
		vector<Segment> liste_segments((liste_coraux[i]).get_segment());
		for(size_t j(0); j < liste_segments.size(); ++j){
			if(doIntersect(segment, liste_segments[j], 0) and 
				(j!=liste_segments.size()-1 or i!=liste_coraux.size()-1)){
				cout << message::segment_collision((liste_coraux[i]).get_id(), j, 
				id, compteur_segment);
				return false;
			}
		}
	}
	return true;
}

bool Simulation::invalid_id(bool statut_sca,int corail_id_cible){
	if(statut_sca==1){
		bool existence_corail(false);
		for(size_t i(0); i<liste_coraux.size(); ++i){
			if((liste_coraux[i]).get_id()==corail_id_cible){
				existence_corail=true;
				liste_coraux[i].set_sca_attribue(true);
			}
		}
		if(existence_corail==false){
			cout << message::lifeform_invalid_id(corail_id_cible);
			return true;
		}
	}
	return false;
}

bool Simulation :: lecture_success(){
	cout << message :: success();
	return true;
}

bool Simulation::segment_valide_simu(Corail& corail, size_t indice, double angle, 
	bool algue_mangee){
	Segment segment((corail.get_segment()).back());
	if(bout_hors_cadre(segment, epsil_zero)){
		corail.annule_rot(angle);
		corail.change_dir_rot();
		if(algue_mangee) corail.raccourcissement();
		return false;
	}
	for(size_t i(0); i < liste_coraux.size(); ++i){
		vector<Segment> liste_segments((liste_coraux[i]).get_segment());
		double nb_segments(liste_segments.size());
		for(size_t j(0); j < liste_segments.size(); ++j){
			if(doIntersect(segment, liste_segments[j], epsil_zero) and i!=indice) {
				corail.annule_rot(angle);
				corail.change_dir_rot();
				if(algue_mangee) corail.raccourcissement();
				return false;
			}
			else if((doIntersect(segment, liste_segments[j], epsil_zero) and i==indice) and j<(nb_segments-2)){
				corail.annule_rot(angle);
				corail.change_dir_rot();
				if(algue_mangee) corail.raccourcissement();
				return false;
			}
		}
	}
	return true;
}

void Simulation::algue_plus_proche(Segment &segment, double &angle_ecart_algue, 
	int &indice_algue, bool sens_rotation){
	double angle_ecart_algue_min(delta_rot+1);
	if(liste_algues.size()>0){
		for(size_t i(0); i<liste_algues.size(); ++i){
			//calcul de l'angle entre l'algue et le segment
			S2d pos_algue(liste_algues[i].get_position());
			S2d d_algue_base;
			d_algue_base.x = pos_algue.x-(segment.get_base()).x;
			d_algue_base.y = pos_algue.y-(segment.get_base()).y;
			double distance_algue_base(sqrt(pow(d_algue_base.x,2)+
				pow(d_algue_base.y,2)));
			if(distance_algue_base <= segment.get_longueur()){
				double angle_algue(atan2(d_algue_base.y, d_algue_base.x));
				double angle_ecart(segment.get_angle()-angle_algue);
				if(angle_ecart<=delta_rot and angle_ecart>=-delta_rot){
					if((angle_ecart>0 and sens_rotation==true)or
						(angle_ecart<0 and sens_rotation==false)){
						if(abs(angle_ecart) < (angle_ecart_algue_min)){
							angle_ecart_algue_min = abs(angle_ecart);
							indice_algue = i;
						}
					}
				}
			}
		}
	}
	angle_ecart_algue = angle_ecart_algue_min;
}

void Simulation::rotation_cor(Corail& corail, size_t indice){
	int indice_algue(-1);
	double angle_ecart_algue(0);
	Segment segment((corail.get_segment()).back());
	algue_plus_proche(segment, angle_ecart_algue, indice_algue, corail.get_dir_rot());
	if(indice_algue!=-1 and corail.get_statut_cor() == true and 
		corail.rotation_avec_algue()){
		corail.corail_rot(abs(angle_ecart_algue));
		corail.allongement();
		segment = (corail.get_segment()).back();
		if(segment_valide_simu(corail, indice, abs(angle_ecart_algue), 
			true)){
			swap(liste_algues[indice_algue], liste_algues[0]);
			liste_algues.erase(liste_algues.begin());
			if(segment.get_longueur()>=l_repro) corail_croissance(corail);
		}
	}
	else{
		if(corail.rotation_sans_algue()==true){
			segment_valide_simu(corail, indice, delta_rot, false);
		}
	}
}

void Simulation::corail_croissance(Corail& corail){
	if(corail.get_statut_dev()==false) corail.corail_extend();
	else{
		Segment segment_rot((corail.get_segment()).back());
		Segment nv_corail(segment_rot.get_base(), segment_rot.get_angle(), 
			l_seg_interne);
		S2d base_corail(nv_corail.point_final());
		int id(1);
		bool id_occupe(false);
		do{
			id_occupe=false;
			for(auto corail:liste_coraux){
				if(corail.get_id()==id){
					id_occupe=true;
				}
			}
			if(id_occupe==true) id+=1;
		}while(id_occupe==true);
		Corail corail_cree(base_corail.x, base_corail.y, 1,id, true, 
			false, false);
		Segment nv_segment(base_corail, segment_rot.get_angle(),l_repro-l_seg_interne);
		corail_cree.add_segment(nv_segment);
		corail.change_statut_dev();
		corail.corail_repro();
		liste_coraux.push_back(corail_cree);
	}
}

int Simulation::cor_plus_proche_sca(Scavenger sca, double& distance_sca){
	double distance_min(363);
	int place_corail(0);
	for(size_t i(0); i<liste_coraux.size(); ++i){
		if(liste_coraux[i].get_statut_cor()==false and 
			liste_coraux[i].get_sca_attribue()==false){
			Segment dernier_seg_cor((liste_coraux[i].get_segment()).back());
			S2d point_fin(dernier_seg_cor.point_final());
			S2d pos_sca(sca.get_position());
			double distance_sca_cor(sqrt(pow((point_fin.x-pos_sca.x),2)
				+pow((point_fin.y-pos_sca.y),2)));
			if(distance_sca_cor<distance_min){
				distance_min=distance_sca_cor;
				place_corail=i;
			}
		}
	}
	distance_sca=distance_min;
	return place_corail;
}

void Simulation::attribution_sca(size_t place_corail){
	double distance_sca(0);
	double distance_min(363);
	int place_sca(-1);
	for(size_t i(0); i<liste_scavengers.size(); ++i){
		if(liste_scavengers[i].get_statut_sca()==false){
			size_t cor_proche(cor_plus_proche_sca(liste_scavengers[i], distance_sca));
			if(cor_proche==place_corail){
				if(distance_sca<distance_min){
					distance_min=distance_sca;
					place_sca=i;
				}
			}
		}
	}
	if(place_sca>=0){
		liste_scavengers[place_sca].set_statut_sca(true);
		liste_scavengers[place_sca].set_corail_id_cible(liste_coraux[place_corail].
			get_id());
		liste_coraux[place_corail].set_sca_attribue(true);
	}
}

void Simulation::mouvement_sca(Scavenger& sca){
	size_t place_corail;
	for(size_t i(0); i<liste_coraux.size(); ++i){
		if(liste_coraux[i].get_id()==sca.get_corail_id_cible()){
			place_corail=i;
		}
	}
	if(sca.get_corail_atteint()==false){
		vers_effecteur(sca, place_corail);
	}
	else{
		sur_corail(sca, place_corail);
	}
}

void Simulation::vers_effecteur(Scavenger& sca, size_t place_corail){
	Segment cible((liste_coraux[place_corail].get_segment()).back());
	S2d point_cible(cible.point_final());
	S2d base_sca(sca.get_position());
	double longueur_sca(sqrt(pow(point_cible.x-base_sca.x,2)
		+pow(point_cible.y-base_sca.y,2)));
	double angle_sca(atan2(point_cible.y-base_sca.y,point_cible.x-base_sca.x));
	Segment sca_corail(base_sca, angle_sca, longueur_sca);
	if(longueur_sca<=delta_l){
		sca.deplacement(sca_corail.point_final());
		sca.set_corail_atteint(true);
	}
	else{
		sca_corail.set_longueur(delta_l);
		sca.deplacement(sca_corail.point_final());
	}	
}

void Simulation::sur_corail(Scavenger &sca, size_t place_corail){
	Segment cible((liste_coraux[place_corail].get_segment()).back());
	Segment sca_corail(cible.point_final(), M_PI+cible.get_angle(), 
		cible.get_longueur());
	if(cible.get_longueur()<=delta_l){
		sca.deplacement(sca_corail.point_final());
		liste_coraux[place_corail].supr_dernier_seg();
		if(liste_coraux[place_corail].get_nb_segments()<1){
			liste_coraux.erase(liste_coraux.begin()+place_corail);
			sca.set_statut_sca(false);
			sca.set_corail_atteint(false);
		}
	}
	else{
		sca_corail.set_longueur(delta_l);
		liste_coraux[place_corail].raccourcissement();
		sca.deplacement(sca_corail.point_final());
	}
	if(sca.sca_extend()){
		create_sca(sca.get_position(), cible.get_angle());
	}
}

void Simulation::create_sca(S2d pos, double angle){
	Segment pos_nv_sca(pos,angle, delta_l);
	Scavenger nv_sca((pos_nv_sca.point_final()).x, 
		(pos_nv_sca.point_final()).y, 1, r_sca, false, -1);
	liste_scavengers.push_back(nv_sca);
}
		
			
