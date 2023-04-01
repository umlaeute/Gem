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

#define RGB 0,1,2
#define BGR 2,1,0
#define RGBA 0,1,2,3
#define BGRA 2,1,0,3
#define ARGB 1,2,3,0
#define ABGR 3,2,1,0

#define UYVY 0,1,2,3

namespace {
  template <int inR, int inG, int inB,
            int outR, int outG, int outB>
  static void three_to_three(
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
  template <int inR, int inG, int inB,
            int outR, int outG, int outB, int outA>
  static void three_to_four(
    const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
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

};


/* GRAY -> */
template<int shift,
         int U, int Y0, int V, int Y1,
         class T>
static void y_to_uyvy(const T*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  while(size--) {
    outdata[U]=outdata[V]=128;
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
static void y_to_four(const T*indata, unsigned char*outdata, size_t width, size_t height) {
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


void YtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_uyvy<0, RGBA>(indata, outdata, width, height);
}
void YtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_three<0, RGB>(indata, outdata, width, height);
}
void YtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_three<0, BGR>(indata, outdata, width, height);
}
void YtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_four<0, RGBA>(indata, outdata, width, height);
}
void YtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_four<0, BGRA>(indata, outdata, width, height);
}
void YtoUYVY(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_uyvy<8, RGBA>(indata, outdata, width, height);
}
void YtoRGB(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_three<8, RGB>(indata, outdata, width, height);
}
void YtoBGR(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_three<8, BGR>(indata, outdata, width, height);
}
void YtoRGBA(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_four<8, RGBA>(indata, outdata, width, height);
}
void YtoBGRA(const unsigned short*indata, unsigned char*outdata, size_t width, size_t height) {
  y_to_four<8, BGRA>(indata, outdata, width, height);
}

/* YUV420planar -> */

