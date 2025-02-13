/////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file for color-conversion routines
//
//    Copyright (c) 2023 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "PixConvert.h"
#include "Utils/Functions.h"
#include <cstring>
/*
  input format:

  - Gray

  - UYVY
  - YU12
  - YUY2
  - YV12
  - YVYU
  - YUV422
  - YUV420P

  - BGR
  - RGB
  - RGB16

  - ABGR
  - ARGB
  - BGRA
  - RGBA

  output format:
*/

/*
  YU12
  YUY2
  YV12
  YVYU
*/

#define Y 0

#define RGB 0,1,2
#define BGR 2,1,0
#define RGBA 0,1,2,3
#define BGRA 2,1,0,3
#define ARGB 1,2,3,0
#define ABGR 3,2,1,0

#define UYVY 0,1,2,3
#define VYUY 2,1,0,3

#define YUYV 1,0,3,2
#define YVYU 3,0,1,2


#undef CONVERTER_MARK
#if GEM_DEBUG_PIXCONVERT
# include "m_pd.h"
# define CONVERTER_MARK() logpost(0, 3+1, "%s(%ux%u)", __FUNCTION__, (unsigned int)width, (unsigned int)height)
#else
# define CONVERTER_MARK() 0
#endif

namespace {
  /* generic channel swizzling conversions */
  template <int inR, int inG, int inB,
            int outR, int outG, int outB>
  static void three_to_three(
    const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;

    if(inR == outR && inG == outG && inB == outB) {
      if(indata != outdata)
        memcpy(outdata, indata, width*height*3);
      return;
    }

    if(indata==outdata) {
      // in place conversion
      while(size--) {
        unsigned char R = indata[inR];
        unsigned char G = indata[inG];
        unsigned char B = indata[inB];
        outdata[outR] = R;
        outdata[outG] = G;
        outdata[outB] = B;

        outdata+=3;
        indata+=3;
      }
    } else {
      while(size--) {
        outdata[outR]=indata[inR];
        outdata[outG]=indata[inG];
        outdata[outB]=indata[inB];

        outdata+=3;
        indata+=3;
      }
    }
  }

  template <int inR, int inG, int inB, int inA,
            int outR, int outG, int outB>
  static void four_to_three(
    const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;

    if(indata==outdata) {
      // in place conversion
      while(size--) {
        unsigned char R = indata[inR];
        unsigned char G = indata[inG];
        unsigned char B = indata[inB];
        outdata[outR] = R;
        outdata[outG] = G;
        outdata[outB] = B;

        outdata+=3;
        indata+=4;
      }
    } else {
      while(size--) {
        outdata[outR]=indata[inR];
        outdata[outG]=indata[inG];
        outdata[outB]=indata[inB];

        outdata+=3;
        indata+=4;
      }
    }
  }
  template <int inR, int inG, int inB, int inA,
            int outR, int outG, int outB, int outA>
  static void four_to_four(
    const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;

    if(inR == outR && inG == outG && inB == outB && inA == outA) {
      if(indata != outdata)
        memcpy(outdata, indata, size*4);
      return;
    }


    if(indata==outdata) {
      // in place conversion
      while(size--) {
        unsigned char R = indata[inR];
        unsigned char G = indata[inG];
        unsigned char B = indata[inB];
        unsigned char A = indata[inA];
        outdata[outR] = R;
        outdata[outG] = G;
        outdata[outB] = B;
        outdata[outA] = A;

        outdata+=4;
        indata+=4;
      }
    } else {
      while(size--) {
        outdata[outR]=indata[inR];
        outdata[outG]=indata[inG];
        outdata[outB]=indata[inB];
        outdata[outA]=indata[inA];

        outdata+=4;
        indata+=4;
      }
    }
  }

  template <int inR, int inG, int inB, int inA,
            int outR, int outG, int outB, int outA>
  static void four_to_four_2(
    const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
    four_to_four<inR, inG, inB, inA, outR, outG, outB, outA>(indata, outdata, width>>1, height);
  }

  /* Y -> ... */
  template<int shift,
           int U, int Y0, int V, int Y1,
           class T>
  static void y_to_yuv4(const T*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = (width*height)>>1;
    while(size--) {
      outdata[U]=outdata[V]=UV_OFFSET;
      outdata[Y0]=(*indata++)>>shift;
      outdata[Y1]=(*indata++)>>shift;
      outdata+=4;
    }
  }

  template<int shift,
           int r, int g, int b,
           class T>
  static void y_to_three(const T*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;
    while(size--) {
      const unsigned char value=(*indata++)>>shift;
      outdata[r]=value;
      outdata[g]=value;
      outdata[b]=value;
      outdata+=3;
    }
  }

  template<int shift,
           int r, int g, int b, int a,
           class T>
  static void y_to_rgb4(const T*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;
    while(size--) {
      const unsigned char value=(*indata++)>>shift;
      outdata[r]=value;
      outdata[g]=value;
      outdata[b]=value;
      outdata[a]=255;
      outdata+=4;
    }
  }

