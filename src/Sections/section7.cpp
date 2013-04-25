//#define GRIB2_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include "../jasper/jasper.h"

#include "section7.h"
#include "../grib2data.h"
#include "../helper.h"
#include "wx/datstrm.h"

Section7::Section7(wxFileInputStream *stream, Grib2Data *data, Grib2Message*grib2Message)
{
	this->stream = stream;
	this->message = grib2Message;	
}

Section7::~Section7()
{
}

int Section7::readSection()
{
	wxUint32 len     = 0;
	wxUint8  sectNo  = 0;
	wxUint32 nextsectLen = 0;
	wxUint8  nextsectNo  = 0;
	
	wxDataInputStream data(*stream);
	data.BigEndianOrdered(true);
#ifdef GRIB2_DEBUG	
	wxFileOffset i = stream->TellI();
#endif	
	data >> len;
	data >> sectNo;
	
	if(sectNo != 7) { Helper::sectionError(sectNo,7); return -1; }

	unsigned char *buffer = new unsigned char[len-5];
	stream->Read(buffer,len-5);
	if(stream->Eof()) return -1;

	message->data=(wxDouble *)calloc(message->noSectors ? message->noSectors : 1,sizeof(wxDouble));	
	if(message->data != 0 && message->templateNo5 == 0)
		this->simplePacked(&buffer[0]);
	else if(message->data != 0 && (message->templateNo5 == 40 || message->templateNo5 == 40000))
		this->jpeg2000Packed(&buffer[0]);
		
	buffer = new unsigned char[4];
	stream->Read(buffer,4);
	
	if((buffer[0] == '7' && buffer[1] == '7' && buffer[2] == '7' && buffer[3] == '7'))
	{ 
		if(stream->Eof()) 
			return -1;  
		else 
		{
#ifdef GRIB2_DEBUG
		  wxLogMessage(wxString::Format(
		    _T("Section 7  nextSectionNo: 0\nFileOffset: %X\nLength: %i messageLength: %u\nnoPointsMeridian: %u  noPointsParallel: %u\n"),
			i,len,(message->noPointsMeridian*message->noPointsParallel*message->noBits)/8,
			message->noPointsMeridian,message->noPointsParallel));
#endif	
		  return 0;  
		}

	}
	else
	stream->SeekI(stream->TellI()-4);
	
	data >> nextsectLen;
	data >> nextsectNo;	
	
#ifdef GRIB2_DEBUG
	wxLogMessage(wxString::Format(_T("Section 7  nextSectionNo: %u\nFileOffset: %X\nLength: %i messageLength: %u\nnoPointsMeridian: %u  noPointsParallel: %u\n"),
									nextsectNo,i,len,(message->noPointsMeridian*message->noPointsParallel*message->noBits)/8,
									message->noPointsMeridian,message->noPointsParallel));
//	wxLogMessage(wxString::Format(_T("\n%f  %f  %f  %f  %f\n"),message->data[0],message->data[1],message->data[2],message->data[3],message->data[4]));								
#endif

	stream->SeekI(stream->TellI()-5);	
	delete buffer;
	return nextsectNo;	
}

g2int Section7::simplePacked(unsigned char * buffer)
{  
	  g2int *ifld;
	  g2float bscale,dscale;
	  g2intu nbits,j;

      bscale = Helper::int_power(2.0,message->binaryScaleFacor);
      dscale = Helper::int_power(10.0,-message->decimalScaleFactor);
      nbits = message->noBits;

      ifld=(g2int *)calloc(message->noSectors,sizeof(g2int));
      if ( ifld == 0 ) {
         fprintf(stderr,"Could not allocate space in simplePacked.\n  Data field NOT upacked.\n");
         return(1);
      }
      
//
//  if nbits equals 0, we have a constant field where the reference value
//  is the data value at each gridpoint
//
      if (nbits != 0) {
         gbits(buffer,ifld,0,nbits,0,message->noSectors);
         for (j=0;j<message->noSectors;j++) {
			message->data[j]=(((g2float)ifld[j]*bscale)+message->referenceVal)*dscale;
			//if(j < 50) printf("%i) %f\n",j,message->data[j]);
         }
#ifdef GRIB2_DEBUG		 
		 int x;
		 for(x =0; x < 10; x++) 	
			 wxLogMessage(wxString::Format(_T("%f"),(g2float)message->data[x]));
#endif			 
      }
      else {
         for (j=0;j<message->noSectors;j++) {
           message->data[j]=message->referenceVal;
         }
      }

      free(ifld);
      return(0);	
}

