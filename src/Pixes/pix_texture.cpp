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

#include "pix_texture.h"

#include "Base/GemPixUtil.h"

CPPEXTERN_NEW(pix_texture)

/////////////////////////////////////////////////////////
//
// pix_texture
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_texture :: pix_texture()
    	     : m_textureOnOff(1), m_textureQuality(GL_LINEAR),
               m_rebuildList(0), m_textureObj(0)
{
	m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_texture :: ~pix_texture()
{ }

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_texture :: setUpTextureState()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureQuality);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureQuality);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_texture :: render(GemState *state)
{
    if ( !state->image || !m_textureOnOff) return;

    state->texture = 1;
    
    glEnable(GL_TEXTURE_2D);

#ifdef GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
#elif GL_EXT_texture_object
    glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);
#else
    // can we build a display list?
    int creatingDispList = 0;
    if (state->image->newimage) m_rebuildList = 1;
    if (!state->inDisplayList && m_rebuildList)
    {
        glNewList(m_textureObj, GL_COMPILE_AND_EXECUTE);
        creatingDispList = 1;
    }
	setUpTextureState();
#endif    

#ifdef GL_VERSION_1_1
    if (state->image->newimage)
#elif GL_EXT_texture_object
    if (state->image->newimage)
#else
    if (m_rebuildList)
#endif
    {
		// if the size changed, then reset the texture
		if (state->image->image.csize != m_dataSize[0] ||
			state->image->image.xsize != m_dataSize[1] ||
			state->image->image.ysize != m_dataSize[2])
		{
			m_dataSize[0] = state->image->image.csize;
			m_dataSize[1] = state->image->image.xsize;
			m_dataSize[2] = state->image->image.ysize;

			glTexImage2D(GL_TEXTURE_2D, 0,
	    		state->image->image.csize,
	    		state->image->image.xsize,
	    		state->image->image.ysize, 0,
	    		state->image->image.format,
    			state->image->image.type,
    			state->image->image.data);
		}
		// the size is the same, so just use subimage
		else
		{
  			glTexSubImage2D(GL_TEXTURE_2D, 0,
	    		0, 0,							// position
	    		state->image->image.xsize,
	    		state->image->image.ysize,
	    		state->image->image.format,
    			state->image->image.type,
    			state->image->image.data);		
		}

#ifdef GL_VERSION_1_1
#elif GL_EXT_texture_object
#else
        if (creatingDispList)
        {
            glEndList();
//            m_rebuildList = 0;
        }
#endif
    }
#ifdef GL_VERSION_1_1
#elif GL_EXT_texture_object
#else
    else glCallList(m_textureObj);
#endif
	m_rebuildList = 0;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_texture :: postrender(GemState *state)
{
    if (!m_textureOnOff) return;

    state->texture = 0;

    glDisable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_texture :: startRendering()
{
#ifdef GL_VERSION_1_1
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
	setUpTextureState();
#elif GL_EXT_texture_object
    glGenTexturesEXT(1, &m_textureObj);
    glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);    
	setUpTextureState();
#else
    m_textureObj = glGenLists(1);
    m_rebuildList = 1;
#endif
	m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;

	if (!m_textureObj)
	{
		error("GEM: pix_texture: Unable to allocate texture object");
		return;
	}
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_texture :: stopRendering()
{
#ifdef GL_VERSION_1_1
    if (m_textureObj) glDeleteTextures(1, &m_textureObj);
#elif GL_EXT_texture_object
    if (m_textureObj) glDeleteTexturesEXT(1, &m_textureObj);
#else
    if (m_textureObj) glDeleteLists(m_textureObj, 1);
#endif
    m_textureObj = 0;
	m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
}

/////////////////////////////////////////////////////////
// textureOnOff
//
/////////////////////////////////////////////////////////
void pix_texture :: textureOnOff(int on)
{
    m_textureOnOff = on;
    setModified();
}

/////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_texture :: textureQuality(int type)
{
    if (type)
        m_textureQuality = GL_LINEAR;
    else
        m_textureQuality = GL_NEAREST;
    if (m_textureObj)
    {
#ifdef GL_VERSION_1_1
        glBindTexture(GL_TEXTURE_2D, m_textureObj);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureQuality);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#elif GL_EXT_texture_object
        glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureQuality);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#else
#endif
    }
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_texture :: obj_setupCallback(t_class *classPtr)
{
    class_addfloat(classPtr, (t_method)&pix_texture::floatMessCallback);    
    class_addmethod(classPtr, (t_method)&pix_texture::textureMessCallback,
    	    gensym("quality"), A_FLOAT, A_NULL);
}
void pix_texture :: floatMessCallback(void *data, float n)
{
    GetMyClass(data)->textureOnOff((int)n);
}
void pix_texture :: textureMessCallback(void *data, t_floatarg quality)
{
    GetMyClass(data)->textureQuality((int)quality);
}
