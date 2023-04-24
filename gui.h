#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>
#include <fstream>
#include "simulation.h"

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
enum Dialog {OPEN, SAVE};

struct Frame 
{
	double xMin; 
	double xMax;
	double yMin;
	double yMax;
	double asp;  
	int width;  
	int height; 
};

class Monde : public Gtk::DrawingArea
{
public:
	Monde();
	virtual ~Monde();
	void setFrame(Frame x); 
	void adjustFrame(int width, int height); 
	void clear();
	void draw();
protected:
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
  
private:
	Frame frame;
	bool empty;
};

class Fenetre : public Gtk::Window
{
	public:
	 Fenetre(char* file, int argc);
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
	 void set_data();
	 void reset_data();
	 void set_interface();
 
	 Monde monde;
	 Simulation* Propre_en_Ordre;
	 Gtk::Box m_Box_All, m_Box_Left, m_Box_Right, m_Box_maj, m_Box_prt, m_Box_rrs,
	          m_Box_rrr, m_Box_rns, m_Box_rnp, m_Box_rnd, m_Box_rnr;
	 
	 Gtk::Label m_Label_general, m_Label_info, m_Label_maj, m_Label_prt;
	 Gtk::Label m_Label_rrs, m_Label_rrr, m_Label_rns, m_Label_rnp, m_Label_rnd;
	 Gtk::Label m_Label_rnr, maj_data, prt_data, rrs_data, rrr_data, rns_data, 
	            rnp_data, rnd_data, rnr_data; 
	 
	 Gtk::Button m_Button_exit, m_Button_open, m_Button_save, m_Button_startstop;
	 Gtk::Button m_Button_step;
	 
	 bool timer_added; 
	 bool disconnect; 
	 const int timeout_value;
	 Dialog dialogue; 
};

#endif
