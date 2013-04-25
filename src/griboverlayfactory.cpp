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
#define PI 3.14159265

#include "griboverlayfactory.h"
#include "grib2.h"
#include "IsoLine2.h"

#include <wx/colour.h>
#include <wx/dynarray.h>
//----------------------------------------------------------------------------------------------------------
//    Grib Overlay Factory Implementation
//----------------------------------------------------------------------------------------------------------
GRIBOverlayFactory::GRIBOverlayFactory()
{
      hash = NULL;
      
      m_pbm_temp 	= NULL;
      m_pbm_current 	= NULL;
      m_pbm_wind 	= NULL;
      m_pbm_wave 	= NULL;
      m_pbm_clouds 	= NULL;
      m_pbm_Gusts 	= NULL;
      m_pbm_Swell 	= NULL;      
      
      m_last_vp_latMax = -99999.0;
      rect = NULL;
      
      space[0]=30; space[1]=50; space[2]=100; space[3]=200; space[4]=400; space[5]=600; space[6]=1200;
	  m_bReadyToRender = false;
      m_space = 0;
}

GRIBOverlayFactory::~GRIBOverlayFactory()
{
    m_bReadyToRender = false;
	renderSelectionRectangle = false;
}

void GRIBOverlayFactory::setData(MainDialog *gui, wxHashTable *hash, wxDouble tlat, wxDouble tlon, wxDouble blat, wxDouble blon, 
		  wxUint8 scanmode, wxDouble incrLon, wxDouble incrLat, wxUint32 sectors, wxUint32 latSectors, wxUint32 lonSectors)
{
    this->hash = hash;
    this->tlat = tlat; this->tlon = tlon; this->blat = blat; this->blon = blon;
    this->incrLon = (scanmode & 128)?-incrLon:incrLon;
    this->incrLat = (scanmode & 64) ?incrLat:-incrLat;
                      
    //printf("LatIncr %f LonIncr %f TLat %f BLat %f\n",this->incrLat,this->incrLon,tlat,blat);
    this->sectors = sectors;
    this->latSectors = latSectors;
    this->lonSectors = lonSectors;
    this->gui = gui;
    
    this->m_bReadyToRender = true;
}

void GRIBOverlayFactory::setSelectionRectangle(Selection *rect)
{
    this->rect = rect;
}

void GRIBOverlayFactory::reset()
{
    this->m_bReadyToRender = false;
    
      //    Clear out the cached isobars
    for(unsigned int i = 0 ; i < m_IsobarArray.GetCount() ; i++)
      {
            IsoLine *piso = (IsoLine *)m_IsobarArray.Item(i);
            delete piso;
      }
    m_IsobarArray.Clear();                            // Will need to rebuild Isobar list    
    
    clearBmp();
}

bool GRIBOverlayFactory::RenderGribOverlay(wxDC &pmdc, PlugIn_ViewPort *vp )
{
    this->vp = vp; this->pmdc = &pmdc; 
    
    if(renderSelectionRectangle && rect != NULL)
	{
      RenderSelectionRectangle();
	//  this->m_bReadyToRender = false;
	  return true;
	}
    
    if(hash == NULL) return true;
    
    if(vp->view_scale_ppm != m_last_vp_scale)
    {
           // ClearCachedData();
      if(vp->view_scale_ppm < 0.001135)
	  m_space = space[0];
      else if(vp->view_scale_ppm <= 0.001135)
	  m_space = space[1];
      else if(vp->view_scale_ppm <= 0.018165)
	  m_space = space[2]; 
      else if(vp->view_scale_ppm <= 0.072659)
	  m_space = space[3];  

     // if(m_pbm_temp || m_pbm_current)
 /*     {
	delete m_pbm_temp;
	delete m_pbm_current;
	m_pbm_temp = NULL;
	m_pbm_current = NULL;
      }*/
    }
    if(m_last_vp_latMax)
//printf("%f %f \n",m_last_vp_latMax,vp->lat_max);

    if(m_last_vp_latMax != vp->lat_max)
    {
      clearBmp();
    }
    
    if(!m_bReadyToRender) return false;


    if(gui->m_checkBoxDWind->GetValue())
      RenderGribWind();
    if(gui->m_checkBoxDWave->GetValue())
      RenderGribWave();
    if(gui->m_checkBoxBmpForce->GetValue())
      RenderGribWindBmp();
    if(gui->m_checkBoxBmpWaveHeight->GetValue())
      RenderGribWaveBmp();
    if(gui->m_checkBoxBmpCurrentForce->GetValue())
      RenderGribCurrentBmp();    
    if(gui->m_checkBoxDCurrent->GetValue())
      RenderGribCurrent();    
    if(gui->m_checkBoxBmpTemperature->GetValue())
      RenderGribTemperatureBmp();   
    if(gui->m_checkBoxBmpClouds->GetValue())
      RenderGribCloudsBmp(); 
    if(gui->m_checkBoxDPressure->GetValue())
      RenderGribPressure();    
    if(gui->m_checkBoxBmpGusts->GetValue())
      RenderGribGustsBmp();
    if(gui->m_checkBoxBmpSwell->GetValue())
      RenderGribSwellBmp();
    
    m_last_vp_scale = vp->view_scale_ppm;
    m_last_vp_latMax = vp->lat_max;    
   // this->m_bReadyToRender = false;
    return true;
}

void GRIBOverlayFactory::clearBmp()
{
      if(m_pbm_temp)
	delete m_pbm_temp;
      if(m_pbm_current)
	delete m_pbm_current;
      if(m_pbm_wind)
	delete m_pbm_wind;
      if(m_pbm_wave)
	delete m_pbm_wave;
      if(m_pbm_clouds)
	delete m_pbm_clouds;      
      if(m_pbm_Gusts)
	delete m_pbm_Gusts;
      if(m_pbm_Swell)
	delete m_pbm_Swell; 
      
	m_pbm_temp 	= NULL;
	m_pbm_current 	= NULL;  
	m_pbm_wind 	= NULL;
	m_pbm_wave 	= NULL;
	m_pbm_clouds 	= NULL;
	m_pbm_Gusts 	= NULL;
	m_pbm_Swell 	= NULL;
}

