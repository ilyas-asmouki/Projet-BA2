#include "gui.h"
#include <iostream>
#include "constantes.h"
#include "graphic.h"
#include <fstream>

constexpr unsigned taille_dessin = 500;
constexpr unsigned aspect_ratio = 1.0;
static std::ifstream empty;

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									const Frame& frame);
static Frame default_frame = {-dmax, dmax, -dmax, dmax, aspect_ratio, taille_dessin,
							  taille_dessin}; 

Monde::Monde() : empty(false) {
    set_content_width(default_frame.width);
	set_content_height(default_frame.height);
	set_draw_func(sigc::mem_fun(*this, &Monde::on_draw));
}

Monde::~Monde() {
}

void Monde::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
	set_context(cr);
	adjustFrame(width,height);
	orthographic_projection(cr, frame); 
	set_world();
	if (not empty) {
		draw_world();
	}
}

void Monde::clear() {
	empty = true;
	queue_draw();
}

void Monde::draw() {
	empty = false;
	queue_draw();
}

Fenetre::Fenetre(char* file, int argc) : 
 m_Box_All(Gtk::Orientation::HORIZONTAL,10), m_Box_Left(Gtk::Orientation::VERTICAL,3),
 m_Box_Right(Gtk::Orientation::VERTICAL,0),m_Box_maj(Gtk::Orientation::HORIZONTAL,200),
 m_Box_prt(Gtk::Orientation::HORIZONTAL,213), 
 m_Box_rrs(Gtk::Orientation::HORIZONTAL,82),m_Box_rrr(Gtk::Orientation::HORIZONTAL,79),
 m_Box_rns(Gtk::Orientation::HORIZONTAL,74),m_Box_rnp(Gtk::Orientation::HORIZONTAL,80),
 m_Box_rnd(Gtk::Orientation::HORIZONTAL,86),m_Box_rnr(Gtk::Orientation::HORIZONTAL,71),
 m_Label_general("General"), m_Label_info("Info : nombre de ..."), 
 m_Label_maj("mises à jour:"), m_Label_prt("particules:"), 
 m_Label_rrs("robots réparateurs en service:"), 
 m_Label_rrr("robots réparateurs en réserve:"), 
 m_Label_rns("robots neutraliseurs en service:"),
 m_Label_rnp("robots neutraliseurs en panne:"),
 m_Label_rnd("robots neutraliseurs détruits:"),
 m_Label_rnr("robots neutraliseurs en réserve:"),
 m_Button_exit("exit"), m_Button_open("open"), m_Button_save("save"), 
 m_Button_startstop("start"), m_Button_step("step"), timer_added(false), 
 disconnect(false), timeout_value(20), dialogue(OPEN)
{
	reset_data();
	if (argc == 1) {
		Propre_en_Ordre = new Simulation(empty);
	}
	else {
		Propre_en_Ordre = new Simulation(file);
		set_data();
		if (!(Propre_en_Ordre->getfile_success())) {
			monde.clear();
			Propre_en_Ordre->destroy_data();
		}
	}
	set_resizable(true);
	set_default_size(600, 500);
	set_title("Propre en ordre");
	set_child(m_Box_All);
	set_interface();	
	
	m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this, 
										   &Fenetre::on_button_clicked_exit));
	m_Button_open.signal_clicked().connect(sigc::mem_fun(*this, 
										   &Fenetre::on_button_clicked_open));
	m_Button_startstop.signal_clicked().connect(sigc::mem_fun(*this, 
											   &Fenetre::on_button_clicked_startstop));
    m_Button_save.signal_clicked().connect(sigc::mem_fun(*this, 
										   &Fenetre::on_button_clicked_save));
	m_Button_step.signal_clicked().connect(sigc::mem_fun(*this, 
										   &Fenetre::on_button_clicked_step));
	auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
                  sigc::mem_fun(*this, &Fenetre::on_window_key_pressed), false);
    add_controller(controller);
}

Fenetre::~Fenetre() {
}

void Fenetre::set_data() {
	if (Propre_en_Ordre->getfile_success()) {
		maj_data.set_text(std::to_string(Propre_en_Ordre->s_getnbUpdate()));
		prt_data.set_text(std::to_string(Propre_en_Ordre->p_getnbP()));
		rrs_data.set_text(std::to_string(Propre_en_Ordre->s_getnbRs()));
		rrr_data.set_text(std::to_string(Propre_en_Ordre->s_getnbRr()));
		rns_data.set_text(std::to_string(Propre_en_Ordre->s_getnbNs()));
		rnp_data.set_text(std::to_string(Propre_en_Ordre->s_getnbNp()));
		rnd_data.set_text(std::to_string(Propre_en_Ordre->s_getnbNd()));
		rnr_data.set_text(std::to_string(Propre_en_Ordre->s_getnbNr()));
	}
}

void Fenetre::reset_data() {
	maj_data.set_text("0");
	prt_data.set_text("0");
	rrs_data.set_text("0");
	rrr_data.set_text("0");
	rns_data.set_text("0");
	rnp_data.set_text("0");
	rnd_data.set_text("0");
	rnr_data.set_text("0");
}

