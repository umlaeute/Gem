////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemOutput.h"
#include "GemMan.h"

#ifdef  __APPLE__
// or is it spelled Carbon/Carbon.h ??
# include <Carbon/Carbon.h>
#include <OpenGL/glu.h>
# include <OpenGL/glext.h>
#include "macosx_math.h"
extern bool HaveValidContext (void);
#else
#include <GL/glu.h>
#include <math.h>
#endif // OS

/////////////////////////////////////////////////////////
//
// GemOutput
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemOutput :: GemOutput() : m_outputState(0), m_outputContext(false),
			   m_clear_mask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
			   m_fogMode(FOG_OFF),
			   m_buffer(2),
			   m_width(500), m_height(500),
			   m_stereo(0), m_stereoSep(-15.f), m_stereoFocal(0.f), m_stereoLine(true)
{

  m_outlet = outlet_new(this->x_obj, 0);

  // setup default values
  perspectiveMess(-1.f, 1.f, -1.f, 1.f, 1.f, 20.f);
  viewMess       (0.f, 0.f, 4.f,  0.f, 0.f, 0.f,  0.f, 1.f, 0.f);

  colorMess      (0.f, 0.f, 0.f, 0.f);
  ambientMess    (0.f, 0.f, 0.f, 1.f);
  specularMess   (1.f, 1.f, 1.f, 1.f);
  shininessMess  (100.f);
  lightMess      (0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemOutput :: ~GemOutput()
{
  outlet_free(m_outlet);
}


/////////////////////////////////////////////////////////
// infoMess
//
/////////////////////////////////////////////////////////
void GemOutput :: infoMess() {
  // fill this with information you want...  
  t_atom ap[2];
  SETFLOAT(ap, (t_float)m_width);
  SETFLOAT(ap+1, (t_float)m_height);
  outlet_anything(m_outlet, gensym("dimen"), 2, ap);
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void GemOutput :: doRender(GemState currentState){
  resetState();

  GemMan::render1(currentState);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 4,
	    0, 0, 0,
	    0, 1, 0);

  GemMan::render2(currentState);
}

void GemOutput :: fillGemState(GemState &state){
  //GemMan::fillGemState(currentState);
  if(m_lightState){
    state.lighting=1;
    state.smooth  =1;
  }
  state.screenWidth = m_width;
  state.screenHeight= m_height;
}

void GemOutput :: renderMess()
{
  if(!m_outputState)return;

  GemState currentState;
  fillGemState(currentState);

  makeCurrent();
  if(m_buffer==1)return;

  doRender(currentState);
  postRender(currentState);
}


/////////////////////////////////////////////////////////
// viewMess
//
/////////////////////////////////////////////////////////
void GemOutput :: viewMess(t_float eyeX,    t_float eyeY,    t_float eyeZ, 
			   t_float centerX, t_float centerY, t_float centerZ, 
			   t_float upX,     t_float upY,     t_float upZ)
{
  m_lookat[0]=eyeX;
  m_lookat[1]=eyeY;
  m_lookat[2]=eyeZ;

  m_lookat[3]=centerX;
  m_lookat[4]=centerY;
  m_lookat[5]=centerZ;

  m_lookat[6]=upX;
  m_lookat[7]=upY;
  m_lookat[8]=upZ;
}
/////////////////////////////////////////////////////////
// perspectiveMess
//
/////////////////////////////////////////////////////////
void GemOutput :: perspectiveMess(t_float left,   t_float right,
				  t_float bottom, t_float top, 
				  t_float front,  t_float back)
{
  m_perspect[0]=left;
  m_perspect[1]=right;

  m_perspect[2]=bottom;
  m_perspect[3]=top;

  m_perspect[4]=front;
  m_perspect[5]=back;
}


/////////////////////////////////////////////////////////
// lightMess
//
/////////////////////////////////////////////////////////
void GemOutput :: lightMess(int on)
{
  m_lightState=(on!=0);
}

/////////////////////////////////////////////////////////
// colorMess
//
/////////////////////////////////////////////////////////
void GemOutput :: colorMess(t_float r, t_float g, t_float b, t_float a)
{
#ifdef __APPLE__
  if (HaveValidContext ())
#endif
  glClearColor(r, g, b, a);
  m_clear_color[0] = r;
  m_clear_color[1] = g;
  m_clear_color[2] = b; 
  m_clear_color[3] = a; 

}
/////////////////////////////////////////////////////////
// clearmaskMess
//
/////////////////////////////////////////////////////////
void GemOutput :: clearmaskMess(float bitmask)
{
  m_clear_mask = (GLbitfield) bitmask;
}

/////////////////////////////////////////////////////////
// ambientMess
//
/////////////////////////////////////////////////////////
void GemOutput :: ambientMess(float r, float g, float b, t_float a)
{
  m_mat_ambient[0] = r;
  m_mat_ambient[1] = g;
  m_mat_ambient[2] = b;
  m_mat_ambient[3] = a;

}

/////////////////////////////////////////////////////////
// specularMess
//
/////////////////////////////////////////////////////////
void GemOutput :: specularMess(float r, float g, float b, t_float a)
{
  m_mat_specular[0] = r;
  m_mat_specular[1] = g;
  m_mat_specular[2] = b;
  m_mat_specular[3] = a;

}

/////////////////////////////////////////////////////////
// shininessMess
//
/////////////////////////////////////////////////////////
void GemOutput :: shininessMess(float val)
{
  m_mat_shininess = val;
}

/////////////////////////////////////////////////////////
// fogDensityMess
//
/////////////////////////////////////////////////////////
void GemOutput :: fogDensityMess(float val)
{
  if (val < 0.f)
    val = 0.f;

  m_fog = val;
}

/////////////////////////////////////////////////////////
// fogRangeMess
//
/////////////////////////////////////////////////////////
void GemOutput :: fogRangeMess(float start, float end)
{
  if (start < 0.f)
    start= 0.f;

  if (end < 0.f)
    end  = 0.f;

  m_fogStart = start;
  m_fogEnd = end;
}

/////////////////////////////////////////////////////////
// fogColorMess
//
/////////////////////////////////////////////////////////
void GemOutput :: fogColorMess(float red, float green, float blue, float alpha)
{
  m_fogColor[0] = red;
  m_fogColor[1] = green;
  m_fogColor[2] = blue;
  m_fogColor[3] = alpha;
}

/////////////////////////////////////////////////////////
// fogModeMess
//
/////////////////////////////////////////////////////////
void GemOutput :: fogModeMess(int mode)
{
  switch (mode)
    {
    case 0 :
      m_fogMode = FOG_OFF;
      break;
    case 1 :
      m_fogMode = FOG_LINEAR;
      break;
    case 2 :
      m_fogMode = FOG_EXP;
      break;
    case 3 :
      m_fogMode = FOG_EXP2;
      break;
    default :
      error("GEM : fogmode must be 0, 1, 2 or 3");
      break;
    }
}




/////////////////////////////////////////////////////////
// reset openGL state
//
/////////////////////////////////////////////////////////
void GemOutput :: resetState()
{
  if (m_lightState)
    {
      glEnable(GL_LIGHTING);
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
      glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_mat_ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_mat_specular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_mat_shininess);
      glEnable(GL_AUTO_NORMAL);
      glEnable(GL_NORMALIZE);
      glShadeModel(GL_SMOOTH);
    }
  else
    {
      glDisable(GL_LIGHTING);
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_AUTO_NORMAL);
      glDisable(GL_NORMALIZE);
      glShadeModel(GL_FLAT);
    }

  // setup the transformation matrices
  float xDivy = (float)m_width / (float)m_height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(m_perspect[0] * xDivy, m_perspect[1] * xDivy,	// left, right
	    m_perspect[2], m_perspect[3],				// bottom, top
	    m_perspect[4], m_perspect[5]);				// front, back
    
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(m_lookat[0], m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
	    m_lookat[5], m_lookat[6], m_lookat[7], m_lookat[8]);

  if (m_fogMode == FOG_OFF) {
    glDisable(GL_FOG);
  } else {
    glEnable(GL_FOG);
    switch (m_fogMode)  {
    case (FOG_LINEAR):
      glFogf(GL_FOG_MODE, GL_LINEAR);
      break;
    case (FOG_EXP):
      glFogf(GL_FOG_MODE, GL_EXP);
      break;
    case (FOG_EXP2):
      glFogf(GL_FOG_MODE, GL_EXP2);
      break;
    case (FOG_OFF):
        glDisable(GL_FOG);
        break;
    }
    glFogf(GL_FOG_DENSITY, m_fog);
    glFogf(GL_FOG_START, m_fogStart);
    glFogf(GL_FOG_END, m_fogEnd);
    glFogfv(GL_FOG_COLOR, m_fogColor);
  }

  glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
}

