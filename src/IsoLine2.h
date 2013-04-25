/**********************************************************************
zyGrib: meteorological GRIB file viewer
Copyright (C) 2008 - Jacques Zaninetti - http://www.zygrib.org

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/*************************************
Dessin des données GRIB (avec QT)
*************************************/

#ifndef ISOLINE_H
#define ISOLINE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <set>

//#include "grib_reader.h"
#include "griboverlayfactory.h"

class PlugIn_ViewPort;
class wxMemoryDC;

class Segment;
WX_DECLARE_LIST(Segment, MySegList);
WX_DECLARE_LIST(MySegList, MySegListList);


// TODO: join segments and draw a spline

//===============================================================
// Elément d'isobare qui passe dans un carré (ab-cd)de la grille.
// a  b
// c  d
// Rejoint l'arête (i,j)-(k,l) à l'arête (m,n)-(o,p) (indices ds la grille GRIB)
class Segment
{
    public:
        Segment (int I, int J,
                char c1, char c2, char c3, char c4,
                double **grid, double pressure, wxDouble tlat,wxDouble tlon, wxDouble incrLat, wxDouble incrLon);

        int   i,j,  k,l;   // arête 1
        double px1,  py1;   // Coordonées de l'intersection (i,j)-(k,l)
        int m,n, o,p;      // arête 2
        double px2,  py2;   // Coordonées de l'intersection (m,n)-(o,p)
        bool  bUsed;

    private:
        wxDouble tlat,tlon;
	wxDouble incrLat,incrLon;
	
        void traduitCode(int I, int J, char c1, int &i, int &j);

        void intersectionAreteGrille(int i,int j, int k,int l,
                double *x, double *y,
                double **grid, double pressure);
};

//===============================================================
class IsoLine
{
    public:
        IsoLine(double val, double **grid, wxUint32 latSectors, wxUint32 lonSectors,
		wxDouble tlat, wxDouble tlon, wxDouble incrLon, wxDouble incrLat);
        ~IsoLine();


        void drawIsoLine(wxDC &pmdc, PlugIn_ViewPort *vp, bool bShowLabels, bool bHiDef);

        void drawIsoLineLabels(wxDC *pmdc, wxColour couleur, PlugIn_ViewPort *vp,
                                int density, int first, double coef);

        int getNbSegments()     {return trace.size();}

    private:
        double value;
        int    W, H;     // taille de la grille
        double **rec;
	wxUint32 latSectors;
	wxUint32 lonSectors; 
	wxDouble tlat; 
	wxDouble tlon;
	wxDouble incrLon;
	wxDouble incrLat;

        wxColour isoLineColor;
        std::list<Segment *> trace;

        void intersectionAreteGrille(int i,int j, int k,int l, double *x, double *y,
                        const double **grid);

        //-----------------------------------------------------------------------
        // Génère la liste des segments.
        // Les coordonnées sont les indices dans la grille du GribRecord
        //---------------------------------------------------------
        void extractIsoLine(double **grid);
        MySegList *BuildContinuousSegment(void);

        MySegList       m_seglist;
        MySegListList   m_SegListList;
};




#endif
