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

//#include "complex.h"

#include "grib2data.h"
#include "grib_reader.h"

Grib2Data::Grib2Data(GribReader *r)
{
    this->reader = r;
    hash = new wxHashTable(wxKEY_INTEGER);    
}

Grib2Data::~Grib2Data()
{
  deleteAllRecords();
  hash->DeleteContents(true);
}

void Grib2Data::deleteAllRecords()
{
	wxHashTable_Node *node;

	hash->BeginFind();
	while( (node = hash->Next()) )
	{ 
		double **d = (double**) node->GetData();

		delete []d[0];
		delete []d;
	}
}

void Grib2Data::addRecord(Grib2Message* message)
{
	this->message = message;
	long field = -1;
	wxDouble* data;

	if(message->discipline == 0)		//METEOROLOGICAL
	{
		if(message->paramCategory == 2 && message->paramNo == 0) 	// WindDirection
			field = WINDDIR;
		if(message->paramCategory == 2 && message->paramNo == 1) 	// WindForce
			field = WIND;       
		if(message->paramCategory == 0 && message->paramNo == 0) 	// Temperature
			field = TEMP;
		if(message->paramCategory == 3 && message->paramNo == 0) 	// Pressure
			field = PRESSURE; 
		if(message->paramCategory == 6 && message->paramNo == 1) 	// Clouds
			field = CLOUD; 
		if(message->paramCategory == 2 && message->paramNo == 22) // Gusts
			field = GUST;   
		//      if(message->paramCategory == 6 && message->paramNo == 1) 	// Rain
		//	field = RAIN;       
	}

	if(message->discipline == 10)		// Oceanographical
	{    
		if(message->paramCategory == 0 && message->paramNo == 3) 		// WaveHeight
			field = WAVEHIGHT;
		if(message->paramCategory == 0 && message->paramNo == 4) 		// WaveDirection
			field = WAVEDIR; 
		if((message->paramCategory == 0 && message->paramNo == 8 && message->scaledValueSurface1==1) ||
			(message->paramCategory == 0 && message->paramNo == 8))	// SwellMin
			field = SWELL; 
		if(message->paramCategory == 0 && message->paramNo == 8 && message->scaledValueSurface1==2) 		// SwellMin
			field = SWELLMAX;      
		if(message->paramCategory == 1 && message->paramNo == 2) 		// CurrentU
			field = CURRENTU;   
		if(message->paramCategory == 1 && message->paramNo == 3) 		// CurrentV
			field = CURRENTV;             
	} 

	if(field != -1)
	{
		data =(wxDouble *)calloc(message->noSectors ? message->noSectors : 1,sizeof(wxDouble));
		for(unsigned int i = 0; i < message->noSectors; i++)
		{
			switch(field)
			{
			case TEMP:	data[i] = message->data[i]-273.15;
				break;
			case PRESSURE:data[i] = message->data[i]/100;
				break;
			default:	data[i] = message->data[i];
				break;
			}
		}  

		int y = message->noPointsMeridian, x =message->noPointsParallel;
		double** grid = new double* [y];
		grid[0] = new double [y*x];
		for (int i = 1; i < y; ++i)
			grid[i] = grid[i-1] + x;

		unsigned int count = 0;
		int bit = 7;
		unsigned int index;
		index = 0;

		for(unsigned int lat = 0; lat < message->noPointsMeridian; lat++)
		{
			for(unsigned int lon = 0; lon < message->noPointsParallel; lon++)
			{
				if(message->bmpMask)
				{
					if(isInBmpMask(&index,&bit))
					{
						grid[lat][lon] = data[count++];
					}
					else
						grid[lat][lon] = GRIB_NOTDEF;

					bit--;
				}
				else
				{
					grid[lat][lon] = data[count++];
				}
			}
		}

		index = 0; bit = 7;
/*		for(unsigned int lat = 0; lat < message->noPointsMeridian;)
		{
			for(unsigned int lon = 0; lon < message->noPointsParallel; )
			{
				if(isInBmpMask(&index,&bit))
				{
					if(lat+2 < message->noPointsMeridian) 
					{
						if(grid[lat+1][lon] == GRIB_NOTDEF && grid[lat+2][lon] != GRIB_NOTDEF)
						{
							grid[lat+1][lon] = grid[lat][lon];// + grid[lat+2][lon])/2;
							//grid[lat+1][lon+1] = (grid[lat][lon] + grid[lat+2][lon])/2;
						}
					}	      
					if(lon+2 < message->noPointsParallel && grid[lat][lon+2] != GRIB_NOTDEF) 
					{
						if(grid[lat][lon+1] == GRIB_NOTDEF)
						{
							grid[lat][lon+1] = grid[lat][lon];// + grid[lat][lon+2])/2;
						}		
					}
				}
				bit--;

				if(lon + 1 == message->noPointsParallel || lat + 1 == message->noPointsMeridian) { index++; break; } 
				grid[lat][lon+1]   = grid[lat][lon];
				grid[lat+1][lon]   = grid[lat][lon];
				grid[lat+1][lon+1] = grid[lat][lon];
				lon += 2;
				bit--;
			}
			bit--;	  
			lat += 2;
		}
*/
/*		unsigned int lat = 0, lon = 0;
		if(message->bmpMask)
		{
			for(count = 0; count < message->bmpSize*8; count++)
			{
				if(isInBmpMask(&index,&bit))
				{
					grid[lat][lon+1]   = grid[lat][lon];
					grid[lat+1][lon]   = grid[lat][lon];
					grid[lat+1][lon+1] = grid[lat][lon];
				}

				bit--;
				lon++;
				if(lon == message->noPointsParallel) { lat++; lon = 0; }
				if(lat == message->noPointsMeridian) break;
*/				
				unsigned int lat = 0, lon = 0;
				if(message->bmpMask)
				{
					for(count = 0; count < (unsigned int)message->bmpSize*8; count++)
					{
						if(isInBmpMask(&index,&bit))
						{
							if( lon+2 < message->noPointsParallel )
								if(grid[lat][lon+2] != GRIB_NOTDEF && grid[lat][lon] != GRIB_NOTDEF && grid[lat][lon+1] == GRIB_NOTDEF)
									grid[lat][lon+1]  = (grid[lat][lon] + grid[lat][lon+2])/2;
						}
						bit--;
						lon++;
						if(lon == message->noPointsParallel) { lat++; lon = 0; }
					//	if(count > 120000)
						//	wxMessageBox(wxString::Format(_T("%d %d %d %f %f %f"),count,lat,lon,grid[lat][lon],grid[lat][lon+1],grid[lat][lon+1]));
						if(lat == message->noPointsMeridian) break;
					}
					for(count = 0,index = 0,bit=7,lat=0,lon=0; count < message->bmpSize*8; count++)
					{
	//					if(isInBmpMask(&index,&bit))
	//					{
							if( lat+2 < message->noPointsMeridian )
								if(grid[lat+2][lon] != GRIB_NOTDEF && grid[lat][lon] != GRIB_NOTDEF && grid[lat+1][lon] == GRIB_NOTDEF )
									grid[lat+1][lon]  =  (grid[lat][lon] + grid[lat+2][lon])/2;
	//					}

	//					bit--;
					//		if(count < 100)
						//		wxMessageBox(wxString::Format(_T("%d %f %f %f\n"),count,lat,lon,grid[lat][lon]));
						lon++;
						if(lon == message->noPointsParallel) { lat++; lon = 0; }
						if(lat == message->noPointsMeridian) break;
					}      
				}

				if(!hash->Get(field))
					hash->Put(field,(wxObject*)grid); 
				if(data != NULL)
					free(data);      
			}
		}
	
	

