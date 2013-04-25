#include "helper.h"

#include <sstream>
#include "wx/wx.h"

Helper::Helper()
{
}

Helper::~Helper()
{
}

wxString Helper::toString(char *start,int length)
{
	wxString s(start,wxConvUTF8,length);
	return s;
}

wxString Helper::toBinary(wxUint8 input)
{
	//if(input == 0) return _T("00000000"); 
	std::string result;
	int n = sizeof(wxUint8);
	
	for(int i = n*7; i > 0; i--) 
		{
			if(input & (1 << i)) 
				result += "1";	
			else 
				result += "0";		
		}
	wxString binstr(result.c_str(),wxConvUTF8); 
	return binstr;
}

void Helper::sectionError(wxUint8 sectNo, wxUint8 correctSectNo)
{
	wxMessageBox(wxString::Format(_T("Read SectioNo: %u\nmust be %u"),correctSectNo,sectNo),
			     wxString::Format(_T("Section %u Error"),correctSectNo));	
}


void Helper::rdieee(g2int *rieee,g2float *a,g2int num)
//$$$  SUBPROGRAM DOCUMENTATION BLOCK
//                .      .    .                                       .
// SUBPROGRAM:    rdieee 
//   PRGMMR: Gilbert         ORG: W/NP11    DATE: 2002-10-25
//
// ABSTRACT: This subroutine reads a list of real values in 
//   32-bit IEEE floating point format.
//
// PROGRAM HISTORY LOG:
// 2002-10-25  Gilbert
//
// USAGE:    void rdieee(g2int *rieee,g2float *a,g2int num)
//   INPUT ARGUMENT LIST:
//     rieee    - g2int array of floating point values in 32-bit IEEE format.
//     num      - Number of floating point values to convert.
//
//   OUTPUT ARGUMENT LIST:      
//     a        - float array of real values.  a must be allocated with at
//                least 4*num bytes of memory before calling this function.
//
// REMARKS: None
//
// ATTRIBUTES:
//   LANGUAGE: C
//   MACHINE:  IBM SP
//
//$$$
{

      wxUint16  j;
      wxInt32  isign,iexp,imant;

      wxFloat32  sign,temp;
      static wxFloat32  two23,two126;
      static wxUint16 test=0;
      wxInt32 msk1=0x80000000;        // 10000000000000000000000000000000 binary
      wxInt32 msk2=0x7F800000;         // 01111111100000000000000000000000 binary
      wxInt32 msk3=0x007FFFFF;         // 00000000011111111111111111111111 binary

      if ( test == 0 ) {
         two23=(wxFloat32)int_power(2.0,-23);
         two126=(wxFloat32)int_power(2.0,-126);
         test=1;
      }

      for (j=0;j<num;j++) {
//
//  Extract sign bit, exponent, and mantissa
//
        isign=(rieee[j]&msk1)>>31;
        iexp=(rieee[j]&msk2)>>23;
        imant=(rieee[j]&msk3);
//        wxLogMessage(wxString::Format(_T("SAGieee= %d %d %d\n"),isign,iexp,imant));

        sign=1.0;
        if (isign == -1) sign=-1.0;
        
        if ( (iexp > 0) && (iexp < 255) ) {
          temp=(wxFloat32)int_power(2.0,(iexp-127));
          a[j]=sign*temp*(1.0+(two23*(wxFloat32)imant));
        }
        else if ( iexp == 0 ) {
          if ( imant != 0 )
            a[j]=sign*two126*two23*(wxFloat32)imant;
          else
            a[j]=sign*0.0;
          
        }
        else if ( iexp == 255 )
           a[j]=sign*(1E+37);
//        wxLogMessage(wxString::Format(_T("%f\n"),a[j]));

      }	  
}

double Helper::int_power(double x, g2int y) 
{

        double value;

        if (y < 0) {
                y = -y;
                x = 1.0 / x;
        }
        value = 1.0;

        while (y) {
                if (y & 1) {
                        value *= x;
                }
                x = x * x;
                y >>= 1;
        }
        return value;
}
