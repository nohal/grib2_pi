#ifndef Section0_H
#define Section0_H

#include "wx/wx.h"
#include "wx/wfstream.h"

#include "../grib_reader.h"

class Section0 {

public:
	Section0(wxFileInputStream *stream, Grib2Message *message);
	~Section0();
	
	int readSection();
	
private:
	wxFileInputStream *stream;
	Grib2Message *message;
};

#endif // Section0_H
