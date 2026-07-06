//nom du fichier : gui.cc, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#include <iostream>
#include <cairomm/context.h>
#include "simulation.h"
#include "gui.h"
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

constexpr unsigned taille_dessin(500);
static Frame default_frame = {0., dmax, 0., dmax, 1, taille_dessin, taille_dessin}; 

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
	const Frame& frame);
	
static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
	const Frame& frame){
	// déplace l'origine au centre de la fenêtre
	cr->translate(frame.width/2, frame.height/2);
	
	// normalise la largeur et hauteur aux valeurs fournies par le cadrage
	// ET inverse la direction de l'axe Y
	cr->scale(frame.width/(frame.xMax - frame.xMin),
		-frame.height/(frame.yMax - frame.yMin));
  
	// décalage au centre du cadrage
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

MyArea::MyArea(Simulation& s) : simulation(s){
	setFrame(default_frame);
	set_content_width(default_frame.width);
	set_content_height(default_frame.height);
	set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
	set_expand(true);
}

MyArea::~MyArea(){}

void MyArea::setFrame(Frame f){
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0)){
		f.asp = f.width/f.height;
		frame = f;
	}
	else{
		std::cout << "incorrect Model framing or window parameters" << std::endl;
	}
} 

void MyArea::adjustFrame(int width, int height){	
	frame.width  = width;
	frame.height = height;

    //Utilise le cadre de référence comme base pour observer une distorsion
	double new_aspect_ratio((double)width/height);
	if(new_aspect_ratio > default_frame.asp){
		frame.yMax = default_frame.yMax ;
		frame.yMin = default_frame.yMin ;
		double delta(default_frame.xMax - default_frame.xMin);
		double mid((default_frame.xMax + default_frame.xMin)/2);
        //Le nouveau cadre est centré sur le milieu de l'axe x
		frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
		frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
	}
	else{
		frame.xMax = default_frame.xMax ;
		frame.xMin = default_frame.xMin ;
		double delta(default_frame.yMax - default_frame.yMin);
		double mid((default_frame.yMax + default_frame.yMin)/2);
        //Le nouveau cadre est centré sur le milieu de l'axe y
		frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
		frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ; 
	}
}

void MyArea::set_simulation(Simulation& s){
	simulation = s;
}

void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height){
	adjustFrame(width,height);
	orthographic_projection(cr, frame);
	graphic_set_context(cr);
	graphic_draw_square(dmax/2,dmax/2,dmax,GREY);
	simulation.affiche();
}

Fenetre::Fenetre (Simulation& s) : dessin(s), simulation(s),
	m_Main_Box(Gtk::Orientation::HORIZONTAL, 0), 
	m_Control_Box(Gtk::Orientation::VERTICAL, 2), 
	m_Buttons_Box(Gtk::Orientation::VERTICAL, 2), m_Label_Buttons("General"), 
	m_Info_Box(Gtk::Orientation::VERTICAL, 2), m_Label_Info("Infos : nombre de "), 
	Button_Exit("Exit"), Button_Open("Open"), Button_Save("Save"), 
	Button_Start("Start"), Button_Step("Step"), Crea_Algues("Naissance d'algues"), 
	m_Box_Maj(Gtk::Orientation::HORIZONTAL, 0),
	m_Box_Algues(Gtk::Orientation::HORIZONTAL, 0),
	m_Box_Coraux(Gtk::Orientation::HORIZONTAL, 0),
	m_Box_Sca(Gtk::Orientation::HORIZONTAL, 0), compteur(0), timer_added(false), 
	disconnect(false), timeout_value(25){
		
	set_title("Microrécif");
	set_child(m_Main_Box);
	set_box();
	affichage_info();
	Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
		&Fenetre::on_button_exit));
	Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
		&Fenetre::on_button_open) );
	Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
		&Fenetre::on_button_save) );
	Button_Start.signal_clicked().connect(sigc::mem_fun(*this,
		&Fenetre::on_button_start));
	Button_Step.signal_clicked().connect(sigc::mem_fun(*this,
		&Fenetre::on_button_step));
	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &Fenetre::on_window_key_pressed), false);
	add_controller(controller);
}

