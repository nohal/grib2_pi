///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GribDialog
///////////////////////////////////////////////////////////////////////////////
class GribDialog : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxStaticLine* m_staticline1;
		
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText2;
		
		wxStaticText* m_staticText31;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		
		wxStaticText* m_staticText311;
		wxStaticText* m_staticText211;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText81;
		wxStaticLine* m_staticline2;
		wxStaticLine* m_staticline5;
		
		wxStaticText* m_staticText34;
		wxStaticText* m_staticText24;
		
		wxStaticText* m_staticTextWaveHeight;
		wxStaticText* m_staticText37;
		
		wxStaticText* m_staticText32;
		wxStaticText* m_staticText33;
		wxStaticText* m_staticText331;
		wxStaticLine* m_staticline6;
		
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText332;
		
		wxStaticText* m_staticText3321;
		
		wxStaticText* m_staticText26;
		wxStaticLine* m_staticline7;
		
		wxStaticText* m_staticText333;
		wxStaticText* m_staticText341;
		
		
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxPanel* m_panel2;
		wxStaticText* m_staticText321;
		wxStaticText* m_staticText342;
		
		wxStaticText* m_staticText36;
		
		
		wxStaticLine* m_staticline8;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText212;
		wxStaticText* m_staticText22;
		wxStaticText* m_staticText23;
		
		
		wxStaticLine* m_staticline9;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onCloseDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnCharNoteBook1( wxKeyEvent& event ) { event.Skip(); }
		virtual void onPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void onDirChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFileButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onTreeItemRightClick( wxTreeEvent& event ) { event.Skip(); }
		virtual void onTreeSelectionChanged( wxTreeEvent& event ) { event.Skip(); }
		virtual void onDWindClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpForceClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpGustsClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDWaveClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpWaveHeightClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpSwellClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpCloudsClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDPressureClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpTemperatureClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDCurrentClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBmpCurrentForceClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonSelectionReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDownLoadOK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_textCtrlDir;
		wxBitmapButton* m_fileButton;
		wxTreeCtrl* m_treeCtrl;
		wxStaticText* m_staticTextDateTime;
		wxCheckBox* m_checkBoxDWind;
		wxTextCtrl* m_WindDir;
		wxCheckBox* m_checkBoxBmpForce;
		wxTextCtrl* m_textCtrlBft;
		wxTextCtrl* m_textCtrlWindKts;
		wxTextCtrl* m_textCtrlWindForce;
		wxCheckBox* m_checkBoxBmpGusts;
		wxTextCtrl* m_textCtrlGustBft;
		wxTextCtrl* m_textCtrlGustKts;
		wxTextCtrl* m_textCtrlGust;
		wxCheckBox* m_checkBoxDWave;
		wxTextCtrl* m_textCtrlWaveDir;
		wxCheckBox* m_checkBoxBmpWaveHeight;
		wxTextCtrl* m_textCtrlWaveHeight;
		wxCheckBox* m_checkBoxBmpSwell;
		wxTextCtrl* m_textCtrlSwMax;
		wxTextCtrl* m_textCtrlSwMin;
		wxCheckBox* m_checkBoxBmpClouds;
		wxTextCtrl* m_textCtrlClouds;
		wxCheckBox* m_checkBoxDPressure;
		wxStaticText* m_staticTextPressure;
		wxTextCtrl* m_textCtrlPressure;
		wxCheckBox* m_checkBoxBmpTemperature;
		wxTextCtrl* m_textCtrlTemperature;
		wxCheckBox* m_checkBoxDCurrent;
		wxTextCtrl* m_textCtrlCurrentDir;
		wxCheckBox* m_checkBoxBmpCurrentForce;
		wxTextCtrl* m_textCtrlCurrentForce;
		wxTextCtrl* m_textCtrlSelectionTopLat;
		wxTextCtrl* m_textCtrlSelectionTopLon;
		wxTextCtrl* m_textCtrlSelectionBottomLat;
		wxTextCtrl* m_textCtrlSelectionBottomLon;
		wxButton* m_buttonSelectionReset;
		wxChoice* m_choiceDays;
		wxChoice* m_choiceHours;
		wxCheckBox* m_checkBoxWind;
		wxCheckBox* m_checkBoxGusts;
		wxCheckBox* m_checkBoxWaves;
		wxCheckBox* m_checkBoxSwell;
		wxCheckBox* m_checkBoxPressure;
		wxCheckBox* m_checkBoxTemperature;
		wxCheckBox* m_checkBoxRain;
		wxCheckBox* m_checkBoxClouds;
		wxButton* m_buttonDownload;
		wxTextCtrl* m_textCtrlDownload;
		
		GribDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Grib2Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 330,630 ), long style =wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~GribDialog();
	
};

#endif //__gui__