  /* YUV4 -> ... */
  template <int U, int Y0, int V, int Y1, int dummy>
  static void yuv4_to_y(
    const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
    size_t size = (width*height)>>1;
    while(size--) {
      *outdata++ = indata[Y0];
      *outdata++ = indata[Y1];
      indata+=4;
    }
  }

  template <int inU, int inY0, int inV, int inY1,
            int outR, int outG, int outB>
  static void yuv4_to_rgb3(const unsigned char*indata, unsigned char*outdata,
                           size_t width, size_t height) {
    size_t size = (width*height)>>1;

    while(size--) {
      int y;
      int u=indata[inU]-UV_OFFSET;
      int v=indata[inV]-UV_OFFSET;
      int uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
      int uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
      int uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

      // 1st pixel
      y=YUV2RGB_11*(indata[inY0] -Y_OFFSET);
      outdata[outR] = CLAMP((y + uv_r) >> 8);
      outdata[outG] = CLAMP((y + uv_g) >> 8);
      outdata[outB] = CLAMP((y + uv_b) >> 8);
      outdata+=3;
      // 2nd pixel
      y=YUV2RGB_11*(indata[inY1] -Y_OFFSET);
      outdata[outR] = CLAMP((y + uv_r) >> 8);
      outdata[outG] = CLAMP((y + uv_g) >> 8);
      outdata[outB] = CLAMP((y + uv_b) >> 8);
      outdata+=3;

      indata+=4;
    }
  }

  template <int inU, int inY0, int inV, int inY1,
            int outR, int outG, int outB, int outA>
  static void yuv4_to_rgb4(const unsigned char*indata, unsigned char*outdata,
                           size_t width, size_t height) {
    size_t size = (width*height)>>1;

    while(size--) {
      int y;
      int u=indata[inU]-UV_OFFSET;
      int v=indata[inV]-UV_OFFSET;
      int uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
      int uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
      int uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

      // 1st pixel
      y=YUV2RGB_11*(indata[inY0] -Y_OFFSET);
      outdata[outR] = CLAMP((y + uv_r) >> 8);
      outdata[outG] = CLAMP((y + uv_g) >> 8);
      outdata[outB] = CLAMP((y + uv_b) >> 8);
      outdata[outA] = 255;
      outdata+=4;
      // 2nd pixel
      y=YUV2RGB_11*(indata[inY1] -Y_OFFSET);
      outdata[outR] = CLAMP((y + uv_r) >> 8);
      outdata[outG] = CLAMP((y + uv_g) >> 8);
      outdata[outB] = CLAMP((y + uv_b) >> 8);
      outdata[outA] = 255;
      outdata+=4;

      indata+=4;
    }
  }


  /* YUV(planar) -> ... */

  template <int outU, int outY0, int outV, int outY1>
  static void yuv420p_to_yuv4(
    const unsigned char*Y_, const unsigned char*U, const unsigned char*V,
    unsigned char*outdata, size_t width, size_t height) {
    unsigned char *pixels1=outdata;
    unsigned char *pixels2=pixels1+width*2;
    const unsigned char*py1=Y_;
    const unsigned char*py2=Y_+width; // plane_1 is luminance (csize==1)
    const unsigned char*pu=U;
    const unsigned char*pv=V;
    int row=height>>1;
    int cols=width>>1;
    /* this is only re-ordering of the data */
    while(row--) {
      int col=cols;
      while(col--) {
        unsigned char u=*pu++;
        unsigned char v=*pv++;
        // yuv422 is U Y0 V Y1
        pixels1[outU ]=u;
        pixels1[outY0]=*py1++;
        pixels1[outV ]=v;
        pixels1[outY1]=*py1++;
        pixels1+=4;

        pixels2[outU ]=u;
        pixels2[outY0]=*py2++;
        pixels2[outV ]=v;
        pixels2[outY1]=*py2++;
        pixels2+=4;
      }
      /* need to skip 1 row, as we keep track of even and odd rows separately */
      pixels1+=width*2;
      pixels2+=width*2;
      py1+=width*1;
      py2+=width*1;
    }
  }

  template <int outR, int outG, int outB>
  static void yuv420p_to_rgb3(
    const unsigned char*Y_, const unsigned char*U, const unsigned char*V,
    unsigned char*outdata, const size_t width, const size_t height) {
    const unsigned char*py1=Y_;
    const unsigned char*py2=Y_+width; // plane_1 is luminance (csize==1)
    const unsigned char*pu=U;
    const unsigned char*pv=V;
    unsigned char*pixels1=outdata;
    unsigned char*pixels2=outdata+width*3;
    const size_t rows = height>>1;
    const size_t cols = width>>1;

    for(int row=0; row<rows; row++) {
      for(int col=0; col<cols; col++) {
        int y;
        int u=*pu++ -UV_OFFSET;
        int v=*pv++ -UV_OFFSET;
        int uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
        int uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
        int uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

        // 1st row - 1st pixel
        y=YUV2RGB_11*(*py1++ -Y_OFFSET);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1+=3;

        // 1st row - 2nd pixel
        y=YUV2RGB_11*(*py1++ -Y_OFFSET);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1+=3;

        // 2nd row - 1st pixel
        y=YUV2RGB_11*(*py2++ -Y_OFFSET);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2+=3;

        // 2nd row - 2nd pixel
        y=YUV2RGB_11*(*py2++ -Y_OFFSET);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2+=3;
      }
      /* need to skip 1 row, as we keep track of even and odd rows separately */
      pixels1+=width*3;
      pixels2+=width*3;
      py1+=width*1;
      py2+=width*1;
    }
  }

