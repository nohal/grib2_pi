//#define GRIB2_DEBUG

#include "section5.h"
#include "../helper.h"
#include "wx/datstrm.h"

Section5::Section5(wxFileInputStream *stream,Grib2Message *grib2Message)
{
	this->stream = stream;
	this->message = grib2Message;
}

Section5::~Section5()
{
}

int Section5::readSection()
{
	wxUint32 len     = 0;
	wxUint8  sectNo  = 0;
	wxUint32 dataPointsNo = 0;
	wxUint16 dataTemplNo = 0;
	wxUint32 nextlen     = 0;
	wxUint8  nextsectNo  = 0;
	
#ifdef GRIB2_DEBUG
	wxFileOffset i = stream->TellI();	
#endif	
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);

	templ_5_0 template_5_0;
	templ_5_40 template_5_40;	
	
	data >> len;
	data >> sectNo;
	if(sectNo != 5) { Helper::sectionError(sectNo,5); return -1; } 
	data >> dataPointsNo;
	data >> dataTemplNo;
	if(dataTemplNo == 0)
	{
		readTemplate5_0(&data,&template_5_0); // Simple Packing
		
		message->templateNo5 = dataTemplNo;
		message->referenceVal = template_5_0.referenceVal;		
		Helper::rdieee((g2int*)&template_5_0.referenceVal,&message->referenceVal,1);
		message->binaryScaleFacor = template_5_0.binaryScaleFacor;
		message->decimalScaleFactor = template_5_0.decimalScaleFactor;
		message->noBits = template_5_0.noBits;
		message->typeOrgFieldValues = template_5_0.typeOrgFieldValues;
		message->compressionType = 255;
		message->compressionRatio = 255;		
	}
	else if(dataTemplNo == 40)
	{
		readTemplate5_40(&data,&template_5_40); // Jpeg 200	
		
		message->templateNo5 = dataTemplNo;	
		Helper::rdieee((g2int*)&template_5_40.referenceVal,&message->referenceVal,1);	
		message->binaryScaleFacor = template_5_40.binaryScaleFacor;
		message->decimalScaleFactor = template_5_40.decimalScaleFactor;
		message->noBits = template_5_40.noBits;
		message->typeOrgFieldValues = template_5_40.typeOrgFieldValues;
		message->compressionType = template_5_40.compressionType;
		message->compressionRatio = template_5_40.compressionRatio;
	}

	
	data >> nextlen;
	data >> nextsectNo;		

#ifdef GRIB2_DEBUG
	wxLogMessage(wxString::Format(_T("Section5\n%X Length: %i nextSection: %u\nDataPointsNo: %i\nDataTempNo: %i\n"),i,len,nextsectNo,dataPointsNo,dataTemplNo));
	if(dataTemplNo == 0)
	wxLogMessage(wxString::Format(_T("Template 5.0 Data\nreferenceVal: %i\nbinaryScaleFacor: %u\ndecimalScaleFactor: %u\nnoBits: %u\ntypeOrgFieldValues: %u\n"),
									 template_5_0.referenceVal,
									 template_5_0.binaryScaleFacor,
									 template_5_0.decimalScaleFactor,
									 template_5_0.noBits,
									 template_5_0.typeOrgFieldValues));
    else
	wxLogMessage(wxString::Format(_T("Template 5.40 Data\nreferenceVal: %i\nbinaryScaleFacor: %u\ndecimalScaleFactor: %u\nnoBits: %u\ntypeOrgFieldValues: %u\ncompressionType: %u  compressionRatio: %u\n"),
									 template_5_40.referenceVal,
									 template_5_40.binaryScaleFacor,
									 template_5_40.decimalScaleFactor,
									 template_5_40.noBits,
									 template_5_40.typeOrgFieldValues,
									 template_5_40.compressionType,
									 template_5_40.compressionRatio));										
#endif
	stream->SeekI(stream->TellI()-5);
	return nextsectNo;	
}

void Section5::readTemplate5_0(wxDataInputStream *data,templ_5_0 *tpl)
{
	*data >> tpl->referenceVal;
	*data >> tpl->binaryScaleFacor;
	*data >> tpl->decimalScaleFactor;
	*data >> tpl->noBits;
	*data >> tpl->typeOrgFieldValues;
}

void Section5::readTemplate5_40(wxDataInputStream *data,templ_5_40 *tpl)
{
	*data >> tpl->referenceVal;
	*data >> tpl->binaryScaleFacor;
	*data >> tpl->decimalScaleFactor;
	*data >> tpl->noBits;
	*data >> tpl->typeOrgFieldValues;	
	*data >> tpl->compressionType;
	*data >> tpl->compressionRatio;
}