void YUV420PtoY(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                unsigned char*outdata, size_t width, size_t height) {
  memcpy(outdata, Y, width*height);
}
namespace {
  template <int outR, int outG, int outB>
  static void yuv420p_to_three(
    const unsigned char*Y, const unsigned char*U, const unsigned char*V,
    unsigned char*outdata, const size_t width, const size_t height) {

    const unsigned char*py1=Y;
    const unsigned char*py2=Y+width; // plane_1 is luminance (csize==1)
    const unsigned char*pu=U;
    const unsigned char*pv=V;
    unsigned char*pixels1=outdata;
    unsigned char*pixels2=outdata+width*3;

    for(int row=0; row<(width>>1); row++) {
      for(int col=0; col<(height>>1); col++) {
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
      pixels1+=width*2;
      pixels2+=width*2;
      py1+=width*1;
      py2+=width*1;
    }
  }
}
namespace {
  template <int outR, int outG, int outB, int outA>
  static void yuv420p_to_four(
    const unsigned char*Y, const unsigned char*U, const unsigned char*V,
    unsigned char*outdata, size_t width, size_t height) {
    const unsigned char*py1=Y;
    const unsigned char*py2=py1+width; // plane_1 is luminance (csize==1)
    const unsigned char*pu=U;
    const unsigned char*pv=V;
    unsigned char*pixels1=outdata;
    unsigned char*pixels2=outdata+width*3;

    for(int row=0; row<(width>>1); row++) {
      for(int col=0; col<(height>>1); col++) {
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
        pixels1+=3;

        // 1st row - 2nd pixel
        y=YUV2RGB_11*(*py1++ -Y_OFFSET);
        pixels1[outR] = CLAMP((y + uv_r) >> 8);
        pixels1[outG] = CLAMP((y + uv_g) >> 8);
        pixels1[outB] = CLAMP((y + uv_b) >> 8);
        pixels1[outA] = 255; // a
        pixels1+=3;

        // 2nd row - 1st pixel
        y=YUV2RGB_11*(*py2++ -Y_OFFSET);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2[outA] = 255; // a
        pixels2+=3;

        // 2nd row - 2nd pixel
        y=YUV2RGB_11*(*py2++ -Y_OFFSET);
        pixels2[outR] = CLAMP((y + uv_r) >> 8);
        pixels2[outG] = CLAMP((y + uv_g) >> 8);
        pixels2[outB] = CLAMP((y + uv_b) >> 8);
        pixels2[outA] = 255; // a
        pixels2+=3;
      }
      /* need to skip 1 row, as we keep track of even and odd rows separately */
      pixels1+=width*2;
      pixels2+=width*2;
      py1+=width*1;
      py2+=width*1;
    }
  }
}
namespace {
  template <int outU, int outY0, int outV, int outY1>
  static void yuv420p_to_yuv4(
    const unsigned char*Y, const unsigned char*U, const unsigned char*V,
    unsigned char*outdata, size_t width, size_t height) {
    unsigned char *pixels1=outdata;
    unsigned char *pixels2=pixels1+width*2;
    const unsigned char*py1=Y;
    const unsigned char*py2=Y+width; // plane_1 is luminance (csize==1)
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
}
void YUV420PtoRGB(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                  unsigned char*outdata, size_t width, size_t height) {
  yuv420p_to_three<RGB>(Y, U, V, outdata, width, height);
}
void YUV420PtoBGR(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                  unsigned char*outdata, size_t width, size_t height) {
  yuv420p_to_three<BGR>(Y, U, V, outdata, width, height);
}
void YUV420PtoRGBA(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                   unsigned char*outdata, size_t width, size_t height) {
  yuv420p_to_four<RGBA>(Y, U, V, outdata, width, height);
}
void YUV420PtoBGRA(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                   unsigned char*outdata, size_t width, size_t height) {
  yuv420p_to_four<BGRA>(Y, U, V, outdata, width, height);
}
void YUV420PtoUYVY(const unsigned char*Y, const unsigned char*U, const unsigned char*V,
                   unsigned char*outdata, size_t width, size_t height) {
  yuv420p_to_yuv4<UYVY>(Y, U, V, outdata, width, height);
}


/* RGB -> */
void RGBtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = width*height;
  while(size--) {
    *outdata++=(indata[0]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[2]*RGB2GRAY_BLUE)>>8;
    indata+=3;
  }
}
void RGBtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  while(size--) {
    *outdata++=((RGB2YUV_21*indata[0]+
                 RGB2YUV_22*indata[1]+
                 RGB2YUV_23*indata[2])>>8)+UV_OFFSET; // U
    *outdata++=((RGB2YUV_11*indata[0]+
                 RGB2YUV_12*indata[1]+
                 RGB2YUV_13*indata[2])>>8)+ Y_OFFSET; // Y
    *outdata++=((RGB2YUV_31*indata[0]+
                 RGB2YUV_32*indata[1]+
                 RGB2YUV_33*indata[2])>>8)+UV_OFFSET; // V
    *outdata++=((RGB2YUV_11*indata[3+0]+
                 RGB2YUV_12*indata[3+1]+
                 RGB2YUV_13*indata[3+2])>>8)+ Y_OFFSET; // Y
    indata+=8;
  }
}
void RGBtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_three<RGB, BGR>(indata, outdata, width, height);
}
void RGBtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_four<RGB, RGBA>(indata, outdata, width, height);
}
void RGBtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_four<RGB, ABGR>(indata, outdata, width, height);
}
void RGBtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_four<RGB, BGRA>(indata, outdata, width, height);
}

/* RGB -> */
void BGRtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = width*height;
  while(size--) {
    *outdata++=(indata[2]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[0]*RGB2GRAY_BLUE)>>8;
    indata+=3;
  }
}
void BGRtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  while(size--) {
    *outdata++=((RGB2YUV_21*indata[2]+
                 RGB2YUV_22*indata[1]+
                 RGB2YUV_23*indata[0])>>8)+UV_OFFSET; // U
    *outdata++=((RGB2YUV_11*indata[2]+
                 RGB2YUV_12*indata[1]+
                 RGB2YUV_13*indata[0])>>8)+ Y_OFFSET; // Y
    *outdata++=((RGB2YUV_31*indata[2]+
                 RGB2YUV_32*indata[1]+
                 RGB2YUV_33*indata[0])>>8)+UV_OFFSET; // V
    *outdata++=((RGB2YUV_11*indata[3+2]+
                 RGB2YUV_12*indata[3+1]+
                 RGB2YUV_13*indata[3+0])>>8)+ Y_OFFSET; // Y
    indata+=8;
  }
}
void BGRtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_three<BGR, RGB>(indata, outdata, width, height);
}
void BGRtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_four<BGR, BGRA>(indata, outdata, width, height);
}
void BGRtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_four<BGR, ABGR>(indata, outdata, width, height);
}
void BGRtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  three_to_four<BGR, RGBA>(indata, outdata, width, height);
}

