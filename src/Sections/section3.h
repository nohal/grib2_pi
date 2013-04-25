#ifndef SECTION3_H
#define SECTION3_H

#include "wx/wx.h"
#include "wx/wfstream.h"
#include <wx/datstrm.h>
#include "../grib_reader.h"

struct templ_3_0
{
								//Grid definition template 3.0 - latitude/longitude (or equidistant cylindrical, or Plate Carrée)
								//Octet No.		Contents
wxUint8  shapeEarth;			//15			Shape of the Earth (see Code table 3.2)
wxUint8  scaleFactor;			//16			Scale factor of radius of spherical Earth
wxUint32 scaledValue;			//17-20			Scaled value of radius of spherical Earth
wxUint8  scaleFactorMajorAxis;	//21			Scale factor of major axis of oblate spheroid Earth
wxUint32 scaledValueMajorAxis;	//22-25			Scaled value of major axis of oblate spheroid Earth
wxUint8  scaleFactorMinorAxis;	//26			Scale factor of minor axis of oblate spheroid Earth
wxUint32 scaledValueMinorAxis;	//27-30			Scaled value of minor axis of oblate spheroid Earth
wxUint32 noPointsParallel;		//31-34			Ni - number of points along a parallel
wxUint32 noPointsMeridian;		//35-38			Nj - number of points along a meridian
wxUint32 basicAngle;			//39-42			Basic angle of the initial production domain (see Note 1)
wxUint32 subdivisionBasicAngle;	//43-46			Subdivisions of basic angle used to define extreme longitudes and latitudes, and direction
								//				increments (see Note 1)
wxInt32  firstGridPointLat;		//47-50			La1 - latitude of first grid point (see Note 1)
wxInt32  firstGridPointLong;	//51-54			Lo1 - longitude of first grid point (see Note 1)
wxUint8  resCompFlag;			//55			Resolution and component flags (see Flag table 3.3)
wxInt32  lastGridPointLat;		//56-59			La2 - latitude of last grid point (see Note 1)
wxInt32  lastGridPointLong;		//60-63			Lo2 - longitude of last grid point (see Note 1)
wxUint32 iDirectionIncr;		//64-67			Di - i direction increment (see Note 1)
wxUint32 jDirectionIncr;		//68-71			Dj - j direction increment (see Note 1)
wxUint8  scanMode;				//72			Scanning mode (flags - see Flag table 3.4)
								//73-nn			List of number of points along each meridian or parallel. (These octets are only present for
								//quasi-regular grids as described in Notes 2 and 3)

};

class Section3 {

public:
	Section3(wxFileInputStream *stream, Grib2Message *message);
	~Section3();

	int readSection();
	
private:
	wxFileInputStream *stream;
	Grib2Message *message;	
	
	void readTemplate_3_0(wxDataInputStream *data, templ_3_0 *tpl);			
};

#endif // SECTION3_H