void Fenetre::set_interface() {
	m_Box_All.append(m_Box_Left);
	m_Box_All.append(m_Box_Right);
	m_Box_Right.append(monde);
	monde.set_expand();
	m_Box_Left.append(m_Label_general);
	m_Box_Left.append(m_Button_exit);
	m_Box_Left.append(m_Button_open);
	m_Box_Left.append(m_Button_save);
	m_Box_Left.append(m_Button_startstop);
	m_Box_Left.append(m_Button_step);
	m_Box_Left.append(m_Label_info);
	m_Box_Left.append(m_Box_maj);
	m_Box_Left.append(m_Box_prt);
	m_Box_Left.append(m_Box_rrs);
	m_Box_Left.append(m_Box_rrr);
	m_Box_Left.append(m_Box_rns);
	m_Box_Left.append(m_Box_rnp);
	m_Box_Left.append(m_Box_rnd);
	m_Box_Left.append(m_Box_rnr);
	m_Box_maj.append(m_Label_maj);
	m_Box_maj.append(maj_data);
	m_Box_prt.append(m_Label_prt);
	m_Box_prt.append(prt_data);
	m_Box_rrs.append(m_Label_rrs);
	m_Box_rrs.append(rrs_data);
	m_Box_rrr.append(m_Label_rrr);
	m_Box_rrr.append(rrr_data);
	m_Box_rns.append(m_Label_rns);
	m_Box_rns.append(rns_data);
	m_Box_rnp.append(m_Label_rnp);
	m_Box_rnp.append(rnp_data);
	m_Box_rnd.append(m_Label_rnd);
	m_Box_rnd.append(rnd_data);
	m_Box_rnr.append(m_Label_rnr);
	m_Box_rnr.append(rnr_data);
}

void Fenetre::on_button_clicked_exit() {
	hide();
}

void Fenetre::on_button_clicked_open() {
	if (timer_added) {
		disconnect = true;
		timer_added = false;
		m_Button_startstop.set_label("start");
	}
	
	dialogue = OPEN;	
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
	                                         Gtk::FileChooser::Action::OPEN);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
	sigc::mem_fun(*this, &Fenetre::on_file_dialog_response), dialog));
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Open", Gtk::ResponseType::OK);
	dialog->show();
}

void Fenetre::on_button_clicked_save() {
	if (timer_added) {
		disconnect = true;
		timer_added = false;
		m_Button_startstop.set_label("start");
	}
	
	dialogue = SAVE;
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
	                                         Gtk::FileChooser::Action::SAVE);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
	sigc::mem_fun(*this, &Fenetre::on_file_dialog_response), dialog));
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Save", Gtk::ResponseType::OK);
	dialog->show();
}

void Fenetre::on_button_clicked_startstop() {
	if (Propre_en_Ordre->getfile_success()) {
		if(not timer_added) {	  
			sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
													&Fenetre::on_timeout));
			auto conn = Glib::signal_timeout().connect(my_slot,timeout_value);
			
			timer_added = true;
			m_Button_startstop.set_label("stop");
		} else {
			disconnect  = true;   
			timer_added = false;
			m_Button_startstop.set_label("start");
		}
	}
}

bool Fenetre::on_timeout() {
	if (Propre_en_Ordre->getfile_success()) {
		unsigned val(Propre_en_Ordre->s_getnbUpdate());
		if(disconnect) {
			disconnect = false;
			return false;
		}
		maj_data.set_text(std::to_string(val)); 
		Propre_en_Ordre->s_setnbUpdate(++val);
		Propre_en_Ordre->desintegration();
		monde.clear();
		monde.draw();
		set_data();
	}
	return true; 
}

void Fenetre::on_button_clicked_step() {
	if (!timer_added) {
		on_timeout();
	}
}

bool Fenetre::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state) {
	switch(gdk_keyval_to_unicode(keyval)) {
	case 's':
		on_button_clicked_startstop();
		return true;
	case '1':
		on_button_clicked_step();
		return true;
	}
	return false;
}
			
void Fenetre::on_file_dialog_response(int response_id, 
										    Gtk::FileChooserDialog* dialog) {
	switch (response_id) {
	case Gtk::ResponseType::OK: {
		auto filename = dialog->get_file()->get_path();
		if (dialogue) {
			Propre_en_Ordre->sauvegarde(filename);
		} else {
			std::ifstream fichier(filename);
			Propre_en_Ordre->destroy_data();
			monde.clear();
			delete Propre_en_Ordre;
			Propre_en_Ordre = new Simulation(fichier);
			reset_data();
			if (Propre_en_Ordre->getfile_success()) {
				set_data();
				monde.draw();
			}
		}
		break;
	}
	case Gtk::ResponseType::CANCEL:
		break;
		
	default:
		break;
	}
	dialog->hide();
}

void Monde::adjustFrame(int width, int height) {	
	frame.width  = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if (new_aspect_ratio > default_frame.asp) {
	    frame.yMax = default_frame.yMax;
	    frame.yMin = default_frame.yMin;	
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta;		  	  
    } else {
	    frame.xMax = default_frame.xMax;
	    frame.xMin = default_frame.xMin;
	  	  
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta;		  	  
    }
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									const Frame& frame) {
	cr->translate(frame.width/2, frame.height/2);
	cr->scale(frame.width/(frame.xMax - frame.xMin),
	-frame.height/(frame.yMax - frame.yMin));
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}
