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


namespace {
  template <int inR, int inG, int inB,
            int outR, int outG, int outB>
  static void rgb_to_rgb(
    const unsigned char*indata, unsigned char*outdata, size_t size) {

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
  static void rgb_to_rgba(
    const unsigned char*indata, unsigned char*outdata, size_t size) {
    if(indata==outdata) {
      #warning FIXME inplace RGB->RGBA
      // in place conversion (this is broken; we have to prcess from the end
      while(size--) {
        unsigned char R = indata[inR];
        unsigned char G = indata[inG];
        unsigned char B = indata[inB];
        outdata[outR] = R;
        outdata[outG] = G;
        outdata[outB] = B;
        outdata[outA] = 255;

        outdata+=4;
        indata+=3;
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
  static void rgba_to_rgb(
    const unsigned char*indata, unsigned char*outdata, size_t size) {

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
  static void rgba_to_rgba(
    const unsigned char*indata, unsigned char*outdata, size_t size) {

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
void YtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  size>>=1;
  while(size--) {
    outdata[0]=outdata[2]=128;
    outdata[1]=*indata++;
    outdata[3]=*indata++;
    outdata+=4;
  }
}
void YtoRGB(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    const unsigned char value=*indata++;
    *outdata++=value;
    *outdata++=value;
    *outdata++=value;
    indata++;
  }
}
void YtoBGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  YtoRGB(indata, outdata, size);
}
void YtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    const unsigned char value=*indata++;
    *outdata++=value;
    *outdata++=value;
    *outdata++=value;
    *outdata++=255;
  }
}
void YtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  YtoRGBA(indata, outdata, size);
}


/* RGB -> */
void RGBtoY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    *outdata++=(indata[0]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[2]*RGB2GRAY_BLUE)>>8;
    indata+=3;
  }
}
void RGBtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  size>>=1;
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
void RGBtoBGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgb<0,1,2, 2,1,0>(indata, outdata, size);
}
void RGBtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgba<0,1,2, 0,1,2,3>(indata, outdata, size);
}
void RGBtoABGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgba<0,1,2, 3,2,1,0>(indata, outdata, size);
}
void RGBtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgba<0,1,2, 2,1,0,3>(indata, outdata, size);
}

/* RGB -> */
void BGRtoY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    *outdata++=(indata[2]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[0]*RGB2GRAY_BLUE)>>8;
    indata+=3;
  }
}
void BGRtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  size>>=1;
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
void BGRtoRGB(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgb<2,1,0, 0,1,2>(indata, outdata, size);
}
void BGRtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgba<2,1,0, 2,1,0,3>(indata, outdata, size);
}
void BGRtoABGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgba<2,1,0, 3,2,1,0>(indata, outdata, size);
}
void BGRtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgb_to_rgba<2,1,0, 0,1,2,3>(indata, outdata, size);
}

/* RGBA -> */

void RGBAtoY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    *outdata++=(indata[0]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[2]*RGB2GRAY_BLUE)>>8;
    indata+=4;
  }
}
void RGBAtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  size>>=1;
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
void RGBAtoRGB(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgb<0,1,2,3, 0,1,2>(indata, outdata, size);
}
void RGBAtoBGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgb<0,1,2,3, 2,1,0>(indata, outdata, size);
}
void RGBAtoABGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgba<0,1,2,3, 3,2,1,0>(indata, outdata, size);
}
void RGBAtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgba<0,1,2,3, 2,1,0,3>(indata, outdata, size);
}


/* BGRA -> */
void BGRAtoY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    *outdata++=(indata[2]*RGB2GRAY_RED
                +indata[1]*RGB2GRAY_GREEN
                +indata[0]*RGB2GRAY_BLUE)>>8;
    indata+=4;
  }
}
void BGRAtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  size>>=1;
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

void BGRAtoRGB(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgb<2,1,0,3, 0,1,2>(indata, outdata, size);
}
void BGRAtoBGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgb<2,1,0,3, 2,1,0>(indata, outdata, size);
}
void BGRAtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgba<2,1,0,3, 0,1,2,3>(indata, outdata, size);
}
void BGRAtoABGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgba<2,1,0,3, 3,2,1,0>(indata, outdata, size);
}


/* ABGR -> */
void ABGRtoY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  while(size--) {
    *outdata++=(indata[3]*RGB2GRAY_RED
                +indata[2]*RGB2GRAY_GREEN
                +indata[1]*RGB2GRAY_BLUE)>>8;
    indata+=4;
  }
}
void ABGRtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size) {
  size>>=1;
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
void ABGRtoRGB(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgb<3,2,1,0, 0,1,2>(indata, outdata, size);
}
void ABGRtoBGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgb<3,2,1,0, 2,1,0>(indata, outdata, size);
}
void ABGRtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgba<3,2,1,0, 0,1,2,3>(indata, outdata, size);
}
void ABGRtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  rgba_to_rgba<3,2,1,0, 2,1,0,3>(indata, outdata, size);
}


/* ARGB -> */
void ARGBtoY(const unsigned char*indata, unsigned char*outdata, size_t size){
  while(size--) {
    *outdata++=((RGB2GRAY_RED*indata[1]+
                 RGB2GRAY_GREEN*indata[2]+
                 RGB2GRAY_BLUE*indata[3]
                  )>>8);
    indata+=4;
  }
}
void ARGBtoUYVY(const unsigned char*indata, unsigned char*outdata, size_t size){
  size>>=1;
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
void ARGBtoRGB(const unsigned char*indata, unsigned char*outdata, size_t size){
  rgba_to_rgb<1,2,3,0, 0,1,2>(indata, outdata, size);
}
void ARGBtoBGR(const unsigned char*indata, unsigned char*outdata, size_t size){
  rgba_to_rgb<1,2,3,0, 2,1,0>(indata, outdata, size);
}
void ARGBtoRGBA(const unsigned char*indata, unsigned char*outdata, size_t size){
  rgba_to_rgba<1,2,3,0, 0,1,2,3>(indata, outdata, size);
}
void ARGBtoBGRA(const unsigned char*indata, unsigned char*outdata, size_t size){
  rgba_to_rgba<1,2,3,0, 2,1,0,3>(indata, outdata, size);
}


/* RGB16 -> */
/*
  RGB16 packs R,G,B into 16bits (2bytes)
   B B B B B G G G   G G G R R R R R
   R R R R R G G G   G G G B B B B B
*/

void RGB16toY(const unsigned char*indata_, unsigned char*outdata, size_t size) {
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
void RGB16toUYVY(const unsigned char*indata_, unsigned char*outdata, size_t size) {
  const unsigned short*indata = (const unsigned short*)indata_;
  size>>=1;
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
static void RGB16_to_rgb(const unsigned char*indata_, unsigned char*outdata, size_t size) {
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
static void RGB16_to_rgba(const unsigned char*indata_, unsigned char*outdata, size_t size) {
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
void RGB16toRGB(const unsigned char*indata, unsigned char*outdata, size_t size) {
  RGB16_to_rgb<0,1,2>(indata, outdata, size);
}
void RGB16toBGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  RGB16_to_rgb<2,1,0>(indata, outdata, size);
}
void RGB16toRGBA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  RGB16_to_rgba<0,1,2,3>(indata, outdata, size);
}
void RGB16toBGRA(const unsigned char*indata, unsigned char*outdata, size_t size) {
  RGB16_to_rgba<2,1,0,3>(indata, outdata, size);
}
void RGB16toABGR(const unsigned char*indata, unsigned char*outdata, size_t size) {
  RGB16_to_rgba<3,2,1,0>(indata, outdata, size);
}
