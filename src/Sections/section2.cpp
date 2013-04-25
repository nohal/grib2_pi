//#define GRIB2_DEBUG

#include "section2.h"
#include "../helper.h"

#include "wx/datstrm.h"

Section2::Section2(wxFileInputStream *stream)
{
	this->stream = stream;	
}

Section2::~Section2()
{
}

int Section2::readSection()
{
	wxUint32 len = 0;
	wxUint8  section = 0;
	wxUint32 nextlen     = 0;
	wxUint8  nextsectNo  = 0;	

#ifdef GRIB2_DEBUG	
	wxFileOffset i = stream->TellI();
#endif	
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);

	data >> len;
	data >> section;
	if(section != 2) { Helper::sectionError(section,2);  return -1; } 
//	stream->SeekI(i+len);		// Don't need data
	data >> nextlen;
	data >> nextsectNo;
	
#ifdef GRIB2_DEBUG
	wxLogMessage(wxString::Format(_T("Section 2\n%X Length: %i\n"),i,len));
#endif
	stream->SeekI(stream->TellI()-5);
	return nextsectNo;
}
