//                        Gaussian3DParam.cc

#include <trial/Functionals/Gaussian3DParam.h>
#include <aips/Arrays/ArrayMath.h>
#include <aips/Arrays/Vector.h>
#include <aips/Exceptions/Error.h>
#include <aips/Mathematics/Constants.h>
#include <aips/Mathematics/Math.h>

//  :fwhm2int(Type(1)/sqrt(log(Type(16)))),

template<class Type> 
Gaussian3DParam<Type>::Gaussian3DParam() 
  : Function<Type>(NPAR)
{
  param_p[H] = Type(1.0);
  param_p[CX] = Type(0.0);
  param_p[CY] = Type(0.0);
  param_p[CZ] = Type(0.0);
  param_p[AX] = Type(1.0);
  param_p[AY] = Type(1.0);
  param_p[AZ] = Type(1.0);
  param_p[THETA] = Type(0.0);
  param_p[PHI] = Type(0.0);
  settrigvals();
}


template<class Type> 
Gaussian3DParam<Type>::Gaussian3DParam(Type height, 
                             Type xCenter, Type yCenter, Type zCenter, 
                             Type xWidth, Type yWidth, Type zWidth, 
                             Type theta, Type phi) 
  : Function<Type>(NPAR)
{
  param_p[H] = height;
  param_p[CX] = xCenter;
  param_p[CY] = yCenter;
  param_p[CZ] = zCenter;
  param_p[AX] = xWidth;
  param_p[AY] = yWidth;
  param_p[AZ] = zWidth;
  param_p[THETA] = theta;
  param_p[PHI] = phi;
  settrigvals();  
}


template<class Type> 
Gaussian3DParam<Type>::Gaussian3DParam(Type height, 
                                           const Vector<Type>& center, 
                                           const Vector<Type>& width, 
                                           Type T, Type P)
  : Function<Type>(NPAR)
{
  setCenter(center);
  setWidth(width);
  settheta(T); setphi(P);
  settrigvals();
}

template<class Type> 
Gaussian3DParam<Type>::Gaussian3DParam(
                                          const Gaussian3DParam<Type>& other)
  : Function<Type>(other)
{  
   settrigvals();   //IMPR: could set vals explicitly to speed things up
}

template<class Type>
Gaussian3DParam<Type>::~Gaussian3DParam()
{}


template<class Type> 
Gaussian3DParam<Type>& Gaussian3DParam<Type>::operator=(
                                          const Gaussian3DParam<Type>& other)
{
  if (this != &other)
  {
    Function<Type>::operator=(other);
    settrigvals();  //IMPR: explicit
  }
  return *this;
}



template<class Type> 
Type Gaussian3DParam<Type>::height() const
{
  return param_p[H];
}

template<class Type> 
void Gaussian3DParam<Type>::setHeight(const Type& height) 
{
  param_p[H] = height;
}

template<class Type> 
Vector<Type> Gaussian3DParam<Type>::center() const 
{
  Vector<Type> center(3);
  center(0) = param_p[CX];
  center(1) = param_p[CY];
  center(2) = param_p[CZ];
  return center;
}

template<class Type> 
void Gaussian3DParam<Type>::setCenter(const Vector<Type>& center) 
{
  if (center.nelements() != 3) 
    throw(AipsError("Gaussian3D<Type>::setCenter(const Vector<Type>& center)"
		    " - center must be of length 3"));
  param_p[CX] = center(0);
  param_p[CY] = center(1);
  param_p[CZ] = center(2);
}

template<class Type> 
Type Gaussian3DParam<Type>::xCenter() const 
{
  return param_p[CX];
}

template<class Type> 
Type Gaussian3DParam<Type>::yCenter() const 
{
  return param_p[CY];
}

template <class Type>
Type Gaussian3DParam<Type>::zCenter() const
{
  return param_p[CZ];
}

template<class Type> 
void Gaussian3DParam<Type>::setXcenter(const Type& xcenter) 
{
  param_p[CX] = xcenter;
}

