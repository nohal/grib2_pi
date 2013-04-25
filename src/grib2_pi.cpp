/******************************************************************************
 * $Id: grib_pi.cpp,v 1.8 2010/06/21 01:54:37 bdbcat Exp $
 *
 * Project:  OpenCPN
 * Purpose:  GRIB Plugin
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   bdbcat@yahoo.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers


#include <wx/treectrl.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

#include "grib2_pi.h"
#include "grib2.h"



// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new grib2_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}





//---------------------------------------------------------------------------------------------------------
//
//    Grib PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons2.h"



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

grib2_pi::grib2_pi(void *ppimgr)
      :opencpn_plugin_17(ppimgr)
{
      // Create the PlugIn icons
	initialize_images();
}

int grib2_pi::Init(void)
{
//      printf("grib_pi Init()\n");
      AddLocaleCatalog( _T("opencpn-grib2_pi") );

      // Set some default private member parameters
      m_grib_dialog_x = 0;
      m_grib_dialog_y = 0;
      m_grib_dialog_sx = 200;
      m_grib_dialog_sy = 200;
      m_pGribDialog = NULL;
      //m_pGRIBOverlayFactory = NULL;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      // Get a pointer to the opencpn display canvas, to use as a parent for the GRIB dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
      if(m_bGRIBShowIcon)
            m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_grib2, _img_grib2, wxITEM_NORMAL,
                  _("Grib2"), _T(""), NULL,
                   GRIB_TOOL_POSITION, 0, this);

      // Create the drawing factory

      m_pGRIBOverlayFactory = new GRIBOverlayFactory;
	  m_pGRIBOverlayFactory->m_bReadyToRender=false;
//      wxMenuItem *pmi = new wxMenuItem(NULL, -1, _("PlugIn Item"));
//      int miid = AddCanvasContextMenuItem(pmi, (PlugInCallBackFunction )&s_ContextMenuCallback );
//      SetCanvasContextMenuItemViz(miid, true);

      return (WANTS_OVERLAY_CALLBACK |
           WANTS_OPENGL_OVERLAY_CALLBACK |
           WANTS_CURSOR_LATLON       |
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
           WANTS_PREFERENCES
            );
}

bool grib2_pi::DeInit(void)
{
//      printf("grib_pi DeInit()\n");

      if(m_pGribDialog)
      {
		 m_pGRIBOverlayFactory->m_bReadyToRender = false;
		 m_pGribDialog->Close();
      }
     
	delete m_pGRIBOverlayFactory;
	m_pGRIBOverlayFactory = NULL;
    return true;
}

int grib2_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int grib2_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int grib2_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int grib2_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *grib2_pi::GetPlugInBitmap()
{
      return _img_grib2_pi;
}

wxString grib2_pi::GetCommonName()
{
      return _("GRIB2");
}


wxString grib2_pi::GetShortDescription()
{
      return _("GRIB2 PlugIn for OpenCPN");
}


wxString grib2_pi::GetLongDescription()
{
      return _("GRIB2 PlugIn for OpenCPN\n\
Provides basic GRIB file overlay capabilities for several GRIB2 file types.\n\n\
Supported GRIB2 file types include:\n\
- wind direction and speed\n\
- significant wave height\n\
- sea surface temperature\n\
- surface current direction and speed.");

}


void grib2_pi::SetDefaults(void)
{
      // If the config somehow says NOT to show the icon, override it so the user gets good feedback
      if(!m_bGRIBShowIcon)
      {
            m_bGRIBShowIcon = true;

            m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_grib2, _img_grib2, wxITEM_NORMAL,
                  _("Grib2"), _T(""), NULL,
                   GRIB_TOOL_POSITION, 0, this);
      }
}


int grib2_pi::GetToolbarToolCount(void)
{
      return 1;
}

void grib2_pi::ShowPreferencesDialog( wxWindow* parent )
{
      wxDialog *dialog = new wxDialog( parent, wxID_ANY, _("GRIB2 Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE );

    //      Build GRIB. Page for Toolbox
    int border_size = 4;

    wxBoxSizer* itemBoxSizerGRIBPanel = new wxBoxSizer(wxVERTICAL);
    dialog->SetSizer(itemBoxSizerGRIBPanel);

    //  Grib toolbox icon checkbox
    wxStaticBox* itemStaticBoxSizerGRIBStatic = new wxStaticBox(dialog, wxID_ANY, _("GRIB2"));
    wxStaticBoxSizer* itemStaticBoxSizerGRIB = new wxStaticBoxSizer(itemStaticBoxSizerGRIBStatic, wxVERTICAL);
    itemBoxSizerGRIBPanel->Add(itemStaticBoxSizerGRIB, 0, wxGROW|wxALL, border_size);

    m_pGRIBShowIcon = new wxCheckBox( dialog, -1, _("Show GRIB2 icon"), wxDefaultPosition, wxSize(-1, -1), 0 );
    itemStaticBoxSizerGRIB->Add(m_pGRIBShowIcon, 1, wxALIGN_LEFT|wxALL, border_size);

    m_pGRIBUseHiDef = new wxCheckBox( dialog, -1, _("Use High Definition Graphics"));
    itemStaticBoxSizerGRIB->Add(m_pGRIBUseHiDef, 1, wxALIGN_LEFT|wxALL, border_size);

    m_pGRIBShowIcon->SetValue(m_bGRIBShowIcon);
    m_pGRIBUseHiDef->SetValue(m_bGRIBUseHiDef);

      wxStdDialogButtonSizer* DialogButtonSizer = dialog->CreateStdDialogButtonSizer(wxOK|wxCANCEL);
      itemBoxSizerGRIBPanel->Add(DialogButtonSizer, 0, wxALIGN_RIGHT|wxALL, 5);

      dialog->Fit();

      if(dialog->ShowModal() == wxID_OK)
      {
            //    Show Icon changed value?
            if(m_bGRIBShowIcon != m_pGRIBShowIcon->GetValue())
            {
                  m_bGRIBShowIcon= m_pGRIBShowIcon->GetValue();

                  if(m_bGRIBShowIcon)
                        m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_grib2, _img_grib2, wxITEM_NORMAL,
                              _("Grib2"), _T(""), NULL, GRIB_TOOL_POSITION,
                              0, this);
                  else
                        RemovePlugInTool(m_leftclick_tool_id);
            }

            m_bGRIBUseHiDef= m_pGRIBUseHiDef->GetValue();

            SaveConfig();
      }

}

void grib2_pi::OnToolbarToolCallback(int id)
{
//      printf("grib_pi ToolCallBack()\n");
//     ::wxBell();

      // show the GRIB dialog
      if(NULL == m_pGribDialog)
      {
        m_pGribDialog = new MainDialog(m_parent_window);
	    m_pGribDialog->setPlugIn(this);
          /*  m_pGribDialog->Create ( m_parent_window, this, -1, _("GRIB2 Display Control"), m_grib_dir,
                               wxPoint( m_grib_dialog_x, m_grib_dialog_y), wxSize( m_grib_dialog_sx, m_grib_dialog_sy));
      */}
      m_pGribDialog->Show();                        // Show modeless, so it stays on the screen
}