/* RGBA -> */

void RGBAtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = width*height;
  while(size--) {
    *outdata++=(indata[0]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[2]*RGB2GRAY_BLUE)>>8;
    indata+=4;
  }
}
void RGBAtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  while(size--) {
    *outdata++=((RGB2YUV_21*indata[0]+
                 RGB2YUV_22*indata[1]+
                 RGB2YUV_23*indata[2])>>8)+UV_OFFSET; // U
    *outdata++=((RGB2YUV_11*indata[0]+
                 RGB2YUV_12*indata[1]+
                 RGB2YUV_13*indata[2])>>8)+ Y_OFFSET; // Y
    *outdata++=((RGB2YUV_31*indata[0]+
                 RGB2YUV_32*indata[1]+
                 RGB2YUV_33*indata[2])>>8)+UV_OFFSET; // V
    *outdata++=((RGB2YUV_11*indata[4+0]+
                 RGB2YUV_12*indata[4+1]+
                 RGB2YUV_13*indata[4+2])>>8)+ Y_OFFSET; // Y
    indata+=8;
  }
}
void RGBAtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_three<RGBA, RGB>(indata, outdata, width, height);
}
void RGBAtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_three<RGBA, BGR>(indata, outdata, width, height);
}
void RGBAtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_four<RGBA, ABGR>(indata, outdata, width, height);
}
void RGBAtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_four<RGBA, BGRA>(indata, outdata, width, height);
}


/* BGRA -> */
void BGRAtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = width*height;
  while(size--) {
    *outdata++=(indata[2]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[0]*RGB2GRAY_BLUE)>>8;
    indata+=4;
  }
}
void BGRAtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  while(size--) {
    *outdata++=((RGB2YUV_21*indata[2]+
                 RGB2YUV_22*indata[1]+
                 RGB2YUV_23*indata[0])>>8)+UV_OFFSET; // U
    *outdata++=((RGB2YUV_11*indata[2]+
                 RGB2YUV_12*indata[1]+
                 RGB2YUV_13*indata[0])>>8)+ Y_OFFSET; // Y
    *outdata++=((RGB2YUV_31*indata[2]+
                 RGB2YUV_32*indata[1]+
                 RGB2YUV_33*indata[0])>>8)+UV_OFFSET; // V
    *outdata++=((RGB2YUV_11*indata[4+2]+
                 RGB2YUV_12*indata[4+1]+
                 RGB2YUV_13*indata[4+0])>>8)+ Y_OFFSET; // Y
    indata+=8;
  }
}

void BGRAtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_three<BGRA, RGB>(indata, outdata, width, height);
}
void BGRAtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_three<BGRA, BGR>(indata, outdata, width, height);
}
void BGRAtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_four<BGRA, RGBA>(indata, outdata, width, height);
}
void BGRAtoABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_four<BGRA, ABGR>(indata, outdata, width, height);
}


/* ABGR -> */
void ABGRtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = width*height;
  while(size--) {
    *outdata++=(indata[3]*RGB2GRAY_RED
                +indata[2]*RGB2GRAY_GREEN
                +indata[1]*RGB2GRAY_BLUE)>>8;
    indata+=4;
  }
}
void ABGRtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  while(size--) {
    *outdata++=((RGB2YUV_21*indata[3]+
                 RGB2YUV_22*indata[2]+
                 RGB2YUV_23*indata[1])>>8)+UV_OFFSET; // U
    *outdata++=((RGB2YUV_11*indata[3]+
                 RGB2YUV_12*indata[2]+
                 RGB2YUV_13*indata[1])>>8)+ Y_OFFSET; // Y
    *outdata++=((RGB2YUV_31*indata[3]+
                 RGB2YUV_32*indata[2]+
                 RGB2YUV_33*indata[1])>>8)+UV_OFFSET; // V
    *outdata++=((RGB2YUV_11*indata[4+3]+
                 RGB2YUV_12*indata[4+2]+
                 RGB2YUV_13*indata[4+1])>>8)+ Y_OFFSET; // Y
    indata+=8;
  }
}
void ABGRtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_three<ABGR, RGB>(indata, outdata, width, height);
}
void ABGRtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_three<ABGR, BGR>(indata, outdata, width, height);
}
void ABGRtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_four<ABGR, RGBA>(indata, outdata, width, height);
}
void ABGRtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  four_to_four<ABGR, BGRA>(indata, outdata, width, height);
}


