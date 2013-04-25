///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

#include "folder.xpm"

///////////////////////////////////////////////////////////////////////////

GribDialog::GribDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textCtrlDir = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textCtrlDir->SetMinSize( wxSize( 250,-1 ) );
	
	bSizer2->Add( m_textCtrlDir, 0, wxALL, 5 );
	
	m_fileButton = new wxBitmapButton( m_panel1, wxID_ANY, wxBitmap( folder ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer2->Add( m_fileButton, 0, wxALL, 5 );
	
	fgSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	m_treeCtrl = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxHSCROLL|wxSUNKEN_BORDER );
	m_treeCtrl->SetFont( wxFont( 9, 74, 90, 90, false, wxT("Sans") ) );
	m_treeCtrl->SetMinSize( wxSize( -1,150 ) );
	
	fgSizer1->Add( m_treeCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextDateTime = new wxStaticText( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDateTime->Wrap( -1 );
	fgSizer1->Add( m_staticTextDateTime, 1, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 5, 1, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxDWind = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxDWind->SetValue(true); 
	bSizer12->Add( m_checkBoxDWind, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	
	bSizer12->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_staticText3 = new wxStaticText( m_panel1, wxID_ANY, _("WindDir"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_staticText3->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer12->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_WindDir = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer12->Add( m_WindDir, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText2 = new wxStaticText( m_panel1, wxID_ANY, _("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer12->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 0 );
	
	fgSizer2->Add( bSizer12, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer13->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpForce = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_checkBoxBmpForce, 0, wxALL, 0 );
	
	m_staticText31 = new wxStaticText( m_panel1, wxID_ANY, _("Force"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText31->Wrap( -1 );
	m_staticText31->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer13->Add( m_staticText31, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlBft = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer13->Add( m_textCtrlBft, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText21 = new wxStaticText( m_panel1, wxID_ANY, _("Bft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer13->Add( m_staticText21, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_textCtrlWindKts = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer13->Add( m_textCtrlWindKts, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText7 = new wxStaticText( m_panel1, wxID_ANY, _("kts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer13->Add( m_staticText7, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_textCtrlWindForce = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer13->Add( m_textCtrlWindForce, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText8 = new wxStaticText( m_panel1, wxID_ANY, _("m/s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer13->Add( m_staticText8, 0, wxALIGN_CENTER|wxALL, 0 );
	
	fgSizer2->Add( bSizer13, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer211;
	bSizer211 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer211->SetMinSize( wxSize( 310,-1 ) ); 
	
	bSizer211->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpGusts = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer211->Add( m_checkBoxBmpGusts, 0, wxALL, 0 );
	
	m_staticText311 = new wxStaticText( m_panel1, wxID_ANY, _("Gusts"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText311->Wrap( -1 );
	m_staticText311->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer211->Add( m_staticText311, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlGustBft = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer211->Add( m_textCtrlGustBft, 0, wxALL, 0 );
	
	m_staticText211 = new wxStaticText( m_panel1, wxID_ANY, _("Bft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	bSizer211->Add( m_staticText211, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_textCtrlGustKts = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer211->Add( m_textCtrlGustKts, 0, wxALL, 0 );
	
	m_staticText71 = new wxStaticText( m_panel1, wxID_ANY, _("kts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer211->Add( m_staticText71, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_textCtrlGust = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer211->Add( m_textCtrlGust, 0, wxALL, 0 );
	
	m_staticText81 = new wxStaticText( m_panel1, wxID_ANY, _("m/s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	bSizer211->Add( m_staticText81, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_staticline2 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer211->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	fgSizer2->Add( bSizer211, 1, wxEXPAND, 0 );
	
	m_staticline5 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer2->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2111;
	bSizer2111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxDWave = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2111->Add( m_checkBoxDWave, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	
	bSizer2111->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_staticText34 = new wxStaticText( m_panel1, wxID_ANY, _("Wave"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText34->Wrap( -1 );
	m_staticText34->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer2111->Add( m_staticText34, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlWaveDir = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer2111->Add( m_textCtrlWaveDir, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText24 = new wxStaticText( m_panel1, wxID_ANY, _("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	bSizer2111->Add( m_staticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	fgSizer2->Add( bSizer2111, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpWaveHeight = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_checkBoxBmpWaveHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticTextWaveHeight = new wxStaticText( m_panel1, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextWaveHeight->Wrap( -1 );
	m_staticTextWaveHeight->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer10->Add( m_staticTextWaveHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlWaveHeight = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer10->Add( m_textCtrlWaveHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText37 = new wxStaticText( m_panel1, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	bSizer10->Add( m_staticText37, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	fgSizer2->Add( bSizer10, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer6->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpSwell = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkBoxBmpSwell, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText32 = new wxStaticText( m_panel1, wxID_ANY, _("Swell"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	m_staticText32->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_staticText32->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer6->Add( m_staticText32, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlSwMax = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer6->Add( m_textCtrlSwMax, 0, wxALL, 0 );
	
	m_staticText33 = new wxStaticText( m_panel1, wxID_ANY, _("max/m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	m_staticText33->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer6->Add( m_staticText33, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_textCtrlSwMin = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer6->Add( m_textCtrlSwMin, 0, wxALL, 0 );
	
	m_staticText331 = new wxStaticText( m_panel1, wxID_ANY, _("min/m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText331->Wrap( -1 );
	m_staticText331->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer6->Add( m_staticText331, 0, wxALIGN_CENTER|wxALL, 0 );
	
	fgSizer2->Add( bSizer6, 1, wxEXPAND, 0 );
	
	m_staticline6 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer2->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer5->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpClouds = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_checkBoxBmpClouds, 0, wxALL, 0 );
	
	m_staticText35 = new wxStaticText( m_panel1, wxID_ANY, _("Clouds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	m_staticText35->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_staticText35->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer5->Add( m_staticText35, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlClouds = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer5->Add( m_textCtrlClouds, 0, wxALL, 0 );
	
	m_staticText332 = new wxStaticText( m_panel1, wxID_ANY, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText332->Wrap( -1 );
	m_staticText332->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer5->Add( m_staticText332, 0, wxALIGN_CENTER|wxALL, 0 );
	
	fgSizer2->Add( bSizer5, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxDPressure = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_checkBoxDPressure, 0, wxALL, 0 );
	
	
	bSizer7->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_staticTextPressure = new wxStaticText( m_panel1, wxID_ANY, _("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPressure->Wrap( -1 );
	m_staticTextPressure->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_staticTextPressure->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer7->Add( m_staticTextPressure, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlPressure = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer7->Add( m_textCtrlPressure, 0, wxALL, 0 );
	
	m_staticText3321 = new wxStaticText( m_panel1, wxID_ANY, _("mb"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3321->Wrap( -1 );
	m_staticText3321->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer7->Add( m_staticText3321, 0, wxALIGN_CENTER|wxALL, 0 );
	
	fgSizer2->Add( bSizer7, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer71->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpTemperature = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_checkBoxBmpTemperature, 0, wxALL, 0 );
	
	m_staticText26 = new wxStaticText( m_panel1, wxID_ANY, _("Temp"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	m_staticText26->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer71->Add( m_staticText26, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlTemperature = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer71->Add( m_textCtrlTemperature, 0, wxALL, 0 );
	
	fgSizer2->Add( bSizer71, 1, wxEXPAND, 0 );
	
	m_staticline7 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer2->Add( m_staticline7, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxDCurrent = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_checkBoxDCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	
	bSizer11->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_staticText333 = new wxStaticText( m_panel1, wxID_ANY, _("Current"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText333->Wrap( -1 );
	m_staticText333->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer11->Add( m_staticText333, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlCurrentDir = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer11->Add( m_textCtrlCurrentDir, 0, wxALL, 0 );
	
	m_staticText341 = new wxStaticText( m_panel1, wxID_ANY, _("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText341->Wrap( -1 );
	bSizer11->Add( m_staticText341, 0, wxALL, 5 );
	
	fgSizer2->Add( bSizer11, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer14->Add( 24, 0, 0, wxEXPAND, 0 );
	
	m_checkBoxBmpCurrentForce = new wxCheckBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_checkBoxBmpCurrentForce, 0, wxALL, 0 );
	
	m_staticText40 = new wxStaticText( m_panel1, wxID_ANY, _("Force"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	m_staticText40->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer14->Add( m_staticText40, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlCurrentForce = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_READONLY|wxTE_RIGHT );
	bSizer14->Add( m_textCtrlCurrentForce, 0, wxALL, 0 );
	
	m_staticText41 = new wxStaticText( m_panel1, wxID_ANY, _("kts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	bSizer14->Add( m_staticText41, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	fgSizer2->Add( bSizer14, 1, wxEXPAND, 5 );
	
	fgSizer1->Add( fgSizer2, 1, wxEXPAND, 0 );
	
	m_panel1->SetSizer( fgSizer1 );
	m_panel1->Layout();
	fgSizer1->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, _("Data"), true );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxPoint( -1,-1 ), wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText321 = new wxStaticText( m_panel2, wxID_ANY, _("TopLeft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText321->Wrap( -1 );
	fgSizer6->Add( m_staticText321, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	m_textCtrlSelectionTopLat = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	fgSizer6->Add( m_textCtrlSelectionTopLat, 0, wxALL, 0 );
	
	m_textCtrlSelectionTopLon = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	fgSizer6->Add( m_textCtrlSelectionTopLon, 0, wxALL, 0 );
	
	m_staticText342 = new wxStaticText( m_panel2, wxID_ANY, _("BotRight"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText342->Wrap( -1 );
	fgSizer6->Add( m_staticText342, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_textCtrlSelectionBottomLat = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	fgSizer6->Add( m_textCtrlSelectionBottomLat, 0, wxALL, 0 );
	
	m_textCtrlSelectionBottomLon = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	fgSizer6->Add( m_textCtrlSelectionBottomLon, 0, wxALL, 0 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_buttonSelectionReset = new wxButton( m_panel2, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_buttonSelectionReset, 0, wxALL, 0 );
	
	m_staticText36 = new wxStaticText( m_panel2, wxID_ANY, _("HOME-key Top Selection\nEND-Key Bottom Selection"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	m_staticText36->SetFont( wxFont( 8, 70, 90, 90, false, wxEmptyString ) );
	
	fgSizer6->Add( m_staticText36, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	fgSizer4->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	m_staticline8 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer4->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText20 = new wxStaticText( m_panel2, wxID_ANY, _("Forcast"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer5->Add( m_staticText20, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	wxString m_choiceDaysChoices[] = { _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7") };
	int m_choiceDaysNChoices = sizeof( m_choiceDaysChoices ) / sizeof( wxString );
	m_choiceDays = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDaysNChoices, m_choiceDaysChoices, 0 );
	m_choiceDays->SetSelection( 0 );
	fgSizer5->Add( m_choiceDays, 0, wxALL, 0 );
	
	m_staticText212 = new wxStaticText( m_panel2, wxID_ANY, _("days"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText212->Wrap( -1 );
	fgSizer5->Add( m_staticText212, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_staticText22 = new wxStaticText( m_panel2, wxID_ANY, _("every"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer5->Add( m_staticText22, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	wxString m_choiceHoursChoices[] = { _("03"), _("06"), _("12") };
	int m_choiceHoursNChoices = sizeof( m_choiceHoursChoices ) / sizeof( wxString );
	m_choiceHours = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceHoursNChoices, m_choiceHoursChoices, 0 );
	m_choiceHours->SetSelection( 0 );
	fgSizer5->Add( m_choiceHours, 0, wxALL, 0 );
	
	m_staticText23 = new wxStaticText( m_panel2, wxID_ANY, _("hours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer5->Add( m_staticText23, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_checkBoxWind = new wxCheckBox( m_panel2, wxID_ANY, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxWind->SetValue(true); 
	fgSizer5->Add( m_checkBoxWind, 0, wxALL, 0 );
	
	m_checkBoxGusts = new wxCheckBox( m_panel2, wxID_ANY, _("Gusts"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxGusts, 0, wxALL, 0 );
	
	m_checkBoxWaves = new wxCheckBox( m_panel2, wxID_ANY, _("Waves"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxWaves->SetValue(true); 
	fgSizer5->Add( m_checkBoxWaves, 0, wxALL, 0 );
	
	m_checkBoxSwell = new wxCheckBox( m_panel2, wxID_ANY, _("Swell"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxSwell->SetValue(true); 
	fgSizer5->Add( m_checkBoxSwell, 0, wxALL, 0 );
	
	m_checkBoxPressure = new wxCheckBox( m_panel2, wxID_ANY, _("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxPressure, 0, wxALL, 0 );
	
	m_checkBoxTemperature = new wxCheckBox( m_panel2, wxID_ANY, _("Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxTemperature, 0, wxALL, 0 );
	
	m_checkBoxRain = new wxCheckBox( m_panel2, wxID_ANY, _("Rain"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxRain, 0, wxALL, 0 );
	
	m_checkBoxClouds = new wxCheckBox( m_panel2, wxID_ANY, _("Clouds"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxClouds, 0, wxALL, 0 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_buttonDownload = new wxButton( m_panel2, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_buttonDownload, 0, wxALL, 0 );
	
	fgSizer4->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	m_staticline9 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer4->Add( m_staticline9, 0, wxEXPAND | wxALL, 5 );
	
	m_textCtrlDownload = new wxTextCtrl( m_panel2, wxID_ANY, _("Download-Tab needs focus to receive key-events\n\nthen place the cursor in the chart, hit HOME-key\nplace cursor bottom right and hit END-key"), wxDefaultPosition, wxSize( -1,300 ), wxTE_MULTILINE );
	fgSizer4->Add( m_textCtrlDownload, 0, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( fgSizer4 );
	m_panel2->Layout();
	fgSizer4->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, _("Download"), false );
	
	fgSizer3->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GribDialog::onCloseDialog ) );
	m_notebook1->Connect( wxEVT_CHAR, wxKeyEventHandler( GribDialog::OnCharNoteBook1 ), NULL, this );
	m_notebook1->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( GribDialog::onPageChanged ), NULL, this );
	m_textCtrlDir->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GribDialog::onDirChanged ), NULL, this );
	m_fileButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GribDialog::onFileButtonClick ), NULL, this );
	m_treeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( GribDialog::onTreeItemRightClick ), NULL, this );
	m_treeCtrl->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( GribDialog::onTreeSelectionChanged ), NULL, this );
	m_checkBoxDWind->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDWindClick ), NULL, this );
	m_checkBoxBmpForce->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpForceClick ), NULL, this );
	m_checkBoxBmpGusts->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpGustsClick ), NULL, this );
	m_checkBoxDWave->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDWaveClick ), NULL, this );
	m_checkBoxBmpWaveHeight->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpWaveHeightClick ), NULL, this );
	m_checkBoxBmpSwell->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpSwellClick ), NULL, this );
	m_checkBoxBmpClouds->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpCloudsClick ), NULL, this );
	m_checkBoxDPressure->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDPressureClick ), NULL, this );
	m_checkBoxBmpTemperature->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpTemperatureClick ), NULL, this );
	m_checkBoxDCurrent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDCurrentClick ), NULL, this );
	m_checkBoxBmpCurrentForce->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpCurrentForceClick ), NULL, this );
	m_buttonSelectionReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GribDialog::onButtonSelectionReset ), NULL, this );
	m_buttonDownload->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GribDialog::onDownLoadOK ), NULL, this );
}

GribDialog::~GribDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GribDialog::onCloseDialog ) );
	m_notebook1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GribDialog::OnCharNoteBook1 ), NULL, this );
	m_notebook1->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( GribDialog::onPageChanged ), NULL, this );
	m_textCtrlDir->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GribDialog::onDirChanged ), NULL, this );
	m_fileButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GribDialog::onFileButtonClick ), NULL, this );
	m_treeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( GribDialog::onTreeItemRightClick ), NULL, this );
	m_treeCtrl->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( GribDialog::onTreeSelectionChanged ), NULL, this );
	m_checkBoxDWind->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDWindClick ), NULL, this );
	m_checkBoxBmpForce->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpForceClick ), NULL, this );
	m_checkBoxBmpGusts->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpGustsClick ), NULL, this );
	m_checkBoxDWave->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDWaveClick ), NULL, this );
	m_checkBoxBmpWaveHeight->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpWaveHeightClick ), NULL, this );
	m_checkBoxBmpSwell->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpSwellClick ), NULL, this );
	m_checkBoxBmpClouds->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpCloudsClick ), NULL, this );
	m_checkBoxDPressure->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDPressureClick ), NULL, this );
	m_checkBoxBmpTemperature->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpTemperatureClick ), NULL, this );
	m_checkBoxDCurrent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onDCurrentClick ), NULL, this );
	m_checkBoxBmpCurrentForce->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GribDialog::onBmpCurrentForceClick ), NULL, this );
	m_buttonSelectionReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GribDialog::onButtonSelectionReset ), NULL, this );
	m_buttonDownload->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GribDialog::onDownLoadOK ), NULL, this );
	
}
