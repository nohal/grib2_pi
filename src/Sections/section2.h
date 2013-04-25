#ifndef SECTION2_H
#define SECTION2_H

#include "wx/wx.h"
#include "wx/wfstream.h"

class Grib2_Reader;

class Section2 {

public:
	Section2(wxFileInputStream *stream);
	~Section2();

	int readSection();
	
private:
	wxFileInputStream *stream;
};

#endif // SECTION2_H