void GRIBOverlayFactory::RenderSelectionRectangle()
{
    wxPoint ptop, pbottom;
    wxInt32 width,height;
    
    GetCanvasPixLL(vp, &ptop, rect->topLat, rect->topLon);
    if(rect->bottomLat != GRIB_NOTDEF)
    {
      GetCanvasPixLL(vp, &pbottom, rect->bottomLat, rect->bottomLon);
//    else
//      GetCanvasPixLL(vp, &pbottom, gui->m_cursor_lat, gui->m_cursor_lon);
    width = pbottom.x  - ptop.x;
    height = pbottom.y - ptop.y;
    
    pmdc->DrawLine(ptop.x,ptop.y, pbottom.x,ptop.y);
    pmdc->DrawLine(pbottom.x,ptop.y, pbottom.x,pbottom.y);
    pmdc->DrawLine(pbottom.x,pbottom.y,ptop.x,pbottom.y);
    pmdc->DrawLine(ptop.x,pbottom.y,ptop.x,ptop.y);  
    }
}

void GRIBOverlayFactory::RenderGribWind()
{
      wxColour colour;
      wxPoint p,startp, stopp, oldp;
      double lat = tlat;
      double lon = tlon;       
   
      double **windForce = (double**)hash->Get((long int)0);
      double **windDir   = (double**)hash->Get((long int)1);

      if(!windForce || !windDir) return;
  
      GetGlobalColor ( _T ( "YELO2" ), &colour );

      double vpLon, vpLat;
      if(vp->lon_min < 0) vpLon = 360.0+vp->lon_min;
	else vpLon = vp->lon_min;

      if(vpLon > lon)
	vpLon = vp->lon_min;
      else
	vpLon = lon;
      
      if(vp->lat_min > lat)
	vpLat = vp->lat_min;
      else
	vpLat = lat;

//      GetCanvasPixLL(vp, &startp, vpLat, vpLon);

      GetCanvasPixLL(vp, &startp, lat, lon);	
      GetCanvasPixLL(vp, &stopp, blat, blon);
      oldp = startp; //startp.x = oldp.x; 
    //  oldp.y -= m_space; startp.y = oldp.y;
      p = startp;
     // printf("%i %i %i %i %i %i\n",startp.x, startp.y,stopp.x, stopp.y,p.x,p.y);
     // for(unsigned x = 0; x < latSectors; x++)
     // int count = 0;
//printf("start %i %i stop %i %i",startp.x,startp.y,stopp.x,stopp.y);
      for(unsigned int y = 0; y < latSectors; y++ )
      { 
//	  if(p.y > oldp.y)
//	    oldp.y += m_space;	
	 //     printf("1)%f %f %f \n",lat,lon);	    
	//  for(unsigned int i = 0; i < lonSectors; i++)
	
	  for(unsigned int x = 0; x < lonSectors; x++ )
	  {
//	printf("lat 0%f lon =%f incr =%f count %i \n",lat,lon,incr,count);
	    GetCanvasPixLL(vp, &p, lat, lon); 
	   //GetCanvasLLPix(vp,p,&lat,&lon);
	  //  printf("1) px=%i oldpx=%i space=%i \n",p.x,oldp.x,m_space);	
	//    if(p.x > oldp.x)
	 //  	      printf("2)%f %f %f \n",lat,lon,p.x);
	    {
	  //    oldp.x += m_space;
		//oldp.x += m_space;
		//p.x += m_space;
	//	printf("if p.x > oldp.x px=%i oldpx=%i space=%i \n",p.x,oldp.x,m_space);	      
	//if(count++ == 0)
	//	printf("%f %f %i %i\n",lat,lon,p.x,p.y);
	      if(PointInLLBox(vp, lon, lat) || PointInLLBox(vp, lon-360., lat))
		//if(p.x >= 0 && p.y >= 0)
	      {
		//printf("if p.x > oldp.x px=%i oldpx=%i space=%i \n",p.x,oldp.x,m_space);
		// double wDir= gui->gribReader->grib2Data->getInterpolatedValue(windDir,lon, lat, true);
		// double wForce= gui->gribReader->grib2Data->getInterpolatedValue(windForce,lon, lat, true);
		 
		//drawWindArrowWithBarbs(pmdc, p.x, p.y, 
		//	      windForce[x][i], windDir[x][i],(lat < 0.), colour);
		 if(windDir[y][x] != GRIB_NOTDEF)
		 {
		  drawWindArrowWithBarbs(pmdc, p.x, p.y,windForce[y][x], windDir[y][x],(lat < 0.), colour);
		//  wxString s = wxString::Format(_("%3.0f"),wDir);
		//  pmdc->DrawText(s,p);
		 }
	      }
	    }
	    lon += incrLon;
	  }
	  //p.y += m_space;
	lon = tlon;
        //oldp.x = startp.x;
	p.x = oldp.x;
	lat += incrLat;
      }
      return;  
}

