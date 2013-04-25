//#define GRIB2_DEBUG

#include "section3.h"
#include "../helper.h"

#include "wx/wx.h"
#include "wx/datstrm.h"

Section3::Section3(wxFileInputStream *stream,Grib2Message *grib2Message)
{
	this->stream = stream;
	this->message = grib2Message;	
}

Section3::~Section3()
{
}

int Section3::readSection()
{
	wxUint32 len = 0;
	wxUint8  section = 0;
	wxUint8  sourcedefination = 0;
	wxUint32 nopoints = 0;
	wxUint8  optionallist = 0;
	wxUint8  interpretation = 0;
	wxUint16 griddeftemplateNo = 0;
	wxUint32 nextlen     = 0;
	wxUint8  nextsectNo  = 0;
	
	templ_3_0 template_3_0;

#ifdef GRIB2_DEBUG	
	wxFileOffset i = stream->TellI();	
#endif	
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);
	
	data >> len;
	data >> section;
	if(section != 3) { Helper::sectionError(section,3); return -1; } 
	data >> sourcedefination;
	data >> nopoints;
	data >> optionallist;
	data >> interpretation;
	data >> griddeftemplateNo;
	
//	unsigned char *buffer = new unsigned char[len-14];
//	stream->Read(&template_3_0,sizeof(template_3_0)/*len-14*/);
	if(griddeftemplateNo == 0)
		readTemplate_3_0(&data,&template_3_0);
	data >> nextlen;
	data >> nextsectNo;	

	double x = 1000000;
	
#ifdef GRIB2_DEBUG
	wxLogMessage(wxString::Format(_T("Section3\n%X Length: %i nextSection: %i\nSourceDefination: %i\nNumber of Points: %i\nOptionalList: %i\nInterpolation: %i\nTemplateNo: %i\n"),
	i,len,nextsectNo,sourcedefination,nopoints,optionallist,interpretation,griddeftemplateNo));

	wxLogMessage(wxString::Format(_T("Template 3.0 Data\nShapeEarth: %i\nscaleFactor: %u\nscaledValue:%u\nscaleFactorMajorAxis: %u\nscaledValueMajorAxis: %u\nscaleFactorMinorAxis: %u\nscaledValueMinorAxis: %u\nnoPointsParallel: %u\nnoPointsMeridian: %u\nbasicAngle: %u\nsubdivisionBasicAngle: %u\nfirstGridPointLat: %f\nfirstGridPointLong: %f\nresCompFlag: %u\nlastGridPointLat: %f\nlastGridPointLong:%f\niDirectionIncr:%f\njDirectionIncr: %f\nscanMode: %u\n"),
	template_3_0.shapeEarth,
	template_3_0.scaleFactor,
	template_3_0.scaledValue,
	template_3_0.scaleFactorMajorAxis,
	template_3_0.scaledValueMajorAxis,
	template_3_0.scaleFactorMinorAxis,
	template_3_0.scaledValueMinorAxis,
	template_3_0.noPointsParallel,
	template_3_0.noPointsMeridian,
	template_3_0.basicAngle,
	template_3_0.subdivisionBasicAngle,
	template_3_0.firstGridPointLat/x,
	template_3_0.firstGridPointLong/x,
	template_3_0.resCompFlag,
	template_3_0.lastGridPointLat/x,
	template_3_0.lastGridPointLong/x,
	template_3_0.iDirectionIncr/x,
	template_3_0.jDirectionIncr/x,
	template_3_0.scanMode));
#endif
	
	message->templateNo3 = griddeftemplateNo;
	message->noSectors = nopoints;
	message->noPointsParallel = template_3_0.noPointsParallel;
	message->noPointsMeridian = template_3_0.noPointsMeridian;
	message->firstGridPointLat = template_3_0.firstGridPointLat/x;
	message->firstGridPointLong = template_3_0.firstGridPointLong/x;
	message->lastGridPointLat = template_3_0.lastGridPointLat/x;
	message->lastGridPointLong = template_3_0.lastGridPointLong/x;
	message->iDirectionIncr = template_3_0.iDirectionIncr/x;
	message->jDirectionIncr = template_3_0.jDirectionIncr/x;
	message->resCompFlag = template_3_0.resCompFlag;
	message->scanMode = template_3_0.scanMode;
	if(message->firstGridPointLat > message->lastGridPointLat )
	{
	  wxDouble tmp = message->lastGridPointLat;
	  message->lastGridPointLat = message->firstGridPointLat;
	  message->firstGridPointLat = tmp;
	}
	
	stream->SeekI(stream->TellI()-5);
	return nextsectNo;
}

void Section3::readTemplate_3_0(wxDataInputStream *data, templ_3_0 *tpl)
{
	*data >> tpl->shapeEarth;
	*data >> tpl->scaleFactor;
	*data >> tpl->scaledValue;
	*data >> tpl->scaleFactorMajorAxis;
	*data >> tpl->scaledValueMajorAxis;
	*data >> tpl->scaleFactorMinorAxis;
	*data >> tpl->scaledValueMinorAxis;
	*data >> tpl->noPointsParallel;
	*data >> tpl->noPointsMeridian;
	*data >> tpl->basicAngle;
	*data >> tpl->subdivisionBasicAngle;
	*data >> tpl->firstGridPointLat;
	*data >> tpl->firstGridPointLong;
	*data >> tpl->resCompFlag;
	*data >> tpl->lastGridPointLat;
	*data >> tpl->lastGridPointLong;
	*data >> tpl->iDirectionIncr;
	*data >> tpl->jDirectionIncr;
	*data >> tpl->scanMode;
	
	if(tpl->firstGridPointLat & 0x80000000)		// I need a sign for North, East, South or West
		tpl->firstGridPointLat = -(int)(tpl->firstGridPointLat ^ 0x80000000);	
	if(tpl->firstGridPointLong & 0x80000000)								
		tpl->firstGridPointLong = -(int)(tpl->firstGridPointLong ^ 0x80000000);
	if(tpl->lastGridPointLat & 0x80000000)
		tpl->lastGridPointLat = -(int)(tpl->lastGridPointLat ^ 0x80000000);
	if(tpl->lastGridPointLong & 0x80000000)
		tpl->lastGridPointLong = -(int)(tpl->lastGridPointLong ^ 0x80000000);		
}











