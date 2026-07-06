//nom du fichier : gui.h, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>

struct Frame{
	double xMin; //paramètres du cadre
	double xMax;
	double yMin;
	double yMax;
	double asp;  //ratio largeur/longueur
	int width;   //largeur fenetre
	int height;  //hauteur fenetre
};

class MyArea : public Gtk::DrawingArea{
public :
	MyArea(Simulation& s);
	virtual ~MyArea();
	void setFrame(Frame x); 
	void adjustFrame(int width, int height); 
	void set_simulation(Simulation& s);
protected :
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	Simulation simulation;
private:
	Frame frame;
};

class Fenetre : public Gtk::Window{
public :
	Fenetre(Simulation& s);
protected :
	MyArea dessin;
	Simulation simulation;
	void set_box();
	void set_simulation(Simulation& s);
	Gtk::Box m_Main_Box, m_Control_Box, m_Buttons_Box;
	Gtk::Label m_Label_Buttons;
	Gtk::Box m_Info_Box;
	Gtk::Label m_Label_Info;
	//boutons
	Gtk::Button Button_Exit, Button_Open, Button_Save, Button_Start, Button_Step;
	Gtk::CheckButton Crea_Algues;
	//affichage nombres
	Gtk::Label m_Label_Maj;
	Gtk::Label m_Label_Algues;
	Gtk::Label m_Label_Coraux;
	Gtk::Label m_Label_Sca;
	Gtk::Box m_Box_Maj, m_Box_Algues, m_Box_Coraux, m_Box_Sca;
	Gtk::Label nombre_maj;
	unsigned int compteur;
	Gtk::Label nombre_algues;
	Gtk::Label nombre_coraux;
	Gtk::Label nombre_sca;
	void affichage_info();
	//gestion boutons
	void on_button_exit();
	void on_button_open();
	void on_button_save();
	void on_button_open_save(bool open);
	void on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog* dialog);
	void on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog);
	void on_button_start();
	void on_button_step();
	void step();
	bool on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state);
	//timer
	bool on_timeout();
	bool timer_added;
	bool disconnect;
	const int timeout_value;
};

#endif
