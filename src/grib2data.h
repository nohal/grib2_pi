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


#ifndef GRIB2DATA_H
#define GRIB2DATA_H

#define GRIB_NOTDEF -999999999

#include <wx/hash.h>

#include "grib_reader.h"


enum FIELDS{WIND,WINDDIR,WINDWAVE,WAVEHIGHT,WAVEDIR,SWELL,SWELLMAX,SWELLDIR,CURRENT,CURRENTDIR,CURRENTU,CURRENTV,
	    PRESSURE,GUST,TEMP,CLOUD,RAIN};
class Grib2Data
{
public:
    Grib2Data(GribReader *r);
    ~Grib2Data();
    
    void addRecord(Grib2Message *grib2message);
    bool isInBmpMask(unsigned int *index, int *bit);
    void deleteAllRecords();
//    void addRecord(wxUint32 discipline, wxUint16 category, wxUint16 parameter, wxFloat32 *data);    
    double getInterpolatedValue(double** arr,double px, double py, bool numericalInterpolation) const;
    
    bool isPointInMap(double x, double y) const;
    bool isXInMap(double x) const;  
    bool isYInMap(double y) const;     
    bool hasValue(double** grid, wxUint32 nolat, wxUint32 nolon, unsigned int i, unsigned int j) const;

    
private:
    GribReader *reader;
    Grib2Message *message;
    
public:
    wxHashTable *hash;
    
};


 
#endif // GRIB2DATA_H
