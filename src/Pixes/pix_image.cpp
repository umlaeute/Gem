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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_image.h"

#ifdef unix
#include <unistd.h>
#include <strings.h>
#endif

#include "Base/GemCache.h"

static inline int powerOfTwo(int value)
{
    int x = 1;
    while(x < value) x <<= 1;
    return(x);
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_image, t_symbol *, A_DEFSYM)

pix_image::singleImageCache *pix_image::s_imageCache = NULL;

/////////////////////////////////////////////////////////
//
// pix_image
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_image :: pix_image(t_symbol *filename)
    	   : m_loadedImage(NULL)
{
    // make sure that there are some characters
    if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_image :: ~pix_image()
{
    cleanImage();
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_image :: openMess(t_symbol *filename)
{
    cleanImage();
    
    // is the image already loaded?
    singleImageCache *cache = s_imageCache;
    int found = 0;
    while (!found && cache)
    {
        if (!strcmp(filename->s_name, cache->imageName)) found = 1;
        else cache = cache->next;
    }
    
    // yep, we have it
    if (found)
    {
        m_loadedImage = cache;
        m_loadedImage->refCount++;
        strcpy(m_filename, filename->s_name);
        copy2Image(&(m_pixBlock.image), m_loadedImage->image);
        m_pixBlock.newimage = 1;
        if (m_cache) m_cache->resendImage = 1;
        return;
    }
    
    singleImageCache *newCache = new singleImageCache(filename->s_name);

    char buf[MAXPDSTRING];
    canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

    if ( !(newCache->image = image2mem(buf)) )
    {
        delete newCache;
        return;
    }

    strcpy(m_filename, filename->s_name);
    m_loadedImage = newCache;
    m_loadedImage->refCount++;

    // insert the image into the cache
    singleImageCache *ptr = s_imageCache;
    if (!ptr) s_imageCache = m_loadedImage;
    else
    {
        while(ptr->next) ptr = ptr->next;
        ptr->next = m_loadedImage;
    }

    copy2Image(&(m_pixBlock.image), m_loadedImage->image);
    m_pixBlock.newimage = 1;
    post("GEM: loaded image: %s", buf);
	if (powerOfTwo(m_pixBlock.image.xsize) != m_pixBlock.image.xsize ||
		powerOfTwo(m_pixBlock.image.ysize) != m_pixBlock.image.ysize)
	{
		error("GEM: pix_image: image size not a power of 2: %s", buf);
	}
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_image :: render(GemState *state)
{
    // if we don't have an image, just return
    if (!m_loadedImage) return;
    
    // do we need to reload the image?    
    if (m_cache->resendImage)
    {
	    refreshImage(&(m_pixBlock.image), m_loadedImage->image);
    	m_pixBlock.newimage = 1;
    	m_cache->resendImage = 0;
    }
    
    state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_image :: postrender(GemState *state)
{
    m_pixBlock.newimage = 0;
    state->image = NULL;
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_image :: startRendering()
{
    if (!m_loadedImage) return;

    refreshImage(&(m_pixBlock.image), m_loadedImage->image);
    m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_image :: cleanImage()
{
    // release previous data
    if (m_loadedImage)
    {
	    m_loadedImage->refCount--;
        if (m_loadedImage->refCount == 0)
        {
            // find the cache
            singleImageCache *ptr = s_imageCache;
            if (ptr == m_loadedImage)
            {
                s_imageCache = m_loadedImage->next;
                delete m_loadedImage;
            }
            else
            {
                while (ptr && ptr->next != m_loadedImage) ptr = ptr->next;
                if (!ptr) error("GEM: pix_image: Unable to find image cache!");
                else
                {
                    ptr->next = m_loadedImage->next;
                    delete m_loadedImage;
                }
            }
        }
	    m_loadedImage = NULL;

    	delete [] m_pixBlock.image.data;
        m_pixBlock.image.data = NULL;
    }
}

/////////////////////////////////////////////////////////
// clickMess
//
/////////////////////////////////////////////////////////
void pix_image :: clickMess()
{
    if (!m_loadedImage) return;

    // fork to display an image
#ifdef __sgi
    if (fork() == 0)
    {
    	execlp("/usr/sbin/imgview", "/usr/sbin/imgview", m_filename, NULL);
    	exit(1);  	
    }
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_image :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_image::openMessCallback,
    	    gensym("open"), A_SYMBOL, A_NULL);
#ifdef __sgi
    class_addmethod(classPtr, (t_method)&pix_image::clickMessCallback,
    	    gensym("click"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
#endif
}
void pix_image :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename);
}
void pix_image :: clickMessCallback(void *data, t_floatarg , t_floatarg , t_floatarg )
{
    GetMyClass(data)->clickMess();
}