g2int Section7::jpeg2000Packed(unsigned char *buffer)
//$$$  SUBPROGRAM DOCUMENTATION BLOCK
//                .      .    .                                       .
// SUBPROGRAM:    jpcunpack
//   PRGMMR: Gilbert          ORG: W/NP11    DATE: 2003-08-27
//
// ABSTRACT: This subroutine unpacks a data field that was packed into a 
//   JPEG2000 code stream
//   using info from the GRIB2 Data Representation Template 5.40 or 5.40000.
//
// PROGRAM HISTORY LOG:
// 2003-08-27  Gilbert
//
// USAGE:    jpcunpack(unsigned char *cpack,g2int len,g2int *idrstmpl,g2int ndpts,
//                     g2float *fld)
//   INPUT ARGUMENT LIST:
//     cpack    - The packed data field (character*1 array)
//     len      - length of packed field cpack().
//     idrstmpl - Pointer to array of values for Data Representation
//                Template 5.40 or 5.40000
//     ndpts    - The number of data values to unpack
//
//   OUTPUT ARGUMENT LIST:
//     fld[]    - Contains the unpacked data values
//
// REMARKS: None
//
// ATTRIBUTES:
//   LANGUAGE: C
//   MACHINE:  IBM SP
//
//$$$
{
      g2int  *ifld;
      g2int  nbits,iret;
      g2float bscale,dscale;
      unsigned int j;
	  
      bscale = Helper::int_power(2.0,message->binaryScaleFacor);
      dscale = Helper::int_power(10.0,-message->decimalScaleFactor);
      nbits = message->noBits;
//
//  if nbits equals 0, we have a constant field where the reference value
//  is the data value at each gridpoint
//
      if (nbits != 0) {

         ifld=(g2int *)calloc(message->noSectors,sizeof(g2int));
         if ( ifld == 0 ) {
            fprintf(stderr,"Could not allocate space in jpcunpack.\n  Data field NOT upacked.\n");
            return(1);
         }
         iret=(g2int)dec_jpeg2000((char*)buffer,message->length-5,ifld);
         for (j=0;j<message->noSectors-1;j++) {
           message->data[j]=(((g2float)ifld[j]*bscale)+message->referenceVal)*dscale;
         }
#ifdef GRIB2_DEBUG		 
/*		int x;
		for(x=0; x < 10; x++) 
		  wxLogMessage(wxString::Format(_T("%f ifld=%i bscale=%f ref=%f dscale=%f "),
				(g2float)message->data[x],ifld[x],bscale,message->referenceVal,dscale));
*/
#endif
         free(ifld);
      }
      else {
         for (j=0;j<message->noSectors;j++) message->data[j]=message->referenceVal;
      }

      return(0);
}


