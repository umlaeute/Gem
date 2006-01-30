////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/config.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef __WIN32__
# pragma warning( disable : 4244 )
# pragma warning( disable : 4305 )
# pragma warning( disable : 4091 )
#endif

#include "m_pd.h"
#include "GemPixUtil.h"

#include <string.h>
#include <ctype.h>


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


pixBlock :: pixBlock()
  : newimage(0), newfilm(0)
{}


imageStruct :: imageStruct() 
  : xsize (0),ysize(0),csize(0),
#ifdef GL_UNSIGNED_SHORT_8_8_REV_APPLE
    // or should type be GL_UNSIGNED_INT_8_8_8_8_REV ? i don't know: jmz
    type(GL_UNSIGNED_SHORT_8_8_REV_APPLE), format(GL_YCBCR_422_GEM),
#else
    type(GL_UNSIGNED_BYTE), format(GL_RGBA),
#endif
    notowned(0),data(NULL),pdata(NULL),datasize(0),
#ifdef __APPLE__
    upsidedown(1)
#else
  upsidedown(0)
#endif
{}

imageStruct :: ~imageStruct()
{
  clear();
}


GEM_EXTERN unsigned char* imageStruct::allocate(size_t size) 
{
  if (pdata){
    delete [] pdata;
    pdata=NULL;
  }
  data = pdata =  new unsigned char [size];

  datasize=size;
  notowned=0;
  return data; 
}

GEM_EXTERN unsigned char* imageStruct::allocate() 
{
	return allocate(xsize*ysize*csize);  
}

GEM_EXTERN unsigned char* imageStruct::reallocate(size_t size)
{
  if (size>datasize){
      return allocate(size);
  }
    return data;
}
GEM_EXTERN unsigned char* imageStruct::reallocate() 
{  
	return reallocate(xsize*ysize*csize);  
}
 
GEM_EXTERN void imageStruct::clear() 
{
  if (pdata) { // pdata is always owned by imageStruct
    delete [] pdata;
  }
  data = pdata = NULL;      
  datasize=0;
}


GEM_EXTERN void imageStruct::copy2ImageStruct(imageStruct *to){
    if (!to || !this || !this->data) {
      error("GEM: Someone sent a bogus pointer to copy2ImageStruct");
      if (to) to->data = NULL;
      return;
    }

    // copy the information over
    to->xsize 	= xsize;
    to->ysize 	= ysize;
    to->csize 	= csize;
    to->format 	= format;
    to->type 	= type;
    to->data    = data;
    /* from SIMD-branch: datasize refers to the private pdata
     * thus we shouldn't set it to something else, in order to not break
     * reallocate() and friends...
     */
    //to->datasize= datasize;
    to->upsidedown=upsidedown;
    to->notowned= true; /* but pdata is always owned by us */
}
GEM_EXTERN void imageStruct::info() {
  post("imageStruct\t:%dx%dx%d\n\t\t%X\t(%x) %d\n\t\t%x\t%x\t%d",
       xsize, ysize, csize,
       data, pdata, datasize,
       format, type, notowned);
}

GEM_EXTERN void imageStruct::copy2Image(imageStruct *to) {
    if (!to || !this || !this->data)
    {
        error("GEM: Someone sent a bogus pointer to copy2Image");
        if (to)
	  to->data = NULL;
        return;
    }

    /* copy without new allocation if possible (speedup in convolve ..) */
    to->xsize 	= xsize;
    to->ysize 	= ysize;
    to->csize 	= csize;
    to->format 	= format;
    to->type 	= type;
    to->reallocate();
    to->upsidedown 	= upsidedown;

    memcpy(to->data, data, xsize*ysize*csize);
}

GEM_EXTERN void imageStruct::refreshImage(imageStruct *to) {
    if (!to || !data)
    {
        error("GEM: Someone sent a bogus pointer to refreshImage");
        return;
    }

    // check if we need to reallocate memory
    if (to->xsize != xsize ||
        to->ysize != ysize ||
        to->csize != csize ||
		!to->data)
    {
        to->clear();
        copy2Image(to);
    }
    else
        // copy the data over
        memcpy(to->data, this->data, to->xsize * to->ysize * to->csize);
}


GEM_EXTERN int imageStruct::setCsizeByFormat(int setformat) {
  switch(setformat){
  case GL_LUMINANCE:  format=GL_LUMINANCE;  csize=1; type=GL_UNSIGNED_BYTE; break;
#ifndef __APPLE__
  case GL_RGB: format=GL_RGB;
#else
  case GL_BGR_EXT: format=GL_BGR_EXT;
#endif
    type=GL_UNSIGNED_BYTE; csize =3; break;
  case GL_YUV422_GEM:
#ifdef __APPLE__
  default:
    type=GL_UNSIGNED_SHORT_8_8_REV_APPLE;
#else
    type=GL_UNSIGNED_BYTE;
#endif
    format=GL_YUV422_GEM; csize=2;
    break;
#ifndef __APPLE__
  default:
  case GL_RGBA:       format=GL_RGBA;     type=GL_UNSIGNED_BYTE;
#else
  case GL_BGRA_EXT:   format=GL_BGRA_EXT; type = GL_UNSIGNED_INT_8_8_8_8_REV;
#endif
    csize=4; break;
  }
  return csize;
}
GEM_EXTERN int imageStruct::setCsizeByFormat() {
  return setCsizeByFormat(format);
}

void pix_addsat(unsigned char *leftPix, unsigned char *rightPix, size_t datasize)
{
  while(datasize--)
    {           
      *leftPix = CLAMP_HIGH((int)*leftPix + (int)*rightPix);
      leftPix++;
      rightPix++;
    }
}


void pix_sub(unsigned char *leftPix, unsigned char *rightPix, size_t datasize)
{
  while(datasize--){
    *leftPix = CLAMP_LOW((int)*leftPix - (int)*rightPix++);
    leftPix++;
  }
}

GEM_EXTERN void imageStruct::setBlack() {
  size_t i = datasize;
  unsigned char* dummy=pdata;
  switch (format){
  case GL_YCBCR_422_GEM:
    i/=4;
    while(i--){
      *dummy++=128;*dummy++=0;
      *dummy++=128;*dummy++=0;
    }
    break;
  default:
    memset(pdata, 0, datasize);
    break;
  }
}
GEM_EXTERN void imageStruct::setWhite() {
  size_t i = datasize;
  unsigned char* dummy=pdata;
  switch (format){
  case GL_YCBCR_422_GEM:
    i/=4;
    while(i--){
      *dummy++=128;*dummy++=255;
      *dummy++=128;*dummy++=255;
    }
    break;
  default:
    memset(pdata, 1, datasize);
    break;
  }
}

GEM_EXTERN void imageStruct::fromRGB(unsigned char *rgbdata) {
  if(!rgbdata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_RGB:
    memcpy(data, rgbdata, pixelnum*csize);
    break;
  case GL_BGR_EXT:
    while(pixelnum--){
      pixels[0]=rgbdata[2];
      pixels[1]=rgbdata[1];
      pixels[2]=rgbdata[0];
      pixels+=3; rgbdata+=3;
    }
    break;
  case GL_RGBA:
    while(pixelnum--){
      *pixels++=*rgbdata++;
      *pixels++=*rgbdata++;
      *pixels++=*rgbdata++;
      *pixels++=255;
    }
    break;
  case GL_BGRA_EXT:
    while(pixelnum--){
      pixels[0]=rgbdata[2];
      pixels[1]=rgbdata[1];
      pixels[2]=rgbdata[0];
      pixels[3]=255;
      pixels+=4;rgbdata+=3;
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(rgbdata[0] * 0.3086f + rgbdata[1] * 0.06094f + rgbdata[2] * 0.0820f);
      *pixels++=(rgbdata[0]*79+rgbdata[1]*156+rgbdata[2]*21)>>8;
      rgbdata+=3;
    }
    break;
  case GL_YUV422_GEM:
#if 0
	RGB_to_YCbCr_altivec(rgbdata, pixelnum, pixels);
#else
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*rgbdata[0]+RGB2YUV_22*rgbdata[1]+RGB2YUV_23*rgbdata[2])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*rgbdata[0]+RGB2YUV_12*rgbdata[1]+RGB2YUV_13*rgbdata[2])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*rgbdata[0]+RGB2YUV_32*rgbdata[1]+RGB2YUV_33*rgbdata[2])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*rgbdata[3]+RGB2YUV_12*rgbdata[4]+RGB2YUV_13*rgbdata[5])>>8)+ Y_OFFSET; // Y
      rgbdata+=6;
    }