void GRIBOverlayFactory::RenderGribWave()
{
       //    Get the the grid
      wxColour colour;
      wxPoint p;
      double lat = tlat;
      double lon = tlon;  
    
      double **waveDir   = (double **)hash->Get((long int)WAVEDIR);
 //     double **waveHeight= (double **)hash->Get((long int)WAVEHIGHT);

      if(!waveDir /*|| !waveHeight*/) return;

      //    Barbs?
      bool barbs = true;

      //    Set minimum spacing between wind arrows
      int space;

      if (barbs)
            space =  30;
      else
            space =  20;

//      int oldx = -1000; int oldy = -1000;
    
      GetGlobalColor ( _T ( "UBLCK" ), &colour );
      
      for(unsigned y = 0; y < latSectors; y++)
      {
	  for(unsigned int x = 0; x < lonSectors; x++)
	  {
//	printf("lat 0%f lon =%f incr =%f count %i \n",lat,lon,incr,count);
	    GetCanvasPixLL(vp, &p, lat, lon);
	 //   wxString s = wxString::Format(_("%.0f"),waveDir[x][i]);
	 //   pmdc->DrawText(s,p);
	    if(PointInLLBox(vp, lon, lat) || PointInLLBox(vp, lon-360.0, lat))
	    {	    //if(y < 2)printf("x=%i y=%i lat=%f lon=%f %f %f %i %f\n",x,y,lat,lon,tlon,blon,lonSectors,waveDir[y][x]);      
		if(waveDir[y][x] != GRIB_NOTDEF)
		    drawWaveArrow(pmdc, p.x, p.y, waveDir[y][x]-90.0, colour);
	    }
	    lon += incrLon;
	  }
	lon = tlon;
	lat += incrLat;
      }  
}

void GRIBOverlayFactory::RenderGribCurrent()
{
       //    Get the the grid
      wxColour colour;
      wxPoint p;
      double lat = tlat;
      double lon = tlon;  
    
      double **currentDir   = (double **)hash->Get((long int)CURRENTU);
      double **currentForce= (double **)hash->Get((long int)CURRENTV);

      if(!currentDir /*|| !waveHeight*/) return;

      //    Barbs?
      bool barbs = true;

      //    Set minimum spacing between wind arrows
      int space;

      if (barbs)
            space =  30;
      else
            space =  20;

//      int oldx = -1000; int oldy = -1000;
    
      GetGlobalColor ( _T ( "UBLCK" ), &colour );
      
      for(unsigned y = 0; y < latSectors; y++)
      {
	  for(unsigned int x = 0; x < lonSectors; x++)
	  {
//	printf("lat 0%f lon =%f incr =%f count %i \n",lat,lon,incr,count);
	    GetCanvasPixLL(vp, &p, lat, lon);

	    if(PointInLLBox(vp, lon, lat) || PointInLLBox(vp, lon-360.0, lat))
	    {	    //if(y < 2)printf("x=%i y=%i lat=%f lon=%f %f %f %i %f\n",x,y,lat,lon,tlon,blon,lonSectors,waveDir[y][x]);      
	        //double force = sqrt(currentForce[y][x]*currentForce[y][x]+currentDir[y][x]*currentDir[y][x])*3.6/1.852;
		double dir = 90. + (atan2(currentForce[y][x], -currentDir[y][x])  * 180. / PI);
	   // wxString s = wxString::Format(_("%3.0f"),(dir < 0)? dir = 360+dir:dir);
	   // pmdc->DrawText(s,p);
		if(currentDir[y][x] != GRIB_NOTDEF)
		    drawWaveArrow(pmdc, p.x, p.y, dir-90, colour);
	    }
	    //printf("%f %f %f %f %f\n",lat,lon,incrLat,incrLon,tlat);
	    lon += incrLon;
	  }
	lon = tlon;
	lat += incrLat;
      }      
}

bool GRIBOverlayFactory::RenderGribWindBmp()
{
      static wxPoint porg;
      static int width, height;  


      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
     if(Intersect(vp, tlat, blat, tlon, blon, 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, blat, tlat, blon-360.0, tlon-360.0, 0.) != _OUT)
            bdraw= true;
 bdraw = true;

      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_wind == NULL)
            {
                  wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 
		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);
		  
                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....

			      
			      double **wind = (double **) hash->Get(WIND);
			      if(wind == NULL) return false;
			      
			      ::wxBeginBusyCursor();
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;

                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;
					  double vh = gui->gribReader->grib2Data->getInterpolatedValue(wind,lon, lat, true);

                                          if (vh != GRIB_NOTDEF)
                                          {
                                                wxColour c = GetTempGraphicColor(vh);
                                                unsigned char r = c.Red();
                                                unsigned char g = c.Green();
                                                unsigned char b = c.Blue();

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 128);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
//                                                       gr_image.SetRGB(ipix + xp, jpix + yp, 0,0,0);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);

                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_wind = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_wind, wxColour(0,0,0));
                              m_pbm_wind->SetMask(gr_mask);

                              ::wxEndBusyCursor();
                  }
            }


            if(m_pbm_wind)
            {	
                  pmdc->DrawBitmap(*m_pbm_wind, porg.x, porg.y, true);
            }
            else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed TEMPERATURE GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }

//      }
      return true;
}

bool GRIBOverlayFactory::RenderGribWaveBmp()
{
//      printf("renderGRIBSeaTemp\n");
      static wxPoint porg;
      static int width, height;

      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
     if(Intersect(vp, tlat, blat, tlon, blon, 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, blat, tlat, blon-360.0, tlon-360.0, 0.) != _OUT)
            bdraw= true;
 bdraw = true;

      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_wave == NULL)
            {
                  wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 
		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);
		  
                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....

			      
			      double **waveHeight = (double **) hash->Get(WAVEHIGHT);
			      if(waveHeight == NULL) return false;
			      
			      ::wxBeginBusyCursor();
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;

                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;
					  double vh= gui->gribReader->grib2Data->getInterpolatedValue(waveHeight,lon, lat, true);

                                          if (vh != GRIB_NOTDEF)
                                          {
                                                wxColour c = GetWaveGraphicColor(vh);
                                                unsigned char r = c.Red();
                                                unsigned char g = c.Green();
                                                unsigned char b = c.Blue();

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 128);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
//                                                       gr_image.SetRGB(ipix + xp, jpix + yp, 0,0,0);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);

                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_wave = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_wave, wxColour(0,0,0));
                              m_pbm_wave->SetMask(gr_mask);

                              ::wxEndBusyCursor();
                  }
            }


            if(m_pbm_wave)
            {	
                  pmdc->DrawBitmap(*m_pbm_wave, porg.x, porg.y, true);
            }
