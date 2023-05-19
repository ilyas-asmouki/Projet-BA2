//NOM PRENOM SCIPER CONTRIBUTION(%)
//LAHLOU SAAD 361150 50%
//ASMOUKI ILYAS 356263 50% 

#include "simulation.h"
#include "gui.h"
#include "constantes.h"

int main(int argc, char* argv[]) {		
	auto app = Gtk::Application::create();
    return app->make_window_and_run<Fenetre>(1, argv, argv[1], argc);
}


