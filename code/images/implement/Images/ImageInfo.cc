//# ImageInfo.cc: Miscellaneous information related to an image
//# Copyright (C) 1998,1999,2001,2002
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
//#
//# $Id$

#include <trial/Images/ImageInfo.h>
#include <aips/Exceptions/Error.h>
#include <aips/Quanta/Quantum.h>
#include <aips/Quanta/QuantumHolder.h>
#include <aips/Arrays/Vector.h>
#include <aips/Mathematics/Math.h>
#include <aips/Containers/Record.h>
#include <aips/Utilities/String.h>


#include <aips/iostream.h>


ImageInfo::ImageInfo()
: itsRestoringBeam(defaultRestoringBeam()),
  itsImageType(defaultImageType())
{}

ImageInfo::~ImageInfo()
{}

void ImageInfo::copy_other(const ImageInfo &other)
{
    if (this != &other) {
       itsRestoringBeam.resize(other.itsRestoringBeam.nelements());
       itsRestoringBeam = other.itsRestoringBeam.copy();
//   
       itsImageType = other.itsImageType;
    }
}

ImageInfo::ImageInfo(const ImageInfo &other)
{
    copy_other(other);
}

ImageInfo &ImageInfo::operator=(const ImageInfo &other)
{
    copy_other(other);
//
    return *this;
}

Vector<Quantum<Double> > ImageInfo::defaultRestoringBeam()
{
    Vector<Quantum<Double> > tmp;
    return tmp;
}

ImageInfo::ImageTypes ImageInfo::defaultImageType()
{
    return ImageInfo::Intensity;
}

Vector<Quantum<Double> > ImageInfo::restoringBeam() const
{
    return itsRestoringBeam;
}

ImageInfo& ImageInfo::setRestoringBeam(const Vector<Quantum<Double> >& beam)
{
    if (beam.nelements()!=0 && beam.nelements()!=3) {
      throw (AipsError (String("ImageInfo::setRestoringBeam - beam ") +
                        String("vector must be of length 0 or 3")));
    }
//
    if (beam.nelements()>uInt(0)) {
       setRestoringBeam(beam(0), beam(1), beam(2));
    } else {
       itsRestoringBeam.resize(0);
    }
    return *this;
}

ImageInfo& ImageInfo::setRestoringBeam(const Quantum<Double>& major,
                                       const Quantum<Double>& minor,
                                       const Quantum<Double>& pa)
{
   if (major.getValue()<=0.0 || minor.getValue()<=0.0) {
         throw (AipsError (String("ImageInfo::setRestoringBeam - ") +
                         String("the beam size must be positive")));
   }
//
   Unit arcsec("arcsec");
   if (major.getFullUnit()!=arcsec || minor.getFullUnit()!=arcsec ||
       pa.getFullUnit()!=arcsec) {
      throw (AipsError (String("ImageInfo::setRestoringBeam - the beam ") +
             String("units must be angular")));
   }
//
   Double majord = major.getValue(arcsec);
   Double minord = minor.getValue(arcsec);
   if (majord < minord) {
      throw (AipsError (String("ImageInfo::setRestoringBeam - the major ") +
             String("axis must be greater than the minor axis")));
   }
//
   itsRestoringBeam.resize(3);
   itsRestoringBeam(0) = major;
   itsRestoringBeam(1) = minor;
//
// When I consolidate Gaussian definitions, this
// should be converted to some standard P.A. range
//
   itsRestoringBeam(2) = pa;
//
   return *this;
}

ImageInfo& ImageInfo::removeRestoringBeam()
{
   itsRestoringBeam.resize(0);
   return *this;
}


ImageInfo::ImageTypes ImageInfo::imageType() const
{
    return itsImageType;
}

ImageInfo& ImageInfo::setImageType(ImageInfo::ImageTypes type) 
{
    itsImageType = type;
//
    return *this;
}


String ImageInfo::imageType(ImageInfo::ImageTypes type)
{
   String typeOut;
   switch(type) {
//
      case ImageInfo::Undefined:
        typeOut = String("Undefined"); 
        break;
      case ImageInfo::Intensity:
        typeOut = String("Intensity"); 
        break;
      case ImageInfo::Beam:
        typeOut = String("Beam"); 
        break;
      case ImageInfo::ColumnDensity:
        typeOut = String("Column Density"); 
        break;
      case ImageInfo::DepolarizationRatio:
        typeOut = String("Depolarization Ratio"); 
        break;
      case ImageInfo::KineticTemperature:
        typeOut = String("Kinetic Temperature"); 
        break;
      case ImageInfo::MagneticField:
        typeOut = String("Magneti Field"); 
        break;
      case ImageInfo::OpticalDepth:
        typeOut = String("Optical Depth"); 
        break;
      case ImageInfo::RotationMeasure:
        typeOut = String("Rotation Measure"); 
        break;
      case ImageInfo::RotationalTemperature:
        typeOut = String("Rotational Temperature"); 
        break;
      case ImageInfo::SpectralIndex:
        typeOut = String("Spectral Index"); 
        break;
      case ImageInfo::Velocity:
        typeOut = String("Velocity"); 
        break;
      case ImageInfo::VelocityDispersion:
        typeOut = String("Velocity Dispersion"); 
        break;
      default:
        typeOut = String("Undefined"); 
        break;
   }
//
   return typeOut;
}