  template <int outR, int outG, int outB, int outA>
  static void yuv420p_to_rgb4(
    const unsigned char*Y_, const unsigned char*U, const unsigned char*V,
    unsigned char*outdata, size_t width, size_t height) {
    const unsigned char*py1=Y_;
    const unsigned char*py2=Y_+width; // plane_1 is luminance (csize==1)
    const unsigned char*pu=U;
    const unsigned char*pv=V;
    unsigned char*pixels1=outdata;
    unsigned char*pixels2=outdata+width*4;
    const size_t rows = height>>1;
    const size_t cols = width>>1;

    for(int row=0; row<rows; row++) {
      for(int col=0; col<cols; col++) {
        int y;
        int u=*pu++ -UV_OFFSET;
        int v=*pv++ -UV_OFFSET;
        int uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
        int uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
        int uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

        // 1st row - 1st pixel
        y=YUV2RGB_11*(*py1++ -Y_OFFSET);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1[outA] = 255; // a
        pixels1+=4;

        // 1st row - 2nd pixel
        y=YUV2RGB_11*(*py1++ -Y_OFFSET);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1[outA] = 255; // a
        pixels1+=4;

        // 2nd row - 1st pixel
        y=YUV2RGB_11*(*py2++ -Y_OFFSET);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2[outA] = 255; // a
        pixels2+=4;

        // 2nd row - 2nd pixel
        y=YUV2RGB_11*(*py2++ -Y_OFFSET);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2[outA] = 255; // a
        pixels2+=4;
      }
      /* need to skip 1 row, as we keep track of even and odd rows separately */
      pixels1+=width*4;
      pixels2+=width*4;
      py1+=width*1;
      py2+=width*1;
    }
  }



  /* RGB3 -> ... */
  template <int inR, int inG, int inB, int dummy>
  static void rgb3_to_y(const unsigned char*indata, unsigned char*outdata,
                        size_t width, size_t height) {
    size_t size = width*height;
    while(size--) {
      *outdata++=(indata[inR]*RGB2GRAY_RED
                  +indata[inG]*RGB2GRAY_GREEN
                  +indata[inB]*RGB2GRAY_BLUE)>>8;
      indata+=3;
    }
  }

  template <int inR, int inG, int inB,
            int U, int Y0, int V, int Y1>
  static void rgb3_to_yuv4(const unsigned char*indata, unsigned char*outdata,
                           size_t width, size_t height) {
    size_t size = (width*height)>>1;
    while(size--) {
      outdata[U ]=((RGB2YUV_21*indata[inR]+
                    RGB2YUV_22*indata[inG]+
                    RGB2YUV_23*indata[inB])>>8)+UV_OFFSET; // U
      outdata[Y0]=((RGB2YUV_11*indata[inR]+
                    RGB2YUV_12*indata[inG]+
                    RGB2YUV_13*indata[inB])>>8)+ Y_OFFSET; // Y
      outdata[V ]=((RGB2YUV_31*indata[inR]+
                    RGB2YUV_32*indata[inG]+
                    RGB2YUV_33*indata[inB])>>8)+UV_OFFSET; // V
      outdata[Y1]=((RGB2YUV_11*indata[3+inR]+
                    RGB2YUV_12*indata[3+inG]+
                    RGB2YUV_13*indata[3+inB])>>8)+ Y_OFFSET; // Y
      outdata+=4;
      indata +=8;
    }
  }

  template <int inR, int inG, int inB,
            int outR, int outG, int outB, int outA>
  static void rgb3_to_rgb4(
    const unsigned char*indata, unsigned char*outdata,
    size_t width, size_t height) {
    size_t size = width*height;
    if(indata==outdata) {
      outdata += size*4;
      indata += size*3;
      // in place conversion: we have to process from the end
      while(size--) {
        outdata-=4;
        indata-=3;

        unsigned char R = indata[inR];
        unsigned char G = indata[inG];
        unsigned char B = indata[inB];
        outdata[outR] = R;
        outdata[outG] = G;
        outdata[outB] = B;
        outdata[outA] = 255;
      }
    } else {
      while(size--) {
        outdata[outR]=indata[inR];
        outdata[outG]=indata[inG];
        outdata[outB]=indata[inB];
        outdata[outA] = 255;

        outdata+=4;
        indata+=3;
      }
    }
  }

  /* RGB4 -> ... */