/* ARGB -> */
void ARGBtoY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height){
  size_t size = width*height;
  while(size--) {
    *outdata++=((RGB2GRAY_RED*indata[1]+
                 RGB2GRAY_GREEN*indata[2]+
                 RGB2GRAY_BLUE*indata[3]
                  )>>8);
    indata+=4;
  }
}
void ARGBtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height){
  size_t size = (width*height)>>1;
  while(size--) {
    *outdata++=((RGB2YUV_21*indata[1]+
                 RGB2YUV_22*indata[2]+
                 RGB2YUV_23*indata[3]
                  )>>8)+UV_OFFSET;
    *outdata++=((RGB2YUV_11*indata[1]+
                 RGB2YUV_12*indata[2]+
                 RGB2YUV_13*indata[3])>>8)+ Y_OFFSET;
    *outdata++=((RGB2YUV_31*indata[1]+
                 RGB2YUV_32*indata[2]+
                 RGB2YUV_33*indata[3])>>8)+UV_OFFSET;
    *outdata++=((RGB2YUV_11*indata[4+1]+
                 RGB2YUV_12*indata[4+2]+
                 RGB2YUV_13*indata[4+3])>>8)+ Y_OFFSET;
    indata+=8;
  }
}
void ARGBtoRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height){
  four_to_three<ARGB, RGB>(indata, outdata, width, height);
}
void ARGBtoBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height){
  four_to_three<ARGB, BGR>(indata, outdata, width, height);
}
void ARGBtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height){
  four_to_four<ARGB, RGBA>(indata, outdata, width, height);
}
void ARGBtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height){
  four_to_four<ARGB, BGRA>(indata, outdata, width, height);
}


/* RGB16 -> */
/*
  RGB16 packs R,G,B into 16bits (2bytes)
   B B B B B G G G   G G G R R R R R
   R R R R R G G G   G G G B B B B B
*/

void RGB16toY(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
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
void RGB16toUYVY(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
  size_t size = (width*height)>>1;
  const unsigned short*indata = (const unsigned short*)indata_;
  while(size--) {
    unsigned short rgb=*indata++;
    unsigned char r=((rgb>>8)&0xF8);
    unsigned char g=((rgb>>3)&0xFC);
    unsigned char b=((rgb<<3)&0xF8);
    *outdata++=((RGB2YUV_21*r+RGB2YUV_22*g+RGB2YUV_23*b)>>8)+UV_OFFSET; // U
    *outdata++=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET; // Y
    *outdata++=((RGB2YUV_31*r+RGB2YUV_32*g+RGB2YUV_33*b)>>8)+UV_OFFSET; // V

    rgb=*indata++;
    r=((rgb>>8)&0xF8);
    g=((rgb>>3)&0xFC);
    b=((rgb<<3)&0xF8);
    *outdata++=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET; // Y
  }
}
template <int r, int g, int b>
static void RGB16_to_three(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
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
template <int r, int g, int b, int a>
static void RGB16_to_four(const unsigned char*indata_, unsigned char*outdata, size_t width, size_t height) {
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
void RGB16toRGB(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  RGB16_to_three<RGB>(indata, outdata, width, height);
}
void RGB16toBGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  RGB16_to_three<BGR>(indata, outdata, width, height);
}
void RGB16toRGBA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  RGB16_to_four<RGBA>(indata, outdata, width, height);
}
void RGB16toBGRA(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  RGB16_to_four<BGRA>(indata, outdata, width, height);
}
void RGB16toABGR(const unsigned char*indata, unsigned char*outdata, size_t width, size_t height) {
  RGB16_to_four<ABGR>(indata, outdata, width, height);
}
