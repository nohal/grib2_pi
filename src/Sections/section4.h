#ifndef SECTION4_H
#define SECTION4_H

#include "wx/wx.h"
#include "wx/wfstream.h"
#include <wx/datstrm.h>

#include "../grib_reader.h"

struct templ_4_0
{
								//Product definition template 4.0 - analysis or forecast at a horizontal level or in a horizontal layer
								//at a point in time
								//Octet No.	Contents
wxUint8  paramCategory;			//10		Parameter category (see Code table 4.1)
wxUint8  paramNo;				//11		Parameter number (see Code table 4.2)
wxUint8  typeOfProcess; 		//12		Type of generating process (see Code table 4.3)
wxUint8  backgroundProcessID;	//13		Background generating process identifier (defined by originating centre)
wxUint8  analysisForcastID;		//14		Analysis or forecast generating process identifier (defined by originating centre)
wxUint16 hoursAfterRefTime;		//15-16		Hours of observational data cutoff after reference time (see Note)
wxUint8  minutesAfterRefTime;	//17		Minutes of observational data cutoff after reference time
wxUint8  indicatorTimeRange;	//18		Indicator of unit of time range (see Code table 4.4)
wxUint32 forcastTimeUnits;		//19-22		Forecast time in units defined by octet 18
wxUint8  type1Surface;			//23		Type of first fixed surface (see Code table 4.5)
wxUint8  scaleFactor1Surface;	//24		Scale factor of first fixed surface
wxUint32 scaledValue1Surface;	//25-28		Scaled value of first fixed surface
wxUint8  type2Surface;			//29		Type of second fixed surface (see Code table 4.5)
wxUint8  scaleFactor2Surface;	//30		Scale factor of second fixed surface
wxUint32 scaledValue2Surface;	//31-34		Scaled value of second fixed surface
};

class Section4 {

public:
	Section4(wxFileInputStream *stream, Grib2Message *message);
	~Section4();

	int readSection();	

private:
	wxFileInputStream *stream;
	Grib2Message *message;		

	void readTemplate4_0(wxDataInputStream *data,templ_4_0 *tpl);
};

#endif // SECTION4_H
