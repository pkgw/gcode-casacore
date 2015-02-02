//# Copyright (C) 2000,2001
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

#ifndef LATTICES_MASKEDLATTICESTATSDATAPROVIDER_H
#define LATTICES_MASKEDLATTICESTATSDATAPROVIDER_H

#include <casacore/lattices/LatticeMath/LatticeStatsDataProviderBase.h>

#include <casacore/lattices/Lattices/MaskedLattice.h>
#include <casacore/lattices/Lattices/MaskedLatticeIterator.h>

#include <casacore/casa/aips.h>

namespace casacore {

// Data provider which allows stats framework to iterate through a masked lattice.

template <class T> class MaskedLatticeStatsDataProvider
	: public LatticeStatsDataProviderBase<T> {
public:

	MaskedLatticeStatsDataProvider(MaskedLattice<T>& lattice);

	~MaskedLatticeStatsDataProvider();

	void operator++();

	uInt estimatedSteps() const;

	// Are there any data sets left to provide?
	Bool atEnd() const;

	// Take any actions necessary to finalize the provider. This will be called when
	// atEnd() returns True.
	void finalize();

	// get the count of elements in the current data set. When implementing this method, be
	// certain to take stride into account; ie for a data set with nominally 100 elements that
	// is to have a stride of two, this method should return 50.
	uInt64 getCount();

	// get the current data set
	const T* getData();

	// Get the associated mask of the current dataset. Only called if hasMask() returns True;
	const Bool* getMask();

	// Does the current data set have an associated mask?
	Bool hasMask() const;

	// reset the provider to point to the first data set it manages.
	void reset();

	// <group>
	// see base class documentation.
	void updateMaxPos(const std::pair<Int64, Int64>& maxpos);

	void updateMinPos(const std::pair<Int64, Int64>& minpos);
	// </group>

private:
	RO_MaskedLatticeIterator<T> _iter;
	Array<T> _currentSlice;
	Array<Bool> _currentMaskSlice;
	const T* _currentPtr;
	const Bool* _currentMaskPtr;
	Bool _delData, _delMask;

	void _freeStorage();

	uInt _nsteps() const;
};

}

#ifndef CASACORE_NO_AUTO_TEMPLATES
#include <casacore/lattices/LatticeMath/MaskedLatticeStatsDataProvider.tcc>
#endif //# CASACORE_NO_AUTO_TEMPLATES

#endif
