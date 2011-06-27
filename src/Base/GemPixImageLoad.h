/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemPixImageLoad.h
       - code to load in and resize an image
       - part of GEM

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
 
#ifndef INCLUDE_GEMPIXIMAGELOAD_H_
#define INCLUDE_GEMPIXIMAGELOAD_H_

#include "Base/GemExportDef.h"

struct imageStruct;

#include <string>


// image2mem() reads an image file into memory
//   and a pointer to an imageStruct
//       NULL = failure
// 
//       format:
//    	  returns either GL_LUMINANCE or GL_RGBA
// 
//   automatically allocates the memory for the user
//
// This can read TIFF, SGI, and JPG images
//
namespace gem {
  struct Properties;
  namespace image {
    namespace load {
      /**
       * loads an image (given as 'filename') synchronously
       * the function blocks until the image is loaded (in which case it returns TRUE)
       * of the image-loading completely failed (in which case it returns FALSE)
       *
       * the loaded image is stored in 'img'
       * 'props' holds a list of additional image properties discovered during loading
       */
      GEM_EXTERN extern bool sync(const std::string filename,
				  imageStruct&img,
				  Properties&props);

      typedef unsigned int id_t;
      const id_t IMMEDIATE= 0;
      const id_t INVALID  =~0;


      /* the callback used for asynchronous image loading
       * userdata is is the pointer supplied when calling async();
       * id is the ID returned by async()
       * img holds a reference to the newly loaded image
       *  the image is allocated by the loder, but
       *  the callback (you!) is responsible for freeing the image
       *  once it is no more needed
       *  if image loading failed, img is set to NULL
       * props holds a list of additional image properties discovered during loading
       *
       * currently (with Pd being the only RTE),
       * the callback will always be called from within the main thread
       * 
       * the callback might be called directly from within async(),
       * in which case the ID given in the callback and returned by async() 
       * is IMMEDIATE
       */
      typedef void (*callback)(void *userdata, 
			       id_t ID,
			       imageStruct*img,
			       const Properties&props);

      /* loads an image (given as 'filename') asynchronously 
       * image loading is done in a separate thread (if possible);
       * when the image is loaded, the callback 'cb' is called with the new image
       *
       * this function returns an ID which is also passed to the callback function,
       * so the caller can identify a certain request (e.g. if several images have been
       * queued for loading before the 1st one was successfully returned;
       *
       * the image might get loaded (and the cb called) before the call to loadAsync()
       * has finished, in which case IMMEDIATE is returned (and used in the CB)
       *
       * if the image cannot be loaded at all, INVALID is returned
       * (and no callback will ever be called)
       *
       */
      GEM_EXTERN extern id_t async(callback cb,
				   void*userdata,
				   const std::string filename);
    
      /* cancels asynchronous loading of an image
       * removes the given ID (as returned by loadAsync()) from the loader queue
       * returns TRUE if item could be removed, or FALSE if no item ID is in the queue
       *
       * there is no point in cancel()ing an IMMEDIATE or ILLEGAL id
       */
      GEM_EXTERN extern bool cancel(id_t ID);


      GEM_EXTERN extern id_t sync(callback cb,
				  void*userdata,
				  const std::string filename);


};};};

GEM_EXTERN extern imageStruct *image2mem(const char *filename);


#endif