int Section7::dec_jpeg2000(char *injpc,g2int bufsize,g2int *outfld)
/*$$$  SUBPROGRAM DOCUMENTATION BLOCK
*                .      .    .                                       .
* SUBPROGRAM:    dec_jpeg2000      Decodes JPEG2000 code stream
*   PRGMMR: Gilbert          ORG: W/NP11     DATE: 2002-12-02
*
* ABSTRACT: This Function decodes a JPEG2000 code stream specified in the
*   JPEG2000 Part-1 standard (i.e., ISO/IEC 15444-1) using JasPer 
*   Software version 1.500.4 (or 1.700.2) written by the University of British
*   Columbia and Image Power Inc, and others.
*   JasPer is available at http://www.ece.uvic.ca/~mdadams/jasper/.
*
* PROGRAM HISTORY LOG:
* 2002-12-02  Gilbert
*
* USAGE:     int dec_jpeg2000(char *injpc,g2int bufsize,g2int *outfld)
*
*   INPUT ARGUMENTS:
*      injpc - Input JPEG2000 code stream.
*    bufsize - Length (in bytes) of the input JPEG2000 code stream.
*
*   OUTPUT ARGUMENTS:
*     outfld - Output matrix of grayscale image values.
*
*   RETURN VALUES :
*          0 = Successful decode
*         -3 = Error decode jpeg2000 code stream.
*         -5 = decoded image had multiple color components.
*              Only grayscale is expected.
*
* REMARKS:
*
*      Requires JasPer Software version 1.500.4 or 1.700.2
*
* ATTRIBUTES:
*   LANGUAGE: C
*   MACHINE:  IBM SP
*
*$$$*/

{
    int ier;
    g2int i,j,k;
    jas_image_t *image=0;
    jas_stream_t *jpcstream;
    jas_image_cmpt_t *pcmpt;
    char *opts = "";
    jas_matrix_t *data;

    int z = jas_init();

    ier=0;
//   
//     Create jas_stream_t containing input JPEG200 codestream in memory.
//       

    jpcstream=jas_stream_memopen(injpc,bufsize);

//   
//     Decode JPEG200 codestream into jas_image_t structure.
//       
    image=jpc_decode(jpcstream,opts);
    if ( image == 0 ) {
       printf(" jpc_decode return = %d \n",ier);
       return -3;
    }
    
    pcmpt=image->cmpts_[0];
/*
    printf(" SAGOUT DECODE:\n");
    printf(" tlx %d \n",image->tlx_);
    printf(" tly %d \n",image->tly_);
    printf(" brx %d \n",image->brx_);
    printf(" bry %d \n",image->bry_);
    printf(" numcmpts %d \n",image->numcmpts_);
    printf(" maxcmpts %d \n\n",image->maxcmpts_);
    
#ifdef JAS_1_500_4
    printf(" colormodel %d \n",image->colormodel_);
#endif
#ifdef JAS_1_700_2
    printf(" colorspace %d \n",image->clrspc_);
#endif
    
    printf(" inmem %d \n",image->inmem_);
    printf(" COMPONENT:\n");
    printf(" tlx %d \n",pcmpt->tlx_);
    printf(" tly %d \n",pcmpt->tly_);
    printf(" hstep %d \n",pcmpt->hstep_);
    printf(" vstep %d \n",pcmpt->vstep_);
    printf(" width %d \n",pcmpt->width_);
    printf(" height %d \n",pcmpt->height_);
    printf(" prec %d \n",pcmpt->prec_);
    printf(" sgnd %d \n",pcmpt->sgnd_);
    printf(" cps %d \n\n",pcmpt->cps_);
    
#ifdef JAS_1_700_2
    printf(" type %d \n",pcmpt->type_);
#endif
*/

//   Expecting jpeg2000 image to be grayscale only.
//   No color components.
//
    if (image->numcmpts_ != 1 ) {
       printf("dec_jpeg2000: Found color image.  Grayscale expected.\n");
       return (-5);
    }

// 
//    Create a data matrix of grayscale image values decoded from
//    the jpeg2000 codestream.
//
    data=jas_matrix_create(jas_image_height(image), jas_image_width(image));
    jas_image_readcmpt(image,0,0,0,jas_image_width(image),
                       jas_image_height(image),data);
//
//    Copy data matrix to output integer array.
//
    k=0;
    for (i=0;i<pcmpt->height_;i++) 
      for (j=0;j<pcmpt->width_;j++) 
        outfld[k++]=data->rows_[i][j];
//
//     Clean up JasPer work structures.
//
    jas_matrix_destroy(data);
    ier=jas_stream_close(jpcstream);
    jas_image_destroy(image);

    return 0;

}

void Section7::gbit(unsigned char *in,g2int *iout,g2int iskip,g2int nbyte)
{
      gbits(in,iout,iskip,nbyte,(g2int)0,(g2int)1);
}