#endif
    break;
  }
}
#ifdef __VEC__
GEM_EXTERN void imageStruct :: RGB_to_YCbCr_altivec(unsigned char *rgbdata, size_t RGB_size, 
                  unsigned char *pixels)
{
  vector signed short  r0, r1, r2, g0, g1, g2, b0, b1, b2, c0, c16, c128;
  vector unsigned char z0, tc0, tc1, tc2, tc3;
  vector signed short tr0, tr1, tg0, tg1, tb0, tb1;
  vector signed short t0, t1, t2, t3, t4, t5;
  unsigned int i;
  
  vector unsigned char	*RGB_ptr = (vector unsigned char*) rgbdata;
  vector unsigned char	*YCC_ptr = (vector unsigned char*) pixels;

  /* Permutation vector is used to extract the interleaved RGB. */
  vector unsigned char vPerm1 =
    (vector unsigned char)( 0,  3,  6,  9, 12, 15, 18, 21, /* R0..R7    */
                            1,  4,  7, 10, 13, 16, 19, 22  /* G0..G7    */);
  vector unsigned char vPerm2 =
    (vector unsigned char)( 2,  5,  8, 11, 14, 17, 20, 23, /* B0..B7    */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);
  vector unsigned char vPerm3 =
    (vector unsigned char)( 8, 11, 14, 17, 20, 23, 26, 29, /* R8..R15   */
                            9, 12, 15, 18, 21, 24, 27, 30  /* G8..G15   */);
  vector unsigned char vPerm4 =
    (vector unsigned char)(10, 13, 16, 19, 22, 25, 28, 31, /* B8..B15   */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);

  /* Load the equation constants. */
  vector signed short vConst1 =
    (vector signed short)( 8432,  16425,  3176,
                          -4818,  -9527, 14345,
                              0,      0 );

  vector signed short vConst2 =
    (vector signed short)( 14345, -12045, -2300,
                              16, 128, 0, 0, 0 );

  r0 = vec_splat( vConst1, 0 ); /*  8432 */
  g0 = vec_splat( vConst1, 1 ); /* 16425 */
  b0 = vec_splat( vConst1, 2 ); /*  3176 */
  r1 = vec_splat( vConst1, 3 ); /* -4818 */
  g1 = vec_splat( vConst1, 4 ); /* -9527 */
  b1 = vec_splat( vConst1, 5 ); /* 14345 */
  r2 = vec_splat( vConst2, 0 ); /* 14345 */
  g2 = vec_splat( vConst2, 1 ); /*-12045 */
  b2 = vec_splat( vConst2, 2 ); /* -2300 */
  c16  = vec_splat( vConst2, 3 ); /*  16 */
  c128 = vec_splat( vConst2, 4 ); /* 128 */
  c0 = (vector signed short) (0); /*   0 */
  z0 = (vector unsigned char) (0); /*  0 */

  for ( i = 0; i < (RGB_size/sizeof(vector unsigned char)); i+=3 ) {

    /* Load the 3 RGB input vectors and seperate into red,
       green and blue from the interleaved format. */
    tc0 = vec_perm( RGB_ptr[i], RGB_ptr[i+1], vPerm1 );   /* R0..R7  G0..G7  */
    tc1 = vec_perm( RGB_ptr[i], RGB_ptr[i+1], vPerm2 );   /* B0..B7          */
    tc2 = vec_perm( RGB_ptr[i+1], RGB_ptr[i+2], vPerm3 ); /* R8..R15 G8..G15 */
    tc3 = vec_perm( RGB_ptr[i+1], RGB_ptr[i+2], vPerm4 ); /* B8..B15         */

    /* Unpack to 16 bit arithmatic for converstion. */
    tr0 = (vector signed short) vec_mergeh( z0, tc0 );  // tr0 = R0 .. R7
    tg0 = (vector signed short) vec_mergel( z0, tc0 );  // tg0 = G0 .. G7
    tb0 = (vector signed short) vec_mergeh( z0, tc1 );  // tb0 = B0 .. B7
    tr1 = (vector signed short) vec_mergeh( z0, tc2 );  // tr0 = R8 .. R15
    tg1 = (vector signed short) vec_mergel( z0, tc2 );  // tg0 = G8 .. G15
    tb1 = (vector signed short) vec_mergeh( z0, tc3 );  // tb0 = B8 .. B15

    /* Convert the first three input vectors.  Note that
       only the top 17 bits of the 32 bit product are
       stored.  This is the same as doing the divide by 32768. */

    t0 = vec_mradds( tr0, r0, c0 ); /* (R0 .. R7) *  8432 */
    t1 = vec_mradds( tr0, r1, c0 ); /* (R0 .. R7) * -4818 */
    t2 = vec_mradds( tr0, r2, c0 ); /* (R0 .. R7) * 14345 */

    t0 = vec_mradds( tg0, g0, t0 ); /* += (G0 .. G7) *  16425 */
    t1 = vec_mradds( tg0, g1, t1 ); /* += (G0 .. G7) *  -9527 */
    t2 = vec_mradds( tg0, g2, t2 ); /* += (G0 .. G7) * -12045 */

    t0 = vec_mradds( tb0, b0, t0 ); /* += (B0 .. B7) *  3176 */
    t1 = vec_mradds( tb0, b1, t1 ); /* += (B0 .. B7) * 14345 */
    t2 = vec_mradds( tb0, b2, t2 ); /* += (B0 .. B7) * -2300 */

    /* Convert the next three input vectors. */
    t3 = vec_mradds( tr1, r0, c0 ); /* (R8 .. R15) *  8432 */
    t4 = vec_mradds( tr1, r1, c0 ); /* (R8 .. R15) * -4818 */
    t5 = vec_mradds( tr1, r2, c0 ); /* (R8 .. R15) * 14345 */

    t3 = vec_mradds( tg1, g0, t3 ); /* += (G8 .. G15) *  16425 */
    t4 = vec_mradds( tg1, g1, t4 ); /* += (G8 .. G15) *  -9527 */
    t5 = vec_mradds( tg1, g2, t5 ); /* += (G8 .. G15) * -12045 */

    t3 = vec_mradds( tb1, b0, t3 ); /* += (B8 .. B15) *  3176 */
    t4 = vec_mradds( tb1, b1, t4 ); /* += (B8 .. B15) * 14345 */
    t5 = vec_mradds( tb1, b2, t5 ); /* += (B8 .. B15) * -2300 */

    /* Add the constants. */
    t0 = vec_adds( t0, c16 );
    t3 = vec_adds( t3, c16 );
    t1 = vec_adds( t1, c128 );
    t4 = vec_adds( t4, c128 );
    t2 = vec_adds( t2, c128 );
    t5 = vec_adds( t5, c128 );

    /* Pack the results, and store them. */
    YCC_ptr[i]   = vec_packsu( t0, t3 );  /*  Y0 .. Y15  */
    YCC_ptr[i+1] = vec_packsu( t1, t4 );  /* Cb0 .. Cb15 */
    YCC_ptr[i+2] = vec_packsu( t2, t5 );  /* Cr0 .. Cr15 */

    }
}
#endif /* __vec__ */

