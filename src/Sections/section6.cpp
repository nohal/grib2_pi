//#define GRIB2_DEBUG

#include "section6.h"
#include "../helper.h"
#include "wx/datstrm.h"

Section6::Section6(wxFileInputStream *stream,Grib2Message *message)
{
	this->stream = stream;
	this->message = message;	
}

Section6::~Section6()
{
}

int Section6::readSection()
{
	wxUint32 len     	 = 0;
	wxUint8  sectNo  	 = 0;
	wxUint8  bitmapID 	 = 0;
	wxUint32 nextlen     = 0;
	wxUint8  nextsectNo  = 0;
	
#ifdef GRIB2_DEBUG	
	wxFileOffset i = stream->TellI();	
#endif	
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);

	data >> len;
	data >> sectNo;
	if(sectNo != 6) { Helper::sectionError(sectNo,6); return -1; } 
	data >> bitmapID;

	if(len-6 > 0)
	{
	  message->bmpMask = (wxInt8 *)calloc(len-6,sizeof(wxInt8));
	  stream->Read(message->bmpMask,len-6);
	  message->bmpSize = len-6;
	}
	else 
	  message->bmpMask = NULL;
	
	data >> nextlen;
	data >> nextsectNo;	

#ifdef GRIB2_DEBUG
	wxLogMessage(wxString::Format(_T("Section6\n%X Length: %i\nBitmapID: %i\n"),i,len,bitmapID));
#endif
	stream->SeekI(stream->TellI()-5);
	return nextsectNo;	
}