  template <int inR, int inG, int inB, int inA, int dummy>
  static void rgb4_to_y(const unsigned char*indata, unsigned char*outdata,
                        size_t width, size_t height) {
    size_t size = width*height;
    while(size--) {
      *outdata++=(indata[inR]*RGB2GRAY_RED
                  +indata[inG]*RGB2GRAY_GREEN
                  +indata[inB]*RGB2GRAY_BLUE)>>8;
      indata+=4;
    }
  }


  template <int inR, int inG, int inB, int inA,
            int U, int Y0, int V, int Y1>
  static void rgb4_to_yuv4(const unsigned char*indata, unsigned char*outdata,
                           size_t width, size_t height) {
    size_t size = (width*height)>>1;
    while(size--) {
      outdata[U ]=((RGB2YUV_21*indata[inR]+
                    RGB2YUV_22*indata[inG]+
                    RGB2YUV_23*indata[inB])>>8)+UV_OFFSET; // U
      outdata[Y0]=((RGB2YUV_11*indata[inR]+
                    RGB2YUV_12*indata[inG]+
                    RGB2YUV_13*indata[inB])>>8)+ Y_OFFSET; // Y
      outdata[V ]=((RGB2YUV_31*indata[inR]+
                    RGB2YUV_32*indata[inG]+
                    RGB2YUV_33*indata[inB])>>8)+UV_OFFSET; // V
      outdata[Y1]=((RGB2YUV_11*indata[4+inR]+
                    RGB2YUV_12*indata[4+inG]+
                    RGB2YUV_13*indata[4+inB])>>8)+ Y_OFFSET; // Y
      outdata+=4;
      indata +=8;
    }
  }

};

/* YUV420planar (signed short) -> */
namespace {
  template <int outR, int outG, int outB>
  static void i420ps16_to_rgb3(
    const short*Y_, const short*U, const short*V,
    unsigned char*outdata, const size_t width, const size_t height) {
    unsigned char *pixels1=outdata;
    unsigned char *pixels2=outdata+width*3;

    const short*py1=Y_;
    const short*py2=Y_+width; // plane_1 is luminance (csize==1)
    const short*pv=V;//(format==GL_BGR)?V:U;
    const short*pu=U;//(format==GL_RGB)?V:U;

    int row=height>>1;
    int cols=width>>1;
    while(row--) {
      int col=cols;
      while(col--) {
        int y, u, v;
        int uv_r, uv_g, uv_b;
        // these are from http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
        u=(*pu++)>>8;
        v=(*pv++)>>8;
        uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
        uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
        uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

        // 1st row - 1st pixel
        y=YUV2RGB_11*((*py1++)>>7); // what about the "16"-offset ?

        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1+=3;

        // 1st row - 2nd pixel
        y=YUV2RGB_11*((*py1++)>>7);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1+=3;

        // 2nd row - 1st pixel
        y=YUV2RGB_11*((*py2++)>>7);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2+=3;

        // 2nd row - 2nd pixel
        y=YUV2RGB_11*((*py2++)>>7);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2+=3;
      }
      /* need to skip 1 row, as we keep track of even and odd rows separately */
      pixels1+=width*3;
      pixels2+=width*3;
      py1+=width*1;
      py2+=width*1;
    }
  }
  template <int outR, int outG, int outB, int outA>
  static void i420ps16_to_rgb4(
    const short*Y_, const short*U, const short*V,
    unsigned char*outdata, const size_t width, const size_t height) {
    unsigned char *pixels1=outdata;
    unsigned char *pixels2=outdata+width*4;

    const short*py1=Y_; // odd row
    const short*py2=Y_+width;// even row
    const short*pv=V;
    const short*pu=U;

    size_t row=height>>1;
    size_t cols=width>>1;
    while(row--) {
      size_t col=cols;
      while(col--) {
        int y, u, v;
        int uv_r, uv_g, uv_b;
        u=(*pu++)>>8;
        v=(*pv++)>>8;
        uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
        uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
        uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

        // 1st row - 1st pixel
        y=YUV2RGB_11*((*py1++)>>7); // the "16" offset is already in the signed Y
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1[outA] = 255;
        pixels1+=4;

        // 1st row - 2nd pixel
        y=YUV2RGB_11*((*py1++)>>7);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1[outA] = 255;
        pixels1+=4;

        // 2nd row - 1st pixel
        y=YUV2RGB_11*((*py2++)>>7);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2[outA] = 255;
        pixels2+=4;

        // 2nd row - 2nd pixel
        y=YUV2RGB_11*((*py2++)>>7);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2[outA] = 255;
        pixels2+=4;
      }
      pixels1+=width*4;
      pixels2+=width*4;
      py1+=width*1;
      py2+=width*1;
    }
  }
  template <int outU, int outY0, int outV, int outY1>
  static void i420ps16_to_yuv4(
    const short*Y_, const short*U, const short*V,
    unsigned char*outdata, size_t width, size_t height) {
    unsigned char *pixels1=outdata;
    unsigned char *pixels2=pixels1+width*2;
    const short*py1=Y_;
    const short*py2=Y_+width; // plane_1 is luminance (csize==1)
    const short*pu=U;
    const short*pv=V;
    int row=height>>1;
    int cols=width>>1;
    /* this is only re-ordering of the data */
    while(row--) {
      int col=cols;
      while(col--) {
        unsigned char u=CLAMP(((*pu++)>>8) + UV_OFFSET);
        unsigned char v=CLAMP(((*pv++)>>8) + UV_OFFSET);
        // yuv422 is U Y0 V Y1
        pixels1[outU ]=u;
        pixels1[outY0]=CLAMP((*py1++)>>7); // (((*py1++)>>8) + Y_OFFSET)??
        pixels1[outV ]=v;
        pixels1[outY1]=CLAMP((*py1++)>>7);
        pixels1+=4;

        pixels2[outU ]=u;
        pixels2[outY0]=CLAMP((*py2++)>>7);
        pixels2[outV ]=v;
        pixels2[outY1]=CLAMP((*py2++)>>7);
        pixels2+=4;
      }
      /* need to skip 1 row, as we keep track of even and odd rows separately */
      pixels1+=width*2;
      pixels2+=width*2;
      py1+=width*1;
      py2+=width*1;
    }
  }
};


