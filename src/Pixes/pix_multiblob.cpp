////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_multiblob
// based on (c) 2004, Jakob Leiner & Theresa Rienmüller
//
/////////////////////////////////////////////////////////


#include "pix_multiblob.h"

////////////////////////
// the Blob-structure
Blob::Blob(){
  m_xmin = 0; m_xmax = 0.0;
  m_ymin = 0; m_ymax = 0.0;
  m_xaccum=0; m_yaccum=0; m_xyaccum=0;
  area = 0;
}

double Blob:: xmin(){
  return m_xmin;
}
double Blob:: xmax(){
  return m_xmax;
}
double Blob:: ymin(){
  return m_ymin;
}
double Blob:: ymax(){
  return m_ymax;
}
double Blob:: xmid(){
  return m_xaccum/m_xyaccum;
}
double Blob:: ymid(){
  return m_yaccum/m_xyaccum;
}
double Blob:: diameter2(){
  return (m_xmax-m_xmin)*(m_xmax-m_xmin)+(m_ymax-m_ymin)*(m_ymax-m_ymin);}
double Blob:: diameter(){
  return sqrt(diameter2());
}
double Blob:: distance2(Blob b){
  return (b.xmid()-xmid())*(b.xmid()-xmid())+(b.ymid()-ymid())*(b.ymid()-ymid());
}
double Blob:: distance(Blob b){
  return sqrt(distance2(b));}
void Blob:: xmin(double x){
  m_xmin=x;
}
void Blob:: xmax(double x){
  m_xmax=x;
}
void Blob:: ymin(double y){
  m_ymin=y;
}
void Blob:: ymax(double y){
  m_ymax=y;
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_multiblob,t_floatarg, A_DEFFLOAT)
  
/*------------------------------------------------------------
  
pix_multiblob

------------------------------------------------------------*/

/*------------------------------------------------------------

Constructor
initializes the pixBlocks and pixBlobs

------------------------------------------------------------*/
  pix_multiblob :: pix_multiblob(t_floatarg f) : m_blobsize(0.001), m_treshold(10)
{
  m_blobNumber = (int)f;
  if(m_blobNumber < 1)m_blobNumber = 6;

  // initialize blob-structures
  currentBlobs = new Blob[m_blobNumber];

  // initialize image
  m_image.xsize=320;
  m_image.ysize=240;
  m_image.setCsizeByFormat(GL_LUMINANCE);
  m_image.allocate();

  // outlets
  m_infoOut = outlet_new(this->x_obj, &s_list);
}

/*------------------------------------------------------------

Destructor

------------------------------------------------------------*/
pix_multiblob :: ~pix_multiblob(){
  outlet_free(m_infoOut);
  if(currentBlobs)delete[]currentBlobs;
}

/*------------------------------------------------------------
makeBlobs
calculates the Blobs, maximal x and y values are set
------------------------------------------------------------*/
void pix_multiblob :: makeBlob(Blob *pb, int x, int y){
  if(!pb)return;

  if(x<0 || y<0)return;
  if(x>m_image.xsize || y>m_image.xsize)return;

  pb->area++;
  t_float grey=((t_float)m_image.GetPixel(y, x, chGray))/255;
  pb->m_xaccum +=grey*(t_float)x;
  pb->m_yaccum +=grey*(t_float)y;
  pb->m_xyaccum+=grey;


  if(x<pb->xmin())pb->xmin(x);
  if(x>pb->xmax())pb->xmax(x);
  if(y<pb->ymin())pb->ymin(y);
  if(y>pb->ymax())pb->ymax(y);

  m_image.SetPixel(y,x,chGray,0);

  if(pb->area > 10000){return;}
  for(int i = -1; i<= 1; i++){
    for(int j = -1; j <= 1; j++){
      if (m_image.GetPixel(y+i, x+j, chGray) > m_treshold)
	{
	  makeBlob(pb, x+j, y+i);
	}
    }
  }
}

/*------------------------------------------------------------
addToBlobArray
adds a detected Blob to the blob list
------------------------------------------------------------*/
void pix_multiblob :: addToBlobArray(Blob *pb, int blobNumber){
  if (blobNumber >= m_blobNumber){
    // look whether we can replace a smaller blob
    float min = pb->area;
    int index=-1;
    int i = m_blobNumber;
    while(i--)
      if (currentBlobs[i].area < min){
	min = currentBlobs[i].area;
	index = i;
      }
    if (index!=-1)currentBlobs[index] = *pb;
  } else {
    currentBlobs[blobNumber] = *pb;
  }
}

