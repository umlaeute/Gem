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
//    Copyright (c) 2002-2003 james tittle/tigital
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"
#include "Base/GemGL.h"

#include "GemPixImageLoad.h"
#include "GemPixUtil.h"

#include "m_pd.h"




#include "plugins/imageloader.h"
namespace gem {
  class PixImageLoader : public gem::plugins::imageloader {
  private:
    static PixImageLoader*s_instance;
    std::vector<gem::plugins::imageloader*>m_loaders;
    std::vector<std::string>m_ids;

    PixImageLoader(void) {
      gem::PluginFactory<gem::plugins::imageloader>::loadPlugins("image");
      std::vector<std::string>available_ids=gem::PluginFactory<gem::plugins::imageloader>::getIDs();
      if(available_ids.size()>0) {
	startpost("Image loading support:");
	int i;
	for(i=0; i<available_ids.size(); i++) {
	  startpost(" %s", available_ids[i].c_str());
	}
	endpost();
      }

      addLoader(available_ids, "magick");
      addLoader(available_ids);
    }
    bool addLoader( std::vector<std::string>available, std::string ID=std::string(""))
    {
      int i=0;
      int count=0;

      std::vector<std::string>id;
      if(!ID.empty()) {
	// if requested 'cid' is in 'available' add it to the list of 'id's
	if(std::find(available.begin(), available.end(), ID)!=available.end()) {
	  id.push_back(ID);
	} else {
	  // request for an unavailable ID
	  verbose(2, "backend '%s' unavailable", ID.c_str());
	  return false;
	}
      } else {
	// no 'ID' given: add all available IDs
	id=available;
      }

      for(i=0; i<id.size(); i++) {
	std::string key=id[i];
	verbose(2, "trying to add '%s' as backend", key.c_str());
	if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
	  // not yet added, do so now!
	  gem::plugins::imageloader*loader=
	    gem::PluginFactory<gem::plugins::imageloader>::getInstance(key); 
	  if(NULL==loader)break;
	  m_ids.push_back(key);
	  m_loaders.push_back(loader);
	  count++;
	  verbose(2, "added backend#%d '%s' @ 0x%x", m_loaders.size()-1, key.c_str(), loader);
	}
      }
      return (count>0);
    }

  public:
    virtual ~PixImageLoader(void) {
      int i;
      for(i=0; i<m_loaders.size(); i++) {
	delete m_loaders[i];
	m_loaders[i]=NULL;
      }
    }

    virtual bool load(std::string filename, imageStruct&result) {
      int i;
      for(i=0; i<m_loaders.size(); i++) {
	if(m_loaders[i]->load(filename, result))
	  return true;
      }
      return false;
    }

    static PixImageLoader*getInstance(void) {
      if(NULL==s_instance) {
	s_instance=new PixImageLoader();
      }
      return s_instance;
    }

}; };

gem::PixImageLoader*gem::PixImageLoader::s_instance=NULL;



#if defined __APPLE__ && !defined __x86_64__
// with OSX10.6, apple has removed loads of Carbon functionality (in 64bit mode)
// LATER make this a real check in configure
# define HAVE_CARBONQUICKTIME
#endif

#ifdef _WIN32
# include <io.h>
# define close _close
#else
# include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#ifdef HAVE_CARBONQUICKTIME
# include <Carbon/Carbon.h>
# include <QuickTime/QuickTime.h>
imageStruct *QTImage2mem(const char *filename);
#endif


#include "sgiimage.h"
imageStruct *sgiImage2mem(const char *filename);


/***************************************************************************
 *
 * image2mem - Read in an image in various file formats
 *
 ***************************************************************************/
