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
//    Copyright (c) 2002 James Tittle
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_movieNEW.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_movieNEW, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_movieNEW
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_movieNEW :: pix_movieNEW(t_symbol *filename) :
  pix_filmNEW(filename)
{
  // we don't want the additional outlet of [pix_texture]
  outlet_free(m_pixtexture.m_out1);  m_pixtexture.m_out1=NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_movieNEW :: ~pix_movieNEW()
{
  // Clean up the movie
  closeMess();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_movieNEW :: render(GemState *state)
{
  /* get the current frame from the file */
  if (!state || !m_handle)return;
  state->image=m_handle->getFrame();
  m_pixtexture.render(state);
  int frame=(int)m_reqFrame;
  if (state->image==0){
    outlet_float(m_outEnd,(m_numFrames>0 && (int)m_reqFrame<0)?(m_numFrames-1):0);
  }
  if (frame!=(int)m_reqFrame)render(state);//state->image=m_handle->getFrame();
}
/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_movieNEW :: postrender(GemState *state)
{
  if (state && state->image)state->image->newimage = 0;
  // automatic proceeding
  if (m_auto!=0){
    if (m_handle&&m_handle->changeImage((int)(m_reqFrame+=m_auto))==FILM_ERROR_FAILURE){
      //      m_reqFrame = m_numFrames;
      outlet_bang(m_outEnd);
    }
  }

  m_pixtexture.postrender(state);
}


/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_movieNEW :: startRendering()
{
  m_pixtexture.startRendering();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_movieNEW :: stopRendering()
{
  m_pixtexture.stopRendering();
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_movieNEW :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_movieNEW, gensym("pix_movie"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_movieNEW::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieNEW::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieNEW::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
 class_addmethod(classPtr, (t_method)&pix_movieNEW::textureMessCallback,
		  gensym("quality"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieNEW::repeatMessCallback,
		  gensym("repeat"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieNEW::modeCallback,
		  gensym("mode"), A_FLOAT, A_NULL);
}

void pix_movieNEW :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename);
}

void pix_movieNEW :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  //  GetMyClass(data)->changeImage((int)imgNum);
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_movieNEW :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
void pix_movieNEW :: textureMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->textureQuality((int)quality);
}
void pix_movieNEW :: repeatMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->repeatMess((int)quality);
}

void pix_movieNEW :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->modeMess((int)quality);
}

