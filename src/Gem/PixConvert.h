/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemPixConvertSIMD.h
       - header-file for color conversion
       - this is mainly for (SIMD-)optimized routines
       - part of GEM

    Copyright (c) 2006-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


#ifndef _INCLUDE__GEM_GEM_PIXCONVERT_H_
#define _INCLUDE__GEM_GEM_PIXCONVERT_H_

/* for size_t */
#include <stddef.h>

// use formulae from http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
/*
 * [Y]    1    [  65.738 129.075  25.064 ]   [R] [ 16]
 * [U] = --- * [ -37.945 -74.494 112.439 ] * [G]+[128]
 * [V] = 256   [ 112.439 -94.154 -18.285 ]   [B] [128]
 *
 * [R]    1    [ 298.082    0.0    408.583 ]    [Y] [ 16]
 * [G] = --- * [ 298.082 -100.291 -208.120 ] * ([U]-[128])
 * [B] = 256   [ 298.082  516.411    0.0   ]    [V] [128]
 */

#define YUV_POYNTON
// here comes something to be afraid of:
// (probably it would be better to define the matrices as real constant-value matrices)
// (instead of element-wise)

#ifdef YUV_POYNTON
# define Y_OFFSET   16
# define UV_OFFSET 128

// RGB2YUV
// poynton-values rounded
# define RGB2YUV_11  66
# define RGB2YUV_12 129
# define RGB2YUV_13  25
# define RGB2YUV_21 -38
# define RGB2YUV_22 -74
# define RGB2YUV_23 112
# define RGB2YUV_31 112
# define RGB2YUV_32 -94
# define RGB2YUV_33 -18

// YUV2RGB
// (we skip _21 and _31 as they are equal to _11)
#if 0
// poynton-values rounded
# define YUV2RGB_11 298
# define YUV2RGB_12   0
# define YUV2RGB_13 409
# define YUV2RGB_22 -100
# define YUV2RGB_23 -208
# define YUV2RGB_32 516
# define YUV2RGB_33   0
#else

// this is round(256*inv(rgb2yuv/256))
// so the general error should be smaller
# define YUV2RGB_11  298
# define YUV2RGB_12   -1
# define YUV2RGB_13  409
# define YUV2RGB_22 -100
# define YUV2RGB_23 -210
# define YUV2RGB_32  519
# define YUV2RGB_33    0
#endif

#else
/* the old ones: */
# define Y_OFFSET   0
# define UV_OFFSET 128
// RGB2YUV
# define RGB2YUV_11 77
# define RGB2YUV_12 150
# define RGB2YUV_13 29
# define RGB2YUV_21 -43
# define RGB2YUV_22 -85
# define RGB2YUV_23 128
# define RGB2YUV_31 128
# define RGB2YUV_32 -107
# define RGB2YUV_33 -21
// YUV2RGB
# define YUV2RGB_11 256
# define YUV2RGB_12 0
# define YUV2RGB_13 359
# define YUV2RGB_22 -88
# define YUV2RGB_23 -183
# define YUV2RGB_32 454
# define YUV2RGB_33 0

#endif /* POYNTON */

#if 0
/* yuv-coefficients would also need an offset! */
# define RGB2GRAY_RED  RGB2YUV_11
# define RGB2GRAY_GREEN  RGB2YUV_12
# define RGB2GRAY_BLUE  RGB2YUV_13
# define RGB2GRAY_OFFSET Y_OFFSET
#else
# define RGB2GRAY_RED  77
# define RGB2GRAY_GREEN  150
# define RGB2GRAY_BLUE  29
# define RGB2GRAY_OFFSET 0
#endif


/* generic conversion functions */
#if 0
void FORMATtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void FORMATtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void FORMATtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void FORMATtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void FORMATtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void FORMATtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void FORMATtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
#endif

void YtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void Yu16toY(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height);
void Yu16toUYVY(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height);
void Yu16toRGB(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height);
void Yu16toBGR(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height);
void Yu16toRGBA(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height);
void Yu16toBGRA(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height);

void YUV420PtoY(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                unsigned char*outdata, size_t width, size_t height);
void YUV420PtoRGB(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                  unsigned char*outdata, size_t width, size_t height);
void YUV420PtoBGR(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                  unsigned char*outdata, size_t width, size_t height);
void YUV420PtoRGBA(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                   unsigned char*outdata, size_t width, size_t height);
void YUV420PtoBGRA(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                   unsigned char*outdata, size_t width, size_t height);
void YUV420PtoUYVY(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                   unsigned char*outdata, size_t width, size_t height);

void YUV420Ps16toY(const short*Y, const short*U, const short*V,
                unsigned char*outdata, size_t width, size_t height);
void YUV420Ps16toRGB(const short*Y, const short*U, const short*V,
                  unsigned char*outdata, size_t width, size_t height);
void YUV420Ps16toBGR(const short*Y, const short*U, const short*V,
                  unsigned char*outdata, size_t width, size_t height);
void YUV420Ps16toRGBA(const short*Y, const short*U, const short*V,
                   unsigned char*outdata, size_t width, size_t height);
void YUV420Ps16toBGRA(const short*Y, const short*U, const short*V,
                   unsigned char*outdata, size_t width, size_t height);
void YUV420Ps16toUYVY(const short*Y, const short*U, const short*V,
                   unsigned char*outdata, size_t width, size_t height);

void UYVYtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void YUYVtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YUYVtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YUYVtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YUYVtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YUYVtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YUYVtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void YVYUtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YVYUtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YVYUtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YVYUtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YVYUtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YVYUtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void RGBtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void BGRtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void RGBAtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void BGRAtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRAtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRAtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRAtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRAtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRAtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void ABGRtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ABGRtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ABGRtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ABGRtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ABGRtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ABGRtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void ARGBtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ARGBtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ARGBtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ARGBtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ARGBtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void ARGBtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);

void RGB16toY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGB16toUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGB16toRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGB16toBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGB16toRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGB16toBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGB16toABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);



void UYVYtoRGB_SSE2(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoBGR_SSE2(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoRGBA_SSE2(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoUYVY_SSE2(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);


void RGBtoUYVY_Altivec(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRtoUYVY_Altivec(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void RGBAtoUYVY_Altivec(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void BGRAtoUYVY_Altivec(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void UYVYtoBGRA_Altivec(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height);
void YUV420Ps16toUYVY_Altivec(const short*Y, const short*U, const short*V, unsigned char*outdata, size_t width, size_t height);

/* AltiVec */
#ifdef __VEC__
void YUV422_to_YV12_altivec(short*pY, short*pY2, short*pU, short*pV, const unsigned char *gem_image, int xsize, int ysize);
#endif /* AltiVec */

#endif /* _INCLUDE__GEM_GEM_PIXCONVERT_H_ */