/////////////////////////////////////////////////////////
// resetValues
//
/////////////////////////////////////////////////////////
void GemOutput :: resetValues()
{
#ifdef DEBUG
  post("GemOutput::resetState entered");
#endif
  colorMess(0.0, 0.0, 0.0, 0.0);
  m_clear_mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

  ambientMess(0.1f, 0.1f, 0.1f, 1.0f);
  specularMess(1.0, 1.0, 1.0, 1.0);
  shininessMess(100.0);

  m_lightState = 0;

  m_buffer = 2;

  m_stereo=0;
  m_stereoSep = -15.f;
  m_stereoFocal = 0.f;
  m_stereoLine = true;

  // setup the perspective values
  perspectiveMess(-1.f, 1.f, -1.f, 1.f, 1.f, 20.f);
  // setup the lookat values
  viewMess(0.f, 0.f, 4.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f);

  // setup the fog
  m_fogMode	= FOG_OFF;
  m_fogStart	= 1.f;
  m_fogEnd	= 20.f;
  m_fog		= 0.5f;
  fogColorMess(1.f, 1.f, 1.f, 1.f);
}

/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void GemOutput :: bufferMess(int buf)
{
  if (buf == 1)
    m_buffer = 1;
  else
    m_buffer = 2;
}





/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemOutput :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&GemOutput::renderMessCallback,
		  gensym("gem_render"), A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::resetMessCallback,
		  gensym("reset"), A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::infoMessCallback,
		  gensym("info"), A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::bufferMessCallback,
		  gensym("buffer"), A_FLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&GemOutput::perspectiveMessCallback,
		  gensym("perspec"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::viewMessCallback,
		  gensym("view"), A_GIMME, A_NULL);

  class_addmethod(classPtr, (t_method)&GemOutput::lightMessCallback,
		  gensym("lighting"), A_FLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&GemOutput::colorMessCallback,
		  gensym("color"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::clearmaskMessCallback,
		  gensym("clearmask"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::ambientMessCallback,
		  gensym("ambient"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::specularMessCallback,
		  gensym("specular"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::shininessMessCallback,
		  gensym("shininess"), A_FLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&GemOutput::fogMessCallback,
		  gensym("fog"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::fogColorMessCallback,
		  gensym("fogcolor"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::fogModeMessCallback,
		  gensym("fogmode"), A_FLOAT, A_NULL);
}
void GemOutput :: renderMessCallback(void *data)
{
  GetMyClass(data)->renderMess();
}
void GemOutput :: resetMessCallback(void *data)
{
  GetMyClass(data)->resetValues();
}
void GemOutput :: infoMessCallback(void *data)
{
  GetMyClass(data)->infoMess();
}
void GemOutput :: bufferMessCallback(void *data, t_floatarg val){
  GetMyClass(data)->bufferMess((int)val);
}
void GemOutput :: lightMessCallback(void *data, t_floatarg val){
  GetMyClass(data)->lightMess((int)val);
}
void GemOutput :: colorMessCallback(void *data, t_symbol*, int argc, t_atom*argv){
  GLfloat r, g, b, a;
  a=0;
  switch (argc){
  case 1:
    r=g=b=atom_getfloat(argv);
    break;
  case 4:
    a=atom_getfloat(argv+3);
  case 3:
    b=atom_getfloat(argv+2);
    g=atom_getfloat(argv+1);
    r=atom_getfloat(argv+0);
    break;
  default:
    error("GEM: GemOutput: color needs 1, 3 or 4 arguments");
    return;
  }
  GetMyClass(data)->colorMess(r, g, b, a);
}
void GemOutput :: clearmaskMessCallback(void *data, t_floatarg bitmask){
  GetMyClass(data)->clearmaskMess((float)bitmask);
}
void GemOutput :: ambientMessCallback(void *data,t_symbol*, int argc, t_atom*argv){
  GLfloat r, g, b, a;
  a=1;
  switch (argc){
  case 1:
    r=g=b=atom_getfloat(argv);
    break;
  case 4:
    a=atom_getfloat(argv+3);
  case 3:
    b=atom_getfloat(argv+2);
    g=atom_getfloat(argv+1);
    r=atom_getfloat(argv+0);
    break;
  default:
    error("GEM: GemOutput: color needs 1, 3 or 4 arguments");
    return;
  }
  GetMyClass(data)->ambientMess(r, g, b, a);
}
void GemOutput :: specularMessCallback(void *data,t_symbol*, int argc, t_atom*argv){
  GLfloat r, g, b, a;
  a=1;
  switch (argc){
  case 1:
    r=g=b=atom_getfloat(argv);
    break;
  case 4:
    a=atom_getfloat(argv+3);
  case 3:
    b=atom_getfloat(argv+2);
    g=atom_getfloat(argv+1);
    r=atom_getfloat(argv+0);
    break;
  default:
    error("GEM: GemOutput: color needs 1, 3 or 4 arguments");
    return;
  }
  GetMyClass(data)->specularMess(r, g, b, a);
}
void GemOutput :: shininessMessCallback(void *data, t_floatarg val){
  GetMyClass(data)->shininessMess((float)val);
}
void GemOutput :: fogMessCallback(void *data, t_symbol *, int argc, t_atom *argv){
  switch (argc)
    {
    case (1):
      GetMyClass(data)->fogDensityMess(atom_getfloat(&argv[0]));
      break;
    case (2):
      GetMyClass(data)->fogRangeMess(atom_getfloat(&argv[0]),atom_getfloat(&argv[1]));
      break;
    default:
      error("GEM : fog message needs 1 or 2 arguments");
    }
}
void GemOutput :: fogColorMessCallback(void *data, t_symbol*, int argc, t_atom*argv){
  // LATER: add fog
  GLfloat r, g, b, a;
  a=0;
  switch (argc){
  case 1:
    r=g=b=atom_getfloat(argv);
    break;
  case 4:
    a=atom_getfloat(argv+3);
  case 3:
    b=atom_getfloat(argv+2);
    g=atom_getfloat(argv+1);
    r=atom_getfloat(argv+0);
    break;
  default:
    error("GEM: GemOutput: color needs 1, 3 or 4 arguments");
    return;
  }
  GetMyClass(data)->fogColorMess(r, g, b, a);
}
void GemOutput :: fogModeMessCallback(void *data, t_floatarg val){
  GetMyClass(data)->fogModeMess((int)val);
}

void GemOutput :: perspectiveMessCallback(void *data, t_symbol *, int argc, t_atom *argv){
  if (argc != 6)  {
    error("GEM: perspec message needs 6 arguments: left, right ,bottom, top, front, back");
    return;
  }
   GetMyClass(data)->perspectiveMess(atom_getfloat(&argv[0]),	// left
				     atom_getfloat(&argv[1]),	// right
				     atom_getfloat(&argv[2]),	// bottom
				     atom_getfloat(&argv[3]),	// top
				     atom_getfloat(&argv[4]),	// front
				     atom_getfloat(&argv[5]));	// back
}
void GemOutput :: viewMessCallback(void *data, t_symbol *, int argc, t_atom *argv){
  const float DEG2RAD = 0.01745329251994f;
  float azimuth = 0.f;
  float theta = 0.f;
  
  switch (argc)  {
    // setting all lookat values directly
  case 9 :
    GetMyClass(data)->viewMess(atom_getfloat(argv+0),	// eyex
			       atom_getfloat(argv+1),	// eyey
			       atom_getfloat(argv+2),	// eyez
			       atom_getfloat(argv+3),	// centerx
			       atom_getfloat(argv+4),	// centery
			       atom_getfloat(argv+5),	// centerz
			       atom_getfloat(argv+6),	// upx
			       atom_getfloat(argv+7),	// upy
			       atom_getfloat(argv+8));  // upz
    break;
  case 5 :
    theta	= (float)DEG2RAD * atom_getfloat(&argv[4]);
  case 4 :
    azimuth = (float)DEG2RAD * atom_getfloat(&argv[3]);
  case 3 : {  // just have position
    float distance = GetMyClass(data)->m_stereoFocal;
    if (distance<=0.f)distance=1.0f;
  
    const float x = atom_getfloat(&argv[0]);
    const float y = atom_getfloat(&argv[1]);
    const float z = atom_getfloat(&argv[2]);
    const float dx =  (float)(cos(theta) * sinf(azimuth));
    const float dy =  (float)(sin(theta));
    const float dz = -(float)(cos(theta) * cosf(azimuth));
    GetMyClass(data)->viewMess(x,		// eyex
			       y,		// eyey
			       z,		// eyez
			       x + dx * distance,	// centerx
			       y + dy * distance,	// centery
			       z + dz * distance,	// centery
			       -dx*dy,			// upx
			       dx*dx+dz*dz,		// upy
			       -dy*dz);			// upz
  }
    break;

  default:
    error("GEM: view message needs 3, 4, 5 or 9 arguments");

    // note :: LATER set the StereoView ...
  }
}
