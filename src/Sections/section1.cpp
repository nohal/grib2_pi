//#define GRIB2_DEBUG

#include "section1.h"
#include "../helper.h"
#include "wx/datstrm.h"
#include "wx/datetime.h"
#include "wx/defs.h"

Section1::Section1(wxFileInputStream *stream,Grib2Message *grib2Message)
{
	this->stream = stream;
	this->message = grib2Message;	
}

Section1::~Section1()
{
}

int Section1::readSection()
{
	wxUint32 len     = 0;
	wxUint8  sectNo  = 0;
	wxUint16 idCenter = 0;
	wxUint16 idSubCenter = 0;
	wxUint8  vermastertable = 0;
	wxUint8  verlocaltable = 0;
	wxUint8  referencetime = 0;
	wxUint16 year = 0 ;
	wxUint8	 month = 0;
	wxUint8  day = 0;
	wxUint8  hour = 0;
	wxUint8  minute = 0;
	wxUint8  second = 0;
	wxUint8  prodstatedata = 0;
	wxUint8  datatype = 0;
	wxUint32 nextlen     = 0;
	wxUint8  nextsectNo  = 0;

	char *buffer;

#ifdef GRIB2_DEBUG
	wxFileOffset i = stream->TellI();	
#endif
	if(stream->Eof()) return -1;
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);

	data >> len;
	data >> sectNo;
	if(sectNo != 1) return -1;
	data >> idCenter;
	data >> idSubCenter;
	data >> vermastertable;
	data >> verlocaltable;
	data >> referencetime;
	data >> year;
	data >> month;
	data >> day;
	data >> hour;
	data >> minute;
	data >> second;
	data >> prodstatedata;
	data >> datatype;
	data >> nextlen;
	data >> nextsectNo;

	month--;
	message->masterTableVersion = vermastertable;
	message->localTableVersion = verlocaltable;
	message->referenceTime = referencetime;
	message->dt = wxDateTime::Now();
	message->dt.SetYear(year);
	message->dt.SetMonth((wxDateTime::Month)month);
	message->dt.SetDay(day);
	message->dt.SetHour(hour);
	message->dt.SetMinute(minute);
	message->dt.SetSecond(second);
	message->productionState = prodstatedata;
	message->dataType = datatype;	

	if(len > 21)		// Dummy Read
	{					// Reserved Bytes not used by now
		buffer = new char [len-21];
		stream->Read(&buffer,sizeof(buffer));
	}

#ifdef GRIB2_DEBUG
  wxLogMessage(wxString::Format(_T("Section 1\n%X Length: %i NextSection:%i\n%i.%i.%i %i:%i:%i\nMastertable: %i\nID Center: %i\nIDSubCenter: %i\nReferencetime: %i\nProductionStateData: %i\nDataType: %i\n"),
  i,len,nextsectNo,day,month,year,hour,minute,second,vermastertable,idCenter,idSubCenter,referencetime,prodstatedata,datatype));
#endif
	stream->SeekI(stream->TellI()-5);
//	delete buffer;
	return nextsectNo;	
}

