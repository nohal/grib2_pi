#ifndef Section1_H
#define Section1_H

#include "wx/wx.h"
#include "wx/wfstream.h"

#include "../grib_reader.h"

class Section1 {

public:
	Section1(wxFileInputStream *stream, Grib2Message *message);
	~Section1();
	
	int readSection();	

private:
	wxFileInputStream *stream;
	Grib2Message *message;	
};

#endif // Section1_H
