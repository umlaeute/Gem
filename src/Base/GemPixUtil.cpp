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
#ifdef _WINDOWS
# pragma warning( disable : 4244 )
# pragma warning( disable : 4305 )
# pragma warning( disable : 4091 )
#endif

#include "m_pd.h"
#include "GemPixUtil.h"

#include <string.h>
#include <ctype.h>

GEM_EXTERN unsigned char* imageStruct::allocate(int size) 
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

GEM_EXTERN unsigned char* imageStruct::reallocate(int size)
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
      error("GEM: Someone sent a bogus pointer to copy2Image");
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
    to->datasize= datasize;
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

void pix_addsat(unsigned char *leftPix, unsigned char *rightPix, int datasize)
{
  while(datasize--)
    {           
      *leftPix = CLAMP_HIGH((int)*leftPix + (int)*rightPix);
      leftPix++;
      rightPix++;
    }
}


void pix_sub(unsigned char *leftPix, unsigned char *rightPix, int datasize)
{
  while(datasize--)
    *leftPix = CLAMP_LOW((int)*leftPix++ - (int)*rightPix++);
}

GEM_EXTERN void imageStruct::setBlack() {
  long i = datasize;
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
  long i = datasize;
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
  int pixelnum=xsize*ysize;
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
	RGB_to_YCbCr(rgbdata, pixelnum, pixels);
#else
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((-43*rgbdata[0]-85 *rgbdata[1]+128*rgbdata[2])>>8)+128; // U
      *pixels++=(77 *rgbdata[0]+150*rgbdata[1]+29 *rgbdata[2])>>8;     // Y
      *pixels++=((128*rgbdata[0]-107*rgbdata[1]-21 *rgbdata[2])>>8)+128; // V
      *pixels++=(77 *rgbdata[3]+150*rgbdata[4]+29 *rgbdata[5])>>8;     // Y
      rgbdata+=6;
    }
