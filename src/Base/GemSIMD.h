
/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    include file for SIMD

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMSIMD_H_
#define INCLUDE_GEMSIMD_H_

const int GEM_SIMD_NONE=0;
const int GEM_SIMD_MMX=1;
const int GEM_SIMD_SSE2=2;
const int GEM_SIMD_ALTIVEC=3;


/* include for SIMD on PC's */
#ifdef __SSE2__
#include <emmintrin.h>
// for icc this should be <dvec.h>
typedef union{
  unsigned char c[16];
  __m128i v;
} vector_128;
#elif defined __VEC__
/* for AltiVec (PowerPC) */
typedef union{
  unsigned char c[16];
  vector unsigned char v;
} vector_128;
#endif

#if defined __MMX__
# include <mmintrin.h>
// for icc this should be <ivec.h>
typedef union{
  __m64 v; unsigned char c[8];
} vector64i;

#endif


#ifdef __SSE__
#include <xmmintrin.h>

typedef union{
  __m128 m; float f[4];
} vector128f;
#endif



#endif /* INCLUDE_GEMSIMD_H_ */