/* RGB16 -> */
/*
  RGB16 packs R,G,B into 16bits (2bytes)
   B B B B B G G G   G G G R R R R R
   R R R R R G G G   G G G B B B B B
*/

namespace {
  template <int U, int Y0, int V, int Y1, int dummy>
  void RGB16_to_yuv4(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
    size_t size = (width*height)>>1;
    const unsigned short*indata = (const unsigned short*)indata_;
    while(size--) {
      unsigned short rgb=*indata++;
      unsigned char r=((rgb>>8)&0xF8);
      unsigned char g=((rgb>>3)&0xFC);
      unsigned char b=((rgb<<3)&0xF8);
      outdata[U ]=((RGB2YUV_21*r+RGB2YUV_22*g+RGB2YUV_23*b)>>8)+UV_OFFSET; // U
      outdata[Y0]=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET; // Y
      outdata[V ]=((RGB2YUV_31*r+RGB2YUV_32*g+RGB2YUV_33*b)>>8)+UV_OFFSET; // V

      rgb=*indata++;
      r=((rgb>>8)&0xF8);
      g=((rgb>>3)&0xFC);
      b=((rgb<<3)&0xF8);
      outdata[Y1]=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET; // Y

      outdata+=4;
    }
  }
  template <int r, int g, int b, int dummy>
  static void RGB16_to_rgb3(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;
    const unsigned short*indata = (const unsigned short*)indata_;
    while(size--) {
      unsigned short rgb=*indata++;
      outdata[r]=((rgb>>8)&0xF8);
      outdata[g]=((rgb>>3)&0xFC);
      outdata[b]=((rgb<<3)&0xF8);
      outdata+=3;
    }
  }
  template <int r, int g, int b, int a, int dummy>
  static void RGB16_to_rgb4(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
    size_t size = width*height;
    const unsigned short*indata = (const unsigned short*)indata_;
    while(size--) {
      unsigned short rgb=*indata++;
      outdata[r]=((rgb>>8)&0xF8);
      outdata[g]=((rgb>>3)&0xFC);
      outdata[b]=((rgb<<3)&0xF8);
      outdata[a]=255;
      outdata+=4;
    }
  }
}

/* the actual converter instances.
 * these macros create <SRC>to<DST>() functions (e.g. RGBAtoUYYV),
 * instantiating the named template.
 * all converters use PACKED formats (except where stated otherwise
 */

/* generic converter: CONVERT(ARGB, UYVY, rgb4_to_yuv4, unsigned char);
 *    converts from (ushort*)ARGB to (uchar*)UYVY using rgb4_to_yuv4
 *    the type 'T' must be 'unsigned char' (it's only there for symmetry)
 */
#define CONVERT(SRC, DST, templ, T)                   \
  void SRC##to##DST(                                  \
    const T*indata, unsigned char*outdata,            \
    size_t width, size_t height) {                    \
    CONVERTER_MARK();                                 \
    templ<SRC, DST>(indata, outdata, width, height);  \
  }
/* same as CONVERTER, but inputdata T can be non-(uchar*)
 * at the expense that 'templ' can only convert from 'SRC'.
 * the 'shift' parameter is 8*(sizeof(T)-sizeof(unsigned char*))
 */
#define CONVERT0(SRC, DST, templ, T, shift)             \
  void SRC##to##DST(                                    \
    const T*indata, unsigned char*outdata,              \
    size_t width, size_t height) {                      \
    CONVERTER_MARK();                                   \
    templ<shift, DST>(indata, outdata, width, height);  \
  }
/* planar->packed converter: CONVERTp(I420, UYVY, yuv420p_to_yuv4, unsigned char);
 *    converts from (uchar*)I420 (3planes) to (uchar*)UYVY (1plane) using yuv420p_to_yuv4
 */
#define CONVERTp(SRC, DST, templ, T)                        \
  void SRC##to##DST(                                        \
    const T*Y_, const T*U, const T*V, unsigned char*outdata, \
    size_t width, size_t height) {                          \
    CONVERTER_MARK();                                       \
    templ<DST>(Y_, U, V, outdata, width, height);            \
  }

