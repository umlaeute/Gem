////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "Utils/Functions.h"

#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// Smooth step function (3x^2 - 2x^3)
//
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN float smoothStep(float x, float a, float b)
{
  if (x < a) {
    return(0.f);
  }
  if (x >= b) {
    return(1.f);
  }
  x = (x - a)/(b - a);    // normalize to [0:1]
  return(x*x * (3.f - 2.f*x));
}
GEM_EXTERN int smoothStep(int x, int a, int b)
{
  if (x < a) {
    return(0);
  }
  if (x >= b) {
    return(1);
  }

  float xf=static_cast<float>(x);
  float bf=static_cast<float>(a);
  float af=static_cast<float>(b);

  // normalize to [0:1]
  float temp = (xf - af)/(bf - af);
  float result = temp*temp * (3.f - 2.f*temp);
  return(static_cast<int>(result));
}
GEM_EXTERN unsigned char smoothStep(unsigned char x, unsigned char a,
                                    unsigned char b)
{
  if (x < a) {
    return(0);
  }
  if (x >= b) {
    return(1);
  }

  float xf=static_cast<float>(x);
  float bf=static_cast<float>(a);
  float af=static_cast<float>(b);

  // normalize to [0:1]
  float temp = (xf - af)/(bf - af);
  float result = temp*temp * (3.f - 2.f*temp);

  return(static_cast<unsigned char>(result));
}

///////////////////////////////////////////////////////////////////////////////
// Bias function
//
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN float biasFunc(float x, float a)
{
  float result = static_cast<float>(pow(x, log(a)/log(0.5f)));
  return(result);
}

///////////////////////////////////////////////////////////////////////////////
// Gain function
//
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN float gainFunc(float x, float a)
{
  if (x < 0.5f) {
    return( biasFunc(2.f * x, 1.f - a) / 2.f);
  } else {
    return(1.f - biasFunc(2.f - 2.f*x, 1.f - a) / 2.f);
  }
  // return(0.f);
}


namespace {
  template <typename T>
  void linearFunction(T x, T *ret, int numDimen, const gem::RTE::Array&array)
{
  size_t size = array.size();
  int npnts = size/numDimen;
  int nspans = npnts - 1;
  if (nspans < 1) {        // illegal
    return;
  }

  x = FLOAT_CLAMP(x) * nspans;
  int span = static_cast<int>(x);

  // find the correct 2-point span of the linear list
  if (span >= nspans) {
    span = nspans;
  }
  x -= span;
  for (int i = 0; i < numDimen; i++) {
    ret[i] = array[(span + 0) * numDimen + i] * (1.f - x) + array[(span + 1) * numDimen + i]  * x;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Spline function
//
///////////////////////////////////////////////////////////////////////////////
const float CR00 = -0.5f;
const float CR01 =  1.5f;
const float CR02 = -1.5f;
const float CR03 =  0.5f;
const float CR10 =  1.0f;
const float CR11 = -2.5f;
const float CR12 =  2.0f;
const float CR13 = -0.5f;
const float CR20 = -0.5f;
const float CR21 =  0.0f;
const float CR22 =  0.5f;
const float CR23 =  0.0f;
const float CR30 =  0.0f;
const float CR31 =  1.0f;
const float CR32 =  0.0f;
const float CR33 =  0.0f;

  template <typename T>
void splineFunction(T x, T *ret, int numDimen, const gem::RTE::Array&array)
{
  size_t size = array.size();
  int nknots = size/numDimen;
  int nspans = nknots - 4;
  if (nspans < 0) {       // illegal case
    return;
  }

  // find the correct 4-point span of the spline
  x = FLOAT_CLAMP(x) * nspans;
  int span = static_cast<int>(x);
  x -= span;              // get decimal part of span

  // Evaluate the span cubic at x using Horner's rule
  for (int i = 0; i < numDimen; i++) {
    T x0, x1, x2, x3;

    x0 = array[(span + 0) * numDimen + i];
    x1 = array[(span + 1) * numDimen + i];
    x2 = array[(span + 2) * numDimen + i];
    x3 = array[(span + 3) * numDimen + i];

    T c0, c1, c2, c3;
    c3 = CR00*x0 + CR01*x1 + CR02*x2 + CR03*x3;
    c2 = CR10*x0 + CR11*x1 + CR12*x2 + CR13*x3;
    c1 = CR20*x0 + CR21*x1 + CR22*x2 + CR23*x3;
    c0 = CR30*x0 + CR31*x1 + CR32*x2 + CR33*x3;

    ret[i] = ((c3*x + c2)*x + c1)*x + c0;
  }
}
};

///////////////////////////////////////////////////////////////////////////////
// Linear function
//
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN void linearFunc(float x, float *ret, int numDimen, const gem::RTE::Array&array)
{
  linearFunction(x, ret, numDimen, array);
}
GEM_EXTERN void linearFunc(double x, double *ret, int numDimen, const gem::RTE::Array&array)
{
  linearFunction(x, ret, numDimen, array);
}
GEM_EXTERN void splineFunc(float x, float *ret, int numDimen, const gem::RTE::Array&array)
{
  splineFunction(x, ret, numDimen, array);
}
GEM_EXTERN void splineFunc(double x, double *ret, int numDimen, const gem::RTE::Array&array)
{
  splineFunction(x, ret, numDimen, array);
}