ImageInfo::ImageTypes ImageInfo::imageType(String type)
{
   String typeUp = upcase(type);
   for (uInt i=0; i<ImageInfo::nTypes; i++) {
      ImageInfo::ImageTypes t0 = static_cast<ImageInfo::ImageTypes>(i);
      String t1Up = upcase(ImageInfo::imageType(t0));
      if (t1Up==typeUp) return t0;     // Exact match
   }
//  
   return defaultImageType();
}

ImageInfo::ImageTypes ImageInfo::imageTypeFromFITS (Int value)
{
    if (value==0) {
       return ImageInfo::Beam;
    } else if (value==8) {
       return ImageInfo::SpectralIndex;
    } else if (value==9) {
       return ImageInfo::OpticalDepth;
    } else {
       return ImageInfo::Undefined;
    }
}



Bool ImageInfo::toRecord(String & error, RecordInterface & outRecord) const
{
    error = "";
    Bool ok = True;
//
// If the beam is null, dont do anything as it will get
// restored as null as well if ist not in the record
//
    if (itsRestoringBeam.nelements()>0) {
       Record restoringBeamRecord;
       Vector<String> names(3);
       names(0) = "major"; names(1) = "minor"; names(2) = "positionangle";
       for (uInt i=0; i<3; i++) {
          QuantumHolder qh(itsRestoringBeam(i));
          Record tmp;
          ok = qh.toRecord(error, tmp);
          if (!ok) return False;
          restoringBeamRecord.defineRecord(names(i), tmp);
       }
       outRecord.defineRecord("restoringbeam", restoringBeamRecord);
    }
//
    {
       String type = ImageInfo::imageType(itsImageType);
       outRecord.define("imagetype", type);
    }
//
    return ok;
}

Bool ImageInfo::fromRecord(String & error, const RecordInterface & inRecord)
//
// Returns default object if none in record
//
{
// Make sure we are "empty" first

    ImageInfo tmp;
    (*this) = tmp; 
//
    error = "";
    QuantumHolder qh;
    Bool ok = True;
//
    if (inRecord.isDefined("restoringbeam")) {
       Vector<Quantum<Double> > restoringBeam(3);
       const RecordInterface& subRec = inRecord.asRecord("restoringbeam");
       if (subRec.nfields()!=3) {
          error = "Restoring beam record does not contain 3 fields";
          return False;
       }
//
       if (subRec.isDefined("major")) {
          const RecordInterface& subRec0 = subRec.asRecord("major");
          ok = qh.fromRecord(error, subRec0);
          if (ok) restoringBeam(0) = qh.asQuantumDouble();
       } else {
          error = "Field major missing from restoring beam record";
          return False;
       }
//
       if (subRec.isDefined("minor")) {
          const RecordInterface& subRec1 = subRec.asRecord("minor");
          ok = qh.fromRecord(error, subRec1);
          if (ok) restoringBeam(1) = qh.asQuantumDouble();
       } else {
          error = "Field minor missing from restoring beam record";
          return False;
       }
//
       if (subRec.isDefined("positionangle")) {
          const RecordInterface& subRec2 = subRec.asRecord("positionangle");
          ok = qh.fromRecord(error, subRec2);
          if (ok) restoringBeam(2) = qh.asQuantumDouble();
       } else {
          error = "Field positionangle missing from restoring beam record";
          return False;
       }
//
       setRestoringBeam(restoringBeam);
   }
//
   if (inRecord.isDefined("imagetype")) {
      String type = inRecord.asString("imagetype");
      setImageType (ImageInfo::imageType(type));
   }
//
   return ok;
}

Bool ImageInfo::toFITS(String & error, RecordInterface & outRecord) const
{
    error = "";
//

    Vector<Quantum<Double> > beam = restoringBeam();
    if (beam.nelements()>0) {
       Double bmaj = beam(0).getValue(Unit("deg"));
       Double bmin = beam(1).getValue(Unit("deg"));
       Double bpa  = beam(2).getValue(Unit("deg"));
//
       outRecord.define("bmaj", bmaj);
       outRecord.define("bmin", bmin);
       outRecord.define("bpa", bpa);
    } else {
       if (!outRecord.isFixed()) {
          Int field = outRecord.fieldNumber("bmaj");
          if (field >= 0) outRecord.removeField(field);
          field = outRecord.fieldNumber("bmin");
          if (field >= 0) outRecord.removeField(field);
          field = outRecord.fieldNumber("bpa");
          if (field >= 0) outRecord.removeField(field);
       } 
    }
//
    ImageInfo::ImageTypes type = imageType();
    if (type!=ImageInfo::Undefined) {
       String type = ImageInfo::imageType(itsImageType);
       outRecord.define("btype", type);
    } else {
       if (!outRecord.isFixed()) {
          Int field = outRecord.fieldNumber("btype");
          if (field >= 0) outRecord.removeField(field);
       }
    }
//
    return True;
}



