#ifndef GUI_H
#define GUI_H

//~ #include "simulation.h"
//~ #include "graphic.h"

#include <gtkmm.h>

class Monde : public Gtk::DrawingArea
{
public:
	Monde();
	virtual ~Monde();
	//~ void setFrame(Frame x); 
	//~ void adjustFrame(int width, int height); 
protected:
  void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};


class Fenetre : public Gtk::Window
{
	public:
	 Fenetre();
	 virtual ~Fenetre();

	protected:
	 void on_button_clicked_exit();
	 void on_button_clicked_open();
	 void on_button_clicked_save();
	 void on_button_clicked_startstop();
	 void on_button_clicked_step();
	 bool on_timeout();
	 bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
	 void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);
	 
	 Monde monde;
	 
	 Gtk::Box m_Box_Top, m_Box_Inter, m_Box_Dessin, m_Box_Info, m_Box_Buttons;
	 
	 Gtk::Label m_Label_general, m_Label_info, m_Label_maj, m_Label_particules;
	 Gtk::Label m_Label_rrs, m_Label_rrr, m_Label_rns, m_Label_rnp, m_Label_rnd;
	 Gtk::Label m_Label_rnr;
	 
	 Gtk::Button m_Button_exit, m_Button_open, m_Button_save, m_Button_startstop;
	 Gtk::Button m_Button_step;
	 
	 bool timer_added; 
	 bool disconnect; 
	 const int timeout_value; 
};



#endif
