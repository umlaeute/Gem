/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemFuncUtil.h
       - contains functions for graphics
       - part of GEM

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMFUNCUTIL_H_
#define INCLUDE_GEMFUNCUTIL_H_

#include "Base/GemExportDef.h"

///////////////////////////////////////////////////////////////////////////////
// Clamp functions
//
///////////////////////////////////////////////////////////////////////////////
//////////
// Clamp a value high
inline unsigned char CLAMP_HIGH(int x)
	{ return((unsigned char )((x > 255) ? 255 : x)); }

//////////
// Clamp a value low
inline unsigned char CLAMP_LOW(int x)
	{ return((unsigned char )((x < 0) ? 0 : x)); }

//////////
// Clamp an int to the range of an unsigned char
inline unsigned char CLAMP(int x)
    { return((unsigned char)((x > 255) ? 255 : ( (x < 0) ? 0 : x))); }

//////////
// Clamp a float to the range of an unsigned char
inline unsigned char CLAMP(float x)
    { return((unsigned char)((x > 255.f) ? 255.f : ( (x < 0.f) ? 0.f : x))); }

//////////
// Clamp a float to 0. <= x <= 1.0
inline float FLOAT_CLAMP(float x)
    { return((x > 1.f) ? 1.f : ( (x < 0.f) ? 0.f : x)); }

///////////////////////////////////////////////////////////////////////////////
// Multiply and interpolation
//
///////////////////////////////////////////////////////////////////////////////
//////////
// Exactly multiply two unsigned chars
// This avoids a float value (important on Intel...)
// From Alvy Ray Smith paper
inline unsigned char INT_MULT(unsigned int a, unsigned int b)
	{ int t = (unsigned int)a * (unsigned int)b + 0x80;
      return((unsigned char)(((t >> 8) + t) >> 8)); }

//////////
// Exactly LERPs two values
// This avoids a float value (important on Intel...)
// From Alvy Ray Smith paper
inline unsigned char INT_LERP(unsigned int p, unsigned int q, unsigned int a)
	{ return((unsigned char)(p + INT_MULT(a, q - p))); }

//////////
// Floating point LERP
inline float FLOAT_LERP(float p, float q, float a)
	{ return( a * (q - p) + p); }


///////////////////////////////////////////////////////////////////////////////
// Step function
//
///////////////////////////////////////////////////////////////////////////////
inline int stepFunc(float x, float a)
    { return(x >= a); }
inline int stepFunc(int x, int a)
    { return(x >= a); }
inline int stepFunc(unsigned char x, unsigned char a)
    { return(x >= a); }

///////////////////////////////////////////////////////////////////////////////
// Pulse function
//
///////////////////////////////////////////////////////////////////////////////
inline int pulseFunc(float x, float a, float b)
    { return(stepFunc(a, x) - stepFunc(b, x)); }
inline int pulseFunc(int x, int a, int b)
    { return(stepFunc(a, x) - stepFunc(b, x)); }
inline int pulseFunc(unsigned char x, unsigned char a, unsigned char b)
    { return(stepFunc(a, x) - stepFunc(b, x)); }


///////////////////////////////////////////////////////////////////////////////
// Clamp function
//
///////////////////////////////////////////////////////////////////////////////
inline float clampFunc(float x, float a, float b)
    { return(x < a ? a : (x > b ? b : x)); }
inline int clampFunc(int x, int a, int b)
    { return(x < a ? a : (x > b ? b : x)); }
inline unsigned char clampFunc(unsigned char x, unsigned char a, unsigned char b)
    { return(x < a ? a : (x > b ? b : x)); }


///////////////////////////////////////////////////////////////////////////////
// Smooth step function (3x^2 - 2x^3)
//
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN extern float         smoothStep(float x, float a, float b);
GEM_EXTERN extern int           smoothStep(int x, int a, int b);
GEM_EXTERN extern unsigned char smoothStep(unsigned char x, unsigned char a, unsigned char b);

///////////////////////////////////////////////////////////////////////////////
// Bias function
//
// Remap unit interval (curve)
// If a == 0.5, then is linear mapping.
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN extern float biasFunc(float x, float a);

///////////////////////////////////////////////////////////////////////////////
// Gain function
//
// Remap unit interval (S-curve)
// Will always return 0.5 when x is 0.5
// If a == 0.5, then is linear mapping.
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN extern float gainFunc(float x, float a);

///////////////////////////////////////////////////////////////////////////////
// Linear function
//
// val should be 0 <= val <= 1.
// ret should point at a float of enough dimensions to hold the returned value
//      For instance, numDimen == 2, should have a ret[2]
// numDimen is the number of dimensions to compute
// npnts is the number of points per dimension.
//
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN extern void linearFunc(float val, float *ret, int numDimen, int npnts, float *pnts);

///////////////////////////////////////////////////////////////////////////////
// Spline function
//
// val should be 0 <= val <= 1.
// ret should point at a float of enough dimensions to hold the returned value
//      For instance, numDimen == 2, should have a ret[2]
// numDimen is the number of dimensions to compute
// nknots is the number of knots per dimension.
//      There must be at least four knots!
//
// Thanks to
//      _Texturing and Modeling: A Procedural Approach_
//          David S. Ebert, Ed.
///////////////////////////////////////////////////////////////////////////////
GEM_EXTERN extern void splineFunc(float val, float *ret, int numDimen, int nknots, float *knot);

#endif  // for header file

