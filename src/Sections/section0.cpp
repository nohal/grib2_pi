//#define GRIB2_DEBUG

#ifdef _FILE_OFFSET_BITS
  #if _FILE_OFFSET_BITS==64
     #define wxHAS_LARGE_FILES
  #endif
#endif
#ifdef _LARGE_FILES
   #define wxHAS_LARGE_FILES
#endif

#include "section0.h"
#include "../helper.h"
#include "wx/datstrm.h"
#include "wx/filefn.h" 

Section0::Section0(wxFileInputStream *stream, Grib2Message *grib2Message)
{
	this->stream = stream;
	this->message = grib2Message;
}

Section0::~Section0()
{
}

int Section0::readSection()
{
	char buffer[4];
	wxUint64 i = stream->TellI();	
	wxString grib;
	wxUint16 dummy;
	wxUint8  product = 0;	
	wxUint8	 version = 0;
	wxUint64 totalLength = 0;	
	wxUint32 len     = 0;
	wxUint8  nextsectNo  = 0;

	wxString table[] = {_("Meteorological"),_("Hydrological"),_("Land Surface"),_("Space"),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_("Oceanographic")};
	// Only Meteorological, Space, Oceanographic ist used	


	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);

	stream->Read(&buffer,4);
	if(stream->Eof()) return -1;

	grib = Helper::toString(&buffer[0],4);
	data >> dummy;
	data >> product;
	data >> version;
	data >> totalLength;
	data >> len;
	data >> nextsectNo;
	
	if(grib != _T("GRIB") || version != 2) 
		{ wxMessageBox(_("This is not a GRIB2-File"),_("Section 0 Error")); return -1; } 

	if(product != 0 && product != 10)  // read only Meteorological and Ozeannographic Products
	{
		stream->SeekI(i+totalLength);
		if(stream->Eof()) return -1;
		else return 0;
	}
	message->version = version;
	message->length = totalLength;
	message->discipline = product;
	
#ifdef GRIB2_DEBUG
  wxLogMessage(wxString::Format(_T("Section 0\n%X  Length: %i\n"),i,len) + grib + wxString::Format(_T("%i "),version) + table[product] + wxString::Format(_T(" Products\nTotalLength: %u\n"),totalLength));
#endif
	
	if((product != 0 && product != 3 && product!= 10) || version != 2) return -1;

	stream->SeekI(stream->TellI()-5);
	return nextsectNo;
}