template<class Type> 
void Gaussian3DParam<Type>::setYcenter(const Type& ycenter) 
{
  param_p[CY] = ycenter;
}

template<class Type>
void Gaussian3DParam<Type>::setZcenter(const Type& zcenter)
{
  param_p[CZ] = zcenter;
}

template<class Type> 
Vector<Type> Gaussian3DParam<Type>::width() const 
{
  Vector<Type> width(3);
  width(0) = param_p[AX];
  width(1) = param_p[AY];
  width(2) = param_p[AZ];
  return width;
}

template<class Type> 
void Gaussian3DParam<Type>::setWidth(const Vector<Type>& width) 
{
  if (width.nelements() != 3)
    throw(AipsError("Gaussian3DParam<Type>::setWidth"
                    "(const Vector<Type>& width)"
                    " - width must be of length 3"));
  param_p[AX] = width(0);
  param_p[AY] = width(1);
  param_p[AZ] = width(2);
}

template<class Type>
void Gaussian3DParam<Type>::setXwidth(const Type & xwidth)
{
  if (xwidth <= Type(0)) 
    throw(AipsError("Gaussian3DParam<Type>::setXwidth(const Type& xwidth)"
                    " - width must be positive"));
  param_p[AX] = xwidth;
}

template<class Type>
void Gaussian3DParam<Type>::setYwidth(const Type & ywidth)
{
  if (ywidth <= Type(0)) 
    throw(AipsError("Gaussian3DParam<Type>::setYwidth(const Type& ywidth)"
                    " - width must be positive"));
  param_p[AY] = ywidth; 
}

template<class Type>
void Gaussian3DParam<Type>::setZwidth(const Type & zwidth)
{
  if (zwidth <= Type(0)) 
    throw(AipsError("Gaussian3DParam<Type>::setZwidth(const Type& zwidth)"
                    " - width must be positive"));
  param_p[AZ] = zwidth;
}

template<class Type>
Type Gaussian3DParam<Type>::xWidth() const
{
  return param_p[AX];
}

template<class Type>
Type Gaussian3DParam<Type>::yWidth() const
{
  return param_p[AY];
}

template<class Type>
Type Gaussian3DParam<Type>::zWidth() const
{
  return param_p[AZ];
}

template<class Type> 
Type Gaussian3DParam<Type>::theta() const 
{
  //IMPR: add multiple of pi to force to be in stated range
  return param_p[THETA];
}

template <class Type>
Type Gaussian3DParam<Type>::phi() const
{
  //IMPR: add multiple of pi to force to be in stated range
  return param_p[PHI];
}

template<class Type> 
void Gaussian3DParam<Type>::settheta(const Type& theta) 
{
  //IMPR: should be pi/4.
  if (abs(theta) > Type(C::_2pi))   
    throw(AipsError("Gaussian3DParam<Type>::settheta(const Type& theta)"
		    " - theta must be in radians and between -2pi and 2pi"));
  
  param_p[THETA] = theta;
  settrigvals();
}

template<class Type> 
void Gaussian3DParam<Type>::setphi(const Type& phi) 
{
  //IMPR: should be pi/4.
  if (abs(phi) > Type(C::pi)) 
    throw(AipsError("Gaussian3D<Type>::setphi(const Type& phi)"
		    " - phi must be in radians and between -pi and pi"));
  
  param_p[PHI] = phi;
  settrigvals();
}




template <class Type>
void Gaussian3DParam<Type>::settrigvals() const
{
  stoT = param_p[THETA];
  stoP = param_p[PHI];
  sinT = sin(param_p[THETA]);
  cosT = cos(param_p[THETA]);
  sinP = sin(param_p[PHI]);
  cosP = cos(param_p[PHI]);
  cosTcosP = cosT * cosP;
  cosTsinP = cosT * sinP;
  sinTcosP = sinT * cosP;
  sinTsinP = sinT * sinP;
}