GEM_EXTERN void imageStruct::fromRGB16(unsigned char *rgb16data) {
  //   B B B B B G G G   G G G R R R R R
  //   R R R R R G G G   G G G B B B B B
  if(!rgb16data)return;
  unsigned short*rgbdata=(unsigned short*)rgb16data;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  unsigned short rgb;
  switch (format){
  case GL_RGBA:
    while(pixelnum--){
      rgb=*rgbdata++;
      pixels[0]=((rgb>>8)&0xF8);
      pixels[1]=((rgb>>3)&0xFC);
      pixels[2]=((rgb<<3)&0xF8);
      pixels[3]=255;
      pixels+=4;
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      rgb=*rgbdata++;
      *pixels++=(((rgb>>8)&0xF8)*79+((rgb>>3)&0xFC)*156+((rgb<<3)&0xF8)*21)>>8;
    }
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      rgb=*rgbdata++;
      unsigned char r=((rgb>>8)&0xF8);
      unsigned char g=((rgb>>3)&0xFC);
      unsigned char b=((rgb<<3)&0xF8);
      *pixels++=((RGB2YUV_21*r+RGB2YUV_22*g+RGB2YUV_23*b)>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET;  // Y
      *pixels++=((RGB2YUV_31*r+RGB2YUV_32*g+RGB2YUV_33*b)>>8)+UV_OFFSET; // V

      rgb=*rgbdata++;
      r=((rgb>>8)&0xF8);
      g=((rgb>>3)&0xFC);
      b=((rgb<<3)&0xF8);
      *pixels++=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET;     // Y
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromRGBA(unsigned char *rgbadata) {
  if(!rgbadata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_RGB:
    while(pixelnum--){
      *pixels++=*rgbadata++;
      *pixels++=*rgbadata++;
      *pixels++=*rgbadata++;
      rgbadata++;
    }
    break;
  case GL_BGR_EXT:
    while(pixelnum--){
      pixels[0]=rgbadata[2];
      pixels[1]=rgbadata[1];
      pixels[2]=rgbadata[0];
      pixels+=3; rgbadata+=4;
    }
    break;
  case GL_RGBA:
    memcpy(data, rgbadata, pixelnum*csize);
    break;
  case GL_BGRA_EXT:
    if(pixels==rgbadata){
      unsigned char dummy=0;
      while(pixelnum--){
	dummy=pixels[2];
	pixels[2]=pixels[0];
	pixels[0]=dummy;
	pixels+=4;
      }
    } else {
      while(pixelnum--){
	pixels[0]=rgbadata[2];
	pixels[1]=rgbadata[1];
	pixels[2]=rgbadata[0];
	pixels[3]=rgbadata[3];
	pixels+=4;rgbadata+=4;
      }
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(rgbadata[0] * 0.3086f + rgbadata[1] * 0.6094f + rgbadata[2] * 0.0820f);
      *pixels++=(rgbadata[0]*79+rgbadata[1]*156+rgbadata[2]*21)>>8;
      rgbadata+=4;
    }
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*rgbadata[chRed]+
		  RGB2YUV_22*rgbadata[chGreen]+
		  RGB2YUV_23*rgbadata[chBlue])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*rgbadata[chRed]+
		  RGB2YUV_12*rgbadata[chGreen]+
		  RGB2YUV_13*rgbadata[chBlue])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*rgbadata[chRed]+
		  RGB2YUV_32*rgbadata[chGreen]+
		  RGB2YUV_33*rgbadata[chBlue])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*rgbadata[4+chRed]+
		  RGB2YUV_12*rgbadata[4+chGreen]+
		  RGB2YUV_13*rgbadata[4+chBlue])>>8)+ Y_OFFSET; // Y
      rgbadata+=8;
    }
    break;
  }
}
#ifdef __VEC__
GEM_EXTERN void imageStruct :: RGBA_to_YCbCr_altivec(unsigned char *rgbadata, size_t RGBA_size, 
                  unsigned char *pixels)
{
  vector signed short  r0, r1, r2, g0, g1, g2, b0, b1, b2, c0, c16, c128;
  vector unsigned char z0, tc0, tc1, tc2, tc3;
  vector signed short tr0, tr1, tg0, tg1, tb0, tb1;
  vector signed short t0, t1, t2, t3, t4, t5;
  unsigned int i;
  
  vector unsigned char	*RGBA_ptr = (vector unsigned char*) rgbadata;
  vector unsigned char	*YCC_ptr = (vector unsigned char*) pixels;

  /* Permutation vector is used to extract the interleaved RGBA. */
  vector unsigned char vPerm1 =
    (vector unsigned char)( 0,  4,  8, 12, 16, 20, 24, 28, /* R0..R7    */
                            1,  5,  9, 13, 17, 21, 25, 29  /* G0..G7    */);
  vector unsigned char vPerm2 =
    (vector unsigned char)( 2,  6, 10, 14, 18, 22, 26, 30, /* B0..B7    */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);
  vector unsigned char vPerm3 =
    (vector unsigned char)( 8, 12, 16, 20, 24, 28, 32, 36, /* R8..R15   */
                            9, 13, 17, 21, 25, 29, 33, 37  /* G8..G15   */);
  vector unsigned char vPerm4 =
    (vector unsigned char)(10, 14, 18, 22, 26, 30, 34, 38, /* B8..B15   */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);

  /* Load the equation constants. */
  vector signed short vConst1 =
    (vector signed short)( 8432,  16425,  3176,
                          -4818,  -9527, 14345,
                              0,      0 );

  vector signed short vConst2 =
    (vector signed short)( 14345, -12045, -2300,
                              16, 128, 0, 0, 0 );

  r0 = vec_splat( vConst1, 0 ); /*  8432 */
  g0 = vec_splat( vConst1, 1 ); /* 16425 */
  b0 = vec_splat( vConst1, 2 ); /*  3176 */
  r1 = vec_splat( vConst1, 3 ); /* -4818 */
  g1 = vec_splat( vConst1, 4 ); /* -9527 */
  b1 = vec_splat( vConst1, 5 ); /* 14345 */
  r2 = vec_splat( vConst2, 0 ); /* 14345 */
  g2 = vec_splat( vConst2, 1 ); /*-12045 */
  b2 = vec_splat( vConst2, 2 ); /* -2300 */
  c16  = vec_splat( vConst2, 3 ); /*  16 */
  c128 = vec_splat( vConst2, 4 ); /* 128 */
  c0 = (vector signed short) (0); /*   0 */
  z0 = (vector unsigned char) (0); /*  0 */

  for ( i = 0; i < (RGBA_size/sizeof(vector unsigned char)); i+=3 ) {

    /* Load the 3 RGB input vectors and seperate into red,
       green and blue from the interleaved format. */
    tc0 = vec_perm( RGBA_ptr[i], RGBA_ptr[i+1], vPerm1 );   /* R0..R7  G0..G7  */
    tc1 = vec_perm( RGBA_ptr[i], RGBA_ptr[i+1], vPerm2 );   /* B0..B7          */
    tc2 = vec_perm( RGBA_ptr[i+1], RGBA_ptr[i+2], vPerm3 ); /* R8..R15 G8..G15 */
    tc3 = vec_perm( RGBA_ptr[i+1], RGBA_ptr[i+2], vPerm4 ); /* B8..B15         */

    /* Unpack to 16 bit arithmatic for converstion. */
    tr0 = (vector signed short) vec_mergeh( z0, tc0 );  /* tr0 = R0 .. R7  */
    tg0 = (vector signed short) vec_mergel( z0, tc0 );  /* tg0 = G0 .. G7  */
    tb0 = (vector signed short) vec_mergeh( z0, tc1 );  /* tb0 = B0 .. B7  */
    tr1 = (vector signed short) vec_mergeh( z0, tc2 );  /* tr0 = R8 .. R15 */
    tg1 = (vector signed short) vec_mergel( z0, tc2 );  /* tg0 = G8 .. G15 */
    tb1 = (vector signed short) vec_mergeh( z0, tc3 );  /* tb0 = B8 .. B15 */

    /* Convert the first three input vectors.  Note that
       only the top 17 bits of the 32 bit product are
       stored.  This is the same as doing the divide by 32768. */

    t0 = vec_mradds( tr0, r0, c0 ); /* (R0 .. R7) *  8432 */
    t1 = vec_mradds( tr0, r1, c0 ); /* (R0 .. R7) * -4818 */
    t2 = vec_mradds( tr0, r2, c0 ); /* (R0 .. R7) * 14345 */

    t0 = vec_mradds( tg0, g0, t0 ); /* += (G0 .. G7) *  16425 */
    t1 = vec_mradds( tg0, g1, t1 ); /* += (G0 .. G7) *  -9527 */
    t2 = vec_mradds( tg0, g2, t2 ); /* += (G0 .. G7) * -12045 */

    t0 = vec_mradds( tb0, b0, t0 ); /* += (B0 .. B7) *  3176 */
    t1 = vec_mradds( tb0, b1, t1 ); /* += (B0 .. B7) * 14345 */
    t2 = vec_mradds( tb0, b2, t2 ); /* += (B0 .. B7) * -2300 */

    /* Convert the next three input vectors. */
    t3 = vec_mradds( tr1, r0, c0 ); /* (R8 .. R15) *  8432 */
    t4 = vec_mradds( tr1, r1, c0 ); /* (R8 .. R15) * -4818 */
    t5 = vec_mradds( tr1, r2, c0 ); /* (R8 .. R15) * 14345 */

    t3 = vec_mradds( tg1, g0, t3 ); /* += (G8 .. G15) *  16425 */
    t4 = vec_mradds( tg1, g1, t4 ); /* += (G8 .. G15) *  -9527 */
    t5 = vec_mradds( tg1, g2, t5 ); /* += (G8 .. G15) * -12045 */

    t3 = vec_mradds( tb1, b0, t3 ); /* += (B8 .. B15) *  3176 */
    t4 = vec_mradds( tb1, b1, t4 ); /* += (B8 .. B15) * 14345 */
    t5 = vec_mradds( tb1, b2, t5 ); /* += (B8 .. B15) * -2300 */

    /* Add the constants. */
    t0 = vec_adds( t0, c16 );
    t3 = vec_adds( t3, c16 );
    t1 = vec_adds( t1, c128 );
    t4 = vec_adds( t4, c128 );
    t2 = vec_adds( t2, c128 );
    t5 = vec_adds( t5, c128 );

    /* Pack the results, and store them. */
    YCC_ptr[i]   = vec_packsu( t0, t3 );  /*  Y0 .. Y15  */
    YCC_ptr[i+1] = vec_packsu( t1, t4 );  /* Cb0 .. Cb15 */
    YCC_ptr[i+2] = vec_packsu( t2, t5 );  /* Cr0 .. Cr15 */

    }
}
#endif

