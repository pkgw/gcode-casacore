//# LCPolygon.cc: Define a 2-dimensional region by a polygon
//# Copyright (C) 1998
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$


#include <trial/Lattices/LCPolygon.h>
#include <aips/Arrays/Vector.h>
#include <aips/Arrays/Matrix.h>
#include <aips/Tables/TableRecord.h>
#include <aips/Utilities/GenSort.h>
#include <aips/Utilities/Assert.h>
#include <aips/Exceptions/Error.h>

typedef Vector<Int> lcpolygon_gppbug1;


LCPolygon::LCPolygon()
{}

LCPolygon::LCPolygon (const Vector<Float>& x, const Vector<Float>& y,
		      const IPosition& latticeShape)
: LCRegionFixed (latticeShape),
  itsX (x.copy()),
  itsY (y.copy())
{
    defineBox();
    defineMask();
}

LCPolygon::LCPolygon (const Vector<Double>& x, const Vector<Double>& y,
		      const IPosition& latticeShape)
: LCRegionFixed (latticeShape),
  itsX (x.nelements()),
  itsY (y.nelements())
{
    for (uInt i=0; i<x.nelements(); i++) {
	itsX(i) = x(i);
	if (i < y.nelements()) {
	    itsY(i) = y(i);
	}
    }
    defineBox();
    defineMask();
}

LCPolygon::LCPolygon (const LCPolygon& that)
: LCRegionFixed (that),
  itsX (that.itsX),
  itsY (that.itsY)
{}

LCPolygon::~LCPolygon()
{}

LCPolygon& LCPolygon::operator= (const LCPolygon& that)
{
    if (this != &that) {
	LCRegionFixed::operator= (that);
	itsX.resize (that.itsX.nelements());
	itsY.resize (that.itsX.nelements());
	itsX = that.itsX;
	itsY = that.itsY;
    }
    return *this;
}

LCRegion* LCPolygon::cloneRegion() const
{
    return new LCPolygon(*this);
}

LCRegion* LCPolygon::doTranslate (const Vector<Float>& translateVector,
				  const IPosition& newLatticeShape) const
{
    uInt ndim = latticeShape().nelements();
    if (translateVector.nelements() != ndim
    ||  newLatticeShape.nelements() != ndim) {
        throw (AipsError ("LCEllipsoid::translate - "
			  "dimensionalities mismatch"));
    }
    Vector<Float> x, y;
    x = itsX;
    y = itsY;
    uInt n = x.nelements();
    for (uInt i=0; i<n; i++) {
        x(i) += translateVector(0);
        y(i) += translateVector(1);
    }
    return new LCPolygon (x, y, newLatticeShape);
}

String LCPolygon::className()
{
    return "LCPolygon";
}

TableRecord LCPolygon::toRecord (const String&) const
{
    TableRecord rec;
    rec.define ("name", className());
    rec.define ("x", itsX);
    rec.define ("y", itsY);
    rec.define ("shape", latticeShape().asVector());
    return rec;
}

LCPolygon* LCPolygon::fromRecord (const TableRecord& rec,
				  const String&)
{
    return new LCPolygon (rec.asArrayFloat ("x"),
			     rec.asArrayFloat ("y"),
			     Vector<Int>(rec.asArrayInt ("shape")));
}

void LCPolygon::defineBox()
{
    const IPosition& shape = latticeShape();
    uInt i;
    uInt nrp = itsX.nelements();
    // First make sure basic things are right.
    if (shape.nelements() != 2) {
	throw (AipsError ("LCPolygon::LCPolygon - "
			  "lattice is not 2-dimensional"));
    }
    if (itsY.nelements() != nrp) {
	throw (AipsError ("LCPolygon::LCPolygon - "
			  "lengths of x and y vector differ"));
    }
    if (nrp < 3) {
	throw (AipsError ("LCPolygon::LCPolygon - "
			  "at least 3 x,y points should be defined"));
    }
    // If the last point is not equal to the first one, add it.
    if (!near (itsX(nrp-1), itsX(0))  ||  !near(itsY(nrp-1), itsY(0))) {
	itsX.resize (nrp+1, True);
	itsY.resize (nrp+1, True);
	nrp++;
    }
    itsX(nrp-1) = itsX(0);        // Make sure they are always equal.
    itsY(nrp-1) = itsY(0);
    // Determine the maximum and minimum x,y.
    // They form the bounding box.
    // Check if at least one point is inside lattice.
    Float minx = itsX(0);
    Float maxx = itsX(0);
    Float miny = itsY(0);
    Float maxy = itsY(0);
    Bool inside = False;
    for (i=1; i<nrp; i++) {
	if (itsX(i) < minx) minx = itsX(i);
	if (itsX(i) > maxx) maxx = itsX(i);
	if (itsY(i) < miny) miny = itsY(i);
	if (itsY(i) > maxy) maxy = itsY(i);
	if (itsX(i) >= 0  &&  itsX(i) <= shape(0)-1
        &&  itsY(i) >= 0  &&  itsY(i) <= shape(1)-1) {
	    inside = True;
	}
    }
    if (!inside) {
	throw (AipsError ("LCPolygon::LCPolygon - "
			  "all x,y points are outside the lattice"));
    }
    // Determine blc and trc. Note that float to int conversion truncates,
    // so add almost 1 to get correct blc.
    IPosition blc(2, 0);
    IPosition trc(shape);
    if (minx > 0) blc(0) = Int(minx+0.5);
    if (miny > 0) blc(1) = Int(miny+0.5);
    if (maxx < shape(0)-1) trc(0) = Int(maxx+0.5);
    if (maxy < shape(1)-1) trc(1) = Int(maxy+0.5);
    setBox (Slicer(blc, trc, Slicer::endIsLast));
}

