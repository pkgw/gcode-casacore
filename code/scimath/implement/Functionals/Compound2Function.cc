//# NQCompound2Function.cc:  Compound of functions AutoDiff specialization
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

//# Includes
#include <aips/Functionals/NQCompoundFunction.h>

//# Constructors

//# Operators
template <class T>
AutoDiff<T> NQCompoundFunction<AutoDiff<T> >::
eval(typename Function<AutoDiff<T> >::FunctionArg x) const {
  if (parset_p) fromParam_p();
  AutoDiff<T> tmp(T(0), nparameters());
  tmp.value() = 0;
  for (uInt j=0; j<tmp.nDerivatives(); j++) tmp.deriv(j) = 0.0;
  // function value
  for (uInt i = 0; i< nFunctions(); ++i) {
    AutoDiff<T> t = function(i)(x);
    tmp.value() += t.value();
    for (uInt j=0; j<t.nDerivatives(); ++j) {
      tmp.deriv(paroff_p[i]+j) += t.deriv(j);
    };
  };
  return tmp;
}

//# Member functions
template <class T>
void NQCompoundFunction<AutoDiff<T> >::fromParam_p() const {
  if (parset_p) {
    for (uInt i=0; i<nparameters(); ++i) {
      uInt k = functionPtr_p[funpar_p[i]]->nparameters();
      uInt l = (*functionPtr_p[funpar_p[i]])[locpar_p[i]].nDerivatives();
      // Set correct number of derivatives in sub-functions
      if (nparameters() == param_p[i].nDerivatives()) {
	if (k != l) {
	  (*functionPtr_p[funpar_p[i]])[locpar_p[i]] =
	    AutoDiff<T>(T(0), k);
	  l = k;
	};
      } else if (param_p[i].nDerivatives() == 0 && l != 0) {
	(*functionPtr_p[funpar_p[i]])[locpar_p[i]] = AutoDiff<T>();
	l = 0;
      };
      // Set the parameter data
      for (uInt j=0; j<l; ++j) {
	(*functionPtr_p[funpar_p[i]])[locpar_p[i]].deriv(j) =
	  param_p[i].deriv(j+paroff_p[funpar_p[i]]);
      };
      (*functionPtr_p[funpar_p[i]])[locpar_p[i]].value() =
	param_p[i].value();
      functionPtr_p[funpar_p[i]]->mask(locpar_p[i]) = param_p.mask(i);
    };
    parset_p = False;
  };
}

template <class T>
void NQCompoundFunction<AutoDiff<T> >::toParam_p() {
  for (uInt i=0; i<nparameters(); ++i) {
    uInt k = functionPtr_p[funpar_p[i]]->nparameters();
    uInt l = (*functionPtr_p[funpar_p[i]])[locpar_p[i]].nDerivatives();
    // Set correct number of derivatives
    if (nparameters() ==
	(*functionPtr_p[funpar_p[i]])[locpar_p[i]].nDerivatives()) {
      if (k != l) {
	param_p[i] = AutoDiff<T>(T(0), k);
	///	(*functionPtr_p[funpar_p[i]])[locpar_p[i]] =
	l = k;
      };
    } else if ((*functionPtr_p[funpar_p[i]])[locpar_p[i]].nDerivatives()
	       == 0 && l != 0) {
      param_p[i] = AutoDiff<T>();
      ///      (*functionPtr_p[funpar_p[i]])[locpar_p[i]] = AutoDiff<T>();
      l = 0;
    };
    // Set the parameter data
    for (uInt j=0; j<l; ++j) {
      param_p[i].deriv(j+paroff_p[funpar_p[i]]) =
	(*functionPtr_p[funpar_p[i]])[locpar_p[i]].deriv(j);
    };
    param_p[i].value() = (*functionPtr_p[funpar_p[i]])[locpar_p[i]].value();
    param_p.mask(i) = functionPtr_p[funpar_p[i]]->mask(locpar_p[i]);
  };
}
