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
//    Copyright (c) 2003 Daniel Heckenberg
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_snap2tex.h"

#include "Base/GemMan.h"
#include "Base/GemFuncUtil.h"

CPPEXTERN_NEW_WITH_GIMME(pix_snap2tex)

/////////////////////////////////////////////////////////
//
// pix_snap2tex
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_snap2tex :: pix_snap2tex(int argc, t_atom *argv)
  : m_textureOnOff(1), m_textureQuality(GL_LINEAR),
    m_texWidth(-1), m_texHeight(-1),
    m_oldWidth(-1), m_oldHeight(-1), m_textureObj(0)
{
  if (argc == 4)
    {
      m_x = (int)atom_getfloat(&argv[0]);
      m_y = (int)atom_getfloat(&argv[1]);
      m_width = (int)atom_getfloat(&argv[2]);
      m_height = (int)atom_getfloat(&argv[3]);
    }
  else if (argc == 2)
    {
      m_x = m_y = 0;
      m_width = (int)atom_getfloat(&argv[0]);
      m_height = (int)atom_getfloat(&argv[1]);
    }
  else if (argc == 0)
    {
      m_x = m_y = 0;
      m_width = m_height = -1;
    }
  else
    {
      error("GEM: pix_snap2tex: needs 0, 2, or 4 values");
      m_x = m_y = 0;
      m_width = m_height = 128;
    }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_size"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_snap2tex :: ~pix_snap2tex()
{
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: setUpTextureState()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureQuality);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureQuality);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/////////////////////////////////////////////////////////
// snapMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: snapMess()
{
  if ( !GemMan::windowExists() ) return;

  int width =(m_width <0)?m_w:m_width;
  int height=(m_height<0)?m_h:m_height;

  if (width <= 0 || height <= 0)
    {
      error("GEM: pix_snap2tex: Illegal size");
      return;
    }

  glEnable(GL_TEXTURE_2D);

#ifdef GL_VERSION_1_1
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
#elif GL_EXT_texture_object
  glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);
#else
  // can we build a display list?
  int creatingDispList = 0;
  glNewList(m_textureObj, GL_COMPILE_AND_EXECUTE);
  creatingDispList = 1;
  setUpTextureState();
#endif    

  // if the size changed, then reset the texture
  int x_2 = powerOfTwo(width);
  int y_2 = powerOfTwo(height);

  if (width != m_oldWidth || height != m_oldHeight) 
    {
      m_oldWidth = width;
      m_oldHeight = height;

      float m_xRatio = (float)width / (float)x_2;
      float m_yRatio = (float)height / (float)y_2;
		
      m_coords[0].s = 0.f;
      m_coords[0].t = 0.f;
		
      m_coords[1].s = m_xRatio;
      m_coords[1].t = 0.f;
		
      m_coords[2].s = m_xRatio;
      m_coords[2].t = m_yRatio;
		
      m_coords[3].s = 0.f;
      m_coords[3].t = m_yRatio;
    }

  if (x_2 != m_texWidth || y_2 != m_texHeight)	{
    m_texWidth = x_2;
    m_texHeight = y_2;
		
    glCopyTexImage2D(	GL_TEXTURE_2D, 0,
			GL_RGB,
			m_x, m_y,
			m_texWidth, m_texHeight, 
			0);

  } else
    {
      glCopyTexSubImage2D(GL_TEXTURE_2D, 0,
			  0, 0,
			  m_x, m_y,		// position
			  m_texWidth,
			  m_texHeight);		
    }

#ifdef GL_VERSION_1_1
#elif GL_EXT_texture_object
#else
  if (creatingDispList)
    {
      glEndList();
    }
#endif

  glDisable(GL_TEXTURE_2D);

}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: render(GemState *state)
{
  if (!m_textureOnOff) return;

  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;

  m_w = state->screenWidth;
  m_h = state->screenHeight;

  glEnable(GL_TEXTURE_2D);

#ifdef GL_VERSION_1_1
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
#elif GL_EXT_texture_object
  glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);
#else
  glCallList(m_textureObj)
#endif    
	
    }


/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: postrender(GemState *state)
{
  if (!m_textureOnOff) return;

  state->texture = 0;
  state->numTexCoords = 0;	
  state->texCoords = NULL;	

  glDisable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: startRendering()
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
  m_oldWidth = m_oldHeight = m_texWidth = m_texHeight = -1;
  if (!m_textureObj)	{
    error("Gem: pix_snap2tex: Unable to allocate texture object");
    return;
  }

}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: stopRendering()
{
#ifdef GL_VERSION_1_1
  if (m_textureObj) glDeleteTextures(1, &m_textureObj);
#elif GL_EXT_texture_object
  if (m_textureObj) glDeleteTexturesEXT(1, &m_textureObj);
#else
  if (m_textureObj) glDeleteLists(m_textureObj, 1);
#endif
  m_textureObj = 0;
  m_oldWidth = m_oldHeight = m_texWidth = m_texHeight = -1;
}


/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: sizeMess(int width, int height)
{
  m_width = width;
  m_height = height;
  setModified();
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: posMess(int x, int y)
{
  m_x = x;
  m_y = y;
  setModified();
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: cleanImage()
{
  // release previous data
  post("pix_snap2tex2tex: clean is unimplemented.");
  setModified();
}

/////////////////////////////////////////////////////////
// textureOnOff
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: textureOnOff(int on)
{
  m_textureOnOff = on;
  setModified();
}

/////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: textureQuality(int type)
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
void pix_snap2tex :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_snap2tex::snapMessCallback,
		  gensym("snap"), A_NULL);
  class_addbang(classPtr, (t_method)&pix_snap2tex::snapMessCallback);

  class_addmethod(classPtr, (t_method)&pix_snap2tex::sizeMessCallback,
		  gensym("vert_size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_snap2tex::posMessCallback,
		  gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);

  class_addfloat(classPtr, (t_method)&pix_snap2tex::floatMessCallback);    
  class_addmethod(classPtr, (t_method)&pix_snap2tex::textureMessCallback,
		  gensym("quality"), A_FLOAT, A_NULL);

}
void pix_snap2tex :: snapMessCallback(void *data)
{
  GetMyClass(data)->snapMess();
}
void pix_snap2tex :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess((int)width, (int)height);
}
void pix_snap2tex :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->posMess((int)x, (int)y);
}
void pix_snap2tex :: floatMessCallback(void *data, float n)
{
  GetMyClass(data)->textureOnOff((int)n);
}
void pix_snap2tex :: textureMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->textureQuality((int)quality);
}