GEM_EXTERN void imageStruct::fromBGR(unsigned char *bgrdata) {
  if(!bgrdata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_BGR_EXT:
    memcpy(data, bgrdata, pixelnum*csize);
    break;
  case GL_RGB:
    while(pixelnum--){
      pixels[0]=bgrdata[2];
      pixels[1]=bgrdata[1];
      pixels[2]=bgrdata[0];
      pixels+=3; bgrdata+=3;
    }
    break;
  case GL_BGRA_EXT:
    while(pixelnum--){
      *pixels++=*bgrdata++;
      *pixels++=*bgrdata++;
      *pixels++=*bgrdata++;
      *pixels++=255;
    }
    break;
  case GL_RGBA:
    while(pixelnum--){
      pixels[0]=bgrdata[2];
      pixels[1]=bgrdata[1];
      pixels[2]=bgrdata[0];
      pixels[3]=255;
      pixels+=4;bgrdata+=3;
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(bgrdata[2] * 0.3086f + bgrdata[1] * 0.06094f + bgrdata[0] * 0.0820f);
      *pixels++=(bgrdata[2]*79+bgrdata[1]*156+bgrdata[0]*21)>>8;
      bgrdata+=3;
    }
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*bgrdata[2]+RGB2YUV_22*bgrdata[1]+RGB2YUV_23*bgrdata[0])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*bgrdata[2]+RGB2YUV_12*bgrdata[1]+RGB2YUV_13*bgrdata[0])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*bgrdata[2]+RGB2YUV_32*bgrdata[1]+RGB2YUV_33*bgrdata[0])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*bgrdata[5]+RGB2YUV_12*bgrdata[4]+RGB2YUV_13*bgrdata[3])>>8)+ Y_OFFSET; // Y
      bgrdata+=6;
    }
    break;
  }
}