/*            else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed TEMPERATURE GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }
*/
//      }
      return true;
}


bool GRIBOverlayFactory::RenderGribTemperatureBmp()
{
//      printf("renderGRIBSeaTemp\n");
      static wxPoint porg;
      static int width, height;

      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
     if(Intersect(vp, tlat, blat, tlon, blon, 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, blat, tlat, blon-360.0, tlon-360.0, 0.) != _OUT)
            bdraw= true;
 bdraw = true;

      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_temp == NULL)
            {
                 wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);
		  
                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....

			      
			      double **temperature = (double **) hash->Get(TEMP);
			      if(temperature == NULL) return false;
			      
			      ::wxBeginBusyCursor();
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;

                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;
					  double vh= gui->gribReader->grib2Data->getInterpolatedValue(temperature,lon, lat, true);

                                          if (vh != GRIB_NOTDEF)
                                          {
                                                wxColour c = GetTempGraphicColor(vh);
                                                unsigned char r = c.Red();
                                                unsigned char g = c.Green();
                                                unsigned char b = c.Blue();

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 128);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
//                                                       gr_image.SetRGB(ipix + xp, jpix + yp, 0,0,0);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);

                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_temp = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_temp, wxColour(0,0,0));
                              m_pbm_temp->SetMask(gr_mask);

                              ::wxEndBusyCursor();
                  }
            }


            if(m_pbm_temp)
            {	
                  pmdc->DrawBitmap(*m_pbm_temp, porg.x, porg.y, true);
            }
 /*           else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed TEMPERATURE GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }
*/
      //}
      return true;
}

bool GRIBOverlayFactory::RenderGribCurrentBmp()
{
      static wxPoint porg;
      static int width, height;    

      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
      if(Intersect(vp, pGRX->getLatMin(), pGRX->getLatMax(), pGRX->getLonMin(), pGRX->getLonMax(), 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, pGRX->getLatMin(), pGRX->getLatMax(), pGRX->getLonMin() - 360., pGRX->getLonMax() - 360., 0.) != _OUT)
            bdraw= true;
      
bdraw = true;
      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_current == NULL)
            {
                 wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);


                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....
			      double **currentDir = (double **) hash->Get(CURRENTU);
			      if(currentDir == NULL) return false;
			      
			      double **currentForce = (double **) hash->Get(CURRENTV);
			      if(currentForce == NULL) return false;
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;
			      
			      ::wxBeginBusyCursor();

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;
                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;

                                          double vx = gui->gribReader->grib2Data->getInterpolatedValue(currentDir,lon, lat, true);
                                          double vy = gui->gribReader->grib2Data->getInterpolatedValue(currentForce,lon, lat, true);


                                          if ((vx != GRIB_NOTDEF) && (vy != GRIB_NOTDEF))
                                          {
                                                double  vkn = sqrt(vx*vx+vy*vy)*3.6/1.852;
                                                wxColour c = GetSeaCurrentGraphicColor(vkn);
                                                unsigned char r = c.Red();
                                                unsigned char g = c.Green();
                                                unsigned char b = c.Blue();

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      //gr_image.SetAlpha(ipix + xp, jpix + yp, 220);
						      gr_image.SetAlpha(ipix + xp, jpix + yp, 128);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);
                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_current = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_current, wxColour(0,0,0));
                              m_pbm_current->SetMask(gr_mask);

                              ::wxEndBusyCursor();

                  }
            }


            if(m_pbm_current)
            {      
                  pmdc->DrawBitmap(*m_pbm_current, porg.x, porg.y, true);
            }
/*            else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed OCEAN CURRENT GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }
*/
//      }
      return true;
}

bool GRIBOverlayFactory::RenderGribCloudsBmp()
{
//      printf("renderGRIBSeaTemp\n");
      static wxPoint porg;
      static int width, height;

      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
     if(Intersect(vp, tlat, blat, tlon, blon, 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, blat, tlat, blon-360.0, tlon-360.0, 0.) != _OUT)
            bdraw= true;
 bdraw = true;

      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_clouds == NULL)
            {
                 wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);
		  
                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....

			      
			      double **clouds = (double **) hash->Get(CLOUD);
			      if(clouds == NULL) return false;
			      
			      ::wxBeginBusyCursor();
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;

                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;
					  double vh= gui->gribReader->grib2Data->getInterpolatedValue(clouds,lon, lat, true);

                                          if (vh != GRIB_NOTDEF)
                                          {
                                                wxColour c ;// GetTempGraphicColor(vh, 12.);                                                
                                                unsigned char r = 255;
                                                unsigned char g = 255;
                                                unsigned char b = 255;

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, vh*2.55);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
//                                                       gr_image.SetRGB(ipix + xp, jpix + yp, 0,0,0);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);

                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_clouds = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_clouds, wxColour(0,0,0));
                              m_pbm_clouds->SetMask(gr_mask);

                              ::wxEndBusyCursor();
                  }
            }


            if(m_pbm_clouds)
            {	
                  pmdc->DrawBitmap(*m_pbm_clouds, porg.x, porg.y, true);
            }
 /*           else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed TEMPERATURE GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }
*/
      //}
      return true;
}

