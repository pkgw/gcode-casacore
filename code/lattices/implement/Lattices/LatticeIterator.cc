//# LatticeIter.cc: defines the RO_LatticeIterator and LatticeIterator classes
//# Copyright (C) 1994,1995,1996,1997,1998,1999,2000
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

#include <aips/aips.h>

#include <aips/Lattices/LatticeIterator.h>
#include <aips/Lattices/LatticeNavigator.h>
#include <aips/Lattices/LatticeStepper.h>
#include <aips/Lattices/TileStepper.h>
#include <aips/Arrays/Array.h>
#include <aips/Arrays/Vector.h>
#include <aips/Arrays/Matrix.h>
#include <aips/Arrays/Cube.h>
#include <aips/Arrays/IPosition.h>
#include <aips/Utilities/Assert.h> 
#include <aips/Exceptions/Error.h>


template <class T>
RO_LatticeIterator<T>::RO_LatticeIterator()
{
  DebugAssert(ok(), AipsError);
}

template <class T>
RO_LatticeIterator<T>::RO_LatticeIterator (const Lattice<T>& lattice)
: itsIterPtr (lattice.makeIter (TileStepper (lattice.shape(),
					     lattice.niceCursorShape())))
{
  DebugAssert(ok(), AipsError);
}

template <class T>
RO_LatticeIterator<T>::RO_LatticeIterator (const Lattice<T>& lattice,
					   const LatticeNavigator& method)
: itsIterPtr (lattice.makeIter (method))
{
  DebugAssert(ok(), AipsError);
}

template <class T>
RO_LatticeIterator<T>::RO_LatticeIterator (const Lattice<T>& lattice,
					   const IPosition& cursorShape)
: itsIterPtr (lattice.makeIter (LatticeStepper(lattice.shape(), cursorShape)))
{
  DebugAssert(ok(), AipsError);
}

template <class T>
RO_LatticeIterator<T>::RO_LatticeIterator (const RO_LatticeIterator<T>& other)
: itsIterPtr (other.itsIterPtr)
{
  DebugAssert(ok(), AipsError);
}

template <class T>
RO_LatticeIterator<T>::~RO_LatticeIterator()
{
  // CountedPtr destructor takes care of deletion of the LatticeIterInterface
}

template <class T>
RO_LatticeIterator<T>& RO_LatticeIterator<T>::operator=
                                 (const RO_LatticeIterator<T>& other)
{
  DebugAssert(ok(), AipsError);
  if (this != &other) {
    itsIterPtr = other.itsIterPtr;
  }
  return *this;
}

template <class T>
RO_LatticeIterator<T> RO_LatticeIterator<T>::copy() const
{
  RO_LatticeIterator<T> tmp;
  if (!isNull()) {
    tmp.itsIterPtr = itsIterPtr->clone();
  }
  return tmp;
}

template <class T>
Bool RO_LatticeIterator<T>::operator++(int)
{
  return itsIterPtr->operator++(0);
}
 
template <class T>
Bool RO_LatticeIterator<T>::operator++()
{
  return itsIterPtr->operator++();
}
 
template <class T>
Bool RO_LatticeIterator<T>::operator--(int)
{
  return itsIterPtr->operator--(0);
}

template <class T>
Bool RO_LatticeIterator<T>::operator--()
{
  return itsIterPtr->operator--();
} 

template <class T>
void RO_LatticeIterator<T>::reset()
{
  itsIterPtr->reset();
}

template <class T>
Bool RO_LatticeIterator<T>::atStart() const
{
  return itsIterPtr->atStart();
}

template <class T>
Bool RO_LatticeIterator<T>::atEnd() const
{
  return itsIterPtr->atEnd();
}

template <class T>
uInt RO_LatticeIterator<T>::nsteps() const
{
  return itsIterPtr->nsteps();
}

template <class T>
IPosition RO_LatticeIterator<T>::position() const
{
  return itsIterPtr->position();
}

template <class T>
IPosition RO_LatticeIterator<T>::endPosition() const
{
  return itsIterPtr->endPosition();
}

