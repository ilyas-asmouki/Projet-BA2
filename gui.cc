#include "gui.h"
#include <iostream>
//~ #include "simulation.h"

Monde::Monde()
{
    set_content_width(500);
	set_content_height(500);
	
	set_draw_func(sigc::mem_fun(*this, &Monde::on_draw));
}

Monde::~Monde()
{
}

void Monde::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
}

Fenetre::Fenetre() : 
 m_Box_Top(Gtk::Orientation::HORIZONTAL,0), m_Box_Inter(Gtk::Orientation::VERTICAL,0),
 m_Box_Dessin(Gtk::Orientation::VERTICAL,0), m_Box_Info(Gtk::Orientation::VERTICAL,0),
 m_Box_Buttons(Gtk::Orientation::VERTICAL,0), m_Label_general("General"), 
 m_Label_info("Info : nombre de ..."), m_Label_maj("mises à jour:"),
 m_Label_particules("particules:"), m_Label_rrs("robots réparateurs en service:"),
 m_Label_rrr("robots réparateurs en réserve:"), 
 m_Label_rns("robots neutraliseurs en service:"),
 m_Label_rnp("robots neutraliseurs en panne:"),
 m_Label_rnd("robots neutraliseurs détruits:"),
 m_Label_rnr("robots neutraliseurs en réserve:"), m_Button_exit("exit"),
 m_Button_open("open"), m_Button_save("save"), m_Button_startstop("start"),
 m_Button_step("step"), timer_added(false), disconnect(false), timeout_value(1000) 
{
	set_default_size(600, 600);
	set_title("Propre en ordre");
	set_child(m_Box_Top);
	m_Box_Top.append(m_Box_Inter);
	m_Box_Top.append(m_Box_Dessin);
	m_Box_Inter.append(m_Box_Buttons);
	m_Box_Inter.append(m_Box_Info);
	m_Box_Dessin.append(monde);
	m_Box_Buttons.append(m_Label_general);
	m_Box_Buttons.append(m_Button_exit);
	m_Box_Buttons.append(m_Button_open);
	m_Box_Buttons.append(m_Button_save);
	m_Box_Buttons.append(m_Button_startstop);
	m_Box_Buttons.append(m_Button_step);
	m_Box_Info.append(m_Label_info);
	m_Box_Info.append(m_Label_maj);
	m_Box_Info.append(m_Label_particules);
	m_Box_Info.append(m_Label_rrs);
	m_Box_Info.append(m_Label_rrr);
	m_Box_Info.append(m_Label_rns);
	m_Box_Info.append(m_Label_rnp);
	m_Box_Info.append(m_Label_rnd);
	m_Box_Info.append(m_Label_rnr);
	
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

Fenetre::~Fenetre()
{
}

void Fenetre::on_button_clicked_exit()
{
	hide();
}

void Fenetre::on_button_clicked_open()
{
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

void Fenetre::on_button_clicked_save()
{
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

void Fenetre::on_button_clicked_startstop()
{
	if(not timer_added)
	{	  
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		                                        &Fenetre::on_timeout));
		auto conn = Glib::signal_timeout().connect(my_slot,timeout_value);
			
		timer_added = true;
		m_Button_startstop.set_label("stop");
	}
	else
	{
	    disconnect  = true;   
		timer_added = false;
		m_Button_startstop.set_label("start");
	}
}

bool Fenetre::on_timeout()
{
	static unsigned int val(1);
	
	if(disconnect)
	{
		disconnect = false;
		return false;
	}
	
	//~ data_label.set_text(std::to_string(val)); 
	std::cout << "This is simulation update number : " << val << std::endl;

	++val;
	return true; 
}

void Fenetre::on_button_clicked_step()
{
}

bool Fenetre::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
	switch(gdk_keyval_to_unicode(keyval))
	{
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
										    Gtk::FileChooserDialog* dialog)
{
	switch (response_id)
	{
		case Gtk::ResponseType::OK:
		{
		    auto filename = dialog->get_file()->get_path();
		    break;
		}
		case Gtk::ResponseType::CANCEL:
		{
		    break;
		}
		default:
		{
		    break;
		}
	}
	delete dialog;
}