#ifdef __VEC__
GEM_EXTERN void imageStruct :: BGR_to_YCbCr_altivec(unsigned char *bgrdata, size_t BGR_size, 
                  unsigned char *pixels)
{
  vector signed short  r0, r1, r2, g0, g1, g2, b0, b1, b2, c0, c16, c128;
  vector unsigned char z0, tc0, tc1, tc2, tc3;
  vector signed short tr0, tr1, tg0, tg1, tb0, tb1;
  vector signed short t0, t1, t2, t3, t4, t5;
  unsigned int i;
  
  vector unsigned char	*BGR_ptr = (vector unsigned char*) bgrdata;
  vector unsigned char	*YCC_ptr = (vector unsigned char*) pixels;

  /* Permutation vector is used to extract the interleaved RGB. */
  vector unsigned char vPerm1 =
    (vector unsigned char)( 0,  3,  6,  9, 12, 15, 18, 21, /* R0..R7    */
                            1,  4,  7, 10, 13, 16, 19, 22  /* G0..G7    */);
  vector unsigned char vPerm2 =
    (vector unsigned char)( 2,  5,  8, 11, 14, 17, 20, 23, /* B0..B7    */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);
  vector unsigned char vPerm3 =
    (vector unsigned char)( 8, 11, 14, 17, 20, 23, 26, 29, /* R8..R15   */
                            9, 12, 15, 18, 21, 24, 27, 30  /* G8..G15   */);
  vector unsigned char vPerm4 =
    (vector unsigned char)(10, 13, 16, 19, 22, 25, 28, 31, /* B8..B15   */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);

  /* Load the equation constants. */
  vector signed short vConst1 =
    (vector signed short)( 8432,  16425,  3176,
                          -4818,  -9527, 14345,
                              0,      0 );

  vector signed short vConst2 =
    (vector signed short)( 14345, -12045, -2300,
                              16, 128, 0, 0, 0 );

  r0 = vec_splat( vConst1, 0 ); /*  8432 */
  g0 = vec_splat( vConst1, 1 ); /* 16425 */
  b0 = vec_splat( vConst1, 2 ); /*  3176 */
  r1 = vec_splat( vConst1, 3 ); /* -4818 */
  g1 = vec_splat( vConst1, 4 ); /* -9527 */
  b1 = vec_splat( vConst1, 5 ); /* 14345 */
  r2 = vec_splat( vConst2, 0 ); /* 14345 */
  g2 = vec_splat( vConst2, 1 ); /*-12045 */
  b2 = vec_splat( vConst2, 2 ); /* -2300 */
  c16  = vec_splat( vConst2, 3 ); /*  16 */
  c128 = vec_splat( vConst2, 4 ); /* 128 */
  c0 = (vector signed short) (0); /*   0 */
  z0 = (vector unsigned char) (0); /*  0 */

  for ( i = 0; i < (BGR_size/sizeof(vector unsigned char)); i+=3 ) {

    /* Load the 3 RGB input vectors and seperate into red,
       green and blue from the interleaved format. */
    tc0 = vec_perm( BGR_ptr[i], BGR_ptr[i+1], vPerm1 );   /* R0..R7  G0..G7  */
    tc1 = vec_perm( BGR_ptr[i], BGR_ptr[i+1], vPerm2 );   /* B0..B7          */
    tc2 = vec_perm( BGR_ptr[i+1], BGR_ptr[i+2], vPerm3 ); /* R8..R15 G8..G15 */
    tc3 = vec_perm( BGR_ptr[i+1], BGR_ptr[i+2], vPerm4 ); /* B8..B15         */

    /* Unpack to 16 bit arithmatic for converstion. */
    tr0 = (vector signed short) vec_mergeh( z0, tc0 );  /* tr0 = R0 .. R7  */
    tg0 = (vector signed short) vec_mergel( z0, tc0 );  /* tg0 = G0 .. G7  */
    tb0 = (vector signed short) vec_mergeh( z0, tc1 );  /* tb0 = B0 .. B7  */
    tr1 = (vector signed short) vec_mergeh( z0, tc2 );  /* tr0 = R8 .. R15 */
    tg1 = (vector signed short) vec_mergel( z0, tc2 );  /* tg0 = G8 .. G15 */
    tb1 = (vector signed short) vec_mergeh( z0, tc3 );  /* tb0 = B8 .. B15 */

    /* Convert the first three input vectors.  Note that
       only the top 17 bits of the 32 bit product are
       stored.  This is the same as doing the divide by 32768. */

    t0 = vec_mradds( tr0, r0, c0 ); /* (R0 .. R7) *  8432 */
    t1 = vec_mradds( tr0, r1, c0 ); /* (R0 .. R7) * -4818 */
    t2 = vec_mradds( tr0, r2, c0 ); /* (R0 .. R7) * 14345 */

    t0 = vec_mradds( tg0, g0, t0 ); /* += (G0 .. G7) *  16425 */
    t1 = vec_mradds( tg0, g1, t1 ); /* += (G0 .. G7) *  -9527 */
    t2 = vec_mradds( tg0, g2, t2 ); /* += (G0 .. G7) * -12045 */

    t0 = vec_mradds( tb0, b0, t0 ); /* += (B0 .. B7) *  3176 */
    t1 = vec_mradds( tb0, b1, t1 ); /* += (B0 .. B7) * 14345 */
    t2 = vec_mradds( tb0, b2, t2 ); /* += (B0 .. B7) * -2300 */

    /* Convert the next three input vectors. */
    t3 = vec_mradds( tr1, r0, c0 ); /* (R8 .. R15) *  8432 */
    t4 = vec_mradds( tr1, r1, c0 ); /* (R8 .. R15) * -4818 */
    t5 = vec_mradds( tr1, r2, c0 ); /* (R8 .. R15) * 14345 */

    t3 = vec_mradds( tg1, g0, t3 ); /* += (G8 .. G15) *  16425 */
    t4 = vec_mradds( tg1, g1, t4 ); /* += (G8 .. G15) *  -9527 */
    t5 = vec_mradds( tg1, g2, t5 ); /* += (G8 .. G15) * -12045 */

    t3 = vec_mradds( tb1, b0, t3 ); /* += (B8 .. B15) *  3176 */
    t4 = vec_mradds( tb1, b1, t4 ); /* += (B8 .. B15) * 14345 */
    t5 = vec_mradds( tb1, b2, t5 ); /* += (B8 .. B15) * -2300 */

    /* Add the constants. */
    t0 = vec_adds( t0, c16 );
    t3 = vec_adds( t3, c16 );
    t1 = vec_adds( t1, c128 );
    t4 = vec_adds( t4, c128 );
    t2 = vec_adds( t2, c128 );
    t5 = vec_adds( t5, c128 );

    /* Pack the results, and store them. */
    YCC_ptr[i]   = vec_packsu( t0, t3 );  /*  Y0 .. Y15  */
    YCC_ptr[i+1] = vec_packsu( t1, t4 );  /* Cb0 .. Cb15 */
    YCC_ptr[i+2] = vec_packsu( t2, t5 );  /* Cr0 .. Cr15 */

    }
}
#endif

GEM_EXTERN void imageStruct::fromBGRA(unsigned char *bgradata) {
  if(!bgradata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_BGR_EXT:
    while(pixelnum--){
      *pixels++=*bgradata++;
      *pixels++=*bgradata++;
      *pixels++=*bgradata++;
      bgradata++;
    }
    break;
  case GL_RGB:
    while(pixelnum--){
      pixels[0]=bgradata[2];
      pixels[1]=bgradata[1];
      pixels[2]=bgradata[0];
      pixels+=3; bgradata+=4;
    }
    break;
  case GL_BGRA_EXT:
    memcpy(data, bgradata, pixelnum*csize);
    break;
  case GL_RGBA:
    if(bgradata==data){
      // in place conversion
      unsigned char dummy=0;
      while(pixelnum--){
	dummy    =pixels[2];
	pixels[2]=pixels[0];
	pixels[0]=dummy;
	pixels+=4;
      } 
    } else {
      while(pixelnum--){
	pixels[0]=bgradata[2];
	pixels[1]=bgradata[1];
	pixels[2]=bgradata[0];
	pixels[3]=bgradata[3];
	pixels+=4;bgradata+=4;
      }
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(bgradata[2] * 0.3086f + bgradata[1] * 0.06094f + bgradata[0] * 0.0820f);
      *pixels++=(bgradata[2]*79+bgradata[1]*156+bgradata[0]*21)>>8;

      bgradata+=4;
    }
    break;
  case GL_YUV422_GEM:
#ifdef __TIMING__
	  UnsignedWide start, end;
	  Microseconds(&start);
#endif
#ifdef __VEC__
	BGRA_to_YCbCr_altivec(bgradata,pixelnum,pixels);
#else
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*bgradata[chRed]+
		  RGB2YUV_22*bgradata[chGreen]+
		  RGB2YUV_23*bgradata[chBlue])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*bgradata[chRed]+
		  RGB2YUV_12*bgradata[chGreen]+
		  RGB2YUV_13*bgradata[chBlue])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*bgradata[chRed]+
		  RGB2YUV_32*bgradata[chGreen]+
		  RGB2YUV_33*bgradata[chBlue])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*bgradata[4+chRed]+
		  RGB2YUV_12*bgradata[4+chGreen]+
		  RGB2YUV_13*bgradata[4+chBlue])>>8)+ Y_OFFSET; // Y
      bgradata+=8;
    }
#endif
#ifdef __TIMING__
	  Microseconds(&end);
	  float seconds = (float)(end.lo - start.lo) / 1000000.f;
	  post("BGRAtoUYVY frame time = %f", seconds);
#endif
    break;
  }
}

