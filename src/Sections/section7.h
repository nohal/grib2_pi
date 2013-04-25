#ifndef SECTION7_H
#define SECTION7_H


#include "wx/wx.h"
#include "wx/wfstream.h"

#include "../grib_reader.h"
#include "../helper.h"

class Grib2Data;

class Section7 {

public:
	Section7(wxFileInputStream *stream, Grib2Data *data, Grib2Message*message);
	~Section7();
	
	int readSection();	

private:
	FILE *fpout;
	wxFileInputStream *stream;
	Grib2Message *message;
	Grib2Data *data;
	
	g2int simplePacked(unsigned char* buffer);
	g2int jpeg2000Packed(unsigned char *buffer);	
	
	void gbit(unsigned char *in,g2int *iout,g2int iskip,g2int nbyte);
	void sbit(unsigned char *out,g2int *in,g2int iskip,g2int nbyte);
	void gbits(unsigned char *in,g2int *iout,g2int iskip,g2int nbyte,g2int nskip,g2int n);
	void sbits(unsigned char *out,g2int *in,g2int iskip,g2int nbyte,g2int nskip,g2int n);
	int dec_jpeg2000(char *injpc,g2int bufsize,g2int *outfld);	
};

#endif // SECTION7_H
