#ifdef __SSE2__

/////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file for SSE2-optimized color-conversion routines
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "Base/GemPixConvert.h"
#include "Base/GemPixUtil.h"

/* for post() */
#include "m_pd.h"

/* just some debugging stuff ... */

#define PRINT_MASK "%03d "
static void print_char(__m128i m){
  _mm_pause();
  unsigned char*s=(unsigned char*)&m;
  int i=0;
  for(i=0; i<(sizeof(__m128i)/sizeof(unsigned char)); i++){
    startpost(PRINT_MASK, *s);s++;
  }
  endpost();
}
static void print_short(__m128i m){
  _mm_pause();
  signed short*s=(signed short*)&m;
  int i=0;
  for(i=0; i<(sizeof(__m128i)/sizeof(signed short)); i++){
    startpost(PRINT_MASK, *s);s++;
  }
  endpost();
}
static void print_int(__m128i m){
  _mm_pause();
  signed int*s=(signed int*)&m;
  int i=0;
  for(i=0; i<(sizeof(__m128i)/sizeof(signed int)); i++){
    startpost(PRINT_MASK, *s);s++;
  }
  endpost();
}


/* convert RGBA to YUV422 */
void RGBA_to_UYVY_SSE2(unsigned char *rgbadata, 
                       size_t size, 
                       unsigned char *yuvdata)
{
  __m128i *rgba_p = (__m128i*)rgbadata; /*  4 RGBA pixels */
  __m128i *yuv_p  = (__m128i*)yuvdata;  /* 4*2 YUV pixels */
  
  const __m128i zero = _mm_setzero_si128();

#define RGB2YUV_14 0
#define RGB2YUV_24 0
#define RGB2YUV_34 0

  const __m128i RG2Y=_mm_set_epi16(RGB2YUV_12, RGB2YUV_11, RGB2YUV_12, RGB2YUV_11, 
                                   RGB2YUV_12, RGB2YUV_11, RGB2YUV_12, RGB2YUV_11);
  const __m128i BA2Y=_mm_set_epi16(RGB2YUV_14, RGB2YUV_13, RGB2YUV_14, RGB2YUV_13, 
                                   RGB2YUV_14, RGB2YUV_13, RGB2YUV_14, RGB2YUV_13);

  const __m128i RG2U=_mm_set_epi16(RGB2YUV_22, RGB2YUV_21, RGB2YUV_22, RGB2YUV_21, 
                                   RGB2YUV_22, RGB2YUV_21, RGB2YUV_22, RGB2YUV_21);
  const __m128i BA2U=_mm_set_epi16(RGB2YUV_24, RGB2YUV_23, RGB2YUV_24, RGB2YUV_23, 
                                   RGB2YUV_24, RGB2YUV_23, RGB2YUV_24, RGB2YUV_23);

  const __m128i RG2V=_mm_set_epi16(RGB2YUV_32, RGB2YUV_31, RGB2YUV_32, RGB2YUV_31, 
                                   RGB2YUV_32, RGB2YUV_31, RGB2YUV_32, RGB2YUV_31);
  const __m128i BA2V=_mm_set_epi16(RGB2YUV_34, RGB2YUV_33, RGB2YUV_34, RGB2YUV_33, 
                                   RGB2YUV_34, RGB2YUV_33, RGB2YUV_34, RGB2YUV_33);

  const __m128i OFFSET=_mm_set_epi16(Y_OFFSET, UV_OFFSET,
                                     Y_OFFSET, UV_OFFSET,
                                     Y_OFFSET, UV_OFFSET,
                                     Y_OFFSET, UV_OFFSET);

  /* nomenclatura:
   *   lower-case letters denote  8bit values (like "r" is red, 8bit)
   *   upper-case letters denote 16bit (or 32bit) values (like "G" is green, 16bit)
   *
   */
  __m128i rgba0, rgba1, RGBA0, RGBA1;
  __m128i RGRG, BABA, RGRG0, BABA0, RGRG1, BABA1;
  __m128i RGRG_BABA0, RGRG_BABA1;
  __m128i Y0, Y1, U, V, UY, VY, UV, YY;

  const int shuffle =  _MM_SHUFFLE(3, 1, 2, 0);

  static int toggle=0;
  toggle=!toggle;

  int i=size/8; /* we do 2*128bit per cycle: this is 2*4*32bit == 8 pixels */
  while(i-->0){
    rgba0=*rgba_p++; /* r0 g0 b0 a0 r1 g1 ... b3 a3 */
    rgba1=*rgba_p++; /* r4 g4 b4 a4 r5 g5 ... b7 a7 */

    /* 1st 4 pixels */
    RGBA0 = _mm_unpacklo_epi8(rgba0, zero); /* R0 G0 B0 A0 ... B1 A1 */
    RGBA1 = _mm_unpackhi_epi8(rgba0, zero); /* R2 G2 B2 A2 ... B3 A3 */

    RGRG_BABA0  = _mm_unpacklo_epi32(RGBA0, RGBA1); /* R0 G0 R2 G2 B0 A0 B2 A2 */
    RGRG_BABA1  = _mm_unpackhi_epi32(RGBA0, RGBA1); /* R1 G1 R3 G3 B1 A1 B3 A3 */

    RGRG0 = _mm_unpacklo_epi64(RGRG_BABA0, RGRG_BABA1); /* R0 G0 R2 G2 R1 G1 R3 G3 */
    BABA0 = _mm_unpackhi_epi64(RGRG_BABA0, RGRG_BABA1); /* B0 A0 B2 A2 B1 A1 B3 A3 */

    // get Y for the 1st 4 pixels (thats 32bit)
    // Y_RG32 = _mm_madd_epi16(RGRG0, RG2Y); /* R0*a+G0*b R2*a+G2*b R1*a+G1*b R3*a+G3*b */
    // Y_BA32 = _mm_madd_epi16(BABA0, BA2Y); /* B0*c+A0*d B2*c+A2*d B1*c+A1*d B3*c+A3*d */
    Y0    = _mm_add_epi32(_mm_madd_epi16(RGRG0, RG2Y), _mm_madd_epi16(BABA0, BA2Y));
    Y0    = _mm_shuffle_epi32(Y0, shuffle);
    //startpost("Y0: "); print_int(Y0); /* Y0, Y1, Y2, Y3 */


    /* 2nd 4 pixels */
    RGBA0 = _mm_unpacklo_epi8(rgba1, zero);     /* R4 G4 B4 A4 R5 G5 B5 A5 */
    RGBA1 = _mm_unpackhi_epi8(rgba1, zero);     /* R6 G6 B6 A6 R7 G7 B7 A7 */

    RGRG_BABA0  = _mm_unpacklo_epi32(RGBA0, RGBA1); /* R4 G4 R6 G6 B4 A4 B6 A6 */
    RGRG_BABA1  = _mm_unpackhi_epi32(RGBA0, RGBA1); /* R5 G5 R7 G7 B5 A5 B7 A7 */

    RGRG1  = _mm_unpacklo_epi64(RGRG_BABA0, RGRG_BABA1); /* R4 G4 R6 G6 R5 G5 R7 G7 */
    BABA1  = _mm_unpackhi_epi64(RGRG_BABA0, RGRG_BABA1); /* B4 A4 B6 A6 B5 A5 B7 A7 */

    // get Y for the 2nd 4 pixels (thats 32bit)
    // Y_RG32 = _mm_madd_epi16(RGRG1, RG2Y); /* R4*a+G4*b R6*a+G6*b R5*a+G5*b R7*a+G7*b */
    // Y_BA32 = _mm_madd_epi16(BABA1, BA2Y); /* B4*c+A4*d B6*c+A6*d B5*c+A5*d B7*c+A7*d */
    Y1       = _mm_add_epi32(_mm_madd_epi16(RGRG1, RG2Y), _mm_madd_epi16(BABA1, BA2Y));
    Y1       = _mm_shuffle_epi32(Y1, shuffle);
    //startpost("Y1: "); print_int(Y1);

    // now get UV
    /* (R01 G01 R23 G23 R45 G45 R67 G67) / 2 */
    RGRG  = _mm_avg_epu16(_mm_unpackhi_epi64(RGRG0, RGRG1), _mm_unpacklo_epi64(RGRG0, RGRG1));
    /* (B01 A01 B23 A23 B45 A45 B67 A67) / 2 */
    BABA  = _mm_avg_epu16(_mm_unpackhi_epi64(BABA0, BABA1), _mm_unpacklo_epi64(BABA0, BABA1));
    
    // get 4 U for 8 pixels (32bit each)
    //U_RG32 = _mm_madd_epi16(RGRG, RG2U); /* R4*a+G4*b R6*a+G6*b R5*a+G5*b R7*a+G7*b */
    //U_BA32 = _mm_madd_epi16(BABA, AB2U); /* B4*c+A4*d B6*c+A6*d B5*c+A5*d B7*c+A7*d */
    U      = _mm_add_epi32 (_mm_madd_epi16(RGRG, RG2U), _mm_madd_epi16(BABA, BA2U));

    // get 4 V for 8 pixels (32bit each)
    //V_RG32 = _mm_madd_epi16(RGRG, RG2V); /* R4*a+G4*b R6*a+G6*b R5*a+G5*b R7*a+G7*b */
    //V_BA32 = _mm_madd_epi16(BABA, AB2V); /* B4*c+A4*d B6*c+A6*d B5*c+A5*d B7*c+A7*d */
    V      = _mm_add_epi32 (_mm_madd_epi16(RGRG, RG2V), _mm_madd_epi16(BABA, BA2V));

    // so now we have (all values in 32bit)
    /*
     *  U  U  U  U
     * Y1 Y1 Y1 Y1
     *  V  V  V  V
     * Y2 Y2 Y2 Y2
     */

    // we still need to right-shift everything by 8
    // and press it into 8bit values, so we have one vector with UYVYUYVYUYVYUYVY
    // (or just take the 3rd 8bit-tuple)

    Y0 = _mm_srai_epi32(Y0, 8);
    U  = _mm_srai_epi32(U , 8);
    Y1 = _mm_srai_epi32(Y1, 8);
    V  = _mm_srai_epi32(V , 8);

    UV = _mm_packs_epi32(U, V);
    YY = _mm_packs_epi32(Y0, Y1);


    UV = _mm_shuffle_epi32(UV, shuffle);
    UV = _mm_shufflehi_epi16(UV, shuffle);
    UV = _mm_shufflelo_epi16(UV, shuffle);


    UY = _mm_unpacklo_epi16(UV, YY);
    VY = _mm_unpackhi_epi16(UV, YY);

    UY = _mm_adds_epi16(UY, OFFSET);
    VY = _mm_adds_epi16(VY, OFFSET);

    *yuv_p++ = _mm_packus_epi16(UY, VY);
  }
}
#endif
