////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "model.h"

CPPEXTERN_NEW_WITH_ONE_ARG(model, t_symbol *, A_DEFSYM)

  /////////////////////////////////////////////////////////
//
// model
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
model :: model(t_symbol *filename)
  : m_model(0), m_dispList(0), m_rescaleModel(1), m_smooth(90), m_flags(GLM_SMOOTH | GLM_TEXTURE)
{
  // make sure that there are some characters
  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
model :: ~model()
{
  cleanModel();
}

/////////////////////////////////////////////////////////
// cleanModel
//
/////////////////////////////////////////////////////////
void model :: cleanModel()
{
  if (m_dispList)
    {
      // destroy display list
      glDeleteLists(m_dispList, 1);
      m_dispList = 0;
    }
}

/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: materialMess(int material)
{
  if (!m_model) return;
  m_material = material;
  switch (material) {
  case 0:
    m_flags=GLM_SMOOTH | GLM_TEXTURE;
    break;
  default:
    m_flags=GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL;
  }
  buildList();
}

/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: textureMess(int state)
{
  if (!m_model) return;

  if (state)glmSpheremapTexture(m_model);
  else glmLinearTexture(m_model);
  buildList();
}

/////////////////////////////////////////////////////////
// smoothMess
//
/////////////////////////////////////////////////////////
void model :: smoothMess(t_float fsmooth)
{
  if (!m_model) return;
  if (fsmooth<0.)fsmooth=0.;
  else if (fsmooth>1) fsmooth=1.;
  m_smooth = fsmooth*180.;
  glmVertexNormals(m_model, m_smooth);
  buildList();
}

/////////////////////////////////////////////////////////
// rescaleMess
//
/////////////////////////////////////////////////////////
void model :: reverseMess(int reverse)
{
  if (!m_model) return;
  glmReverseWinding(m_model);
  buildList();
}
/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void model :: rescaleMess(int state)
{
  m_rescaleModel = state;
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void model :: openMess(t_symbol *filename)
{
  cleanModel();
    
  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  // read the object in
  m_model = glmReadOBJ(buf);

  if (!m_model) return;    

  // set the size to -1 to 1
  //
  if (m_rescaleModel)
    glmUnitize(m_model);

  // generate normals if this
  // object doesn't have them.
  //
  glmFacetNormals (m_model);
  glmVertexNormals(m_model, m_smooth);

  glmLinearTexture(m_model);

  buildList();
  this->setModified();
}

/////////////////////////////////////////////////////////
// buildList
//
/////////////////////////////////////////////////////////
void model :: buildList()
{
  if (!m_model) return;
  if (m_dispList)glDeleteLists(m_dispList, 1);

  //  m_flags = GLM_SMOOTH | GLM_MATERIAL;
  m_dispList = glmList(m_model, m_flags);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void model :: render(GemState *)
{
  if (!m_dispList)return;
  glCallList(m_dispList);
}

void model :: startRendering()
{
  // build a display list
  buildList();
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void model :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&model::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&model::rescaleMessCallback,
		  gensym("rescale"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&model::smoothMessCallback,
		  gensym("smooth"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&model::reverseMessCallback,
		  gensym("revert"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&model::materialMessCallback,
		  gensym("material"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&model::textureMessCallback,
		  gensym("texture"), A_FLOAT, A_NULL);


}
void model :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}
void model :: rescaleMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->rescaleMess((int)state);
}
void model :: smoothMessCallback(void *data, t_floatarg smooth)
{
  GetMyClass(data)->smoothMess(smooth);
}
void model :: reverseMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->reverseMess((int)state);
}
void model :: textureMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->textureMess((int)state);
}
void model :: materialMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->materialMess((int)state);
}
