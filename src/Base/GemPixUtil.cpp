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
#ifndef MACOSX
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
  switch (format){
  default:
    memset(pdata, 0, datasize);
    break;
  }
}
GEM_EXTERN void imageStruct::setWhite() {
  switch (format){
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
  }
}
GEM_EXTERN void imageStruct::fromYV12(unsigned char *yuvdata) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!yuvdata)return;
  int pixelnum=xsize*ysize;
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_LUMINANCE:
    csize=1;
#if 1
    notowned=true;
    data=yuvdata;
#else
    memcpy(pdata, yuvdata, pixelnum);
#endif
    break;
  case GL_RGB:  case GL_BGR_EXT: // of course this is stupid, RGB isn't BGR
    csize=3;
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

      unsigned char*py1=yuvdata;
      unsigned char*py2=yuvdata+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pv=yuvdata+pixelnum+((format==GL_BGR_EXT)?(pixelnum>>2):0);
      unsigned char*pu=yuvdata+pixelnum+((format==GL_RGB)?(pixelnum>>2):0);
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

      unsigned char*py1=yuvdata;
      unsigned char*py2=yuvdata+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pv=yuvdata+pixelnum+((format==GL_BGRA_EXT)?(pixelnum>>2):0);
      unsigned char*pu=yuvdata+pixelnum+((format==GL_RGBA)?(pixelnum>>2):0);
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
      unsigned char*py1=yuvdata;
      unsigned char*py2=yuvdata+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pu=yuvdata+pixelnum;
      unsigned char*pv=yuvdata+pixelnum+(pixelnum>>2);
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
    pixelnum>>1;
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
      pixelnum>>1;

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
    pixelnum>>1;
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
    pixelnum>>1;
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
      pixelnum>>1;

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
    pixelnum>>1;
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
    pixelnum>>1;
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
      pixelnum>>1;

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
#ifndef MACOSX
    return GL_RGBA;
#else
    return GL_BGRA_EXT;
#endif
  }
  return 0;
}