void Fenetre::set_box(){
	m_Main_Box.append(m_Control_Box);
	m_Main_Box.append(dessin);
	m_Control_Box.append(m_Buttons_Box);
	m_Control_Box.append(m_Info_Box);
	m_Buttons_Box.append(m_Label_Buttons);
	m_Buttons_Box.append(Button_Exit);
	m_Buttons_Box.append(Button_Open);
	m_Buttons_Box.append(Button_Save);
	m_Buttons_Box.append(Button_Start);
	m_Buttons_Box.append(Button_Step);
	m_Buttons_Box.append(Crea_Algues);
	m_Buttons_Box.append(m_Label_Info);
	m_Info_Box.append(m_Box_Maj);
	m_Info_Box.append(m_Box_Algues);
	m_Info_Box.append(m_Box_Coraux);
	m_Info_Box.append(m_Box_Sca);
	m_Box_Maj.append(m_Label_Maj);
	m_Box_Algues.append(m_Label_Algues);
	m_Box_Coraux.append(m_Label_Coraux);
	m_Box_Sca.append(m_Label_Sca);
	m_Label_Maj.set_text("Mises à jour : ");
	m_Label_Algues.set_text("Algues : ");
	m_Label_Coraux.set_text("Coraux : ");
	m_Label_Sca.set_text("Charognards : ");
	m_Box_Maj.append(nombre_maj);
	m_Box_Algues.append(nombre_algues);
	m_Box_Coraux.append(nombre_coraux);
	m_Box_Sca.append(nombre_sca);
}

void Fenetre::set_simulation(Simulation& s){
	simulation = s;
}

void Fenetre::affichage_info(){
	nombre_maj.set_text(to_string(compteur));
	nombre_algues.set_text(to_string(simulation.get_nb_algues()));
	nombre_coraux.set_text(to_string(simulation.get_nb_coraux()));
	nombre_sca.set_text(to_string(simulation.get_nb_sca()));
}

void Fenetre::on_button_exit(){
	exit(EXIT_SUCCESS);
}

void Fenetre::on_button_open(){
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
	Gtk::FileChooser::Action::OPEN);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
		sigc::mem_fun(*this, &Fenetre::on_file_dialog_response_open), dialog));

	//Ajoute les boutons de réponse
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Open", Gtk::ResponseType::OK);
	//Ajoute les filtres pour sélectionner certains types de fichier
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	auto filter_cpp = Gtk::FileFilter::create();
	filter_cpp->set_name("C/C++ files");
	filter_cpp->add_mime_type("text/x-c");
	filter_cpp->add_mime_type("text/x-c++");
	filter_cpp->add_mime_type("text/x-c-header");
	dialog->add_filter(filter_cpp);
	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog->add_filter(filter_any);
	dialog->show();
} 

void Fenetre::on_button_save(){
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
	Gtk::FileChooser::Action::SAVE);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
		sigc::mem_fun(*this, &Fenetre::on_file_dialog_response_save), dialog));

	//Ajoute les boutons de réponse
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Save", Gtk::ResponseType::OK);
	//Ajoute les filtres pour sélectionner certains types de fichier
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	auto filter_cpp = Gtk::FileFilter::create();
	filter_cpp->set_name("C/C++ files");
	filter_cpp->add_mime_type("text/x-c");
	filter_cpp->add_mime_type("text/x-c++");
	filter_cpp->add_mime_type("text/x-c-header");
	dialog->add_filter(filter_cpp);
	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog->add_filter(filter_any);
	dialog->show();
}

void Fenetre::on_file_dialog_response_open(int response_id,
	Gtk::FileChooserDialog* dialog){
	switch (response_id){
		case Gtk::ResponseType::OK:{
			auto filename = dialog->get_file()->get_path();
			Simulation nv_simulation(filename);
			nv_simulation.lecture();
			set_simulation(nv_simulation);
			compteur = 0;
			affichage_info();
			dessin.set_simulation(nv_simulation);
			dessin.queue_draw();
			break;
		}
		case Gtk::ResponseType::CANCEL:{
			break;
		}
		default:{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
	delete dialog;
}

void Fenetre::on_file_dialog_response_save(int response_id,
	Gtk::FileChooserDialog* dialog){
	switch (response_id){
		case Gtk::ResponseType::OK:{
			auto filename = dialog->get_file()->get_path();
			simulation.ecriture(filename);
			break;
		}
		case Gtk::ResponseType::CANCEL:{
			break;
		}
		default:{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
	delete dialog;
}

void Fenetre::on_button_start(){
	if(not timer_added){
		Button_Start.set_label("Stop");
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
			&Fenetre::on_timeout));
		auto conn = Glib::signal_timeout().connect(my_slot,timeout_value);
		timer_added = true;
	}
	else{
		Button_Start.set_label("Start");
		disconnect = true;
		timer_added = false;
	}
}

void Fenetre::on_button_step(){
	step();
}

void Fenetre::step(){
	++compteur;
	simulation.execution(Crea_Algues.get_active());
	dessin.set_simulation(simulation);
	affichage_info();
	dessin.queue_draw(); 
}

bool Fenetre::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state){
	switch(gdk_keyval_to_unicode(keyval)){
		case 's':
			on_button_start();
			return true;
		case '1':
			on_button_step();
			return true;
	}
	return false;
}

bool Fenetre::on_timeout(){
	if(disconnect){
		disconnect = false;
		return false;
	}
	step();
	return true;
}
