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

#include "m_pd.h"
#include "GemPixUtil.h"

#include <string.h>
#include <ctype.h>

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
    to->pdata   = pdata;
    to->datasize= datasize;
    to->notowned= true;
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
  case GL_LUMINANCE:  format=GL_LUMINANCE;  csize=1; break;
  case GL_YUV422_GEM: format=GL_YUV422_GEM; csize=2; break;
  default:
#ifndef __APPLE__
  case GL_RGBA:       format=GL_RGBA;
#else
  case GL_BGRA_EXT:   format=GL_BGRA_EXT;
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
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_RGB:
    csize=3;
    memcpy(data, rgbdata, pixelnum*csize);
    break;
  case GL_BGR_EXT:
    csize=3;
    while(pixelnum--){
      pixels[0]=rgbdata[2];
      pixels[1]=rgbdata[1];
      pixels[2]=rgbdata[0];
      pixels+=3; rgbdata+=3;
    }
    break;
  case GL_RGBA:
    csize=4;
    while(pixelnum--){
      *pixels++=*rgbdata++;
      *pixels++=*rgbdata++;
      *pixels++=*rgbdata++;
      *pixels++=255;
    }
    break;
  case GL_BGRA_EXT:
    csize=4;
    while(pixelnum--){
      pixels[0]=rgbdata[2];
      pixels[1]=rgbdata[1];
      pixels[2]=rgbdata[0];
      pixels[3]=255;
      pixels+=4;rgbdata+=3;
    }
    break;
  case GL_LUMINANCE:
    csize=1;
    while(pixelnum--){
      //      *pixels++=(unsigned char)(rgbdata[0] * 0.3086f + rgbdata[1] * 0.06094f + rgbdata[2] * 0.0820f);
      *pixels++=(rgbdata[0]*79+rgbdata[1]*156+rgbdata[2]*21)>>8;
      rgbdata+=3;
    }
    break;
  case GL_YUV422_GEM:
    csize=2;
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((-43*rgbdata[0]-85 *rgbdata[1]+128*rgbdata[2])>>8)+128; // U
      *pixels++=(77 *rgbdata[0]+150*rgbdata[1]+29 *rgbdata[2])>>8;     // Y
      *pixels++=((128*rgbdata[0]-107*rgbdata[1]-21 *rgbdata[2])>>8)+128; // V
      *pixels++=(77 *rgbdata[3]+150*rgbdata[4]+29 *rgbdata[5])>>8;     // Y
      rgbdata+=6;
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromRGB16(unsigned char *rgb16data) {
  //   B B B B B G G G   G G G R R R R R
  //   R R R R R G G G   G G G B B B B B
  if(!rgb16data)return;
  unsigned short*rgbdata=(unsigned short*)rgb16data;
  int pixelnum=xsize*ysize;
    reallocate();
  unsigned char *pixels=data;
  unsigned short rgb;
  switch (format){
  case GL_RGBA:
    csize=4;
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
    csize=1;
    while(pixelnum--){
      rgb=*rgbdata++;
      *pixels++=(((rgb>>8)&0xF8)*79+((rgb>>3)&0xFC)*156+((rgb<<3)&0xF8)*21)>>8;
    }
    break;
  case GL_YUV422_GEM:
    csize=2;
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
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_RGB:
    csize=3;
    while(pixelnum--){
      *pixels++=*rgbadata++;
      *pixels++=*rgbadata++;
      *pixels++=*rgbadata++;
      rgbadata++;
    }
    break;
  case GL_BGR_EXT:
    csize=3;
    while(pixelnum--){
      pixels[0]=rgbadata[2];
      pixels[1]=rgbadata[1];
      pixels[2]=rgbadata[0];
      pixels+=3; rgbadata+=4;
    }
    break;
  case GL_RGBA:
    csize=4;
    memcpy(data, rgbadata, pixelnum*csize);
    break;
  case GL_BGRA_EXT:
    csize=4;
    while(pixelnum--){
      pixels[0]=rgbadata[2];
      pixels[1]=rgbadata[1];
      pixels[2]=rgbadata[0];
      pixels[3]=rgbadata[3];
      pixels+=4;rgbadata+=4;
    }
    break;
  case GL_LUMINANCE:
    csize=1;
    while(pixelnum--){
      //      *pixels++=(unsigned char)(rgbadata[0] * 0.3086f + rgbadata[1] * 0.06094f + rgbadata[2] * 0.0820f);
      *pixels++=(rgbadata[0]*79+rgbadata[1]*156+rgbadata[2]*21)>>8;
      rgbadata+=4;
    }
    break;
  case GL_YUV422_GEM:
    csize=2;
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((-43*rgbadata[0]-85 *rgbadata[1]+128*rgbadata[2])>>8)+128; // U
      *pixels++=(77 *rgbadata[0]+150*rgbadata[1]+29 *rgbadata[2])>>8;     // Y
      *pixels++=((128*rgbadata[0]-107*rgbadata[1]-21 *rgbadata[2])>>8)+128; // V
      *pixels++=(77 *rgbadata[4]+150*rgbadata[5]+29 *rgbadata[6])>>8;     // Y
      rgbadata+=8;
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromBGR(unsigned char *bgrdata) {
  if(!bgrdata)return;
  int pixelnum=xsize*ysize;
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_BGR_EXT:
    csize=3;
    memcpy(data, bgrdata, pixelnum*csize);
    break;
  case GL_RGB:
    csize=3;
    while(pixelnum--){
      pixels[0]=bgrdata[2];
      pixels[1]=bgrdata[1];
      pixels[2]=bgrdata[0];
      pixels+=3; bgrdata+=3;
    }
    break;
  case GL_BGRA_EXT:
    csize=4;
    while(pixelnum--){
      *pixels++=*bgrdata++;
      *pixels++=*bgrdata++;
      *pixels++=*bgrdata++;
      *pixels++=255;
    }
    break;
  case GL_RGBA:
    csize=4;
    while(pixelnum--){
      pixels[0]=bgrdata[2];
      pixels[1]=bgrdata[1];
      pixels[2]=bgrdata[0];
      pixels[3]=255;
      pixels+=4;bgrdata+=3;
    }
    break;
  case GL_LUMINANCE:
    csize=1;
    while(pixelnum--){
      //      *pixels++=(unsigned char)(bgrdata[2] * 0.3086f + bgrdata[1] * 0.06094f + bgrdata[0] * 0.0820f);
      *pixels++=(bgrdata[2]*79+bgrdata[1]*156+bgrdata[0]*21)>>8;
      bgrdata+=3;
    }
    break;
  case GL_YUV422_GEM:
    csize=2;
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
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_BGR_EXT:
    csize=3;
    while(pixelnum--){
      *pixels++=*bgradata++;
      *pixels++=*bgradata++;
      *pixels++=*bgradata++;
      bgradata++;
    }
    break;
  case GL_RGB:
    csize=3;
    while(pixelnum--){
      pixels[0]=bgradata[2];
      pixels[1]=bgradata[1];
      pixels[2]=bgradata[0];
      pixels+=3; bgradata+=4;
    }
    break;
  case GL_BGRA_EXT:
    csize=4;
    memcpy(data, bgradata, pixelnum*csize);
    break;
  case GL_RGBA:
    csize=4;
    while(pixelnum--){
      pixels[0]=bgradata[2];
      pixels[1]=bgradata[1];
      pixels[2]=bgradata[0];
      pixels[3]=bgradata[3];
      pixels+=4;bgradata+=4;
    }
    break;
  case GL_LUMINANCE:
    csize=1;
    while(pixelnum--){
      //      *pixels++=(unsigned char)(bgradata[2] * 0.3086f + bgradata[1] * 0.06094f + bgradata[0] * 0.0820f);
      *pixels++=(bgradata[2]*79+bgradata[1]*156+bgradata[0]*21)>>8;

      bgradata+=4;
    }
    break;
  case GL_YUV422_GEM:
    csize=2;
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((-43*bgradata[2]-85 *bgradata[1]+128*bgradata[0])>>8)+128; // U
      *pixels++=(77 *bgradata[2]+150*bgradata[1]+29 *bgradata[0])>>8;     // Y
      *pixels++=((128*bgradata[2]-107*bgradata[1]-21 *bgradata[0])>>8)+128; // V
      *pixels++=(77 *bgradata[6]+150*bgradata[5]+29 *bgradata[4])>>8;     // Y
      bgradata+=8;
    }
    break;
  }
}
GEM_EXTERN void imageStruct::fromGray(unsigned char *greydata) {
  if(!greydata)return;
  int pixelnum=xsize*ysize;
  reallocate();
  unsigned char *pixels=data;
  register unsigned char grey=0;
  switch (format){
  case GL_RGB:
  case GL_BGR_EXT:
    csize=3;
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
    csize=4;
    while(pixelnum--){
      grey=*greydata++;
      *pixels++=grey;
      *pixels++=grey;
      *pixels++=grey;
      *pixels++=255;
    }
    break;
  case GL_LUMINANCE:
    csize=1;
    memcpy(pdata, greydata, pixelnum);
    break;
  case GL_YUV422_GEM:
    csize=2;
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
  fromYV12(yuvdata, yuvdata+(pixelnum+pixelnum>>2), yuvdata+(pixelnum));  
}
GEM_EXTERN void imageStruct::fromYV12(unsigned char*Y, unsigned char*U, unsigned char*V) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V)fromGray(Y);
  if(!Y || !U || !V)return;

  int pixelnum=xsize*ysize;
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_LUMINANCE:
    csize=1;
    memcpy(pdata, Y, pixelnum);
    break;
  case GL_RGB:  case GL_BGR_EXT: // of course this is stupid, RGB isn't BGR
    csize=3;
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

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
    csize=4;
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

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
    csize=2;
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
    csize=2;
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
      pixels[0]=yuvdata[3]; // u
      pixels[1]=yuvdata[0]; // y
      pixels[2]=yuvdata[1]; // v
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
