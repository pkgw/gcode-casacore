//# NewMSSimulator.h: this defines the MeasurementSet Simulator
//# Copyright (C) 1995,1996,1998,1999,2000,2002
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

#if !defined(TRIAL_MSSIMULATOR_H)
#define TRIAL_MSSIMULATOR_H


//# Includes
#include <aips/Utilities/String.h>
#include <aips/Arrays/Vector.h>
#include <aips/Arrays/Matrix.h>
#include <aips/Arrays/Cube.h>
#include <aips/Mathematics/Complex.h>
#include <aips/Quanta/Quantum.h>
#include <aips/Measures/MPosition.h>
#include <aips/Measures/MEpoch.h>
#include <aips/Measures/MDirection.h>
#include <aips/Tables/TiledDataStManAccessor.h>

//# Forward Declarations
class MeasurementSet;

// <category lib=aips module="ModuleName">
// <summary> Create an empty MeasurementSet from observation and telescope descriptions. </summary>
// <reviewed reviewer="" date="" tests="">
//
// <prerequisite>
//# Classes you should understand before using this one.
// <list>
//   <item> MeasurementSet
// </list>
// </prerequisite>
//
// <etymology>
// MS is from MeasurementSet, Simulator refers to the generation of
// 'fake' data from a set of parameters for instrument and sources.
// </etymology>
//
// <synopsis> // This class creates a MeasurementSet from a set of parameters for instrument
// and sources. It does not simulate the data, only the coordinates of a 
// measurement. The application "simulator" uses this class to create a true
// simulated MS with perfect or corrupted data.
// </synopsis> 
//
// <motivation>
// To test calibration and imaging programs it is necessary to have flawless
// data and data with errors that are known exactly. This class generates
// empty MeasurementSets (only coordinates filled in) that can be filled 
// with predicted data.
// </motivation>
//
// <todo asof="$DATE:$">
//# A List of bugs, limitations, extensions or planned refinements.
// <li>     The amount of information to be specified by the user
//          could be much larger. For the moment it has been restricted to
//          what is needed for testing the synthesis imaging code. Already
//          it is possible to create MeasurementSets that cannot be processed
//          yet.
// </todo>

class NewMSSimulator
{
public: 
  
  // Constructor from name only
  NewMSSimulator(const String&);
  
  // Copy constructor - for completeness only
  NewMSSimulator(const NewMSSimulator & mss);
  
  //# Destructor
  ~NewMSSimulator();
  
//# Operators
  // Assignment
  NewMSSimulator & operator=(const NewMSSimulator &);
  
  // set the antenna and array data. These are written immediately to the
  // existing MS. The same model is used for the other init infor.
  void initAnt(const String& telname,
	       const Vector<Double>& x, 
	       const Vector<Double>& y, 
	       const Vector<Double>& z,
	       const Vector<Double>& dishDiameter,
	       const Vector<Double>& offset,
	       const Vector<String>& mount,
	       const Vector<String>& name,
	       const String& coordsystem,
	       const MPosition& mRefLocation);

  // set the observed fields
  void initFields(const String& sourceName, 
		  const MDirection& sourceDirection,
		  const String& calCode);

  // set the Feeds;  brain dead version
  void initFeeds(const String& mode);

  // set the spectral windows information
  void initSpWindows(const String& spWindowName,
		     const Int& nChan,
		     const Quantity& startFreq,
		     const Quantity& freqInc,
		     const Quantity& freqRes,
		     const String& stokesString);

  void setFractionBlockageLimit(const Double fraclimit) 
    { fractionBlockageLimit_p = fraclimit; }

  void setElevationLimit(const Quantity& ellimit) 
    { elevationLimit_p = ellimit; }

  void setAutoCorrelationWt(const Float autocorrwt) 
    { autoCorrelationWt_p = autocorrwt; }

  void observe(const String& sourceName,
	       const String& spWindowName,
	       const Quantity& qIntegrationTime, 
	       const Bool      useHourAngles,
	       const Quantity& qStartTime, 
	       const Quantity& qStopTime, 
	       const MEpoch&   mRefTime);

private:

  // Prevent use of default constructor
  NewMSSimulator() {};

//# Data Members
  Double fractionBlockageLimit_p;
  Quantity elevationLimit_p;
  Float autoCorrelationWt_p;
  String telescope_p;

  MeasurementSet* ms_p;

  TiledDataStManAccessor dataAcc_p, scratchDataAcc_p, sigmaAcc_p, flagAcc_p, imweightAcc_p;
  Int hyperCubeID_p;

  void local2global(Vector<Double>& xReturned,
		    Vector<Double>& yReturned,
		    Vector<Double>& zReturned,
		    const MPosition& mRefLocation,
		    const Vector<Double>& xIn,
		    const Vector<Double>& yIn,
		    const Vector<Double>& zIn);

  void longlat2global(Vector<Double>& xReturned,
		      Vector<Double>& yReturned,
		      Vector<Double>& zReturned,
		      const MPosition& mRefLocation,
		      const Vector<Double>& xIn,
		      const Vector<Double>& yIn,
		      const Vector<Double>& zIn);

  // Returns the fractional blockage of one antenna by another
  // We will want to put this somewhere else eventually, but I don't yet know where!
  // Till then.
  // fraction1: fraction of antenna 1 that is blocked by 2
  // fraction2: fraction of antenna 2 that is blocked by 1
  // hint: at least one of the two will be 0.0
  void  blockage(Double &fraction1, Double &fraction2,
		 const Vector<Double>& uvw,             // uvw in same units as diam!
		 const Double diam1, const Double diam2);

  String formatDirection(const MDirection&);
  String formatTime(const Double);

  void addHyperCubes(const Int nChan, const Int nChan, const Int nCorr,
		     const Int obsType);
};

#endif




