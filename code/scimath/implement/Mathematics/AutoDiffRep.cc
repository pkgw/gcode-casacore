//# AutoDiffRep.cc: Representation of an automatic differential class data
//# Copyright (C) 2001
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
/// 24.05 : add neg, exp, derivatives(res)

//# Includes
#include <trial/Mathematics/AutoDiffRep.h>
#include <aips/Arrays/Vector.h>

//# Constants
template <class T> T AutoDiffRep<T>::aZero = T(0);

//# Constructors
template <class T>
AutoDiffRep<T>::AutoDiffRep() :
  val_p(T(0.0)), nd_p(0), nocopy_p(False), grad_p(0) {}

template <class T>
AutoDiffRep<T>::AutoDiffRep(const T &v) :
  val_p(v), nd_p(0), nocopy_p(False), grad_p(0) {}

template <class T>
AutoDiffRep<T>::AutoDiffRep(const T &v, const uInt ndiffs, const uInt n) :
  val_p(v), nd_p(ndiffs), nocopy_p(False),
  grad_p(ndiffs>0 ? new T[ndiffs] : 0) {
  for (uInt i=0; i<nd_p; i++) grad_p[i] = (i==n ? T(1) : T(0));
}

template <class T>
AutoDiffRep<T>::AutoDiffRep(const T &v, const uInt ndiffs) :
  val_p(v), nd_p(ndiffs), nocopy_p(False),
  grad_p(ndiffs>0 ? new T[ndiffs] : 0) {
  for (uInt i=0; i<nd_p; i++) grad_p[i] = T(0);
}

template <class T>
AutoDiffRep<T>::AutoDiffRep(const uInt ndiffs) :
  val_p(0), nd_p(ndiffs), nocopy_p(False),
  grad_p(ndiffs>0 ? new T[ndiffs] : 0) {
  for (uInt i=0; i<nd_p; i++) grad_p[i] = T(0);
}

template <class T>
AutoDiffRep<T>::AutoDiffRep(const AutoDiffRep<T> &other) :
  val_p(other.val_p), nd_p(other.nd_p), nocopy_p(False),
  grad_p(other.nd_p>0 ? new T[other.nd_p] : 0) {
  for (uInt i=0; i<nd_p; i++) grad_p[i] = other.grad_p[i];
}

template <class T>
AutoDiffRep<T>::AutoDiffRep(const T &v, const Vector<T> &derivs) :
  val_p(v), nd_p(derivs.nelements()), nocopy_p(False),
  grad_p(nd_p>0 ? new T[nd_p] : 0) {
  for (uInt i=0; i<nd_p; i++) grad_p[i] = derivs(i);
}

template<class T>
AutoDiffRep<T>::~AutoDiffRep() {
  delete [] grad_p; grad_p = 0;
}

template <class T>
AutoDiffRep<T> &AutoDiffRep<T>::operator=(const T &v) {
  val_p = v;
  nd_p = 0;
  grad_p = 0;
  return *this;
}

template <class T>
AutoDiffRep<T> &AutoDiffRep<T>::operator=(const AutoDiffRep<T> &other) { 
  if (this != &other) {
    val_p = other.val_p;
    nd_p = other.nd_p;
    grad_p = (nd_p>0 ? new T[nd_p] : 0);
    for (uInt i=0; i<nd_p; i++) grad_p[i] = other.grad_p[i];
  };
  return *this;
}

template <class T> void AutoDiffRep<T>::derivatives(Vector<T> &res) const { 
  res.resize(nd_p);
  for (uInt i=0; i<nd_p; i++) res(i) = grad_p[i];
}

