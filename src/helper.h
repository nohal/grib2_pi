#ifndef HELPER_H
#define HELPER_H
#define __64BIT__

#include "wx/wx.h"

#ifdef __64BIT__
typedef int g2int;
typedef unsigned int g2intu;
#else
typedef long g2int;
typedef unsigned long g2intu;
#endif
typedef float g2float;

class Helper {

public:
	Helper();
	~Helper();
	
	static wxString toString(char *start,int length);
	static wxString toBinary(wxUint8 i);
	static void rdieee(g2int *rieee,g2float *a,g2int num);
	static void sectionError(wxUint8 sectNo, wxUint8 correctSectNo);
	static double int_power(double x, g2int y);	
	
private:

};

#endif // HELPER_H
