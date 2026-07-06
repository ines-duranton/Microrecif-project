//nom du fichier : projet.cc, autrices : Louise OUTTERS (381098) 50%
//et Ines DURANTON (378408) 50%, version n30

#include <iostream>
#include <cmath>
#include <string>
#include <gtkmm/application.h>
#include "simulation.h"
#include "gui.h"
using namespace std;

int main(int argc, char** argv){
	if(argc > 2) exit(EXIT_FAILURE);
	else{
		if (argc == 2){
			string filename(argv[1]);
			Simulation simulation(filename);
			simulation.lecture();
			auto app = Gtk::Application::create();
			return app->make_window_and_run<Fenetre>(1,argv, simulation);
		} 
		else{
			Simulation simulation;
			auto app = Gtk::Application::create();
			return app->make_window_and_run<Fenetre>(1,argv, simulation);
		}
	}
}

