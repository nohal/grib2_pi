/*
    Copyright (c) 2011, Konni <email>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Konni <email> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Konni <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef GRIBOVERLAYFACTORY_H
#define GRIBOVERLAYFACTORY_H

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers
#include <wx/dynarray.h>

#include "../../../include/ocpn_plugin.h"

#include "grib2data.h"

#include <vector>
#include "grib2.h"

//typedef std::vector<int> ArrayOfInts;

enum OVERLAP {_IN,_ON,_OUT};

OVERLAP Intersect(PlugIn_ViewPort *vp,
       double lat_min, double lat_max, double lon_min, double lon_max, double Marge);
bool PointInLLBox(PlugIn_ViewPort *vp, double x, double y);

class GRIBOverlayFactory
{
public:
            GRIBOverlayFactory();
            ~GRIBOverlayFactory();

     void setData(MainDialog *gui, wxHashTable *hash, wxDouble tlat, wxDouble tlon, wxDouble blat, wxDouble blon, 
		  wxUint8 scanmode, wxDouble incrLon, wxDouble incrLat, wxUint32 sectors, wxUint32 latSectors, wxUint32 lonSectors);
     void reset();
     void setSelectionRectangle(Selection *rect);
     bool isReadyToRender(){ return m_bReadyToRender; }
     void clearBmp();
     bool RenderGribOverlay(wxDC &pmdc, PlugIn_ViewPort *vp );
     void RenderSelectionRectangle();
     
     void RenderGribWind();
     bool RenderGribGustsBmp();
     void RenderGribWave();
     bool RenderGribSwellBmp();
     void RenderGribCurrent();
     bool RenderGribWindBmp();
     bool RenderGribWaveBmp();
     bool RenderGribCurrentBmp();
     bool RenderGribTemperatureBmp();
     bool RenderGribCloudsBmp();
     bool RenderGribPressure();
     
     void drawWindArrowWithBarbs(wxDC *pmdc,
                                      double i, double j, double vx, double vy,
                                      bool south,
                                      wxColour arrowColor
                                     );
     void drawWaveArrow(wxDC *pmdc, int i, int j, double ang, wxColour arrowColor);
     
     void drawTransformedLine( wxDC *pmdc, wxPen pen,
                               double si, double co,int di, int dj, int i,int j, int k,int l);
     void drawPetiteBarbule(wxDC *pmdc, wxPen pen, bool south,
                               double si, double co, int di, int dj, int b);
     void drawGrandeBarbule(wxDC *pmdc, wxPen pen, bool south,
                               double si, double co, int di, int dj, int b);
     void drawTriangle(wxDC *pmdc, wxPen pen, bool south,
			       double si, double co, int di, int dj, int b); 
     wxColour GetTempGraphicColor(double val_in);
     wxColour GetSeaCurrentGraphicColor(double val_in); 
     wxColour GetWaveGraphicColor(double val_in);
     wxColour GetSwellGraphicColor(double val_in); 
     
     PlugIn_ViewPort 	*vp;
	 bool 		m_bReadyToRender;
	 bool		renderSelectionRectangle;
     
private:
     MainDialog		*gui;
     double 		m_last_vp_scale;
     wxDouble		tlon,tlat,blon,blat;
     wxDouble		incrLon, incrLat;
     wxUint32		sectors, latSectors, lonSectors;
     wxHashTable 	*hash;
     wxFloat32 		*data;
     wxDC		 	*pmdc;
     PlugIn_ViewPort 	*m_last_vp;
     double	 	m_last_vp_latMax;     
     int			space[7];
     wxUint16 		m_space;
     
     wxBitmap		*m_pbm_temp;
     wxBitmap		*m_pbm_current;
     wxBitmap		*m_pbm_wind;
     wxBitmap		*m_pbm_wave;
     wxBitmap		*m_pbm_clouds;
     wxBitmap		*m_pbm_Swell;
     wxBitmap		*m_pbm_Gusts;
     
     Selection		*rect;
     
     wxArrayPtrVoid     m_IsobarArray;
};


#endif // GRIBOVERLAYFACTORY_H