void LCPolygon::defineMask()
{
    uInt i;
    // Create and initialize the mask.
    const IPosition& blc = box().start();
    const IPosition& shape = box().length();
    Matrix<Bool> mask(shape);
    mask = False;
    uInt nrline = itsX.nelements() - 1;
    Block<Float> a(nrline);
    Block<Float> b(nrline);
    Block<Int>  dir(nrline, -1);     // -1=same dir, 0=vertical, 1=change dir
    // Fill the mask for all vertical lines.
    // Also determine the index of the last non-vertical line, which
    // is used in the slope-calculation loop.
    Int prev = -1;
    for (i=0; i<nrline; i++) {
	if (!near (itsX(i), itsX(i+1))) {
	    prev = i;
	} else {
	    dir[i] = 0;                         // vertical line
	    Int x = Int(itsX(i) + 0.5) - blc(0);
	    Int ys = Int(itsY(i)+0.5) - blc(1);
	    Int ye = Int(itsY(i+1)+0.5) - blc(1);
	    fillLine (mask, x, ys, ye);
	}
    }
    // Calculate the slope (a) and offset (b) for all line segments.
    // Vertical line segments are ignored.
    // Determine if a line segment changes direction with respect to
    // the last non-vertical line segment.
    for (i=0; i<nrline; i++) {
	if (dir[i] != 0) {
	    a[i] = (itsY(i) - itsY(i+1)) / (itsX(i) - itsX(i+1));
	    b[i] = itsY(i) - a[i] * itsX(i);
	    if ((itsX(i) > itsX(i+1)  &&  itsX(prev) < itsX(prev+1))
	    ||  (itsX(i) < itsX(i+1)  &&  itsX(prev) > itsX(prev+1))) {
		dir[i] = 1;
	    }
	    prev = i;
	}
    }
    // Loop through all x-es and determine which y-points are inside.
    // This is done by determining the crossing point of all the x-lines
    // with all line segments (the in/out algorithm).
    Block<Int> cross(nrline);
    for (Int x=0; x<shape(0); x++) {
	uInt nrcross = 0;
	Float xf = x + blc(0);
	for (i=0; i<nrline; i++) {
	    // Ignore vertical lines.
	    if (dir[i] != 0) {
		// A pixel is not a single point but in fact an interval.
		Float xfs = xf - 0.5;
		Float xfe = xf + 0.5;
		Int cr = -1;
		// When a polygon point is a pixel point (in x), always
		// count the ending point of the line segment.
		// Count the starting point only if the direction has
		// changed with respect to the previous non-vertical segment.
		if (itsX(i) > xfs  &&  itsX(i) < xfe) {
		    if (dir[i] == 1) {
			cr = Int(itsY(i) + 0.5) - blc(1);
		    }
		} else if (itsX(i+1) > xfs  &&  itsX(i+1) < xfe) {
		    cr = Int(itsY(i+1) + 0.5) - blc(1);
		} else {
		    // Calculate the crossing point for the interval around xf
		    // if that interval and the line segment overlap.
		    if (xfs < itsX(i+1)  &&  xfe > itsX(i)
		    || (xfs < itsX(i)  &&  xfe > itsX(i+1))) {
			cr = Int(a[i] * xfs + b[i] + 0.5) - blc(1);
			Int cre = Int(a[i] * xfe + b[i] + 0.5) - blc(1);
			if (cr != cre) {
			    fillLine (mask, x, cr, cre);
			}
		    }
		}
		if (cr >= 0) {
		    cross[nrcross++] = cr;
		}
	    }
	}
	DebugAssert (nrcross >= 2, AipsError);
	DebugAssert (nrcross%2 == 0, AipsError);
	GenSort<Int>::sort (cross, nrcross);
	for (i=0; i<nrcross; i+=2) {
	    Int ys = cross[i];
	    Int ye = cross[i+1];
	    while (ys <= ye) {
		mask(x,ys++) = True;
	    }
	}
    }
    setMask (mask);
}

void LCPolygon::fillLine (Matrix<Bool>& mask, Int x, Int y1, Int y2)
{
    Int ys = y1;
    if (y2 < y1) {
	ys = y2;
	y2 = y1;
    }
    while (ys <= y2) {
	mask(x,ys++) = True;
    }
}