GEM_EXTERN imageStruct *image2mem(const char *filename)
{
   imageStruct *image_block = NULL;
   gem::PixImageLoader*piximageloader=gem::PixImageLoader::getInstance();
   if(piximageloader) {
     std::string fname=filename;
     image_block=new imageStruct();
     if(piximageloader->load(filename, *image_block)) {
       return image_block;
     }
   }
   return NULL;



   char newName[256];

   ::verbose(2, "image2mem(%s)", filename);

   // does the file even exist?
   if (filename[0] == '/' || filename[0] == '\\')
   {
      strcpy(newName, filename);
   }
   else
   {
     char realName[256];
     char *realResult;
     
     int fd = open_via_path(".", filename, "", realName, &realResult, 256, 1);
     if (fd < 0)
       {
         error("GemImageLoad: Unable to find file: %s", filename);
         return(NULL);
       }
     else
       {
         int res = close(fd);
         if (res == -1)
      {
        verbose(1, "GemImageLoad: Unable to close file-handle %d for '%s'", fd, filename);
      }
       }
     
     sprintf(newName, "%s/%s", realName, realResult);
   }

# ifdef HAVE_CARBONQUICKTIME
   // try to load via QuickTime
   if ( (image_block = QTImage2mem(newName)) )
         return(image_block);
# endif

   // try to load in an SGI file
   if ( (image_block = sgiImage2mem(newName)) )
         return(image_block);
   // unable to load image
   return(NULL);
}

/***************************************************************************
 *
 * Read in a image utilizing QuickTime GraphicsImporterComponent
 *
 ***************************************************************************/
#ifdef HAVE_CARBONQUICKTIME
/*****************************************************************************/

imageStruct *QuickTimeImage2mem(GraphicsImportComponent inImporter)
{
   Rect      r;
   if (::GraphicsImportGetNaturalBounds(inImporter, &r)) return NULL;   //get an image size
   ::OffsetRect(&r, -r.left, -r.top);                           
   if (::GraphicsImportSetBoundsRect(inImporter, &r)) return NULL;      
   ImageDescriptionHandle imageDescH = NULL;
   if (::GraphicsImportGetImageDescription(inImporter, &imageDescH)) return NULL;
      
   imageStruct *image_block = new imageStruct;
   image_block->xsize   = (*imageDescH)->width;
   image_block->ysize   = (*imageDescH)->height;

        OSType pixelformat = 0;

       /* afaik, there is no 8bit grayscale format....
        * and even if it was, k8GrayPixelFormat would not be a define...
        */
#ifdef k8GrayPixelFormat
       /* from the docs on "depth": what depth is this data (1-32) or ( 33-40 grayscale ) */
   if ((*imageDescH)->depth <= 32) {
       image_block->setCsizeByFormat(GL_RGBA_GEM);
            pixelformat = k32ARGBPixelFormat;
   } else {
       image_block->setCsizeByFormat(GL_LUMINANCE);
            pixelformat = k8GrayPixelFormat;
   }
#else
   image_block->setCsizeByFormat(GL_RGBA_GEM);
        pixelformat = k32ARGBPixelFormat;
#endif

	::DisposeHandle(reinterpret_cast<Handle>(imageDescH));
   imageDescH = NULL;
  image_block->allocate();

#ifdef __DEBUG__
   post("QuickTimeImage2mem() : allocate %d bytes", image_block->xsize*image_block->ysize*image_block->csize);
#endif
        GWorldPtr   gw = NULL;

   OSErr err = QTNewGWorldFromPtr(&gw,
                                 /* taken from pix_filmDarwin */
                                 pixelformat,   // gives noErr
                                 &r, NULL, NULL, 0,
                                 // keepLocal,   
                                 //useDistantHdwrMem, 
                                 image_block->data, 
                                 static_cast<long>(image_block->xsize * image_block->csize));
   if (image_block->data == NULL || err) {
      error("Can't allocate memory for an image.");
   }
   ::GraphicsImportSetGWorld(inImporter, gw, NULL);
   ::GraphicsImportDraw(inImporter);
        ::DisposeGWorld(gw);         //dispose the offscreen
   gw = NULL;

   return image_block;
}