bool GRIBOverlayFactory::RenderGribGustsBmp()
{
//      printf("renderGRIBSeaTemp\n");
      static wxPoint porg;
      static int width, height;

      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
     if(Intersect(vp, tlat, blat, tlon, blon, 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, blat, tlat, blon-360.0, tlon-360.0, 0.) != _OUT)
            bdraw= true;
 bdraw = true;

      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_Gusts == NULL)
            {
                 wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);
		  
                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....

			      
			      double **gusts = (double **) hash->Get(GUST);
			      if(gusts == NULL) return false;
			      
			      ::wxBeginBusyCursor();
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;

                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;
					  double vh = gui->gribReader->grib2Data->getInterpolatedValue(gusts,lon, lat, true);

                                          if (vh != GRIB_NOTDEF)
                                          {
                                                wxColour c = GetTempGraphicColor(vh);                                                
                                                unsigned char r = c.Red();
                                                unsigned char g = c.Green();
                                                unsigned char b = c.Blue();

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 128);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
//                                                       gr_image.SetRGB(ipix + xp, jpix + yp, 0,0,0);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);

                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_Gusts = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_Gusts, wxColour(0,0,0));
                              m_pbm_Gusts->SetMask(gr_mask);

                              ::wxEndBusyCursor();
                  }
            }


            if(m_pbm_Gusts)
            {	
                  pmdc->DrawBitmap(*m_pbm_Gusts, porg.x, porg.y, true);
            }
 /*           else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed TEMPERATURE GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }
*/
      //}
      return true;
}

bool GRIBOverlayFactory::RenderGribSwellBmp()
{
//      printf("renderGRIBSeaTemp\n");
      static wxPoint porg;
      static int width, height;

      //    Check two BBoxes....
      //    TODO Make a better Intersect method
/*      bool bdraw = false;
     if(Intersect(vp, tlat, blat, tlon, blon, 0.) != _OUT)
            bdraw= true;
      if(Intersect(vp, blat, tlat, blon-360.0, tlon-360.0, 0.) != _OUT)
            bdraw= true;
 bdraw = true;

      if(bdraw)
      {
*/
      // If needed, create the bitmap
            if(m_pbm_Swell == NULL)
            {
                 wxPoint pmin;
                  GetCanvasPixLL(vp,  &pmin, tlat, tlon);
                  wxPoint pmax;
                  GetCanvasPixLL(vp,  &pmax, blat, blon);

                  width = abs(pmax.x - pmin.x);
                  height = abs(pmax.y - pmin.y);

	          if(vp->pix_width < width) width = vp->pix_width;
		  if(vp->pix_height < height || vp->lat_max < blat) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, tlon);
		    height = vp->pix_height; 		    
		  }
		  if(vp->pix_width == width && vp->pix_height == height) 
		  { 
		    GetCanvasPixLL(vp,  &porg, vp->lat_max, vp->lon_min);
		  }
		  else	if(vp->pix_width != width && vp->pix_height != height)  
		    GetCanvasPixLL(vp,  &porg, blat, tlon);
		  
                  //    Dont try to create enormous GRIB bitmaps
 //                 if((width < 2000)  && (height < 2000))
                  {
                        //    This could take a while....

			      
			      double **swell = (double **) hash->Get(SWELL);
			      if(swell == NULL) return false;
			      
			      ::wxBeginBusyCursor();
			      
                              wxImage gr_image(width, height);
                              gr_image.InitAlpha();

                              int grib_pixel_size = 8;

                              wxPoint p;
                              for(int ipix = 0 ; ipix < (width + 1) ; ipix += grib_pixel_size)
                              {
                                    for(int jpix = 0 ; jpix < (height + 1) ; jpix += grib_pixel_size)
                                    {
                                          double lat, lon;
                                          p.x = ipix + porg.x;
                                          p.y = jpix + porg.y;

                                          GetCanvasLLPix( vp, p, &lat, &lon);
					  if(!PointInLLBox(vp, lon, lat) && !PointInLLBox(vp, lon-360.0, lat)) continue;
					  double vh = gui->gribReader->grib2Data->getInterpolatedValue(swell,lon, lat, true);

                                          if (vh != GRIB_NOTDEF)
                                          {
                                                wxColour c = GetSwellGraphicColor(vh);                                                
                                                unsigned char r = c.Red();
                                                unsigned char g = c.Green();
                                                unsigned char b = c.Blue();

                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
                                                      gr_image.SetRGB(ipix + xp, jpix + yp, r,g,b);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 128);

                                                }
                                          }
                                          else
                                          {
                                                for(int xp=0 ; xp < grib_pixel_size ; xp++)
                                                      for(int yp=0 ; yp < grib_pixel_size ; yp++)
                                                {
//                                                       gr_image.SetRGB(ipix + xp, jpix + yp, 0,0,0);
                                                      gr_image.SetAlpha(ipix + xp, jpix + yp, 0);

                                                }
                                          }
                                    }
                              }

                              wxImage bl_image = (gr_image.Blur(4));

                        //    Create a Bitmap
                              m_pbm_Swell = new wxBitmap(bl_image);
                              wxMask *gr_mask = new wxMask(*m_pbm_Swell, wxColour(0,0,0));
                              m_pbm_Swell->SetMask(gr_mask);

                              ::wxEndBusyCursor();
                  }
            }


            if(m_pbm_Swell)
            {	
                  pmdc->DrawBitmap(*m_pbm_Swell, porg.x, porg.y, true);
            }
 /*           else
            {
                  wxFont sfont = pmdc->GetFont();
                  wxFont mfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
                  pmdc->SetFont(mfont);

                  wxString msg = _("Please Zoom or Scale Out to view suppressed TEMPERATURE GRIB");
                  int w;
                  pmdc->GetTextExtent(msg, &w, NULL);
                  pmdc->DrawText(msg, vp->pix_width/2 - w/2, vp->pix_height/2);

                  pmdc->SetFont(sfont);
            }
*/
      //}
      return true;
}


bool GRIBOverlayFactory::RenderGribPressure()
{
      double **pressure = (double **) hash->Get(PRESSURE);
      if(pressure == NULL) return false;
			             
      //    Initialize the array of Isobars if necessary
      if(!m_IsobarArray.GetCount())
      {
            IsoLine *piso;
            for (double press=840; press<1120; press += 2/*isobarsStep*/)
            {
                  piso = new IsoLine(press, pressure, latSectors, lonSectors, tlat, tlon,incrLat,incrLon);
                  m_IsobarArray.Add(piso);
            }
      }

      //    Draw the Isobars
      for(unsigned int i = 0 ; i < m_IsobarArray.GetCount() ; i++)
      {
            IsoLine *piso = (IsoLine *)m_IsobarArray.Item(i);
            piso->drawIsoLine(*pmdc, vp, true, true/*g_bGRIBUseHiDef*/);
      } 
      
      return true;
}