bool Grib2Data::isInBmpMask(unsigned int *index, int* bit)
{
   if(*bit < 0) 
    { *bit = 7; *index = *index+1; }
    
   if(message->bmpMask[*index] & (1 << *bit)) 
   {
	 //wxLogMessage(_T("%d %d %d"),*index,*bit,1);
     return true;
   }
   else
   {
	// wxLogMessage(_T("%d %d %d"),*index,*bit,0);
     return false;
   }
}


double Grib2Data::getInterpolatedValue(double** grid,double px, double py, bool numericalInterpolation) const
{
    double val;

 //   if (!isPointInMap(px,py)) return GRIB_NOTDEF;   
    if (!isPointInMap(px,py)) {
        px += 360.0;               // tour du monde à droite ?
        if (!isPointInMap(px,py)) {
            px -= 2*360.0;              // tour du monde à gauche ?
            if (!isPointInMap(px,py)) {
                return GRIB_NOTDEF;
            }
        }
    }

    double pi, pj;     // coord. in grid unit
    pi = (px-message->firstGridPointLong)/message->iDirectionIncr;//(px-Lo1)/Di;
    pj = (py-message->firstGridPointLat)/message->jDirectionIncr;//(py-La1)/Dj;
    
    // 00 10      point is in a square
    // 01 11
    int i0 = (int) pi;  // point 00
    int j0 = (int) pj;
    
    // distances to 00
    double dx = pi-i0;
    double dy = pj-j0;
    
   // i0 = message->noPointsParallel-2-i0;  // long
   // j0 = message->noPointsMeridian-2-j0;  // lat;

    bool h00,h01,h10,h11;
    int nbval = 0;     // how many values in grid ?
    if ((h00=hasValue(grid,message->noPointsMeridian, message->noPointsParallel, i0,   j0)))
        nbval ++;
    if ((h10=hasValue(grid,message->noPointsMeridian, message->noPointsParallel, i0+1, j0)))
        nbval ++;
    if ((h01=hasValue(grid,message->noPointsMeridian, message->noPointsParallel, i0,   j0+1)))
        nbval ++;
    if ((h11=hasValue(grid,message->noPointsMeridian, message->noPointsParallel, i0+1, j0+1)))
        nbval ++;

    if (nbval <3) {
        return GRIB_NOTDEF;
    }
//printf("nbval=%i h00=%i h10=%i h01=%i h11=%i\n",nbval,h00,h10,h01,h11);


	if (! numericalInterpolation)
	{
		if (dx < 0.5) {
			if (dy < 0.5)
				val = grid[j0][   i0];
			else
				val = grid[j0+1][ i0];
		}
		else {
			if (dy < 0.5)
				val = grid[  j0][i0+1];
			else
				val = grid[j0+1][i0+1];
		}
//		printf("%i %i %f\n",j0,i0,val);
		return val;
	}

    dx = (3.0 - 2.0*dx)*dx*dx;   // pseudo hermite interpolation
    dy = (3.0 - 2.0*dy)*dy*dy;

    double xa, xb, xc, kx, ky;
    // Triangle :
    //   xa  xb
    //   xc
    // kx = distance(xa,x)
    // ky = distance(xa,y)
    if (nbval == 4)
    {
        int n=1;

        double x00 = grid[j0]  [i0  ];		// x01 x11
        double x01 = grid[j0+n]  [i0];		// x00 x10
        double x10 = grid[j0][i0+n  ];
        double x11 = grid[j0+n][i0+n];
	if(x10 - x11 > 90 ) x11 += 360.0;
	else if(x11 - x10 > 90 ) x10 += 360.0;
	if(x00 - x01 > 90 ) x01 += 360.0;
	else if(x01 - x00 > 90 ) x00 += 360.0;
	if(x00 - x10 > 90 ) x10 += 360.0;
	else if(x10 - x00 > 90 ) x00 += 360.0;
	if(x01 - x11 > 90 ) x11 += 360.0;
	else if(x11 - x01 > 90 ) x01 += 360.0;
	
        double x1 = (1.0-dx)*x00 + dx*x10;
        double x2 = (1.0-dx)*x01 + dx*x11;

        val =  (1.0-dy)*x1 + dy*x2;
/*	printf("j0=%i i0=%i dx=%f dy=%f\nx00=%f x01=%f\nx10=%f x11=%f\n x1=%f x2=%f\nval=%f\n",j0,i0,dx,dy,x00,x01,x10,x11,x1,x2,val);
	printf("1-dx=%f * x00(%f)=%f\n",1.0-dx,x00,(1.0-dx)*x00);
	printf("dx(%f) * x10(%f)=%f\n",dx,x10,dx*x10);
	printf("1-dy=%f * x1(%f) = %f\n",1-dy,x1,(1-dy)*x1);
	printf("dy(%f) * x2(%f)=%f\n\ndx+dy=%f\nval=%f\n\n",dy,x2,dy*x2,dx+dy,val);
*/	
//printf("dx=%f dy=%f lat=%f lon=%f\n",dx,dy,px,py);
        return val;
    }
    else {
      return GRIB_NOTDEF;
        // here nbval==3, check the corner without data
	// 00 10      point is in a square
	// 01 11	        
        if (!h00) {
            printf("! h00  %f %f %i %i\n", dx,dy,j0,i0);
            xa = grid[j0][i0+1];     // A = point 11        |
            xb = grid[j0][  i0];     // B = point 01	   --
            xc = grid[j0+1][i0+1];   // C = point 10
            kx = 1-dx;
            ky = 1-dy;
	    
        }
        else if (!h01) {
            printf("! h01  %f %f\n", dx,dy);
            xa = grid[j0+1][ i0];     // A = point 10	    --
            xb = grid[j0+1][ i0+1];   // B = point 11	      |
            xc = grid[j0][   i0];     // C = point 00
            kx = dy;
            ky = 1-dx;
        }
        else if (!h10) {
            printf("! h10  %f %f\n", dx,dy);
            xa = grid[j0][   i0+1];     // A = point 01      |
            xb = grid[j0][   i0];       // B = point 00       --
            xc = grid[j0+1][ i0+1];     // C = point 11
            kx = 1-dy;
            ky = dx;    
        }
        else {
            printf("! h11  %f %f\n", dx,dy);
            xa = grid[j0][   i0];    // A = point 00	      --
            xb = grid[j0+1][ i0];    // B = point 10         |
            xc = grid[j0][   i0+1];  // C = point 01
            kx = dx;
            ky = dy;
        }
    }
    double k = kx + ky;
    if (k<0 || k>1) {
        val = GRIB_NOTDEF;
    }
    else if (k == 0) {
        val = xa;
    }
    else {
        // axes interpolation
        double vx = k*xb + (1-k)*xa;
        double vy = k*xc + (1-k)*xa;
        // diagonal interpolation
        double k2 = kx / k;
        val =  k2*vx + (1-k2)*vy;
    }
  //  printf("ret %f\n",val);
    return val;
}

