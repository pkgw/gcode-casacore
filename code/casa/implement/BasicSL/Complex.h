//# Complex.h: Single and double precision complex numbers
//# Copyright (C) 2000,2001,2002,2004
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


#if !defined(AIPS_COMPLEX_H)
#define AIPS_COMPLEX_H


//# Includes
#include <aips/aips.h>
#include <aips/Mathematics/Complexfwd.h>
#include <aips/complex.h>

// <summary>
// Single and double precision complex numbers
// </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>

// <synopsis>
// The class <src>Complex</src> is a straight typedef as the 
// standard library <src>complex<float></src>.
//
// In a similar way <src>DComplex</src> is typedef-ed as
// <src>complex<double></src>.
//
// <linkto class=IComplex>IComplex</linkto> is defined as a specific class.
// It is only used by the <src>FITS</src> classes.
//
// <src>lDComplex</src> has not been defined: <src>long double</src> is not
// part of the standard aips++ data suite (yet)
//
// A set of global functions are added for historic reasons (they were present
// in the original aips++/gcc complex implementation).
//
// See the standard library documentation for the expected behaviour of 
// the <src>Complex</src> and <src>DComplex</src> classes.
//
// <note role=tip> In the following all references to <src>Complex</src>
// can be replaced with <src>DComplex</src>. with simultaneous
// replacement of <src>Float</src> with <src>Double</src>. </note>
//
// Complex numbers may be constructed and used in the following ways:
// <dl>
// <dt>Complex x;</dt>
// <dd>  Declares an uninitialized Complex. </dd>
// 
// <dt>Complex x = 2; Complex y(2.0);</dt>
// <dd>  Set x and y to the Complex value (2.0, 0.0); </dd>
// 
// <dt>Complex x(2, 3);</dt>
// <dd>  Sets x to the Complex value (2, 3); </dd>
// 
// <dt>Complex u(x); Complex v = x;</dt>
// <dd>  Set u and v to the same value as x. </dd>
// 
// <dt>Float real(Complex& x);</dt>
// <dd>  returns the real part of x. </dd>
// 
// <dt>Float imag(Complex& x);</dt>
// <dd>  returns the imaginary part of x. </dd>
// 
// <dt>Float abs(Complex& x);</dt>
// <dd>  returns the magnitude of x. </dd>
// 
// <dt>Float norm(Complex& x);</dt>
// <dd>  returns the square of the magnitude of x. </dd>
// 
// <dt>Float arg(Complex& x);</dt>
// <dd>  returns the argument (amplitude) of x. </dd>
// 
// <dt>Complex polar(Float r, Float t = 0.0);</dt>
// <dd>  returns a Complex with abs of r and arg of t. </dd>
// 
// <dt>Complex conj(Complex& x);</dt>
// <dd>  returns the complex conjugate of x </dd>
// 
// <dt>Complex cos(Complex& x);</dt>
// <dd>  returns the complex cosine of x. </dd>
// 
// <dt>Complex sin(Complex& x);</dt>
// <dd>  returns the complex sine of x. </dd>
// 
// <dt>Complex cosh(Complex& x);</dt>
// <dd>  returns the complex hyperbolic cosine of x. </dd>
// 
// <dt>Complex sinh(Complex& x);</dt>
// <dd>  returns the complex hyperbolic sine of x. </dd>
// 
// <dt>Complex exp(Complex& x);</dt>
// <dd>  returns the exponential of x. </dd>
// 
// <dt>Complex log(Complex& x);</dt>
// <dd>  returns the natural log of x. </dd>
// 
// <dt>Complex pow(Complex& x, long p);</dt>
// <dd>  returns x raised to the p power. </dd>
// 
// <dt>Complex pow(Complex& x, Complex& p);</dt>
// <dd>  returns x raised to the p power. </dd>
// 
// <dt>Complex sqrt(Complex& x);</dt>
// <dd>  returns the square root of x. </dd>
// 
// <dt> Complex min(Complex x,Complex y);
// <dd> Returns the minumum of x,y (using operator<=, i.e. the norm).
//
// <dt> Complex max(Complex x,Complex y);
// <dd> Returns the maximum of x,y (using operator>=, i.e. the norm).
//
// <dt>Bool near(Complex val1, Complex val2, Double tol = 1.0e-5);</dt>
// <dd>  returns whether val1 is relatively near val2 (see Math.h).
//	(Note the Double tolerance) </dd>
//
// <dt>Bool nearAbs(Complex val1, Complex val2, Double tol = 1.0e-5);</dt>
// <dd>  returns whether val1 is absolutely near val2 (see Math.h).
//	(Note the Double tolerance) </dd>
//
// <dt>ostream << x;</dt>
// <dd>  prints x in the form (re, im). </dd>
// 
// <dt>istream >> x;</dt>
//  <dd> reads x in the form (re, im), or just (re) or re in which case the
//      imaginary part is set to zero. </dd>
// </dl> 
// </synopsis>

//# <todo asof="2000/11/27">
//# </todo>

// <group name="Complex NaN">
Bool isNaN (const Complex& val);
void setNaN(Complex& val);
// </group>

// <summary>Complex comparisons </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name="Complex comparisons">
//# On Linux comparing the norm does not work well in debug mode
//# for equal values. Therefore they are compared for equality first.
inline Bool operator>= (const Complex& left, const Complex& right)
  { return left==right  ?  True : norm(left) >= norm(right); }