void GRIBOverlayFactory::drawWindArrowWithBarbs(wxDC *pmdc,
                                      double i, double j, double vx, double vy,
                                      bool south,
                                      wxColour arrowColor
                                     )
{
//  if(j > 180.0f) { j = -(360.0f - j);  }

      double  vkn = vx*3.6/1.852;//sqrt(vx*vx+vy*vy)*3.6/1.852;

      double ang = (vy-90) * (PI/180);//atan2(vy, -vx);
      double si=sin(ang),  co=cos(ang);

      wxPen pen( arrowColor, 2);
      pmdc->SetPen(pen);
      pmdc->SetBrush(*wxTRANSPARENT_BRUSH);

      if (vkn < 1)
      {
            int r = 5;     // wind is very light, draw a circle
            pmdc->DrawCircle(i,j,r);
      }
      else {
        // Arrange for arrows to be centered on origin
            int windBarbuleSize = 26;
            int dec = -windBarbuleSize/2;
            drawTransformedLine(pmdc, pen, si,co, i,j,  dec,0,  dec+windBarbuleSize, 0);   // hampe
            drawTransformedLine(pmdc, pen, si,co, i,j,  dec,0,  dec+5, 2);    // flèche
            drawTransformedLine(pmdc, pen, si,co, i,j,  dec,0,  dec+5, -2);   // flèche

            int b1 = dec+windBarbuleSize -4;  // position de la 1ère barbule
            if (vkn >= 7.5  &&  vkn < 45 ) {
                  b1 = dec+windBarbuleSize;  // position de la 1ère barbule si >= 10 noeuds
            }

            if (vkn < 7.5) {  // 5 ktn
                  drawPetiteBarbule(pmdc, pen, south, si,co, i,j, b1);
            }
            else if (vkn < 12.5) { // 10 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
            }
            else if (vkn < 17.5) { // 15 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
                  drawPetiteBarbule(pmdc, pen, south, si,co, i,j, b1-4);
            }
            else if (vkn < 22.5) { // 20 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-4);
            }
            else if (vkn < 27.5) { // 25 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-4);
                  drawPetiteBarbule(pmdc, pen, south, si,co, i,j, b1-8);
            }
            else if (vkn < 32.5) { // 30 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-4);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-8);
            }
            else if (vkn < 37.5) { // 35 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-4);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-8);
                  drawPetiteBarbule(pmdc, pen, south, si,co, i,j, b1-12);
            }
            else if (vkn < 45) { // 40 ktn
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-4);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-8);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-12);
            }
            else if (vkn < 55) { // 50 ktn
                  drawTriangle(pmdc, pen, south, si,co, i,j, b1-4);
            }
            else if (vkn < 65) { // 60 ktn
                  drawTriangle(pmdc, pen, south, si,co, i,j, b1-4);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-8);
            }
            else if (vkn < 75) { // 70 ktn
                  drawTriangle(pmdc, pen, south, si,co, i,j, b1-4);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-8);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-12);
            }
            else if (vkn < 85) { // 80 ktn
                  drawTriangle(pmdc, pen, south, si,co, i,j, b1-4);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-8);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-12);
                  drawGrandeBarbule(pmdc, pen, south, si,co, i,j, b1-16);
            }
            else { // > 90 ktn
                  drawTriangle(pmdc, pen, south, si,co, i,j, b1-4);
                  drawTriangle(pmdc, pen, south, si,co, i,j, b1-12);
            }

      }
}

void GRIBOverlayFactory::drawWaveArrow(wxDC *pmdc, int i, int j, double ang, wxColour arrowColor)
{
      double si=sin(ang * PI / 180.),  co=cos(ang * PI / 180.);

      wxPen pen( arrowColor, 1);
      pmdc->SetPen(pen);
      pmdc->SetBrush(*wxTRANSPARENT_BRUSH);

      int arrowSize = 26;
      int dec = -arrowSize/2;


      drawTransformedLine(pmdc, pen, si,co, i,j,  dec,-2,  dec + arrowSize, -2);
      drawTransformedLine(pmdc, pen, si,co, i,j,  dec, 2,  dec + arrowSize, +2);

      drawTransformedLine(pmdc, pen, si,co, i,j,  dec-2,  0,  dec+5,  6);    // flèche
      drawTransformedLine(pmdc, pen, si,co, i,j,  dec-2,  0,  dec+5, -6);   // flèche

}

void GRIBOverlayFactory::drawTransformedLine( wxDC *pmdc, wxPen pen,
                                    double si, double co,int di, int dj, int i,int j, int k,int l)
{
      int ii, jj, kk, ll;
      ii = (int) (i*co-j*si +0.5) + di;
      jj = (int) (i*si+j*co +0.5) + dj;
      kk = (int) (k*co-l*si +0.5) + di;
      ll = (int) (k*si+l*co +0.5) + dj;

#if wxUSE_GRAPHICS_CONTEXT
      if(0/*g_bGRIBUseHiDef*/)
      {
       ;  /*   if(m_pgc)
            {
                  m_pgc->SetPen(pen);
                  m_pgc->StrokeLine(ii, jj, kk, ll);
            }*/
      }
      else
      {
            pmdc->SetPen(pen);
            pmdc->SetBrush(*wxTRANSPARENT_BRUSH);
            pmdc->DrawLine(ii, jj, kk, ll);
      }

#else
      pmdc->SetPen(pen);
      pmdc->SetBrush(*wxTRANSPARENT_BRUSH);
      pmdc->DrawLine(ii, jj, kk, ll);
#endif

}