#ifdef __VEC__
GEM_EXTERN void imageStruct :: BGRA_to_YCbCr_altivec(unsigned char *bgradata, size_t BGRA_size, 
                  unsigned char *pixels)
{
  vector signed short  r0, r1, r2, g0, g1, g2, b0, b1, b2, c0, c16, c128;
  vector unsigned char z0, tc0, tc1, tc2, tc3;
  vector signed short tr0, tr1, tg0, tg1, tb0, tb1;
  vector signed short t0, t1, t2, t3, t4, t5;
  vector signed short u1, u2, uAvg, v1, v2, vAvg, out1, out2, out3, out4;
  vector signed short temp1, temp2, temp3, temp4, uv1, uv2;
  int i;
  
  vector unsigned char	*BGRA_ptr = (vector unsigned char*) bgradata;
  vector unsigned char	*UYVY_ptr = (vector unsigned char*) pixels;

  /* Permutation vector is used to extract the interleaved BGRA. */
  vector unsigned char vPerm1 =
    (vector unsigned char)( 3,  7, 11, 15, 19, 23, 27, 31, // B0..B7    
                            2,  6, 10, 14, 18, 22, 26, 30  /* G0..G7    */);
  vector unsigned char vPerm2 =
    (vector unsigned char)( 1,  5,  9, 13, 17, 21, 25, 29, /* R0..R7    */
                            0,  0,  0,  0,  0,  0,  0,  0  /* dont care */);

  /* Load the equation constants. */
  vector signed short vConst1 =
    (vector signed short)( 8432,  16425,  3176,
                          -4818,  -9527, 14345,
                              0,      0 );
  vector signed short vConst2 =
    (vector signed short)( 14345, -12045, -2300,
                              16, 128, 0, 0, 0 );
							  
  vector unsigned char avgPerm1 =
    (vector unsigned char)(  0,  1,  4,  5,  8,  9, 12, 13,
	                        16, 17, 20, 21, 24, 25, 28, 29 );
  vector unsigned char avgPerm2 =
    (vector unsigned char)(  2,  3,  6,  7, 10, 11, 14, 15,
	                        18, 19, 22, 23, 26, 27, 30, 31 );
  vector unsigned char Perm1 = 
         (vector unsigned char)( 0, 1, 16, 17, 2, 3, 18, 19,
						         4, 5, 20, 21, 6, 7, 22, 23 );
  vector unsigned char Perm2 = 
         (vector unsigned char)(  8,  9, 24, 25, 10, 11, 26, 27,
						         12, 13, 28, 29, 14, 15, 30, 31 );
								 							
  r0 = vec_splat( vConst1, 2 ); /*  8432 */
  g0 = vec_splat( vConst1, 1 ); /* 16425 */
  b0 = vec_splat( vConst1, 0 ); /*  3176 */
  r1 = vec_splat( vConst1, 5 ); /* -4818 */
  g1 = vec_splat( vConst1, 4 ); /* -9527 */
  b1 = vec_splat( vConst1, 3 ); /* 14345 */
  r2 = vec_splat( vConst2, 2 ); /* 14345 */
  g2 = vec_splat( vConst2, 1 ); /*-12045 */
  b2 = vec_splat( vConst2, 0 ); /* -2300 */
  c16  = vec_splat( vConst2, 3 ); /*  16 */
  c128 = vec_splat( vConst2, 4 ); /* 128 */
  c0 = (vector signed short) (0); /*   0 */
  z0 = (vector unsigned char) (0); /*  0 */

  for ( i = 0; i < (BGRA_size/sizeof(vector unsigned char)); i++ ) {

    /* Load the 4 BGRA input vectors and seperate into red,
       green and blue from the interleaved format. */
	vector unsigned char *vec1 = BGRA_ptr++;
	vector unsigned char *vec2 = BGRA_ptr++;
	vector unsigned char *vec3 = BGRA_ptr++;
	vector unsigned char *vec4 = BGRA_ptr++;
	
	tc0 = vec_perm( *vec1, *vec2, vPerm1 ); // B0..B7  G0..G7
	tc1 = vec_perm( *vec1, *vec2, vPerm2 ); // R0..R7
	tc2 = vec_perm( *vec3, *vec4, vPerm1 ); // B8..B15 G8..G15
	tc3 = vec_perm( *vec3, *vec4, vPerm2 ); // R8..R15

    /* Unpack to 16 bit arithmatic for conversion. */
    tr0 = (vector signed short) vec_mergeh( z0, tc0 );  /* tr0 = R0 .. R7  */
    tg0 = (vector signed short) vec_mergel( z0, tc0 );  /* tg0 = G0 .. G7  */
    tb0 = (vector signed short) vec_mergeh( z0, tc1 );  /* tb0 = B0 .. B7  */
    tr1 = (vector signed short) vec_mergeh( z0, tc2 );  /* tr0 = R8 .. R15 */
    tg1 = (vector signed short) vec_mergel( z0, tc2 );  /* tg0 = G8 .. G15 */
    tb1 = (vector signed short) vec_mergeh( z0, tc3 );  /* tb0 = B8 .. B15 */

    /* Convert the first three input vectors.  Note that
       only the top 17 bits of the 32 bit product are
       stored.  This is the same as doing the divide by 32768. */

    t0 = vec_mradds( tr0, r0, c0 ); /* (R0 .. R7) *  8432 */
    t1 = vec_mradds( tr0, r1, c0 ); /* (R0 .. R7) * -4818 */
    t2 = vec_mradds( tr0, r2, c0 ); /* (R0 .. R7) * 14345 */

    t0 = vec_mradds( tg0, g0, t0 ); /* += (G0 .. G7) *  16425 */
    t1 = vec_mradds( tg0, g1, t1 ); /* += (G0 .. G7) *  -9527 */
    t2 = vec_mradds( tg0, g2, t2 ); /* += (G0 .. G7) * -12045 */

    t0 = vec_mradds( tb0, b0, t0 ); /* += (B0 .. B7) *  3176 */
    t1 = vec_mradds( tb0, b1, t1 ); /* += (B0 .. B7) * 14345 */
    t2 = vec_mradds( tb0, b2, t2 ); /* += (B0 .. B7) * -2300 */

    /* Convert the next three input vectors. */
    t3 = vec_mradds( tr1, r0, c0 ); /* (R8 .. R15) *  8432 */
    t4 = vec_mradds( tr1, r1, c0 ); /* (R8 .. R15) * -4818 */
    t5 = vec_mradds( tr1, r2, c0 ); /* (R8 .. R15) * 14345 */

    t3 = vec_mradds( tg1, g0, t3 ); /* += (G8 .. G15) *  16425 */
    t4 = vec_mradds( tg1, g1, t4 ); /* += (G8 .. G15) *  -9527 */
    t5 = vec_mradds( tg1, g2, t5 ); /* += (G8 .. G15) * -12045 */

    t3 = vec_mradds( tb1, b0, t3 ); /* += (B8 .. B15) *  3176 */
    t4 = vec_mradds( tb1, b1, t4 ); /* += (B8 .. B15) * 14345 */
    t5 = vec_mradds( tb1, b2, t5 ); /* += (B8 .. B15) * -2300 */

    /* Add the constants. */
    t0 = vec_adds( t0, c16 );
    t3 = vec_adds( t3, c16 );
    t1 = vec_adds( t1, c128 );
    t4 = vec_adds( t4, c128 );
    t2 = vec_adds( t2, c128 );
    t5 = vec_adds( t5, c128 );
	
	u1 = vec_perm( t1, t4, avgPerm1 ); // rearrange U's for averaging
	u2 = vec_perm( t1, t4, avgPerm2 );
	uAvg = vec_avg( u1, u2 );
	v1 = vec_perm( t2, t5, avgPerm1 ); // rearrange V's for averaging
	v2 = vec_perm( t2, t5, avgPerm2 );
	vAvg = vec_avg( v1, v2 );
	
	uv1 = vec_perm( uAvg, vAvg, Perm1 );
	uv2 = vec_perm( uAvg, vAvg, Perm2 );
	out1 = vec_perm( uv1, t0, Perm1 );
	out2 = vec_perm( uv1, t0, Perm2 );
	out3 = vec_perm( uv2, t3, Perm1 );
	out4 = vec_perm( uv2, t3, Perm2 );

    *UYVY_ptr = vec_packsu( out1, out2 );	// pack down to char's
	UYVY_ptr++;
	*UYVY_ptr = vec_packsu( out3, out4 );
	UYVY_ptr++;
  }
}
#endif

