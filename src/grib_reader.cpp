#define GRIB2_DEBUG

#include "grib2_pi.h"
#include "grib_reader.h"
#include "grib2.h"
#include "helper.h"
#include "Sections/section0.h"
#include "Sections/section1.h"
#include "Sections/section2.h"
#include "Sections/section3.h"
#include "Sections/section4.h"
#include "Sections/section5.h"
#include "Sections/section6.h"
#include "Sections/section7.h"
#include "grib2data.h"

#include <wx/wfstream.h>

GribReader::GribReader(MainDialog *md)
{
	gui = md;
	grib2Message.data = NULL;
	grib2Message.bmpMask = NULL;
	grib2Data = NULL;
	isReading = false;
}

GribReader::~GribReader()
{
	if(grib2Data)
		delete grib2Data;
}

void GribReader::readGribFile(wxString file, wxUint64 fp, wxUint32 hour, wxDateTime dt)
{
#ifdef GRIB2_DEBUG
	wxString table0_0[] = { _T("Meteorological"),_T("Hydrological"),_T("Land Surface"),_T("Space"),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("Oceanographic")};
	wxString table1_2[] = { _T("Analysis"),_T("Start of forecast"),_T("Verifying time of forecast") };	
	wxString table1_3[] = { _T("Operational products"),_T("Operational test products"),_T("Research products"),_T("Re-analysis products"),_T("") };
	wxString table1_4[] = { _T("Analysis products"),_T("Forecast products"),_T("Analysis and forecast products") };
	wxString codeTable_4_1[] = { _T("Temperature"),_T("Moisture"),_T("Momentum"),_T("Mass"),_T("Short-wave radiation"),_T("Long-wave radiation"),_T("Cloud") };	
	wxString codeTable_5_0[] = { _T("Grid point data - simple packing"),_T("Matrix value - simple packing"),_T("Grid point data - complex packing"),
								 _T("Grid point data - complex packing and spatial differencing"),_T("Grid point data – IEEE floating point data"),
								 _T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
								 _T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
								 _T("Grid point data - JPEG 2000 code stream format"),_T("Grid point data - Portable Network Graphics (PNG)")};								
	wxString codeTable_5_1[] = { _T("Floating Point"),_T("Integer") };								
	wxString codeTable5_40[] = { _T("Lossless"),_T("Lossy") };	
#endif
	wxULongLong length = 0;
	int nextSection = 0;
	int records = 0;

	isReading = true;

	if(grib2Message.data)
			{ 
				free(grib2Message.data);
				grib2Message.data = NULL;
			}
	if(grib2Message.bmpMask)
			{ 
				free(grib2Message.bmpMask);
				grib2Message.bmpMask = NULL;
			}
	
	gui->pPlugIn->GetGRIBOverlayFactory()->reset();
	
	wxFileInputStream stream(file);
	if(!stream.IsOk()) return;
	stream.SeekI(fp);

	gui->pPlugIn->GetGRIBOverlayFactory()->reset();
	if(grib2Data) delete grib2Data;
	grib2Data = new Grib2Data(this);
	
	Section0 *section0 = new Section0(&stream,&grib2Message);
	Section1 *section1 = new Section1(&stream,&grib2Message);
	Section2 *section2 = new Section2(&stream);	
	Section3 *section3 = new Section3(&stream,&grib2Message);	
	Section4 *section4 = new Section4(&stream,&grib2Message);	
	Section5 *section5 = new Section5(&stream,&grib2Message);	
	Section6 *section6 = new Section6(&stream,&grib2Message);
	Section7 *section7 = new Section7(&stream,grib2Data,&grib2Message);
	

	
	do
	{
		if(nextSection == 0)
			{
				nextSection = section0->readSection();
				if(nextSection != -1) records ++;
			}
		if(nextSection == 1)
			nextSection = section1->readSection();
		if(nextSection == 2)
			nextSection = section2->readSection();
		if(nextSection == 3)
			nextSection = section3->readSection();
		if(nextSection == 4)
		{
			nextSection = section4->readSection();
			if(grib2Message.forcastTimeUnits > hour)
			{ nextSection = -1; records--; }
		}	
		if(nextSection == 5)
			nextSection = section5->readSection();	
		if(nextSection == 6)
			nextSection = section6->readSection();	
		if(nextSection == 7)
		{
			nextSection = section7->readSection();
			grib2Data->addRecord(&grib2Message);
			
		}

if(!stream.Eof() && nextSection != -1)
{
#ifdef GRIB2_DEBUG
		
	grib2Message.dt.Now();
	char *str = "GribMessage Version: %u\ntotelLength: %u\ndiscipline: %s Products\n"
									 "masterTableVersion: %i\nlocalTableVersion: %i\nreferenceTime: %s\n"
									 "DateTime: %s\nproductionState: %s\ndataType: %s\n"
									 "templateNo3: 3.%u\nnoSectors: %u\nnoPointsParallel:%u  noPointsMeridian:%u\n"
									 "firstGridPointLat: %f  firstGridPointLong: %f\nlastGridPointLat: %f  lastGridPointLong:%f\n"
									 "iDirectionIncr: %f  jDirectionIncr: %f\nresCompFlag: %s  scanMode: %s\n"
									 "templateNo4: 4.%u\nparamCategory: %s paramNo:%u\nscaledValueSurface1: %u typeOfProcess: %u\nhoursAfterRefTime: %02u:%02u\n"
									 "indicatorTimeRange: %u  forcastTimeUnits:%u\n"
									 "templateNo5: 5.%u %s\nreferenceVal: %f\nbinaryScaleFacor %u  decimalScaleFactor:%u\n"
									 "noBits: %u  typeOrgFieldValues: %s\ncompressionType: %s  compressionRatio: %u\n";
	wxString s(str,wxConvUTF8);
	
/*	wxLogMessage(wxString::Format(s, grib2Message.version,grib2Message.length,table0_0[grib2Message.discipline].c_str(),
									 grib2Message.masterTableVersion,
									 grib2Message.localTableVersion,
									 table1_2[grib2Message.referenceTime].c_str(),
									 grib2Message.dt.Format().c_str(),
									 table1_3[grib2Message.productionState].c_str(),
									 table1_4[grib2Message.dataType].c_str(),
									 grib2Message.templateNo3,
									 grib2Message.noSectors,
									 grib2Message.noPointsParallel,
									 grib2Message.noPointsMeridian,
									 grib2Message.firstGridPointLat, grib2Message.firstGridPointLong,
									 grib2Message.lastGridPointLat, grib2Message.lastGridPointLong,
									 grib2Message.iDirectionIncr, grib2Message.jDirectionIncr,
									 Helper::toBinary(grib2Message.resCompFlag).c_str(), Helper::toBinary(grib2Message.scanMode).c_str(),
									 grib2Message.templateNo4,
									 (grib2Message.discipline == 0 && grib2Message.paramCategory != 255)?codeTable_4_1[grib2Message.paramCategory].c_str():wxString::Format(_("%i"),grib2Message.paramCategory).c_str(),
									 grib2Message.paramNo,
									 grib2Message.scaledValueSurface1,
									 grib2Message.typeOfProcess,
									 grib2Message.hoursAfterRefTime,
									 grib2Message.minutesAfterRefTime,
									 grib2Message.indicatorTimeRange,
									 grib2Message.forcastTimeUnits,
									 grib2Message.templateNo5,
									 codeTable_5_0[grib2Message.templateNo5].c_str(),
									 grib2Message.referenceVal,
									 grib2Message.binaryScaleFacor,
									 grib2Message.decimalScaleFactor,
									 grib2Message.noBits,
									 codeTable_5_1[grib2Message.typeOrgFieldValues].c_str(),
									 (grib2Message.compressionType != 255) ? codeTable5_40[grib2Message.compressionType].c_str():_T("n/a"),
									 grib2Message.compressionRatio));
*/

#endif 	

		if(grib2Message.discipline == 0 && grib2Message.paramCategory == 2 && grib2Message.paramNo == 0)
			this->gui->m_WindDir->SetValue(wxString::Format(_("%03.1f"),grib2Message.data[0]));				//WindDirection
		if(grib2Message.discipline == 0 && grib2Message.paramCategory == 2 && grib2Message.paramNo == 1)
			this->gui->m_textCtrlWindForce->SetValue(wxString::Format(_("%3.1f"),grib2Message.data[0]));	//WindForce	
			if(grib2Message.discipline == 0 && grib2Message.paramCategory == 2 && grib2Message.paramNo == 22)
			this->gui->m_textCtrlGust->SetValue(wxString::Format(_("%3.1f"),grib2Message.data[0]));	//WindForce(Gust)
		if(grib2Message.discipline == 10 && grib2Message.paramCategory == 0 && grib2Message.paramNo == 4)
			this->gui->m_textCtrlWaveDir->SetValue(wxString::Format(_("%03.1f"),grib2Message.data[0]));		//WaveDirection
		if(grib2Message.discipline == 10 && grib2Message.paramCategory == 0 && grib2Message.paramNo == 3)
				this->gui->m_textCtrlWaveHeight->SetValue(wxString::Format(_("%03.1f"),grib2Message.data[0]));	//WaveHeight 			
		if(grib2Message.discipline == 10 && grib2Message.paramCategory == 0 && grib2Message.paramNo == 8  && grib2Message.scaledValueSurface1 == 1)
			this->gui->m_textCtrlSwMin->SetValue(wxString::Format(_("%3.1f"),grib2Message.data[0]));	//SwellHeight Min	
		if(grib2Message.discipline == 10 && grib2Message.paramCategory == 0 && grib2Message.paramNo == 8  && grib2Message.scaledValueSurface1 == 2)
			this->gui->m_textCtrlSwMax->SetValue(wxString::Format(_("%3.1f"),grib2Message.data[0]*grib2Message.scaledValueSurface1));	//SwellHeight Max
		if(grib2Message.discipline == 0 && grib2Message.paramCategory == 3 && grib2Message.paramNo == 0)	// Pressure
			this->gui->m_textCtrlPressure->SetValue(wxString::Format(_("%4.0f"),grib2Message.data[0]/100));
		if(grib2Message.discipline == 0 && grib2Message.paramCategory == 0 && grib2Message.paramNo == 0)	// Temperature
			this->gui->m_textCtrlTemperature->SetValue(wxString::Format(_("%3.1f°C"),grib2Message.data[0]-273.15));
		if(grib2Message.discipline == 0 && grib2Message.paramCategory == 6 && grib2Message.paramNo == 1)	// Cloud cover %.
			this->gui->m_textCtrlClouds->SetValue(wxString::Format(_("%3.1f"),grib2Message.data[0]));	
		if(grib2Message.discipline == 10 && grib2Message.paramCategory == 1 && grib2Message.paramNo == 2)	// CurrentDir
			this->gui->m_textCtrlCurrentDir->SetValue(wxString::Format(_("%3.0f°"),grib2Message.data[0]-273.15));
		if(grib2Message.discipline == 10 && grib2Message.paramCategory == 1 && grib2Message.paramNo == 3)	// CurrentForce
			this->gui->m_textCtrlCurrentForce->SetValue(wxString::Format(_("%3.1f"),grib2Message.data[0]));	

	}
		if(grib2Message.data && !stream.Eof())
			{ 
				free(grib2Message.data);
				grib2Message.data = NULL;
			}
		if(grib2Message.bmpMask && !stream.Eof())
			{ 
				free(grib2Message.bmpMask);
				grib2Message.bmpMask = NULL;
			}
	}while(nextSection != -1  && !stream.Eof());	

	gui->pPlugIn->GetGRIBOverlayFactory()->setData(gui,grib2Data->hash,
						       grib2Message.firstGridPointLat,
						       grib2Message.firstGridPointLong,
						       grib2Message.lastGridPointLat,
						       grib2Message.lastGridPointLong,
						       grib2Message.scanMode,
						       grib2Message.iDirectionIncr,
						       grib2Message.jDirectionIncr,
						       grib2Message.noSectors,
						       grib2Message.noPointsMeridian,
						       grib2Message.noPointsParallel);
	
#ifdef GRIB2_DEBUG
    wxLogMessage(wxString::Format(_T("Total Recods read = %u nextSection = %i  EOF = %u\n"),records,nextSection,stream.Eof()));
      
    grib2Data->hash->BeginFind();
    wxHashTable_Node *node;
    while((node = grib2Data->hash->Next()))
    {
      wxDouble** data = (wxDouble**)node->GetData();
      if(data)
      printf("(%li) %f %f %f ....\n",node->GetKeyInteger(),data[0][0],data[0][1],data[0][2]);
       
    }  
#endif
	delete section0;
	delete section1;
	delete section2;
	delete section3;
	delete section4;
	delete section5;
	delete section6;	
	delete section7;

	isReading = false;
}