void Section7::sbit(unsigned char *out,g2int *in,g2int iskip,g2int nbyte)
{
      sbits(out,in,iskip,nbyte,(g2int)0,(g2int)1);
}


void Section7::gbits(unsigned char *in,g2int *iout,g2int iskip,g2int nbyte,g2int nskip,
           g2int n)
/*          Get bits - unpack bits:  Extract arbitrary size values from a
/          packed bit string, right justifying each value in the unpacked
/          iout array.
/           *in    = pointer to character array input
/           *iout  = pointer to unpacked array output
/            iskip = initial number of bits to skip
/            nbyte = number of bits to take
/            nskip = additional number of bits to skip on each iteration
/            n     = number of iterations
/ v1.1
*/
{
      g2int i,tbit,bitcnt,ibit,itmp;
      g2int nbit,index;
      static g2int ones[]={1,3,7,15,31,63,127,255};

//     nbit is the start position of the field in bits
      nbit = iskip;
      for (i=0;i<n;i++) {
         bitcnt = nbyte;
         index=nbit/8;
         ibit=nbit%8;
         nbit = nbit + nbyte + nskip;

//        first byte
         tbit= ( bitcnt < (8-ibit) ) ? bitcnt : 8-ibit;  // find min
         itmp = (int)*(in+index) & ones[7-ibit];
         if (tbit != 8-ibit) itmp >>= (8-ibit-tbit);
         index++;
         bitcnt = bitcnt - tbit;

//        now transfer whole bytes
         while (bitcnt >= 8) {
             itmp = itmp<<8 | (int)*(in+index);
             bitcnt = bitcnt - 8;
             index++;
         }

//        get data from last byte
         if (bitcnt > 0) {
             itmp = ( itmp << bitcnt ) | ( ((int)*(in+index) >> (8-bitcnt)) & ones[bitcnt-1] );
         }

         *(iout+i) = itmp;
      }
      printf("Bytes decoded = %i\n",i);
}


void Section7::sbits(unsigned char *out,g2int *in,g2int iskip,g2int nbyte,g2int nskip,
           g2int n)
/*C          Store bits - pack bits:  Put arbitrary size values into a
/          packed bit string, taking the low order bits from each value
/          in the unpacked array.
/           *iout  = pointer to packed array output
/           *in    = pointer to unpacked array input
/            iskip = initial number of bits to skip
/            nbyte = number of bits to pack
/            nskip = additional number of bits to skip on each iteration
/            n     = number of iterations
/ v1.1
*/
{
      g2int i,bitcnt,tbit,ibit,itmp,imask,itmp2,itmp3;
      g2int nbit,index;
      static g2int ones[]={1,3,7,15,31,63,127,255};

//     number bits from zero to ...
//     nbit is the last bit of the field to be filled

      nbit = iskip + nbyte - 1;
      for (i=0;i<n;i++) {
         itmp = *(in+i);
         bitcnt = nbyte;
         index=nbit/8;
         ibit=nbit%8;
         nbit = nbit + nbyte + nskip;

//        make byte aligned 
         if (ibit != 7) {
             tbit= ( bitcnt < (ibit+1) ) ? bitcnt : ibit+1;  // find min
             imask = ones[tbit-1] << (7-ibit);
             itmp2 = (itmp << (7-ibit)) & imask;
             itmp3 = (int)*(out+index) & (255-imask);
             out[index] = (unsigned char)(itmp2 | itmp3);
             bitcnt = bitcnt - tbit;
             itmp = itmp >> tbit;
             index--;
         }

//        now byte aligned

//        do by bytes
         while (bitcnt >= 8) {
             out[index] = (unsigned char)(itmp & 255);
             itmp = itmp >> 8;
             bitcnt = bitcnt - 8;
             index--;
         }

//        do last byte

         if (bitcnt > 0) {
             itmp2 = itmp & ones[bitcnt-1];
             itmp3 = (int)*(out+index) & (255-ones[bitcnt-1]);
             out[index] = (unsigned char)(itmp2 | itmp3);
         }
      }

}