GEM_EXTERN void imageStruct::fromGray(unsigned char *greydata) {
  if(!greydata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  register unsigned char grey=0;
  switch (format){
  case GL_RGB:
  case GL_BGR_EXT:
    while(pixelnum--){
      grey=*greydata++;
      *pixels++=grey;
      *pixels++=grey;
      *pixels++=grey;
      greydata++;
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    while(pixelnum--){
      grey=*greydata++;
      pixels[chRed]=grey;
      pixels[chGreen]=grey;
      pixels[chBlue]=grey;
      pixels[chAlpha]=255;
      pixels+=4;
    }
    break;
  case GL_LUMINANCE:
    memcpy(pdata, greydata, pixelnum);
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[chY0]=*greydata++;
      pixels[chY1]=*greydata++;
      pixels[chU]=pixels[chV]=128;
      pixels+=4;      
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromGray(short *greydata) {
  if(!greydata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  register short grey=0;
  switch (format){
  case GL_RGB:
  case GL_BGR_EXT:
    while(pixelnum--){
      grey=*greydata++;
      *pixels++=grey;
      *pixels++=grey;
      *pixels++=grey;
      greydata++;
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    while(pixelnum--){
      grey=*greydata++;
      pixels[chRed]=grey;
      pixels[chGreen]=grey;
      pixels[chBlue]=grey;
      pixels[chAlpha]=255;
      pixels+=4;
    }
    break;
  case GL_LUMINANCE:
    memcpy(pdata, greydata, pixelnum);
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[chY0]=(*greydata++)>>7;
      pixels[chY1]=(*greydata++)>>7;
      pixels[chU]=pixels[chV]=128;
      pixels+=4;      
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromYV12(unsigned char*yuvdata) {
  if(!yuvdata)return;
  size_t pixelnum=xsize*ysize;
  fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN void imageStruct::fromYV12(unsigned char*Y, unsigned char*U, unsigned char*V) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V)fromGray(Y);
  if(!Y || !U || !V)return;

  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  switch (format){
  case GL_LUMINANCE:
    memcpy(pdata, Y, pixelnum);
    break;
  case GL_RGB:  case GL_BGR_EXT: // of course this is stupid, RGB isn't BGR
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*3;

      unsigned char*py1=Y;
      unsigned char*py2=Y+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pv=(format==GL_BGR_EXT)?U:V;
      unsigned char*pu=(format==GL_RGB)?U:V;

      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=*pu++ -UV_OFFSET;
	  v=*pv++ -UV_OFFSET;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r
	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*4;

      unsigned char*py1=Y;//yuvdata;
      unsigned char*py2=Y+xsize;//yuvdata+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pv=(format==GL_BGRA_EXT)?V:U;
      unsigned char*pu=(format==GL_RGBA)?V:U;
 
      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=*pu++-UV_OFFSET;
	  v=*pv++-UV_OFFSET;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels1++ = 255; // a

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels1++ = 255; // a

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels2++ = 255; // a

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels2++ = 255; // a
	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  case GL_YUV422_GEM:
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;
      unsigned char*py1=Y;
      unsigned char*py2=Y+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pu=U;
      unsigned char*pv=V;
      int row=ysize>>1;
      int cols=xsize>>1;
      unsigned char u, v;
      /* this is only re-ordering of the data */
      while(row--){
		int col=cols;
		while(col--){
			// yuv422 is U Y0 V Y1
			u=*pu++;	  v=*pv++;
			*pixels1++=u;
			*pixels1++=*py1++;
			*pixels1++=v;
			*pixels1++=*py1++;
			*pixels2++=u;
			*pixels2++=*py2++;
			*pixels2++=v;
			*pixels2++=*py2++;	  
		}
		pixels1+=xsize*csize;	pixels2+=xsize*csize;
		py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  }
}
//  for gem2pdp
GEM_EXTERN void imageStruct::fromYV12(short*yuvdata) {
  if(!yuvdata)return;
  int pixelnum=xsize*ysize;
  fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN void imageStruct::fromYV12(short*Y, short*U, short*V) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V)fromGray((unsigned char*)(*Y>>7));
  if(!Y || !U || !V)return;

  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  switch (format){
  case GL_LUMINANCE:
    memcpy(pdata, Y, pixelnum);
    break;
  case GL_RGB:  case GL_BGR_EXT: // of course this is stupid, RGB isn't BGR
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

      short*py1=Y;
      short*py2=Y+xsize; // plane_1 is luminance (csize==1)
      short*pv=V;//(format==GL_BGR_EXT)?V:U;
      short*pu=U;//(format==GL_RGB)?V:U;

      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  // these are from http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
	  u=(*pu++)>>8;
	  v=(*pv++)>>8;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*((*py1++)>>7); // what about the "16"-offset ?
#ifndef __APPLE__
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*((*py1++)>>7);
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;

#else
	  pixels1[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[0] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*((*py1++)>>7);
	  pixels1[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[0] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[0 ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[0] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;
#endif

	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

      short*py1=Y;//yuvdata;
      short*py2=Y+xsize;//yuvdata+xsize; // plane_1 is luminance (csize==1)
      short*pv=V;//(format==GL_BGRA_EXT)?U:V;
      short*pu=U;//(format==GL_RGBA)?U:V;

      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=(*pu++)>>8;
	  v=(*pv++)>>8;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*((*py1++)>>7); // what about the "16"-offset ?
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1[chAlpha] = 255; // a
	  pixels1+=4;

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*((*py1++)>>7);
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1[chAlpha] = 255; // a
	  pixels1+=4;

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2[chAlpha] = 255; // a
	  pixels2+=4;

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2[chAlpha] = 255; // a
	  pixels2+=4;
	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }

    break;
  case GL_YUV422_GEM:
    {
#ifdef __TIMING__
	  UnsignedWide start, end;
	  Microseconds(&start);
#endif
#ifdef __VEC__
	YV12_to_YUV422_altivec(Y, U, V, pixelnum);
#else
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;
      short*py1=Y;
      short*py2=Y+xsize; // plane_1 is luminance (csize==1)
      short*pu=U;
      short*pv=V;
      int row=ysize>>1;
      int cols=xsize>>1;
      unsigned char u, v;
      /* this is only re-ordering of the data */
      while(row--){
		int col=cols;
		while(col--){
			// yuv422 is U Y0 V Y1
			u=((*pu++)>>8)+128;	  v=((*pv++)>>8)+128;
			*pixels1++=u;
			*pixels1++=(*py1++)>>7;
			*pixels1++=v;
			*pixels1++=(*py1++)>>7;
			*pixels2++=u;
			*pixels2++=(*py2++)>>7;
			*pixels2++=v;
			*pixels2++=(*py2++)>>7;	  
		}
		pixels1+=xsize*csize;	pixels2+=xsize*csize;
		py1+=xsize*1;	py2+=xsize*1;
      }
#endif
#ifdef __TIMING__
	  Microseconds(&end);
	  float seconds = (float)(end.lo - start.lo) / 1000000.f;
	  post("YV12toUYVY frame time = %f\n", seconds);
#endif
    }
    break;
  }
}

#ifdef __VEC__
GEM_EXTERN void imageStruct :: YV12_to_YUV422_altivec(short*Y, short*U, short*V, size_t pixelnum)
{
  vector unsigned char *pixels1=(vector unsigned char *)data;
  vector unsigned char *pixels2=(vector unsigned char *)(data+(xsize*csize));
  vector unsigned short *py1 = (vector unsigned short *)Y;
  vector unsigned short *py2 = (vector unsigned short *)(Y + xsize );
  vector unsigned short *pu = (vector unsigned short *)U;
  vector unsigned short *pv = (vector unsigned short *)V;
  vector unsigned short uvAdd = (vector unsigned short)( 128, 128, 128, 128,
                                                         128, 128, 128, 128 );
  vector unsigned short yShift = (vector unsigned short)( 7, 7, 7, 7, 7, 7, 7, 7 );
  vector unsigned short uvShift = (vector unsigned short)( 8, 8, 8, 8, 8, 8, 8, 8 );
  vector unsigned short tempU, tempV, doneU, doneV, tempY1, tempY2, tempY3, tempY4,
		uv1, uv2, Y1, Y2, out1, out2, out3, out4, out5, out6, out7, out8;
  vector unsigned char Perm1 = 
         (vector unsigned char)( 0, 1, 16, 17, 2, 3, 18, 19,
						         4, 5, 20, 21, 6, 7, 22, 23 );
  vector unsigned char Perm2 = 
         (vector unsigned char)(  8,  9, 24, 25, 10, 11, 26, 27,
						         12, 13, 28, 29, 14, 15, 30, 31 );
  int row=ysize>>1;
  int cols=xsize>>4;
#if 0
# ifndef PPC970
  UInt32	prefetchSize = GetPrefetchConstant( 16, 1, 256 );
  vec_dst( pu, prefetchSize, 0 );
  vec_dst( pv, prefetchSize, 0 );
  vec_dst( py1, prefetchSize, 0 );
  vec_dst( py2, prefetchSize, 0 );
# endif
#endif
  while(row--){
    int col=cols;
	while(col--){
#if 0
# ifndef PPC970
      vec_dst( );
# endif
#endif
	  tempU = vec_sra( (*pu++), uvShift );
	  tempV = vec_sra( (*pv++), uvShift );
	  doneU = vec_add( tempU, uvAdd );
	  doneV = vec_add( tempV, uvAdd );
	  
	  uv1 = vec_perm( doneU, doneV, Perm1 ); // uvuvuvuv uvuvuvuv
	  uv2 = vec_perm( doneU, doneV, Perm2 );
	  
	  tempY1 = vec_sra( (*py1++), yShift );
	  tempY2 = vec_sra( (*py2++), yShift );
	  
	  out1 = vec_perm( uv1, tempY1, Perm1 ); //fill Y's, U's & V's
	  out2 = vec_perm( uv1, tempY1, Perm2 );
	  out3 = vec_perm( uv1, tempY2, Perm1 ); //fill 2nd Y's, U's & V's
	  out4 = vec_perm( uv1, tempY2, Perm2 );
	  
	  *pixels1 = vec_packsu( out1, out2 );
	  *pixels2 = vec_packsu( out3, out4 );
	  pixels1++; pixels2++; 
	  
	  tempY3 = vec_sra( (*py1++), yShift ); // load second set of Y's
	  tempY4 = vec_sra( (*py2++), yShift );
	  
	  out5 = vec_perm( uv2, tempY3, Perm1 );
	  out6 = vec_perm( uv2, tempY3, Perm2 );
	  out7 = vec_perm( uv2, tempY4, Perm1 );
	  out8 = vec_perm( uv2, tempY4, Perm2 );
	  
	  *pixels1 = vec_packsu( out5, out6 );
      *pixels2 = vec_packsu( out7, out8 );
	  pixels1++; pixels2++;
	}
    pixels1+=(xsize*2)>>4; pixels2+=(xsize*2)>>4;
	py1+=xsize>>3; py2+=xsize>>3;
  }
}
#endif

GEM_EXTERN void imageStruct::fromUYVY(unsigned char *yuvdata) {
  // this is the yuv-format with Gem
  if(!yuvdata)return;
  data=pdata;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=pdata;
  switch (format){
  case GL_YUV422_GEM:
    memcpy(data, yuvdata, pixelnum*csize);
    break;
  case GL_LUMINANCE:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=yuvdata[1];
      *pixels++=yuvdata[3];
      yuvdata+=4;
    }
    break;
  case GL_RGB:
  case GL_BGR:
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[0]-UV_OFFSET;
	v=yuvdata[2]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[1] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[3] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
#ifdef __TIMING__
	  UnsignedWide start, end;
	  Microseconds(&start);
#endif
#if 0
	  YUV422_to_BGRA_altivec( yuvdata, pixelnum, data);
#else
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;
      while(pixelnum--){
	    u=yuvdata[0]-UV_OFFSET;
		v=yuvdata[2]-UV_OFFSET;
		uv_r=YUV2RGB_12*v+YUV2RGB_13*v;
		uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
		uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

		// 1st pixel
		y=YUV2RGB_11*(yuvdata[1] -Y_OFFSET);
		pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
		pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
		pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
		pixels[chAlpha] = 255;
		pixels+=4;
		// 2nd pixel
		y=YUV2RGB_11*(yuvdata[3] -Y_OFFSET);
		pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
		pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
		pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
		pixels[chAlpha] = 255;
		pixels+=4;

		yuvdata+=4;
      }
#endif
#ifdef __TIMING__
	  Microseconds(&end);
	  float seconds = (float)(end.lo - start.lo) / 1000000.f;
	  post("UYVYtoRGBA/BGRA frame time = %f\n", seconds);
#endif
    }
    break;
  }
}

#ifdef __VEC__
GEM_EXTERN void imageStruct :: YUV422_to_BGRA_altivec( unsigned char *yuvdata, 
					size_t pixelnum, unsigned char *data)
{
}
#endif

GEM_EXTERN void imageStruct::fromYUY2(unsigned char *yuvdata) { // YUYV
  if(!yuvdata)return;
  data=pdata;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=pdata;
  switch (format){
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[0]=yuvdata[1]; // u
      pixels[1]=yuvdata[0]; // y
      pixels[2]=yuvdata[3]; // v
      pixels[3]=yuvdata[2]; // y
      pixels+=4;
      yuvdata+=4;
    }
    break;
  case GL_LUMINANCE:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=yuvdata[0];
      *pixels++=yuvdata[2];
      yuvdata+=4;
    }
    break;
  case GL_RGB:
  case GL_BGR:
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[1]-UV_OFFSET;
	v=yuvdata[3]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[1]-UV_OFFSET;
	v=yuvdata[3]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;

	yuvdata+=4;
      }
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromYVYU(unsigned char *yuvdata) {
  // this is the yuv-format with Gem
  if(!yuvdata)return;
  data=pdata;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=pdata;
  switch (format){
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[chU]=yuvdata[1]; // u
      pixels[chY0]=yuvdata[0]; // y
      pixels[chV]=yuvdata[3]; // v
      pixels[chY1]=yuvdata[2]; // y
      pixels+=4;
      yuvdata+=4;
    }
    break;
  case GL_LUMINANCE:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=yuvdata[0];
      *pixels++=yuvdata[2];
      yuvdata+=4;
    }
    break;
  case GL_RGB:  case GL_BGR:
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[3]-UV_OFFSET;
	v=yuvdata[1]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[3]-UV_OFFSET;
	v=yuvdata[1]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;

	yuvdata+=4;
      }
    }
    break;
  }
}

GEM_EXTERN extern int getPixFormat(char*cformat){
  char c=tolower(*cformat);
  switch(c){
  case 'g': return GL_LUMINANCE;
  case 'y': return GL_YUV422_GEM;
  case 'r': 
#ifndef __APPLE__
    return GL_RGBA;
#else
    return GL_BGRA_EXT;
#endif
  }
  return 0;
}

/* swap the Red and Blue channel _in-place_ */
GEM_EXTERN void imageStruct::swapRedBlue() {
  size_t pixelnum=xsize*ysize;
  unsigned char *pixels=data;
  unsigned char dummy=0;
  switch (format){
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      dummy=pixels[chU];
      pixels[chU]=pixels[chV]; // u
      pixels[chV]=dummy; // v
      pixels+=4;
    }
    break;
  case GL_RGB:  case GL_BGR:
    while(pixelnum--){
      dummy=pixels[chRed];
      pixels[chRed]=pixels[chBlue];
      pixels[chBlue]=dummy;
      pixels+=3;
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    while(pixelnum--){
      dummy=pixels[chRed];
      pixels[chRed]=pixels[chBlue];
      pixels[chBlue]=dummy;
      pixels+=4;
    }
    break;
  }

}
