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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "world_light.h"

#include "Gem/Manager.h"
#include "Base/GemWindow.h"

CPPEXTERN_NEW_WITH_ONE_ARG(world_light, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// world_light
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
world_light :: world_light(t_floatarg lightNum)
  : m_change(1), m_on(1), m_debug(0),
    m_light(0), m_lightID(0),
    m_thing(NULL)
{
  m_color[0] = m_color[1] = m_color[2] = m_color[3] = 1.0;

  m_position[0] = 0.0;
  m_position[1] = 0.0;
  m_position[2] = 2.0;
  m_position[3] = 0.0;

  if (lightNum < 1.f)
    m_lightID = 0;
  else
    m_lightID = static_cast<int>(lightNum);

  // create the color inlet
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("color"));
}
////////////////////////////////////////////////////////
// Destructor
//
////////////////////////////////////////////////////////
world_light :: ~world_light()
{
  struct cb_struct {
    static void callback(void*data) {
      world_light*wl=reinterpret_cast<world_light*>(data);
      if(wl->gem_amRendering) wl->stopRendering();
      if(wl->m_light)         GemMan::freeLight(wl->m_light);
    }
    cb_struct(void*data) { GemWindow::call4all(callback, data); }
  };
  cb_struct callme(this);
}

////////////////////////////////////////////////////////
// lightOnOffMess
//
////////////////////////////////////////////////////////
void world_light :: lightOnOffMess(int state)
{
  m_on = state;
  m_change = 1;
  setModified();
}

////////////////////////////////////////////////////////
// debugMess
//
////////////////////////////////////////////////////////
void world_light :: debugMess(int state)
{
  m_debug = state;
  m_change = 1;
  setModified();
}

////////////////////////////////////////////////////////
// lightColorMess
//
////////////////////////////////////////////////////////
void world_light :: lightColorMess(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  m_color[0] = red;
  m_color[1] = green;
  m_color[2] = blue;
  m_color[3] = alpha;
  m_change = 1;
  setModified();
}
void world_light :: lightColorMess(t_symbol*s, int argc, t_atom*argv) {
  GLfloat red=1.f, green=1.f, blue=1.f, alpha=1.f;
  switch(argc) {
  case 4:
    alpha=atom_getfloat(argv+3);
  case 3:
    red=atom_getfloat(argv);
    green=atom_getfloat(argv+1);
    blue=atom_getfloat(argv+2);
    lightColorMess(red, green, blue, alpha);
    break;
  case 1:
    red=atom_getfloat(argv);
    lightColorMess(red, red, red, 1.f);
    break;
  default:
    error("\"%s\" must be 1, 3 or 4 parameters", s->s_name);
  }
}

////////////////////////////////////////////////////////
// startRendering
//
////////////////////////////////////////////////////////
void world_light :: startRendering()
{
  if (m_thing)stopRendering();
  GLenum light=m_light;
  if(!light) {
    m_light = GemMan::requestLight(m_lightID);
  }
  m_thing = gluNewQuadric();
  gluQuadricTexture(m_thing, GL_FALSE);
  gluQuadricDrawStyle(m_thing, static_cast<GLenum>(GLU_FILL));
  m_change = 1;
}

////////////////////////////////////////////////////////
// stopRendering
//
////////////////////////////////////////////////////////
void world_light :: stopRendering()
{
  if (m_thing)gluDeleteQuadric(m_thing);
  m_thing = NULL;

  if (m_light)glDisable(m_light);
  m_change = 1;
}

////////////////////////////////////////////////////////
// render
//
////////////////////////////////////////////////////////
void world_light :: renderDebug()
{
  const GLfloat size=0.2f;
  if (m_debug)
    {
      glPushMatrix();
      glDisable(GL_LIGHTING);
      glColor3fv(m_color);
      glTranslatef(m_position[0], m_position[1], m_position[2]);
      gluCylinder(m_thing, size, size, size * 2.f, 10, 10);
      glEnable(GL_LIGHTING);
      glPopMatrix();
    }
}

void world_light :: render(GemState *state)
{
  int light=m_light;
  if (!m_light)return;

  if (m_change) {
    m_change = 0;
    if ( !m_on ) {
      glDisable(light);
      return;
    }

    glEnable(light);
    glLightfv(light, GL_DIFFUSE,  m_color);
    glLightfv(light, GL_SPECULAR, m_color);
  }

  if (m_on) {
    glLightfv(light, GL_POSITION, m_position);
    renderDebug();
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void world_light :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "color", lightColorMess);
  CPPEXTERN_MSG1(classPtr, "debug", debugMess, int);
  CPPEXTERN_MSG1(classPtr, "float", lightOnOffMess, int);
}
