//#define GRIB2_DEBUG

#include "section4.h"

#include "../helper.h"
#include "wx/datstrm.h"

Section4::Section4(wxFileInputStream *stream,Grib2Message *grib2Message)
{
	this->stream = stream;
	this->message = grib2Message;	
}

Section4::~Section4()
{
}

int Section4::readSection()
{
	wxUint32 len     = 0;
	wxUint8  sectNo  = 0;
	wxUint16 cordValueNo = 0;
	wxUint16 prodTemplNo = -1;
	wxUint32 nextlen     = 0;
	wxUint8  nextsectNo  = 0;
	
	templ_4_0 template_4_0;

	wxFileOffset i = stream->TellI();		
	
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);

	data >> len;
	data >> sectNo;
	if(sectNo != 4) { Helper::sectionError(sectNo,4); return -1; } 
	data >> cordValueNo;
	data >> prodTemplNo;

	if(prodTemplNo == 0)
	{
		readTemplate4_0(&data,&template_4_0);
		message->templateNo4 = prodTemplNo;
		message->paramCategory = template_4_0.paramCategory;
		message->paramNo = template_4_0.paramNo;
		message->typeOfProcess = template_4_0.typeOfProcess;
		message->hoursAfterRefTime = template_4_0.hoursAfterRefTime;
		message->minutesAfterRefTime = template_4_0.minutesAfterRefTime;
		message->indicatorTimeRange = template_4_0.indicatorTimeRange;
		message->forcastTimeUnits = template_4_0.forcastTimeUnits;
		message->scaledValueSurface1 = template_4_0.scaledValue1Surface;
	}
	else 
		stream->SeekI(i+len); // no other templates used by now 
		
	
	data >> nextlen;
	data >> nextsectNo;	
	
#ifdef GRIB2_DEBUG
	wxLogMessage(wxString::Format(_T("Section 4\n%X Length: %i\nnextSection=%u\nCoordValueNo: %i\nprodTempNo:%i\nnextSection: %u\n"),i,len,nextsectNo,prodTemplNo,nextsectNo));
	if(prodTemplNo == 0)
	wxLogMessage(wxString::Format(_T("Template 4.0 Data\nparamCategory: %u\nparamNo: %u\ntypeOfProcess: %u\nbackgroundProcessID: %u\nanalysisForcastID: %u\nhoursAfterRefTime: %u\nminutesAfterRefTime: %u\nindicatorTimeRange: %u\nforcastTimeUnits: %u\ntype1Surface:%u\nscaleFactor1Surface: %u\nscaledValue1Surface: %u\ntype2Surface: %u\nscaleFactor2Surface: %u\nscaledValue2Surface: %u\n"),
									 template_4_0.paramCategory,
									 template_4_0.paramNo,
									 template_4_0.typeOfProcess,
									 template_4_0.backgroundProcessID,
									 template_4_0.analysisForcastID,
									 template_4_0.hoursAfterRefTime,
									 template_4_0.minutesAfterRefTime,
									 template_4_0.indicatorTimeRange,
									 template_4_0.forcastTimeUnits,
									 template_4_0.type1Surface,
									 template_4_0.scaleFactor1Surface,
									 template_4_0.scaledValue1Surface,
									 template_4_0.type2Surface,
									 template_4_0.scaleFactor2Surface,
									 template_4_0.scaledValue2Surface));
#endif
	stream->SeekI(stream->TellI()-5);
	return nextsectNo;	
}

void Section4::readTemplate4_0(wxDataInputStream *data,templ_4_0 *tpl)
{
	*data >> tpl->paramCategory;
	*data >> tpl->paramNo;
	*data >> tpl->typeOfProcess;
	*data >> tpl->backgroundProcessID;
	*data >> tpl->analysisForcastID;
	*data >> tpl->hoursAfterRefTime;
	*data >> tpl->minutesAfterRefTime;
	*data >> tpl->indicatorTimeRange;
	*data >> tpl->forcastTimeUnits;
	*data >> tpl->type1Surface;
	*data >> tpl->scaleFactor1Surface;
	*data >> tpl->scaledValue1Surface;
	*data >> tpl->type2Surface;
	*data >> tpl->scaleFactor2Surface;
	*data >> tpl->scaledValue2Surface;
}