bool Grib2Data::isPointInMap(double x, double y) const
{
    return isXInMap(x) && isYInMap(y);
/*    if (Dj < 0)
        return x>=Lo1 && y<=La1 && x<=Lo1+(Ni-1)*Di && y>=La1+(Nj-1)*Dj;
    else
        return x>=Lo1 && y>=La1 && x<=Lo1+(Ni-1)*Di && y<=La1+(Nj-1)*Dj;*/
}

bool Grib2Data::isXInMap(double x) const
{
  wxDouble lastlon;

  if(message->firstGridPointLong > 180. && message->lastGridPointLong < 180.0)  
    lastlon  = message->lastGridPointLong+360;  else lastlon  = message->lastGridPointLong;

    if (message->iDirectionIncr > 0)
        return x>=message->firstGridPointLong && x<=lastlon;
    else
        return x>=lastlon && x<=message->firstGridPointLong;
}

bool Grib2Data::isYInMap(double y) const
{
    if (message->jDirectionIncr < 0)
        return y<=message->firstGridPointLat && y>=message->lastGridPointLat;
    else
        return y>=message->firstGridPointLat && y<=message->lastGridPointLat; 
}

bool Grib2Data::hasValue(double** grid, wxUint32 nolat, wxUint32 nolon, unsigned int indexlon, unsigned int indexlat) const
{
  if((indexlat >= 0 && indexlat < nolat) && (indexlon >= 0 && indexlon < nolon))
    if(grid[indexlat][indexlon] != GRIB_NOTDEF)
      return true;
    else
      return false;
  else
    return false;
}