inline Bool operator>  (const Complex& left, const Complex& right)
  { return left==right  ?  False : norm(left) > norm(right); }
inline Bool operator<= (const Complex& left, const Complex& right)
  { return left==right  ?  True : norm(left) <= norm(right); }
inline Bool operator<  (const Complex& left, const Complex& right)
  { return left==right  ?  False : norm(left) < norm(right); }
// </group>
// </group>


// <summary>
// Double precision complex numbers
// </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>

// <synopsis>
// The class <src>DComplex</src> is a straight typedef as the 
// standard library <src>complex<double></src>.
// It is defined in a similar way as <src>Complex</src>.
// </synopsis>

// <group name="DComplex NaN">
Bool isNaN (const DComplex& val);
void setNaN(DComplex& val);
// </group>

// <summary> DComplex comparisons </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name="DComplex comparisons">
inline Bool operator>= (const DComplex& left, const DComplex& right)
  { return norm(left) >= norm(right); }
inline Bool operator>  (const DComplex& left, const DComplex& right)
  { return norm(left) >  norm(right); }
inline Bool operator<= (const DComplex& left, const DComplex& right)
  { return norm(left) <= norm(right); }
inline Bool operator<  (const DComplex& left, const DComplex& right)
  { return norm(left) <  norm(right); }
// </group>
// </group>


//# Global functions
// <summary> Additional complex mathematical functions </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name=math>
inline Double fabs(const DComplex &val) { return abs(val); };
inline Float fabs(const Complex &val) { return abs(val); };

// The log10 should be in stl
// <group>
#if defined(NEEDS_LOG10_COMPLEX)
Complex log10(const Complex &val);
DComplex log10(const DComplex &val);
#endif
// </group>

// ArrayMath::pow needs this pow function (on SGI).
inline Complex pow(const Complex& val, Double p) { return pow(val,Float(p)); }

// QMath needs these operators * and / (on SGI).
// <group>
inline Complex operator*(const Complex& val, Double f) { return val*Float(f); }
inline Complex operator/(const Complex& val, Double f) { return val/Float(f); }
// </group>
// These operators are useful, otherwise both Float and Double are applicable
// for Ints.
// <group>
inline Complex operator*(const Complex& val, Int f) { return val*Float(f); }
inline Complex operator/(const Complex& val, Int f) { return val/Float(f); }
// </group>
// </group>

// <summary> The near functions </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name=near>
Bool near(const Complex &val1, const Complex &val2, Double tol=1.0e-5);
Bool near(const DComplex &val1, const DComplex &val2, Double tol=1.0e-13);
Bool nearAbs(const Complex &val1, const Complex &val2, Double tol=1.0e-5);
Bool nearAbs(const DComplex &val1, const DComplex &val2, Double tol=1.0e-13);
inline Bool allNear(const Complex &val1, const Complex &val2,
		    Double tol=1.0e-5)
  { return near(val1, val2, tol); }
inline Bool allNear(const DComplex &val1, const DComplex &val2, 
		    Double tol=1.0e-13)
  { return near(val1, val2, tol); }
inline Bool allNearAbs(const Complex &val1, const Complex &val2, 
		       Double tol=1.0e-5)
  { return nearAbs(val1, val2, tol); }
inline Bool allNearAbs(const DComplex &val1, const DComplex &val2, 
		       Double tol=1.0e-13)
  { return nearAbs(val1, val2, tol); }
// </group>

// <summary> Max and min, floor and ceil functions </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name=maxmin>
inline Complex max(const Complex &x, const Complex &y)
  { return x >= y ? x : y; }
inline DComplex max(const DComplex &x, const DComplex &y)
  { return x >= y ? x : y; }

inline Complex min(const Complex &x, const Complex &y)
  { return x <= y ? x : y; }
inline DComplex min(const DComplex &x, const DComplex &y)
  { return x <= y ? x : y; }

inline Complex floor(const Complex &x) {
  return Complex(floor(x.real()), floor(x.imag())); }
inline DComplex floor(const DComplex &x) {
  return DComplex(floor(x.real()), floor(x.imag())); }

inline Complex ceil(const Complex &x) {
  return Complex(ceil(x.real()), ceil(x.imag())); }
inline DComplex ceil(const DComplex &x) {
  return DComplex(ceil(x.real()), ceil(x.imag())); }
// </group>

// <summary> fmod </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name=fmod>
DComplex fmod(const DComplex &in, const DComplex &f);
Complex fmod(const Complex &in, const Complex &f);
// </group>

// <summary> Inverse trigonometry </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name=inverse>
// atan not valid for z == -1
DComplex atan(const DComplex &in);
Complex atan(const Complex &in);
DComplex asin(const DComplex &in);
Complex asin(const Complex &in);
DComplex acos(const DComplex &in);
Complex acos(const Complex &in);
DComplex atan2(const DComplex &in, const DComplex &t2);
Complex atan2(const Complex &in, const Complex &t2);
// </group>

// <summary> Error function </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
// <group name=erf>
// Preliminary to get Functionals working. erf(z) will return erf(real(z))
// only for now.
DComplex erf(const DComplex &in);
Complex erf(const Complex &in);
DComplex erfc(const DComplex &in);
Complex erfc(const Complex &in);
// </group>

#endif
