//# imhead.cc: List image header
//# Copyright (C) 1996,1997
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
//
#include <aips/aips.h>
#include <aips/Exceptions/Error.h>
#include <aips/Inputs/Input.h>
#include <aips/Logging.h>
#include <aips/Utilities/DataType.h>
#include <aips/Utilities/String.h>
#include <trial/Images/ImageSummary.h>
#include <trial/Images/PagedImage.h>


main (int argc, char **argv)
{
try {

   Input inputs(1);
   inputs.Version ("$Revision$");

   inputs.Create("in", "","Input image name?","in");
   inputs.ReadArguments(argc, argv);
   const String in = inputs.GetString("in");


// Open image, construct helper class object and list header

   if (in.empty()) {
      cout << "You must specify the image file name" << endl;
      return 1;
   }
   DataType imageType = imagePixelType(in);
   LogOrigin or("imhead", "main()", WHERE);
   LogIO os(or);

   if (imageType==TpFloat) {    
      const PagedImage<Float> inImage(in);
      const ImageSummary<Float> header(inImage);
      header.list(os);
   } else if (imageType==TpComplex) {    
      const PagedImage<Complex> inImage(in);
      const ImageSummary<Complex> header(inImage);
      header.list(os);
   } else {
      cout << "images of type " << imageType << " not yet supported" << endl;
   }
}

  catch (AipsError x) {
     cerr << "aipserror: error " << x.getMesg() << endl;
     return 1;
  } end_try;

return 0;
}
