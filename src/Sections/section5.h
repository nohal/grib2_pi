#ifndef SECTION5_H
#define SECTION5_H

#include "wx/wx.h"
#include "wx/wfstream.h"
#include <wx/datstrm.h>

#include "../grib_reader.h"

struct templ_5_0
{
							//Data representation template 5.0 - Grid point data - simple packing
							//Note: For most templates, details of the packing process are described in regulation 92.9.4.
							//Octet No.	Contents
wxInt32  referenceVal; 	//12-15	Reference value (R) (IEEE 32-bit floating-point value)
wxUint16 binaryScaleFacor;	//16-17	Binary scale factor (E)
wxUint16 decimalScaleFactor;//18-19	Decimal scale factor (D)
wxUint8  noBits;			// 20	Number of bits used for each packed value for simple packing, or for each group reference
							//		value for complex packing or spatial differencing
wxUint8  typeOrgFieldValues;// 21	Type of original field values (see Code table 5.1)
};

struct templ_5_40
{
							//Data representation template 5.40 - Grid point data - JPEG 2000 code stream format
							//Note: For most templates, details of the packing process are described in regulation 92.9.4.
							//Octet No.	Contents
wxInt32  referenceVal;		//12-15		Reference value (R) (IEEE 32-bit floating-point value)
wxUint16 binaryScaleFacor;	//16-17		Binary scale factor (E)
wxUint16 decimalScaleFactor;//18-19		Decimal scale factor (D)
wxUint8  noBits;			//20		Number of bits required to hold the resulting scaled and referenced data values (i.e. depth
							//			of the grayscale image) (see Note 2)
wxUint8  typeOrgFieldValues;//21		Type of original field values (see Code table 5.1)
wxUint8  compressionType;	//22		Type of compression used. (see Code table 5.40)
wxUint8  compressionRatio;	//23		Target compression ratio, M:1 (with respect to the bit-depth specified in octet 20), when
							//			octet 22 indicates lossy compression. Otherwise, set to missing (see Note 3)	
};

class Section5 {

public:
	Section5(wxFileInputStream *stream, Grib2Message *message);
	~Section5();
	
	int readSection();	

private:
	wxFileInputStream *stream;
	Grib2Message *message;	
	
	void readTemplate5_0(wxDataInputStream *data,templ_5_0 *tpl);	
	void readTemplate5_40(wxDataInputStream *data,templ_5_40 *tpl);	//Jpeg2000 Compression
};

#endif // SECTION5_H
