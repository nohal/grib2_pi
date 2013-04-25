#ifndef SECTION6_H
#define SECTION6_H

#include "wx/wx.h"
#include "wx/wfstream.h"

#include "../grib_reader.h"

class Section6 {

public:
	Section6(wxFileInputStream *stream, Grib2Message *message);
	~Section6();
	
	int readSection();	

private:
	wxFileInputStream *stream;
	Grib2Message *message;	
};

#endif // SECTION6_H