template <class T>
IPosition RO_LatticeIterator<T>::latticeShape() const
{
  return itsIterPtr->latticeShape();
}

template <class T>
IPosition RO_LatticeIterator<T>::cursorShape() const
{
  return itsIterPtr->cursorShape();
}

template <class T>
const Vector<T>& RO_LatticeIterator<T>::vectorCursor() const
{
  return itsIterPtr->vectorCursor (True, False);
}

template <class T>
const Matrix<T>& RO_LatticeIterator<T>::matrixCursor() const
{
  return itsIterPtr->matrixCursor (True, False);
}

template <class T>
const Cube<T>& RO_LatticeIterator<T>::cubeCursor() const
{
  return itsIterPtr->cubeCursor (True, False);
}

template <class T>
const Array<T>& RO_LatticeIterator<T>::cursor() const
{
  return itsIterPtr->cursor (True, False);
}


template <class T>
Bool RO_LatticeIterator<T>::ok() const
{
  if (!isNull()) {
    if (! itsIterPtr->ok()) {
      throw AipsError ("The actual Lattice Iterator class is inconsistent");
      return False;
    }
  }
  return True;
}



template <class T>
LatticeIterator<T>::LatticeIterator()
{}

template <class T>
LatticeIterator<T>::LatticeIterator (Lattice<T>& lattice)
: RO_LatticeIterator<T> (lattice)
{
  if (! lattice.isWritable()) {
    throw (AipsError ("LatticeIterator cannot be constructed; "
		      "lattice is not writable"));
  }
}

template <class T>
LatticeIterator<T>::LatticeIterator (Lattice<T>& lattice,
				     const LatticeNavigator& method)   
: RO_LatticeIterator<T> (lattice, method)
{
  if (! lattice.isWritable()) {
    throw (AipsError ("LatticeIterator cannot be constructed; "
		      "lattice is not writable"));
  }
}

template <class T>
LatticeIterator<T>::LatticeIterator (Lattice<T>& lattice,
				     const IPosition& cursorShape)   
: RO_LatticeIterator<T> (lattice, cursorShape)
{
  if (! lattice.isWritable()) {
    throw (AipsError ("LatticeIterator cannot be constructed; "
		      "lattice is not writable"));
  }
}

template <class T>
LatticeIterator<T>::LatticeIterator (const LatticeIterator<T>& other)
: RO_LatticeIterator<T> (other)
{}

template <class T>
LatticeIterator<T>::~LatticeIterator()
{}

template <class T>
LatticeIterator<T>& LatticeIterator<T>::operator=
                                      (const LatticeIterator<T>& other)
{
  RO_LatticeIterator<T>::operator= (other);
  return *this;
}

template <class T>
LatticeIterator<T> LatticeIterator<T>::copy() const
{
  LatticeIterator<T> tmp;
  if (!isNull()) {
    tmp.itsIterPtr = itsIterPtr->clone();
  }
  return tmp;
}

template <class T>
Vector<T>& LatticeIterator<T>::rwVectorCursor()
{
  return itsIterPtr->vectorCursor (True, True);
}

template <class T>
Matrix<T>& LatticeIterator<T>::rwMatrixCursor()
{
  return itsIterPtr->matrixCursor (True, True);
}

template <class T>
Cube<T>& LatticeIterator<T>::rwCubeCursor()
{
  return itsIterPtr->cubeCursor (True, True);
}

template <class T>
Array<T>& LatticeIterator<T>::rwCursor()
{
  return itsIterPtr->cursor (True, True);
}

template <class T>
Vector<T>& LatticeIterator<T>::woVectorCursor()
{
  return itsIterPtr->vectorCursor (False, True);
}

template <class T>
Matrix<T>& LatticeIterator<T>::woMatrixCursor()
{
  return itsIterPtr->matrixCursor (False, True);
}

template <class T>
Cube<T>& LatticeIterator<T>::woCubeCursor()
{
  return itsIterPtr->cubeCursor (False, True);
}

template <class T>
Array<T>& LatticeIterator<T>::woCursor()
{
  return itsIterPtr->cursor (False, True);
}