/* GRAY -> */
void YtoY(const unsigned char*indata,
          unsigned char*outdata, size_t width, size_t height) {
  CONVERTER_MARK();
  if(indata != outdata)
    memcpy(outdata, indata, width*height);
}
CONVERT0(Y, UYVY, y_to_yuv4, unsigned char, 0);
CONVERT0(Y, VYUY, y_to_yuv4, unsigned char, 0);
CONVERT0(Y, YVYU, y_to_yuv4, unsigned char, 0);
CONVERT0(Y, YUYV, y_to_yuv4, unsigned char, 0);
CONVERT0(Y, RGB , y_to_three, unsigned char, 0);
CONVERT0(Y, BGR , y_to_three, unsigned char, 0);
CONVERT0(Y, RGBA, y_to_rgb4, unsigned char, 0);
CONVERT0(Y, BGRA, y_to_rgb4, unsigned char, 0);
CONVERT0(Y, ABGR, y_to_rgb4, unsigned char, 0);
CONVERT0(Y, ARGB, y_to_rgb4, unsigned char, 0);

void Yu16toY(const unsigned short*indata,
             unsigned char*outdata, size_t width, size_t height) {
  CONVERTER_MARK();
  size_t size = width*height;
  while(size--) {
    *outdata++ = (*indata++)>>8;
  }
}
CONVERT0(Yu16, UYVY, y_to_yuv4, unsigned short, 8);
CONVERT0(Yu16, VYUY, y_to_yuv4, unsigned short, 8);
CONVERT0(Yu16, YVYU, y_to_yuv4, unsigned short, 8);
CONVERT0(Yu16, YUYV, y_to_yuv4, unsigned short, 8);
CONVERT0(Yu16, RGB, y_to_three, unsigned short, 8);
CONVERT0(Yu16, BGR, y_to_three, unsigned short, 8);
CONVERT0(Yu16, RGBA, y_to_rgb4, unsigned short, 8);
CONVERT0(Yu16, BGRA, y_to_rgb4, unsigned short, 8);
CONVERT0(Yu16, ABGR, y_to_rgb4, unsigned short, 8);
CONVERT0(Yu16, ARGB, y_to_rgb4, unsigned short, 8);


/* YUV420planar -> */
void I420toY(const unsigned char*Y_, const unsigned char*U, const unsigned char*V,
             unsigned char*outdata, size_t width, size_t height) {
  CONVERTER_MARK();
  if(Y_ != outdata)
    memcpy(outdata, Y_, width*height);
}

CONVERTp(I420, UYVY, yuv420p_to_yuv4, unsigned char);
CONVERTp(I420, VYUY, yuv420p_to_yuv4, unsigned char);
CONVERTp(I420, YVYU, yuv420p_to_yuv4, unsigned char);
CONVERTp(I420, YUYV, yuv420p_to_yuv4, unsigned char);
CONVERTp(I420, RGB , yuv420p_to_rgb3, unsigned char);
CONVERTp(I420, BGR , yuv420p_to_rgb3, unsigned char);
CONVERTp(I420, RGBA, yuv420p_to_rgb4, unsigned char);
CONVERTp(I420, BGRA, yuv420p_to_rgb4, unsigned char);
CONVERTp(I420, ABGR, yuv420p_to_rgb4, unsigned char);
CONVERTp(I420, ARGB, yuv420p_to_rgb4, unsigned char);

void I420S16toY(const short*Y_, const short*U, const short*V,
                unsigned char*outdata, size_t width, size_t height) {
  CONVERTER_MARK();
  size_t size = width*height;
  while(size--) {
    *outdata++ = ((*Y_++)>>8) + Y_OFFSET;
  }
}
CONVERTp(I420S16, UYVY, i420ps16_to_yuv4, short);
CONVERTp(I420S16, VYUY, i420ps16_to_yuv4, short);
CONVERTp(I420S16, YVYU, i420ps16_to_yuv4, short);
CONVERTp(I420S16, YUYV, i420ps16_to_yuv4, short);
CONVERTp(I420S16, RGB , i420ps16_to_rgb3, short);
CONVERTp(I420S16, BGR , i420ps16_to_rgb3, short);
CONVERTp(I420S16, RGBA, i420ps16_to_rgb4, short);
CONVERTp(I420S16, BGRA, i420ps16_to_rgb4, short);
CONVERTp(I420S16, ARGB, i420ps16_to_rgb4, short);
CONVERTp(I420S16, ABGR, i420ps16_to_rgb4, short);

