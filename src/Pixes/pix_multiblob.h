/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Calculate the center of gravity of a pixBlock.

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
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

#ifndef INCLUDE_PIX_MULTIBLOB_H_
#define INCLUDE_PIX_MULTIBLOB_H_

#include "Base/GemPixObj.h"
#include <stdio.h>


class GEM_EXTERN Blob {
 public:

  Blob();
  void setPosition(int p);
  int  getPosition();
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

  // the squared distance to another blob
  double distance2(Blob b);
  // the distance to another blob
  double distance(Blob b);

  void xmin(double x);
  void xmax(double x);
  void ymin(double y);
  void ymax(double y);

  int area;

  bool valid; // 0=invalid; 1=ok;
  bool rightPosition;
 private :
  double m_xmin;
  double m_xmax;
  double m_ymin;
  double m_ymax;
  int position;
};

class GEM_EXTERN pix_multiblob : public GemPixObj
{
  CPPEXTERN_HEADER(pix_multiblob, GemPixObj)
		
    public:
  
  //////////
  // Constructor
  pix_multiblob(t_float f);

  // outlets for results
  t_outlet        *m_infoOut;

  

 protected:  
  //Destructor
  ~pix_multiblob();

  void processImage(imageStruct &image);
  void doProcessing();
  imageStruct m_image;

  int m_blobNumber;
  Blob *currentBlobs;
  void addToBlobArray(Blob *pblob, int blobNumber);
  void makeBlob(Blob *pb, int x, int y);

  // the minimum size of a blob (relative to the image)
  void blobSizeMess(t_float blobSize);
  t_float m_blobsize;
 private: 
  static void blobSizeMessCallback(void *data, t_floatarg blobSize);
};


#endif 	// for header file