Bool ImageInfo::fromFITS(Vector<String>& error, const RecordInterface& header)
{
   error.resize(2);
   Bool ok = True;
   ImageInfo tmp;
   (*this) = tmp; // Make sure we are "empty" first;
//
   if (header.isDefined("bmaj") && header.isDefined("bmin") &&
       header.isDefined("bpa")) {
      Bool ok = header.dataType("bmaj")==TpDouble && header.dataType("bmin")==TpDouble &&
                header.dataType("bpa")==TpDouble;
      if (!ok) ok = header.dataType("bmaj")==TpFloat && header.dataType("bmin")==TpFloat &&
                header.dataType("bpa")==TpFloat;
      if (ok) {
         Double bmaj = header.asDouble("bmaj");
         Double bmin = header.asDouble("bmin");
         Double bpa = header.asDouble("bpa");
//   
         Quantum<Double> bmajq(max(bmaj,bmin), "deg");
         Quantum<Double> bminq(min(bmaj,bmin), "deg");
         bmajq.convert(Unit("arcsec"));
         bminq.convert(Unit("arcsec"));
         setRestoringBeam(bmajq, bminq, Quantum<Double>(bpa, "deg"));
      } else {
         error(0) = "BMAJ, BMIN, BPA fields are not of type Double or Float";
         ok = False;
      }
   }
//
   if (header.isDefined("btype")) {
      if (header.dataType("btype")==TpString) {
         String type = header.asString("btype");

// We are going to cope with aips++ values and Miriad values
// For Miriad there are a few extra ones (which we put on the Stokes
// axis in aips++ - e.g. position angle).  For the ones that are common
// the Miriad ones have underscores and the aips++ ones have spaces

         ImageInfo::ImageTypes imageType = ImageInfo::imageType(type);
         if (imageType != ImageInfo::Undefined) {
            setImageType(imageType);
         } else {
            imageType = MiriadImageType (type);
            if (imageType != ImageInfo::Undefined) {
               setImageType(imageType);
            }
         }
      }  else {
         error(1) = "BTYPE field is not of type String";
         ok = False;
      }
   }
//
   if (ok) error.resize(0);
   return ok;
}


ostream &operator<<(ostream &os, const ImageInfo &info)
{
    if (info.restoringBeam().nelements()>0) {
       os << "Restoring beam : " << info.restoringBeam()(0) << ", " 
          << info.restoringBeam()(1) << ", " << info.restoringBeam()(2) << endl;
       os << "Image Type = " << info.imageType(info.imageType()) << endl;
    }
    return os;
}


Vector<String> ImageInfo::keywordNamesFITS()
{
    Vector<String> vs(4);
    vs(0) = "bmaj";
    vs(1) = "bmin";
    vs(2) = "bpa";
    vs(3) = "btype";              // Miriad convention
    return vs;
}  


ImageInfo::ImageTypes ImageInfo::MiriadImageType (const String& type) 
//
// We don't fully handle all the Miriad values because
// some of them  (see below) are dealt with in aips++ by 
// the Stokes axis.  
//
{
   String typeUp = upcase(type);
   ImageInfo::ImageTypes typeOut = ImageInfo::Undefined;
//
   if (typeUp==String("POLARIZED_INTENSITY") ||
       typeUp==String("FRACTIONAL_POLARIZATION") ||
       typeUp==String("POSITION_ANGLE")) {
      typeOut = ImageInfo::Undefined;
   } else if (typeUp==String("INTENSITY")) {
      typeOut = ImageInfo::Intensity;
   } else if (typeUp==String("BEAM")) {
      typeOut = ImageInfo::Beam;
   } else if (typeUp==String("COLUMN_DENSITY")) {
      typeOut = ImageInfo::ColumnDensity;
   } else if (typeUp==String("DEPOLARIZATION_RATIO")) {
      typeOut = ImageInfo::DepolarizationRatio;
   } else if (typeUp==String("KINETIC_TEMPERATURE")) {
      typeOut = ImageInfo::KineticTemperature;
   } else if (typeUp==String("MAGNETIC_FIELD")) {
      typeOut = ImageInfo::MagneticField;
   } else if (typeUp==String("OPTICAL_DEPTH")) {
      typeOut = ImageInfo::OpticalDepth;
   } else if (typeUp==String("ROTATION_MEASURE")) {
      typeOut = ImageInfo::RotationMeasure;
   } else if (typeUp==String("ROTATIONAL_TEMPERATURE")) {
      typeOut = ImageInfo::RotationalTemperature;
   } else if (typeUp==String("SPECTRAL_INDEX")) {
      typeOut = ImageInfo::SpectralIndex;
   } else if (typeUp==String("VELOCITY")) {
      typeOut = ImageInfo::Velocity;
   } else if (typeUp==String("VELOCITY_DISPERSION")) {
      typeOut = ImageInfo::VelocityDispersion;
   }
//
   return typeOut;
}
