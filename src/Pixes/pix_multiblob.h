/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Calculate the center of gravity of a pixBlock.

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2011 Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
pix_multiblob
 tracks multiple blobs in one image

 LATER: split this object into 2 :
    - pix_multiblob (do the image-processing, unsorted!)
    - multiblob (ensure that blob3 of the last frame is blob3 f the current one...)

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_MULTIBLOB_H_
#define _INCLUDE__GEM_PIXES_PIX_MULTIBLOB_H_

#include "Base/GemPixObj.h"
#include <stdio.h>


class GEM_EXTERN Blob
{
public:

  Blob();
  double xmin();
  double xmax();
  double ymin();
  double ymax();

  double xmid();
  double ymid();
  // the squared diameter of the blob
  double diameter2();
  // the diamter
  double diameter();

  double angle()
  {
    double mm_11 = m_11 - xmid()*m_yaccum;
    double mm_02 = m_02 - ymid()*m_yaccum;
    double mm_20 = m_20 - xmid()*m_xaccum;
    return .5*atan2(2.*mm_11,(mm_20-mm_02));
  }

  // the angular orientation of the principal axis in radians
  double orientation();

  // the squared distance to another blob
  double distance2(Blob b);
  // the distance to another blob
  double distance(Blob b);

  void xmin(double x);
  void xmax(double x);
  void ymin(double y);
  void ymax(double y);

  // area:  moment M_00
  int area;
  // m_xaccum: moment M_10
  // m_yaccum: moment M_01
  double m_xaccum, m_yaccum, m_xyaccum;

  // raw moments for orientation calculation
  double m_11, m_02, m_20;

private :
  double m_xmin, m_xmax;
  double m_ymin, m_ymax;
};

class GEM_EXTERN pix_multiblob : public GemPixObj
{
  CPPEXTERN_HEADER(pix_multiblob, GemPixObj);

public:

  //////////
  // Constructor
  pix_multiblob(t_float f);

  //Destructor
  ~pix_multiblob(void);

  void processImage(imageStruct &image);
  void doProcessing(void);

  void addToBlobArray(Blob *pblob, int blobNumber);
  void makeBlob(Blob *pb, int x, int y);

  void numBlobsMess(unsigned int blobs);
  void blobSizeMess(t_float blobSize);
  void threshMess(t_float thresh);

protected:

  imageStruct m_image;

  int m_blobNumber;
  Blob *m_currentBlobs;

  // the minimum size of a blob (relative to the image)
  t_float m_blobsize;

  // the minimum value of a pixel to be within a blob
  unsigned char m_threshold;

  // outlets for results
  t_outlet        *m_infoOut;
};

#endif  // for header file