#endif
    break;
  }
}
void RGB_to_YCbCr(unsigned char *rgbdata, int RGB_size, 
                  unsigned char *pixels)
{
#ifdef __VEC__
  vector signed short  r0, r1, r2, g0, g1, g2, b0, b1, b2, c0, c16, c128;
  vector unsigned char z0, tc0, tc1, tc2, tc3;
  vector signed short tr0, tr1, tg0, tg1, tb0, tb1;
  vector signed short t0, t1, t2, t3, t4, t5;
  int i;
  
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
    tr0 = vec_unpack2sh( z0, tc0 );  /* tr0 = R0 .. R7  */
    tg0 = vec_unpack2sl( z0, tc0 );  /* tg0 = G0 .. G7  */
    tb0 = vec_unpack2sh( z0, tc1 );  /* tb0 = B0 .. B7  */
    tr1 = vec_unpack2sh( z0, tc2 );  /* tr0 = R8 .. R15 */
    tg1 = vec_unpack2sl( z0, tc2 );  /* tg0 = G8 .. G15 */
    tb1 = vec_unpack2sh( z0, tc3 );  /* tb0 = B8 .. B15 */

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
#endif
}
GEM_EXTERN void imageStruct::fromRGB16(unsigned char *rgb16data) {
  //   B B B B B G G G   G G G R R R R R
  //   R R R R R G G G   G G G B B B B B
  if(!rgb16data)return;
  unsigned short*rgbdata=(unsigned short*)rgb16data;
  int pixelnum=xsize*ysize;
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

      *pixels++=((-43*r-85 *g+128*b)>>8)+128; // U
      *pixels++=(77 *r+150*g+29 *b)>>8;     // Y
      *pixels++=((128*r-107*g-21 *b)>>8)+128; // V
      rgb=*rgbdata++;
      r=((rgb>>8)&0xF8);
      g=((rgb>>3)&0xFC);
      b=((rgb<<3)&0xF8);
      *pixels++=(77 *r+150*g+29 *b)>>8;     // Y
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromRGBA(unsigned char *rgbadata) {
  if(!rgbadata)return;
  int pixelnum=xsize*ysize;
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
    while(pixelnum--){
      pixels[0]=rgbadata[2];
      pixels[1]=rgbadata[1];
      pixels[2]=rgbadata[0];
      pixels[3]=rgbadata[3];
      pixels+=4;rgbadata+=4;
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
      *pixels++=((-43*rgbadata[chRed]-85 *rgbadata[chGreen]+128*rgbadata[chBlue])>>8)+128; // U
      *pixels++=(77 *rgbadata[chRed]+150*rgbadata[chGreen]+29 *rgbadata[chBlue])>>8;     // Y
      *pixels++=((128*rgbadata[chRed]-107*rgbadata[chGreen]-21 *rgbadata[chBlue])>>8)+128; // V
      *pixels++=(77 *rgbadata[4+chRed]+150*rgbadata[4+chGreen]+29 *rgbadata[4+chBlue])>>8;     // Y
      rgbadata+=8;
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromBGR(unsigned char *bgrdata) {
  if(!bgrdata)return;
  int pixelnum=xsize*ysize;
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
      *pixels++=((-43*bgrdata[2]-85 *bgrdata[1]+128*bgrdata[0])>>8)+128; // U
      *pixels++=(77 *bgrdata[2]+150*bgrdata[1]+29 *bgrdata[0])>>8;     // Y
      *pixels++=((128*bgrdata[2]-107*bgrdata[1]-21 *bgrdata[0])>>8)+128; // V
      *pixels++=(77 *bgrdata[5]+150*bgrdata[4]+29 *bgrdata[3])>>8;     // Y
      bgrdata+=6;
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromBGRA(unsigned char *bgradata) {
  if(!bgradata)return;
  int pixelnum=xsize*ysize;
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
    while(pixelnum--){
      pixels[0]=bgradata[2];
      pixels[1]=bgradata[1];
      pixels[2]=bgradata[0];
      pixels[3]=bgradata[3];
      pixels+=4;bgradata+=4;
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
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((-43*bgradata[chRed]-85 *bgradata[chGreen]+128*bgradata[chBlue])>>8)+128; // U
      *pixels++=(77 *bgradata[chRed]+150*bgradata[chGreen]+29 *bgradata[chBlue])>>8;     // Y
      *pixels++=((128*bgradata[chRed]-107*bgradata[chGreen]-21 *bgradata[chBlue])>>8)+128; // V
      *pixels++=(77 *bgradata[4+chRed]+150*bgradata[4+chGreen]+29 *bgradata[4+chBlue])>>8;     // Y
      bgradata+=8;
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromGray(unsigned char *greydata) {
  if(!greydata)return;
  int pixelnum=xsize*ysize;
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
GEM_EXTERN void imageStruct::fromYV12(unsigned char*yuvdata) {
  if(!yuvdata)return;
  int pixelnum=xsize*ysize;
  fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN void imageStruct::fromYV12(unsigned char*Y, unsigned char*U, unsigned char*V) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V)fromGray(Y);
  if(!Y || !U || !V)return;

  int pixelnum=xsize*ysize;
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
#if 0
      unsigned char*pv=yuvdata+pixelnum+((format==GL_BGR_EXT)?(pixelnum>>2):0);
      unsigned char*pu=yuvdata+pixelnum+((format==GL_RGB)?(pixelnum>>2):0);
#else
      unsigned char*pv=(format==GL_BGR_EXT)?U:V;
      unsigned char*pu=(format==GL_RGB)?U:V;
#endif
      int y, u, v, yy, vr, ug, vg, ub;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=*pu++-128;
	  v=*pv++-128;
	  ug=88*u;
	  ub=454*u;
	  vg =  183 * v;
	  vr =  359 * v;

	  // 1st row - 1st pixel
	  y=*py1++;
 	  yy=y<<8;
	  *pixels1++ = (yy + ub     ) >> 8; // b
	  *pixels1++ = (yy - ug - vg) >> 8; // g
	  *pixels1++ = (yy +      vr) >> 8; // r
	  // 1st row - 2nd pixel
	  y=*py1++;
	  yy=y<<8;
	  *pixels1++ = (yy + ub     ) >> 8;
	  *pixels1++ = (yy - ug - vg) >> 8;
	  *pixels1++ = (yy +      vr) >> 8;

	  // 2nd row - 1st pixel
	  y=*py2++;
	  yy=y<<8;
	  *pixels2++ = (yy + ub     ) >> 8; // b
	  *pixels2++ = (yy - ug - vg) >> 8; // g
	  *pixels2++ = (yy +      vr) >> 8; // r

	  // 2nd row - 2nd pixel
	  y=*py2++;
	  yy=y<<8;
	  *pixels2++ = (yy + ub     ) >> 8;
	  *pixels2++ = (yy - ug - vg) >> 8;
	  *pixels2++ = (yy +      vr) >> 8;
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
      /*
      unsigned char*pv=yuvdata+pixelnum+((format==GL_BGRA_EXT)?(pixelnum>>2):0);
      unsigned char*pu=yuvdata+pixelnum+((format==GL_RGBA)?(pixelnum>>2):0);
      */
      unsigned char*pv=(format==GL_BGRA_EXT)?V:U;
      unsigned char*pu=(format==GL_RGBA)?V:U;
 
      int y, u, v, yy, vr, ug, vg, ub;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=*pu++-128;
	  ug=88*u;
	  ub=454*u;
	  v=*pv++-128;
	  vg =  183 * v;
	  vr =  359 * v;

	  // 1st row - 1st pixel
	  y=*py1++;
	  yy=y<<8;
	  *pixels1++ = (yy + ub     ) >> 8; // r
	  *pixels1++ = (yy - ug - vg) >> 8; // g
	  *pixels1++ = (yy +      vr) >> 8; // b
	  *pixels1++ = 255; // a
	  // 1st row - 2nd pixel
	  y=*py1++;
	  yy=y<<8;
	  *pixels1++ = (yy + ub     ) >> 8; // r
	  *pixels1++ = (yy - ug - vg) >> 8; // g
	  *pixels1++ = (yy +      vr) >> 8; // b
	  *pixels1++ = 255; // a

	  // 2nd row - 1st pixel
	  y=*py2++;
	  yy=y<<8;
	  *pixels2++ = (yy + ub     ) >> 8; // r
	  *pixels2++ = (yy - ug - vg) >> 8; // g
	  *pixels2++ = (yy +      vr) >> 8; // b
	  *pixels2++ = 255; // a

	  // 2nd row - 2nd pixel
	  y=*py2++;
	  yy=y<<8;
	  *pixels2++ = (yy + ub     ) >> 8; // r
	  *pixels2++ = (yy - ug - vg) >> 8; // g
	  *pixels2++ = (yy +      vr) >> 8; // b
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

  int pixelnum=xsize*ysize;
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
      short*pv=(format==GL_BGR_EXT)?V:U;
      short*pu=(format==GL_RGB)?V:U;

      int y, u, v, yy, vr, ug, vg, ub;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
		int col=cols;
		while(col--){
		  u=((*pu++)>>8);
		  v=((*pv++)>>8);
		  ug=88*u;
		  ub=454*u;
		  vg =  183 * v;
		  vr =  358 * v;

		  // 1st row - 1st pixel
		  y=(*py1++)>>7;
		  yy=y<<8;
		  *pixels1++ = CLAMP((yy + ub     ) >> 8); // b
		  *pixels1++ = CLAMP((yy - ug - vg) >> 8); // g
		  *pixels1++ = CLAMP((yy +      vr) >> 8); // r
		  // 1st row - 2nd pixel
		  y=(*py1++)>>7;
		  yy=y<<8;
		  *pixels1++ = CLAMP((yy + ub     ) >> 8);
		  *pixels1++ = CLAMP((yy - ug - vg) >> 8);
		  *pixels1++ = CLAMP((yy +      vr) >> 8);

		  // 2nd row - 1st pixel
		  y=(*py2++)>>7;
		  yy=y<<8;
		  *pixels2++ = CLAMP((yy + ub     ) >> 8); // b
		  *pixels2++ = CLAMP((yy - ug - vg) >> 8); // g
		  *pixels2++ = CLAMP((yy +      vr) >> 8); // r

		  // 2nd row - 2nd pixel
		  y=(*py2++)>>7;
		  yy=y<<8;
		  *pixels2++ = CLAMP((yy + ub     ) >> 8);
		  *pixels2++ = CLAMP((yy - ug - vg) >> 8);
		  *pixels2++ = CLAMP((yy +      vr) >> 8);
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
	  short*pv=(format==GL_BGRA_EXT)?U:V;
      short*pu=(format==GL_RGBA)?U:V;

      int y, u, v, yy, vr, ug, vg, ub;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
		int col=cols;
		while(col--){
		  u=(*pu++)>>8;
		  ug=88*u;
		  ub=454*u;
		  v=(*pv++)>>8;
		  vg =  183 * v;
		  vr =  358 * v;

		  // 1st row - 1st pixel
		  y=(*py1++)>>7;
		  yy=y<<8;
		  *pixels1++ = 0; // a
		  *pixels1++ = CLAMP((yy + ub     ) >> 8); // r
		  *pixels1++ = CLAMP((yy - ug - vg) >> 8); // g
		  *pixels1++ = CLAMP((yy +      vr) >> 8); // b
		  //*pixels1++ = 0; // a
		  // 1st row - 2nd pixel
		  y=(*py1++)>>7;
		  yy=y<<8;
		  *pixels1++ = 0; // a
		  *pixels1++ = CLAMP((yy + ub     ) >> 8); // r
		  *pixels1++ = CLAMP((yy - ug - vg) >> 8); // g
		  *pixels1++ = CLAMP((yy +      vr) >> 8); // b
		  //*pixels1++ = 0; // a

		  // 2nd row - 1st pixel
		  y=(*py2++)>>7;
		  yy=y<<8;
		  *pixels2++ = 0; // a
		  *pixels2++ = CLAMP((yy + ub     ) >> 8); // r
		  *pixels2++ = CLAMP((yy - ug - vg) >> 8); // g
		  *pixels2++ = CLAMP((yy +      vr) >> 8); // b
		  //*pixels2++ = 0; // a

		  // 2nd row - 2nd pixel
		  y=(*py2++)>>7;
		  yy=y<<8;
		  *pixels2++ = 0; // a
		  *pixels2++ = CLAMP((yy + ub     ) >> 8); // r
		  *pixels2++ = CLAMP((yy - ug - vg) >> 8); // g
		  *pixels2++ = CLAMP((yy +      vr) >> 8); // b
		  //*pixels2++ = 0; // a
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
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromUYVY(unsigned char *yuvdata) {
  // this is the yuv-format with Gem
  if(!yuvdata)return;
  data=pdata;
  int pixelnum=xsize*ysize;
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
      int y, u, v, yy, vr, ug, vg, ub;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[0]-128;
	ug=88*u;
	ub=454*u;
	v=yuvdata[2]-128;
	vg =  183 * v;
	vr =  359 * v;
	
	// 1st pixel
	y=yuvdata[1];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels+=3;

	// 2nd pixel
	y=yuvdata[3];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v, yy, vr, ug, vg, ub;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[0]-128;
	ug=88*u;
	ub=454*u;
	v=yuvdata[2]-128;
	vg =  183 * v;
	vr =  359 * v;
	
	// 1st pixel
	y=yuvdata[1];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=yuvdata[3];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels[chAlpha] = 255;
	pixels+=4;

	yuvdata+=4;
      }
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromYUY2(unsigned char *yuvdata) { // YUYV
  if(!yuvdata)return;
  data=pdata;
  int pixelnum=xsize*ysize;
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
      int y, u, v, yy, vr, ug, vg, ub;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[1]-128;
	ug=88*u;
	ub=454*u;
	v=yuvdata[3]-128;
	vg =  183 * v;
	vr =  359 * v;
	
	// 1st pixel
	y=yuvdata[0];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels+=3;

	// 2nd pixel
	y=yuvdata[2];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v, yy, vr, ug, vg, ub;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[1]-128;
	ug=88*u;
	ub=454*u;
	v=yuvdata[2]-128;
	vg =  183 * v;
	vr =  359 * v;
	
	// 1st pixel
	y=yuvdata[0];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=yuvdata[2];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
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
  int pixelnum=xsize*ysize;
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
      int y, u, v, yy, vr, ug, vg, ub;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[3]-128;
	ug=88*u;
	ub=454*u;
	v=yuvdata[1]-128;
	vg =  183 * v;
	vr =  359 * v;
	
	// 1st pixel
	y=yuvdata[0];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels+=3;

	// 2nd pixel
	y=yuvdata[2];
	yy=y<<8;
	pixels[chRed  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chBlue ] = (yy + ub     ) >> 8; // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v, yy, vr, ug, vg, ub;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[3]-128;
	ug=88*u;
	ub=454*u;
	v=yuvdata[1]-128;
	vg =  183 * v;
	vr =  359 * v;
	
	// 1st pixel
	y=yuvdata[0];
	yy=y<<8;
	// somehow i have colours swapped with my dv-cam...
	pixels[chBlue  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chRed ] = (yy + ub     ) >> 8; // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=yuvdata[2];
	yy=y<<8;
	pixels[chBlue  ] = (yy +      vr) >> 8; // r
	pixels[chGreen] = (yy - ug - vg) >> 8; // g
	pixels[chRed ] = (yy + ub     ) >> 8; // b
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