OSStatus
FSPathMakeFSSpec(
   const UInt8 *path,
   FSSpec *spec,
   Boolean *isDirectory)   /* can be NULL */
{
   OSStatus   result;
   FSRef      ref;
   
   /* check parameters */
   require_action(NULL != spec, BadParameter, result = paramErr);
   
   /* convert the POSIX path to an FSRef */
   result = FSPathMakeRef(path, &ref, isDirectory);
   require_noerr(result, FSPathMakeRef);
   
   /* and then convert the FSRef to an FSSpec */
   result = FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, spec, NULL);
   require_noerr(result, FSGetCatalogInfo);
   
FSGetCatalogInfo:
FSPathMakeRef:
BadParameter:

   return ( result );
}

imageStruct *QTImage2mem(const char *filename)
{
   OSErr            err;
   imageStruct          *image_block = NULL;
   GraphicsImportComponent    importer = NULL;

   ::verbose(2, "reading '%s' with QuickTime", filename);

   // does the file even exist?
   if (filename[0] != '\0') {
      FSSpec   spec;

      err = ::FSPathMakeFSSpec( reinterpret_cast<const UInt8*>(filename), &spec, NULL);
      if (err) {
         error("GemImageLoad: Unable to find file: %s", filename);
         error("parID : %d", spec.parID); 
         return NULL;
      }
      err = ::GetGraphicsImporterForFile(&spec, &importer);
      if (err) {
         error("GemImageLoad: Unable to import an image: %#s", spec.name);
         return NULL;
      }
   }
   image_block = QuickTimeImage2mem(importer);
   ::CloseComponent(importer);
   if (image_block)   return image_block;
   else         return NULL;
}
#endif /* HAVE_CARBONQUICKTIME */

/***************************************************************************
 *
 * Read in an SGI image.
 *
 ***************************************************************************/
imageStruct *sgiImage2mem(const char *filename)
{
   int32 xsize, ysize, csize;
   if (!sizeofimage(filename, &xsize, &ysize, &csize) )
      return(NULL);

   ::verbose(2, "reading '%s' with SGI", filename);

   imageStruct *image_block = new imageStruct;
   image_block->type  = GL_UNSIGNED_BYTE;
   image_block->xsize = xsize;
   image_block->ysize = ysize;

   if (csize == 4 || csize == 3)
   {
      image_block->csize = 4;
      image_block->format = GL_RGBA;
   }
   else if (csize == 1)
   {
      image_block->csize = 1;
      image_block->format = GL_LUMINANCE;
   }
   else
   {
      //error("GemImageLoad(SGI): unknown color components in SGI file: %s", filename);
      delete image_block;
      return(NULL);
   }
   
   unsigned int32 *readData = longimagedata((char *)filename);
   if (!readData)
   {
      //error("GemImageLoad: error reading SGI image file: %s", filename);
      delete image_block;
      return(NULL);
   }

   image_block->allocate();
   unsigned char *src = reinterpret_cast<unsigned char*>(readData);
   unsigned char *dst = &(image_block->data[0]);
   const int yStride = image_block->xsize * image_block->csize;

   // do RGBA data
   if (csize == 4)
   {
      while (ysize--)
      {
            unsigned char *pixels = dst;
         int count = xsize;
         while(count--)
         {
            pixels[chRed]   = src[0];
            pixels[chGreen] = src[1];
            pixels[chBlue]  = src[2];
            pixels[chAlpha] = src[3];
            pixels += 4;
            src += 4;
         }
            dst += yStride;
      }
   }
   // do RGB data
   else if (csize == 3)
   {
      while (ysize--)
      {
	unsigned char *pixels = dst;
         int count = xsize;
         while(count--)
         {
            pixels[chRed]   = src[0];
            pixels[chGreen] = src[1];
            pixels[chBlue]  = src[2];
            pixels[chAlpha] = 255;;
            pixels += 4;
            src += 4;
         }
            dst += yStride;
      }
   }
   // do grayscale
   else
   {
      while (ysize--)
      {
	unsigned char *pixels = dst;
         int count = xsize;
         while(count--)
         {
            pixels[0] = src[0];
            pixels++;
            src += 4;
         }
            dst += yStride;
      }
   }

   free(readData);
   
   return(image_block);
}