/* UYVY -> */
CONVERT(UYVY, Y, yuv4_to_y, unsigned char);
CONVERT(UYVY, UYVY, four_to_four_2, unsigned char);
CONVERT(UYVY, VYUY, four_to_four_2, unsigned char);
CONVERT(UYVY, YVYU, four_to_four_2, unsigned char);
CONVERT(UYVY, YUYV, four_to_four_2, unsigned char);
CONVERT(UYVY, RGB , yuv4_to_rgb3, unsigned char);
CONVERT(UYVY, BGR , yuv4_to_rgb3, unsigned char);
CONVERT(UYVY, RGBA, yuv4_to_rgb4, unsigned char);
CONVERT(UYVY, BGRA, yuv4_to_rgb4, unsigned char);
CONVERT(UYVY, ABGR, yuv4_to_rgb4, unsigned char);
CONVERT(UYVY, ARGB, yuv4_to_rgb4, unsigned char);

CONVERT(VYUY, Y, yuv4_to_y, unsigned char);
CONVERT(VYUY, UYVY, four_to_four_2, unsigned char);
CONVERT(VYUY, VYUY, four_to_four_2, unsigned char);
CONVERT(VYUY, YVYU, four_to_four_2, unsigned char);
CONVERT(VYUY, YUYV, four_to_four_2, unsigned char);
CONVERT(VYUY, RGB , yuv4_to_rgb3, unsigned char);
CONVERT(VYUY, BGR , yuv4_to_rgb3, unsigned char);
CONVERT(VYUY, RGBA, yuv4_to_rgb4, unsigned char);
CONVERT(VYUY, BGRA, yuv4_to_rgb4, unsigned char);
CONVERT(VYUY, ABGR, yuv4_to_rgb4, unsigned char);
CONVERT(VYUY, ARGB, yuv4_to_rgb4, unsigned char);

CONVERT(YUYV, Y, yuv4_to_y, unsigned char);
CONVERT(YUYV, UYVY, four_to_four_2, unsigned char);
CONVERT(YUYV, VYUY, four_to_four_2, unsigned char);
CONVERT(YUYV, YVYU, four_to_four_2, unsigned char);
CONVERT(YUYV, YUYV, four_to_four_2, unsigned char);
CONVERT(YUYV, RGB , yuv4_to_rgb3, unsigned char);
CONVERT(YUYV, BGR , yuv4_to_rgb3, unsigned char);
CONVERT(YUYV, RGBA, yuv4_to_rgb4, unsigned char);
CONVERT(YUYV, BGRA, yuv4_to_rgb4, unsigned char);
CONVERT(YUYV, ABGR, yuv4_to_rgb4, unsigned char);
CONVERT(YUYV, ARGB, yuv4_to_rgb4, unsigned char);

CONVERT(YVYU, Y, yuv4_to_y, unsigned char);
CONVERT(YVYU, UYVY, four_to_four_2, unsigned char);
CONVERT(YVYU, VYUY, four_to_four_2, unsigned char);
CONVERT(YVYU, YVYU, four_to_four_2, unsigned char);
CONVERT(YVYU, YUYV, four_to_four_2, unsigned char);
CONVERT(YVYU, RGB , yuv4_to_rgb3, unsigned char);
CONVERT(YVYU, BGR , yuv4_to_rgb3, unsigned char);
CONVERT(YVYU, RGBA, yuv4_to_rgb4, unsigned char);
CONVERT(YVYU, BGRA, yuv4_to_rgb4, unsigned char);
CONVERT(YVYU, ABGR, yuv4_to_rgb4, unsigned char);
CONVERT(YVYU, ARGB, yuv4_to_rgb4, unsigned char);

/* RGB -> */
void RGB16toY(const unsigned char*indata_,
              unsigned char*outdata, size_t width, size_t height) {
  CONVERTER_MARK();
  size_t size = width*height;
  const unsigned short*indata = (const unsigned short*)indata_;
  while(size--) {
    unsigned short rgb=*indata++;
    *outdata++=(
      ((rgb>>8)&0xF8)*RGB2GRAY_RED   +
      ((rgb>>3)&0xFC)*RGB2GRAY_GREEN +
      ((rgb<<3)&0xF8)*RGB2GRAY_BLUE
      )>>8;
  }
}
CONVERT0(RGB16, UYVY, RGB16_to_yuv4, unsigned char, 0);
CONVERT0(RGB16, VYUY, RGB16_to_yuv4, unsigned char, 0);
CONVERT0(RGB16, YVYU, RGB16_to_yuv4, unsigned char, 0);
CONVERT0(RGB16, YUYV, RGB16_to_yuv4, unsigned char, 0);
CONVERT0(RGB16, RGB , RGB16_to_rgb3, unsigned char, 0);
CONVERT0(RGB16, BGR , RGB16_to_rgb3, unsigned char, 0);
CONVERT0(RGB16, RGBA, RGB16_to_rgb4, unsigned char, 0);
CONVERT0(RGB16, BGRA, RGB16_to_rgb4, unsigned char, 0);
CONVERT0(RGB16, ABGR, RGB16_to_rgb4, unsigned char, 0);
CONVERT0(RGB16, ARGB, RGB16_to_rgb4, unsigned char, 0);