void GRIBOverlayFactory::drawPetiteBarbule(wxDC *pmdc, wxPen pen, bool south,
                                 double si, double co, int di, int dj, int b)
{
      if (south)
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b,0,  b+2, -5);
      else
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b,0,  b+2, 5);
}

void GRIBOverlayFactory::drawGrandeBarbule(wxDC *pmdc, wxPen pen, bool south,
                                 double si, double co, int di, int dj, int b)
{
      if (south)
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b,0,  b+4,-10);
      else
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b,0,  b+4,10);
}


void GRIBOverlayFactory::drawTriangle(wxDC *pmdc, wxPen pen, bool south,
                            double si, double co, int di, int dj, int b)
{
      if (south) {
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b,0,  b+4,-10);
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b+8,0,  b+4,-10);
      }
      else {
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b,0,  b+4,10);
            drawTransformedLine(pmdc, pen, si,co, di,dj,  b+8,0,  b+4,10);
      }
}

// Is the given point in the vp ??
bool PointInLLBox(PlugIn_ViewPort *vp, double x, double y)
{


    if (  x >= (vp->lon_min) && x <= (vp->lon_max) &&
            y >= (vp->lat_min) && y <= (vp->lat_max) )
            return TRUE;
    return FALSE;
}

wxColour GRIBOverlayFactory::GetTempGraphicColor(double val)
{
      wxColour c;
      if((val < -10) )      		      	c.Set(_T("#002ad9"));
      else if((val >= -10)&& (val < -5))   	c.Set(_T("#006ed9"));
      else if((val >= -5) && (val < 0))   	c.Set(_T("#00b2d9"));
      else if((val >= 0)  && (val < 3))   	c.Set(_T("#00d4d4"));
      else if((val >= 3)  && (val < 6))   	c.Set(_T("#00d950"));
      else if((val >= 6)  && (val < 9))   	c.Set(_T("#00d900"));
      else if((val >= 9)  && (val < 12))   	c.Set(_T("#95d900"));
      else if((val >= 12) && (val < 15))  	c.Set(_T("#d9d900"));
      else if((val >= 15) && (val < 18))  	c.Set(_T("#d9ae00"));
      else if((val >= 18) && (val < 21))  	c.Set(_T("#d98300"));
      else if((val >= 21) && (val < 24)) 	c.Set(_T("#d95700"));
      else if((val >= 24) && (val < 27))  	c.Set(_T("#d90000"));
      else if((val >= 27) && (val < 30))  	c.Set(_T("#870000"));
      else if((val >= 30) && (val < 33))  	c.Set(_T("#8c0000"));
      else if((val >= 33) && (val < 36))  	c.Set(_T("#9000A0"));
      else if((val >= 36) && (val < 39)) 	c.Set(_T("#9900FF"));      
      else if((val >= 39) && (val < 42))  	c.Set(_T("#F000FF"));
      else if( val >= 42)                 	c.Set(_T("#FF00FF"));
      
      return c;
/*      
      double val = val_in;// - 273.0;
      wxColour c;
      wxColour initialColour;
      initialColour.Set(_T("#002ad9"));      
      wxColour destColour;
      destColour.Set(_T("#ff0000"));      
      wxUint8 nR, nG, nB; 
      wxUint8 nR1 = destColour.Red();
      wxUint8 nG1 = destColour.Green();
      wxUint8 nB1 = destColour.Blue();
      wxUint8 nR2 = initialColour.Red();
      wxUint8 nG2 = initialColour.Green();
      wxUint8 nB2 = initialColour.Blue();
            wxInt32 x = val;//rect.GetWidth();
            wxInt32 w = val;	// width of area to shade
            wxInt32 xDelta = val/256;//dwColorShades;	//height of one shade bend
                if (xDelta > 1)
                    xDelta = 1;

                while (x >= xDelta)
                {
                    x -= xDelta;
		    nR = (0.0 - val)*nR1 + val*nR2;
		    nG = (0.0 - val)*nG1 + val*nG2;
		    nB = (0.0 - val)*nB1 + val*nB2;
                        nR = nR2 + ((nR1-nR2))*(256/val);//(w-x)/w;
			nG = nG2 + ((nG1-nG2))*(256/val);//(w-x)/w;
                        nB = nB2 + ((nB1-nB2))*(256/val);//(w-x)/w;	
			printf("%f %i %f nR=%i nR2=%i nG=%i nG2=%i nB=%i nB2=%i\n",val,nR1-nR2,((nR1-nR2)/val)*val,nR,nR2,nG,nG2,nB,nB2);
                   if (nR1 > nR2)
                        nR = nR1 - (nR1-nR2)*(w-x)/w;
                    else
                        nR = nR1 + (nR2-nR1)*(w-x)/w;

                    if (nG1 > nG2)
                        nG = nG1 - (nG1-nG2)*(w-x)/w;
                    else
                        nG = nG1 + (nG2-nG1)*(w-x)/w;

                    if (nB1 > nB2)
                        nB = nB1 - (nB1-nB2)*(w-x)/w;
                    else
                        nB = nB1 + (nB2-nB1)*(w-x)/w;
		    
                }
return wxColour(nR,nG,nB);*/
}

