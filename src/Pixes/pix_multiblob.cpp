////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_multiblob
// based on (c) 2004, Jakob Leiner & Theresa Rienmüller
// and stack-based code from animal.sf.net which is
// (c) Ricardo Fabbri labmacambira.sf.net
//
//
/////////////////////////////////////////////////////////


#include <assert.h>
#include "pix_multiblob.h"
#include "Utils/Functions.h"
#include "Utils/pstk.h"

////////////////////////
// the Blob-structure
Blob::Blob() :
  area(0),
  m_xaccum(0), m_yaccum(0), m_xyaccum(0),
  m_11(0.), m_02(0.), m_20(0.),
  m_xmin(0.), m_xmax(0.),
  m_ymin(0.), m_ymax(0.)
{}

double Blob:: xmin() const
{
  return m_xmin;
}
double Blob:: xmax() const
{
  return m_xmax;
}
double Blob:: ymin() const
{
  return m_ymin;
}
double Blob:: ymax() const
{
  return m_ymax;
}
double Blob:: xmid() const
{
  return m_xaccum/m_xyaccum;
}
double Blob:: ymid() const
{
  return m_yaccum/m_xyaccum;
}
double Blob:: diameter2() const
{
  return (m_xmax-m_xmin)*(m_xmax-m_xmin)+(m_ymax-m_ymin)*(m_ymax-m_ymin);
}
double Blob:: diameter() const
{
  return sqrt(diameter2());
}
double Blob:: distance2(const Blob&b) const
{
  return (b.xmid()-xmid())*(b.xmid()-xmid())+(b.ymid()-ymid())*
         (b.ymid()-ymid());
}
double Blob:: distance(const Blob&b) const
{
  return sqrt(distance2(b));
}
void Blob:: xmin(double x)
{
  m_xmin=x;
}
void Blob:: xmax(double x)
{
  m_xmax=x;
}
void Blob:: ymin(double y)
{
  m_ymin=y;
}
void Blob:: ymax(double y)
{
  m_ymax=y;
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_multiblob,t_floatarg, A_DEFFLOAT);

/*------------------------------------------------------------

pix_multiblob

------------------------------------------------------------*/

/*------------------------------------------------------------

Constructor
initializes the pixBlocks and pixBlobs

------------------------------------------------------------*/
pix_multiblob :: pix_multiblob(t_floatarg f) :
  m_blobNumber(0),
  m_currentBlobs(NULL),
  m_blobsize(0.001),
  m_threshold(10),
  m_infoOut(NULL)
{
  // initialize image
  m_image.xsize=320;
  m_image.ysize=240;
  m_image.setCsizeByFormat(GEM_GRAY);
  m_image.allocate();

  // outlets
  m_infoOut = outlet_new(this->x_obj, &s_list);

  m_blobNumber = static_cast<int>(f);
  if(m_blobNumber < 1) {
    m_blobNumber = 6;
  }
  numBlobsMess(m_blobNumber);

}

/*------------------------------------------------------------

Destructor

------------------------------------------------------------*/
pix_multiblob :: ~pix_multiblob()
{
  outlet_free(m_infoOut);
  if(m_currentBlobs) {
    delete[]m_currentBlobs;
  }
}

/*------------------------------------------------------------
makeBlobs
calculates the Blobs, maximal x and y values are set

algorithm adapted from imfill from animal.sf.net
by Ricardo Fabbri (labmacambira.sf.net)
------------------------------------------------------------*/
void pix_multiblob :: makeBlob(Blob *pb, int x_ini, int y_ini)
{
  if(!pb) {
    return;
  }

  point *cp, np; // current pixel
  pstk_ptr current; // stack of current pixels

  point seed;
  seed.x = x_ini;
  seed.y = y_ini;
  current = new_pstk();
  ptpush(&current, &seed);
  do {
    cp = ptpop(&current);
    assert(cp);

    pb->area++;
    t_float grey=(static_cast<t_float>(m_image.GetPixel(cp->y, cp->x,
                                       chGray))/255.0);
    double x = static_cast<t_float>(cp->x);
    double y = static_cast<t_float>(cp->y);
    pb->m_xaccum  += grey*x;
    pb->m_yaccum  += grey*y;
    pb->m_xyaccum += grey;
    pb->m_11 += grey*x*y;
    pb->m_20 += grey*x*x;
    pb->m_02 += grey*y*y;

    if (cp->x < pb->xmin()) {
      pb->xmin(cp->x);
    }
    if (cp->x > pb->xmax()) {
      pb->xmax(cp->x);
    }
    if (cp->y < pb->ymin()) {
      pb->ymin(cp->y);
    }
    if (cp->y > pb->ymax()) {
      pb->ymax(cp->y);
    }

    m_image.SetPixel(cp->y,cp->x,chGray,0);
    for(int i = -1; i<= 1; i++) {
      for(int j = -1; j <= 1; j++) {
        np.x = cp->x + j;
        np.y = cp->y + i;

        if(np.x >= 0 && np.y >= 0 &&
            np.x < m_image.xsize && np.y < m_image.ysize &&
            m_image.GetPixel(np.y, np.x, chGray) > m_threshold ) {
          ptpush(&current, &np);
        }
      }
    }
    free(cp);
  } while (pstk_isnt_empty(current));

  free_pstk(&current);
}