CONVERT(RGB, Y, rgb3_to_y, unsigned char);
CONVERT(RGB, UYVY, rgb3_to_yuv4, unsigned char);
CONVERT(RGB, VYUY, rgb3_to_yuv4, unsigned char);
CONVERT(RGB, YVYU, rgb3_to_yuv4, unsigned char);
CONVERT(RGB, YUYV, rgb3_to_yuv4, unsigned char);
CONVERT(RGB, RGB, three_to_three, unsigned char);
CONVERT(RGB, BGR, three_to_three, unsigned char);
CONVERT(RGB, RGBA, rgb3_to_rgb4, unsigned char);
CONVERT(RGB, ABGR, rgb3_to_rgb4, unsigned char);
CONVERT(RGB, BGRA, rgb3_to_rgb4, unsigned char);
CONVERT(RGB, ARGB, rgb3_to_rgb4, unsigned char);

CONVERT(BGR, Y, rgb3_to_y, unsigned char);
CONVERT(BGR, UYVY, rgb3_to_yuv4, unsigned char);
CONVERT(BGR, VYUY, rgb3_to_yuv4, unsigned char);
CONVERT(BGR, YVYU, rgb3_to_yuv4, unsigned char);
CONVERT(BGR, YUYV, rgb3_to_yuv4, unsigned char);
CONVERT(BGR, RGB, three_to_three, unsigned char);
CONVERT(BGR, BGR, three_to_three, unsigned char);
CONVERT(BGR, RGBA, rgb3_to_rgb4, unsigned char);
CONVERT(BGR, ABGR, rgb3_to_rgb4, unsigned char);
CONVERT(BGR, BGRA, rgb3_to_rgb4, unsigned char);
CONVERT(BGR, ARGB, rgb3_to_rgb4, unsigned char);

/* RGBA -> */
CONVERT(RGBA, Y, rgb4_to_y, unsigned char);
CONVERT(RGBA, UYVY, rgb4_to_yuv4, unsigned char);
CONVERT(RGBA, VYUY, rgb4_to_yuv4, unsigned char);
CONVERT(RGBA, YVYU, rgb4_to_yuv4, unsigned char);
CONVERT(RGBA, YUYV, rgb4_to_yuv4, unsigned char);
CONVERT(RGBA, RGB, four_to_three, unsigned char);
CONVERT(RGBA, BGR, four_to_three, unsigned char);
CONVERT(RGBA, RGBA, four_to_four, unsigned char);
CONVERT(RGBA, ABGR, four_to_four, unsigned char);
CONVERT(RGBA, BGRA, four_to_four, unsigned char);
CONVERT(RGBA, ARGB, four_to_four, unsigned char);

CONVERT(BGRA, Y, rgb4_to_y, unsigned char);
CONVERT(BGRA, UYVY, rgb4_to_yuv4, unsigned char);
CONVERT(BGRA, VYUY, rgb4_to_yuv4, unsigned char);
CONVERT(BGRA, YVYU, rgb4_to_yuv4, unsigned char);
CONVERT(BGRA, YUYV, rgb4_to_yuv4, unsigned char);
CONVERT(BGRA, RGB, four_to_three, unsigned char);
CONVERT(BGRA, BGR, four_to_three, unsigned char);
CONVERT(BGRA, RGBA, four_to_four, unsigned char);
CONVERT(BGRA, ABGR, four_to_four, unsigned char);
CONVERT(BGRA, BGRA, four_to_four, unsigned char);
CONVERT(BGRA, ARGB, four_to_four, unsigned char);

CONVERT(ABGR, Y, rgb4_to_y, unsigned char);
CONVERT(ABGR, UYVY, rgb4_to_yuv4, unsigned char);
CONVERT(ABGR, VYUY, rgb4_to_yuv4, unsigned char);
CONVERT(ABGR, YVYU, rgb4_to_yuv4, unsigned char);
CONVERT(ABGR, YUYV, rgb4_to_yuv4, unsigned char);
CONVERT(ABGR, RGB, four_to_three, unsigned char);
CONVERT(ABGR, BGR, four_to_three, unsigned char);
CONVERT(ABGR, RGBA, four_to_four, unsigned char);
CONVERT(ABGR, ABGR, four_to_four, unsigned char);
CONVERT(ABGR, BGRA, four_to_four, unsigned char);
CONVERT(ABGR, ARGB, four_to_four, unsigned char);

CONVERT(ARGB, Y, rgb4_to_y, unsigned char);
CONVERT(ARGB, UYVY, rgb4_to_yuv4, unsigned char);
CONVERT(ARGB, VYUY, rgb4_to_yuv4, unsigned char);
CONVERT(ARGB, YVYU, rgb4_to_yuv4, unsigned char);
CONVERT(ARGB, YUYV, rgb4_to_yuv4, unsigned char);
CONVERT(ARGB, RGB, four_to_three, unsigned char);
CONVERT(ARGB, BGR, four_to_three, unsigned char);
CONVERT(ARGB, RGBA, four_to_four, unsigned char);
CONVERT(ARGB, ABGR, four_to_four, unsigned char);
CONVERT(ARGB, BGRA, four_to_four, unsigned char);
CONVERT(ARGB, ARGB, four_to_four, unsigned char);