wxColour GRIBOverlayFactory::GetWaveGraphicColor(double val)
{
      wxColour c;
      if((val == 0) )      		 	c.Set(_T("#0000ff"));
      else if((val >= 0.0) && (val < 0.5))   	c.Set(_T("#006ed9"));
      else if((val >= 0.5) && (val < 1.0))   	c.Set(_T("#00b2d9"));
      else if((val >= 1.0) && (val < 1.5))   	c.Set(_T("#00d4d4"));
      else if((val >= 1.5) && (val < 2.0))   	c.Set(_T("#00d950"));
      else if((val >= 2.0) && (val < 2.5))   	c.Set(_T("#00d900"));
      else if((val >= 2.5) && (val < 3.0))   	c.Set(_T("#95d900"));
      else if((val >= 3.0) && (val < 3.5))  	c.Set(_T("#d9d900"));
      else if((val >= 3.5) && (val < 4.0))  	c.Set(_T("#d9ae00"));
      else if((val >= 4.0) && (val < 4.5))  	c.Set(_T("#d98300"));
      else if((val >= 4.5) && (val < 5.0)) 	c.Set(_T("#d95700"));
      else if((val >= 5.0) && (val < 5.5))  	c.Set(_T("#d90000"));
      else if((val >= 5.5) && (val < 6.0))  	c.Set(_T("#ae0000"));
      else if((val >= 6.0) && (val < 6.5))  	c.Set(_T("#8c0000"));
      else if((val >= 6.5) && (val < 7.0))  	c.Set(_T("#870000"));
      else if((val >= 7.0) && (val < 7.5)) 	c.Set(_T("#690000"));
      else if((val >= 7.5) && (val < 8.0))  	c.Set(_T("#550000"));
      else if( val >= 8.0)                 	c.Set(_T("#410000"));
      
      return c;
}

wxColour GRIBOverlayFactory::GetSwellGraphicColor(double val)
{
      wxColour c;
      if((val == 0) )      		 	c.Set(_T("#0000ff"));
      else if((val >= 0.0) && (val < 0.2))   	c.Set(_T("#006ed9"));
      else if((val >= 0.2) && (val < 0.4))   	c.Set(_T("#00b2d9"));
      else if((val >= 0.4) && (val < 0.6))   	c.Set(_T("#00d4d4"));
      else if((val >= 0.6) && (val < 0.8))   	c.Set(_T("#00d950"));
      else if((val >= 0.8) && (val < 1.0))   	c.Set(_T("#00d900"));
      else if((val >= 1.0) && (val < 1.2))   	c.Set(_T("#95d900"));
      else if((val >= 1.2) && (val < 1.4))  	c.Set(_T("#d9d900"));
      else if((val >= 1.4) && (val < 1.6))  	c.Set(_T("#d9ae00"));
      else if((val >= 1.6) && (val < 1.8))  	c.Set(_T("#d98300"));
      else if((val >= 1.8) && (val < 2.0)) 	c.Set(_T("#d95700"));
      else if((val >= 2.0) && (val < 2.2))  	c.Set(_T("#d90000"));
      else if((val >= 2.2) && (val < 2.5))  	c.Set(_T("#ae0000"));
      else if((val >= 2.5) && (val < 2.7))  	c.Set(_T("#8c0000"));
      else if((val >= 2.7) && (val < 2.9))  	c.Set(_T("#870000"));
      else if((val >= 2.9) && (val < 3.1)) 	c.Set(_T("#690000"));
      else if((val >= 3.1) && (val < 3.3))  	c.Set(_T("#550000"));
      else if( val >= 3.3)                 	c.Set(_T("#410000"));
      
      return c;
}

wxColour GRIBOverlayFactory::GetSeaCurrentGraphicColor(double val_in)
{
      //    HTML colors taken from NOAA WW3 Web representation

      double val = val_in;
     // val *= 50. / 2.;

      val = wxMax(val, 0.0);

      wxColour c;
      if((val >= 0) && (val < .25))            c.Set(_T("#002ad9"));
      else if((val >= 0.25) && (val < 0.50))   c.Set(_T("#006ed9"));
      else if((val >= 0.50) && (val < 1.00))   c.Set(_T("#00b2d9"));
      else if((val >= 1.00) && (val < 1.25))   c.Set(_T("#00d4d4"));
      else if((val >= 1.25) && (val < 1.50))   c.Set(_T("#00d9a6"));
      else if((val >= 1.50) && (val < 1.75))   c.Set(_T("#00d900"));
      else if((val >= 1.75) && (val < 2.00))   c.Set(_T("#95d900"));
      else if((val >= 2.00) && (val < 2.25))   c.Set(_T("#d9d900"));
      else if((val >= 2.25) && (val < 2.50))   c.Set(_T("#d9ae00"));
      else if((val >= 2.50) && (val < 2.75))   c.Set(_T("#d98300"));
      else if((val >= 2.75) && (val < 3.00))   c.Set(_T("#d95700"));
      else if((val >= 3.00) && (val < 3.25))   c.Set(_T("#d90000"));
      else if((val >= 3.25) && (val < 3.50))   c.Set(_T("#ae0000"));
      else if((val >= 3.50) && (val < 3.75))   c.Set(_T("#8c0000"));
      else if((val >= 3.75) && (val < 4.00))   c.Set(_T("#870000"));
      else if((val >= 4.00) && (val < 4.25))   c.Set(_T("#690000"));
      else if((val >= 4.25) && (val < 4.50))   c.Set(_T("#550000"));
      else if( val >= 4.50)                    c.Set(_T("#410000"));

      return c;
}


// Calculates if two boxes intersect. If so, the function returns _ON.
// If they do not intersect, two scenario's are possible:
// other is outside this -> return _OUT
// other is inside this -> return _IN
OVERLAP Intersect(PlugIn_ViewPort *vp,
       double lat_min, double lat_max, double lon_min, double lon_max, double Marge)
{

    if (((vp->lon_min - Marge) > (lon_max + Marge)) ||
         ((vp->lon_max + Marge) < (lon_min - Marge)) ||
         ((vp->lat_max + Marge) < (lat_min - Marge)) ||
         ((vp->lat_min - Marge) > (lat_max + Marge)))
        return _OUT;

    // Check if other.bbox is inside this bbox
    if ((vp->lon_min <= lon_min) &&
         (vp->lon_max >= lon_max) &&
         (vp->lat_max >= lat_max) &&
         (vp->lat_min <= lat_min))
        return _IN;

    // Boundingboxes intersect
    return _ON;
}