/*------------------------------------------------------------
addToBlobArray
adds a detected Blob to the blob list
------------------------------------------------------------*/
void pix_multiblob :: addToBlobArray(Blob *pb, int blobNumber)
{
  if (blobNumber >= m_blobNumber) {
    // look whether we can replace a smaller blob
    float min = pb->area;
    int index=-1;
    int i = m_blobNumber;
    while(i--)
      if (m_currentBlobs[i].area < min) {
        min = m_currentBlobs[i].area;
        index = i;
      }
    if (index!=-1) {
      m_currentBlobs[index] = *pb;
    }
  } else {
    m_currentBlobs[blobNumber] = *pb;
  }
}

/*------------------------------------------------------------

render

------------------------------------------------------------*/
void pix_multiblob :: doProcessing(void)
{
  int blobNumber = 0;
  int blobsize = static_cast<int>(m_blobsize * m_image.xsize *
                                  m_image.ysize);

  // reset the currentblobs array


  // detect blobs and add them to the currentBlobs-array
  for(int y = 0; y < m_image.ysize; y++) {
    for(int x = 0; x < m_image.xsize; x++) {
      if (m_image.GetPixel(y,x,0) > 0) {
        Blob *blob = new Blob();
        if(0 == blob) {
          continue;
        }
        blob->xmin(m_image.xsize);
        blob->ymin(m_image.ysize);

        makeBlob(blob, x, y);
        if(blob->area > blobsize) {
          addToBlobArray(blob, blobNumber);
          blobNumber++;
        }
        delete blob;
      }
    }
  }

  // ok, we have found some blobs

  // since we can only handle m_blobNumber blobs, we might want to clip
  if(blobNumber > m_blobNumber) {
    blobNumber = m_blobNumber;
  }

  t_float scaleX = 1./m_image.xsize;
  t_float scaleY = 1./m_image.ysize;
  t_float scaleXY=scaleX*scaleY;

  // now create a matrix of [blobNumber*3] elements
  // each row holds all information on our blob
  t_atom*ap = new t_atom[2+blobNumber*9];
  if(ap) {
    SETFLOAT(ap, static_cast<t_float>(blobNumber));
    SETFLOAT(ap+1, 9.0);

    int bn=blobNumber;
    for(bn=0; bn<blobNumber; bn++) {
      SETFLOAT(ap+bn*9+2, m_currentBlobs[bn].xmid()*scaleX); // weighted X
      SETFLOAT(ap+bn*9+3, m_currentBlobs[bn].ymid()*scaleY); // weighted Y
      SETFLOAT(ap+bn*9+4, m_currentBlobs[bn].m_xyaccum*scaleXY); // weighted Area

      SETFLOAT(ap+bn*9+5, m_currentBlobs[bn].xmin()*scaleX); // minX
      SETFLOAT(ap+bn*9+6, m_currentBlobs[bn].ymin()*scaleY); // minY
      SETFLOAT(ap+bn*9+7, m_currentBlobs[bn].xmax()*scaleX); // maxX
      SETFLOAT(ap+bn*9+8, m_currentBlobs[bn].ymax()*scaleY); // maxY

      SETFLOAT(ap+bn*9+9, m_currentBlobs[bn].area*scaleXY);  // unweighted Area
      SETFLOAT(ap+bn*9+10,
               m_currentBlobs[bn].angle());      // weighted orientation
    }

    // i admit that it is naughty to use "matrix" from zexy/iemmatrix
    // but it is the best thing i can think of for 2-dimensional arrays
    outlet_anything(m_infoOut, gensym("matrix"), 2+9*blobNumber, ap);

    delete[]ap;
    ap=NULL;
  }
}

void pix_multiblob :: processImage(imageStruct &image)
{
  // store the image in greyscale
  // since the algorithm is destructive we do it in a sandbox...
  m_image.convertFrom(&image);
  doProcessing();
}


/*------------------------------------------------------------
blobSizeMess
------------------------------------------------------------*/
void pix_multiblob :: blobSizeMess(t_float blobSize)
{
  if((blobSize < 0.0)||(blobSize > 1.0)) {
    error("blobsize %f out of range (0..1)!", blobSize);
    return;
  }
  m_blobsize = blobSize/100.0;
}

/*------------------------------------------------------------
threshMess
------------------------------------------------------------*/
void pix_multiblob :: threshMess(t_float thresh)
{
  if((thresh < 0.0)||(thresh > 1.0)) {
    error("threshold %f out of range (0..1)!", thresh);
  }
  m_threshold = CLAMP(thresh*255);
}

/*------------------------------------------------------------
threshMess
------------------------------------------------------------*/
void pix_multiblob :: numBlobsMess(unsigned int blobs)
{
  if(m_currentBlobs) {
    delete[]m_currentBlobs;
  }

  // initialize blob-structures
  m_currentBlobs = new Blob[blobs];
  m_blobNumber=blobs;
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_multiblob :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "blobSize", blobSizeMess, t_float);
  CPPEXTERN_MSG1(classPtr, "thresh", threshMess, t_float);
  CPPEXTERN_MSG1(classPtr, "threshold", threshMess, t_float);

  CPPEXTERN_MSG1(classPtr, "blobs", numBlobsMess, unsigned int);
}