void grib2_pi::OnGribDialogClose()
{
	  m_pGribDialog->Hide();
      if(m_pGRIBOverlayFactory)
      {
        m_pGRIBOverlayFactory->reset();
	    RequestRefresh(m_parent_window);
      }
      SaveConfig();     
}

bool grib2_pi::RenderOverlay(wxDC &pmdc, PlugIn_ViewPort *vp)
{
      if(m_pGribDialog && m_pGRIBOverlayFactory)
      {
            if(m_pGRIBOverlayFactory->isReadyToRender())
            {
                  m_pGRIBOverlayFactory->RenderGribOverlay ( pmdc, vp );
                  return true;
            }
            else
                  return false;
      }
      else
            return false;
}

void grib2_pi::SetCursorLatLon(double lat, double lon)
{
      if(m_pGribDialog)
      {
            m_pGribDialog->SetCursorLatLon(lat, lon);
      }
      
}


bool grib2_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings" ) );
            pConf->Read ( _T( "GRIB2UseHiDef" ),  &m_bGRIBUseHiDef, 0 );
            pConf->Read ( _T( "ShowGRIB2Icon" ),  &m_bGRIBShowIcon, 1 );


            m_grib_dialog_sx = pConf->Read ( _T ( "GRIB2DialogSizeX" ), 300L );
            m_grib_dialog_sy = pConf->Read ( _T ( "GRIB2DialogSizeY" ), 540L );
            m_grib_dialog_x =  pConf->Read ( _T ( "GRI2BDialogPosX" ), 20L );
            m_grib_dialog_y =  pConf->Read ( _T ( "GRIB2DialogPosY" ), 20L );

            if((m_grib_dialog_x < 0) || (m_grib_dialog_x > m_display_width))
                  m_grib_dialog_x = 5;
            if((m_grib_dialog_y < 0) || (m_grib_dialog_y > m_display_height))
                  m_grib_dialog_y = 5;

            pConf->SetPath ( _T ( "/Directories" ) );
			wxStandardPathsBase &std = wxStandardPathsBase::Get(); 
			pConf->Read ( _T ( "GRIB2Directory" ), &m_grib_dir, std.GetDocumentsDir() );

            return true;
      }
      else
            return false;
}

bool grib2_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings" ) );

            pConf->Write ( _T ( "GRIB2UseHiDef" ), m_bGRIBUseHiDef );
            pConf->Write ( _T ( "ShowGRIB2Icon" ), m_bGRIBShowIcon );

            pConf->Write ( _T ( "GRIB2DialogSizeX" ),  m_grib_dialog_sx );
            pConf->Write ( _T ( "GRIB2DialogSizeY" ),  m_grib_dialog_sy );
            pConf->Write ( _T ( "GRIB2DialogPosX" ),   m_grib_dialog_x );
            pConf->Write ( _T ( "GRIB2DialogPosY" ),   m_grib_dialog_y );

            pConf->SetPath ( _T ( "/Directories" ) );
            pConf->Write ( _T ( "GRIB2Directory" ), m_grib_dir );

            return true;
      }
      else
            return false;
}