/*------------------------------------------------------------
  
render

------------------------------------------------------------*/
void pix_multiblob :: doProcessing() {
  int blobNumber = 0;
  int blobsize = (int)(m_blobsize * m_image.xsize * m_image.ysize);

  // reset the currentblobs array


  // detect blobs and add them to the currentBlobs-array
  for(int y = 0; y < m_image.ysize; y++){
    for(int x = 0; x < m_image.xsize; x++){
      if (m_image.GetPixel(y,x,0) > 0)
	{
	  Blob *blob = new Blob();
	  blob->xmin(m_image.xsize);
	  blob->ymin(m_image.ysize);

	  makeBlob(blob, x, y);
	  if(blob->area > blobsize){
	    addToBlobArray(blob, blobNumber);
	    blobNumber++;
	  }
	  if(blob)delete blob;
	}
    }
  }

  // ok, we have found some blobs

  // since we can only handle m_blobNumber blobs, we might want to clip
  if(blobNumber > m_blobNumber)blobNumber = m_blobNumber;

  t_float scaleX = 1./m_image.xsize;
  t_float scaleY = 1./m_image.ysize;
  t_float scaleXY=scaleX*scaleY;

  // no create a matrix of [blobNumber*3] elements
  // each row holds all information on our blob
  t_atom*ap = new t_atom[2+blobNumber*8];
  SETFLOAT(ap, (t_float)blobNumber);
  SETFLOAT(ap+1, 8.0);

  int bn=blobNumber;
  for(bn=0; bn<blobNumber; bn++){
    SETFLOAT(ap+bn*8+2, currentBlobs[bn].xmid()*scaleX); // weighted X
    SETFLOAT(ap+bn*8+3, currentBlobs[bn].ymid()*scaleY); // weighted Y
    SETFLOAT(ap+bn*8+4, currentBlobs[bn].m_xyaccum*scaleXY); // weighted Area

    SETFLOAT(ap+bn*8+5, currentBlobs[bn].xmin()*scaleX); // minX
    SETFLOAT(ap+bn*8+6, currentBlobs[bn].ymin()*scaleY); // minY
    SETFLOAT(ap+bn*8+7, currentBlobs[bn].xmax()*scaleX); // maxX
    SETFLOAT(ap+bn*8+8, currentBlobs[bn].ymax()*scaleY); // maxY

    SETFLOAT(ap+bn*8+9, currentBlobs[bn].area*scaleXY);  // unweighted Area
  }

  // i admit that it is naughty to use "matrix" from zexy/iemmatrix
  // but it is the best thing i can think of for 2-dimensional arrays
  outlet_anything(m_infoOut, gensym("matrix"), 2+8*blobNumber, ap);
}

void pix_multiblob :: processImage(imageStruct &image){
  // store the image in greyscale
  // since the algorithm is destructive we do it in a sandbox...
  
  m_image.xsize=image.xsize;
  m_image.ysize=image.ysize;
 
  switch (image.format){
  case GL_RGBA: 
    m_image.fromRGBA(image.data);
    break;
  case GL_RGB:  
    m_image.fromRGB(image.data);
    break;
  case GL_BGR_EXT:
    m_image.fromBGR(image.data);
    break;
  case GL_BGRA_EXT: /* "RGBA" on apple */
    m_image.fromBGRA(image.data);
    break;
  case GL_LUMINANCE:
    m_image.fromGray(image.data);
    break;
  case GL_YCBCR_422_GEM: // YUV
    m_image.fromUYVY(image.data);
    break;
  }

  doProcessing();

}


/*------------------------------------------------------------
blobSizeMess
------------------------------------------------------------*/
void pix_multiblob :: blobSizeMess(t_float blobSize){
  if((blobSize < 0.0)||(blobSize > 1.0))
    {
      error("blobsize %f out of range (0..1)!", blobSize);
      return;
    }
  m_blobsize = blobSize/100.0;
}

/*------------------------------------------------------------
treshMess
------------------------------------------------------------*/
void pix_multiblob :: treshMess(t_float tresh){
  if((tresh < 0.0)||(tresh > 1.0))
    {
      error("treshold %f out of range (0..1)!", tresh);
    }
  m_treshold = CLAMP(tresh*255);
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_multiblob :: obj_setupCallback(t_class *classPtr){
  class_addmethod(classPtr, (t_method)&pix_multiblob :: blobSizeMessCallback,
		  gensym("blobSize"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_multiblob :: treshMessCallback,
		  gensym("tresh"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_multiblob :: treshMessCallback,
		  gensym("treshold"), A_FLOAT, A_NULL);
}

/*------------------------------------------------------------
blobSizeMessCallback
------------------------------------------------------------*/
void pix_multiblob :: blobSizeMessCallback(void *data, t_floatarg blobSize){
  GetMyClass(data)->blobSizeMess((t_float) blobSize);
}

/*------------------------------------------------------------
treshMessCallback
------------------------------------------------------------*/
void pix_multiblob :: treshMessCallback(void *data, t_floatarg tresh){
  GetMyClass(data)->treshMess((t_float) tresh);
}
