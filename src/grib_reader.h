#ifndef GRIBREADER_H
#define GRIBREADER_H

#include <wx/wx.h>

class MainDialog;
class Grib2Data;

struct message
{
	wxUint8		version;
	wxUint64 	length;
	wxUint8		discipline;
	// end Sector 0
	wxUint8		masterTableVersion;
	wxUint8		localTableVersion;
	wxUint8  	referenceTime;
	wxDateTime	dt;
	wxUint8		productionState;
	wxUint8		dataType;
	// end Secstor 1
	wxUint16	templateNo3;	
	wxUint32	noSectors;
	wxUint32	noPointsParallel;
	wxUint32	noPointsMeridian;
	wxDouble	firstGridPointLat;
	wxDouble	firstGridPointLong;
	wxDouble	lastGridPointLat;
	wxDouble	lastGridPointLong;
	wxDouble	iDirectionIncr; // i = parallel
	wxDouble	jDirectionIncr; // j = Meridian;
	wxUint8		resCompFlag;
	wxUint8		scanMode;
	// end Secstor 3
	wxUint16	templateNo4;
	wxUint8		paramCategory;
	wxUint8		paramNo;
	wxUint8		typeOfProcess;
	wxUint16	hoursAfterRefTime;
	wxUint8		minutesAfterRefTime;
	wxUint8		indicatorTimeRange;
	wxUint32	forcastTimeUnits;
	// end Secstor 4
	wxUint16	templateNo5;
	wxFloat32	referenceVal;
	wxUint16	binaryScaleFacor;
	wxUint16	decimalScaleFactor;
	wxUint8		noBits;
	wxUint8		typeOrgFieldValues;
	wxUint32	scaledValueSurface1;
	wxUint8		compressionType;
	wxUint8		compressionRatio;
	// end Secstor 5
	wxInt8*		bmpMask;
	wxInt32		bmpSize;
	// end Secstor 6
	wxDouble*	data;
	// end Sector 7
};
typedef struct message Grib2Message;

class GribReader {

public:
	GribReader(MainDialog *dlg);
	~GribReader();
	
	void readGribFile(wxString file, wxUint64 fp, wxUint32 hour, wxDateTime dt);
	
	Grib2Data *grib2Data;
	Grib2Message grib2Message;

	bool isReading;
	
private:
	MainDialog *gui;

};

#endif // GRIBREADER_H
