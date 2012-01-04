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

#include "model.h"
#include "Gem/State.h"
#include "plugins/modelloader.h"


CPPEXTERN_NEW_WITH_ONE_ARG(model, t_symbol *, A_DEFSYM);

  /////////////////////////////////////////////////////////
//
// model
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
model :: model(t_symbol *filename) :
  m_loader(gem::plugins::modelloader::getInstance()),
  m_loaded(false)
{
  // make sure that there are some characters
  if (filename&&filename->s_name&&*filename->s_name) openMess(filename->s_name);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
model :: ~model(void)
{
  if(m_loaded) {
    m_loader->close();
  }
  if(m_loader) {
    delete m_loader;
    m_loader=NULL;
  }
}

void model :: applyProperties(void)
{
  if(m_loader)
    m_loader->setProperties(m_properties);
}

/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: materialMess(int material)
{
  gem::any value=material;
  m_properties.set("usematerials", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: textureMess(int state)
{
  std::string textype;
  switch(state) {
  case 0:
    textype="linear";
    break;
  case 1:
    textype="spheremap";
    break;
  case 2:
    textype="UV";
    break;
  default:
    break;
  }
  if(textype.empty()) {
    m_properties.erase("textype");
  } else {
    gem::any value=textype;
    m_properties.set("textype", value);
  }
  applyProperties();
}

/////////////////////////////////////////////////////////
// smoothMess
//
/////////////////////////////////////////////////////////
void model :: smoothMess(t_float fsmooth)
{  
  m_properties.set("smooth", fsmooth);
  applyProperties();
}

/////////////////////////////////////////////////////////
// rescaleMess
//
/////////////////////////////////////////////////////////
void model :: reverseMess(bool reverse)
{
  gem::any value=reverse;
  m_properties.set("reverse", value);
  applyProperties();
}
/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void model :: rescaleMess(bool state)
{
  gem::any value=state;
  m_properties.set("rescale", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void model :: groupMess(int state)
{
  gem::any value=state;
  m_properties.set("group", value);
  applyProperties();
}


/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void model :: openMess(const std::string&filename)
{
  if(!m_loader) {
    error("no model loader backends found");
    return;
  }
  m_loader->close();
  m_loaded=false;

  char buf[MAXPDSTRING];
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()), const_cast<char*>(filename.c_str()), buf, MAXPDSTRING);
  if(!m_loader->open(buf, m_properties)) {
      error("unable to read model '%s'", buf);
      return;
  }

  m_loaded=true;
  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void model :: render(GemState *state)
{
  if(!m_loaded)return;
  if (state && (m_currentH != state->texCoordX(2) || m_currentW != state->texCoordY(2))) {
    m_currentH = state->texCoordX(2);
    m_currentW = state->texCoordY(2);

    m_properties.set("texwidth", m_currentW);
    m_properties.set("texheight", m_currentH);
    applyProperties();
  }

  m_loader->render();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void model :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "open", openMess, std::string);
  CPPEXTERN_MSG1(classPtr, "rescale", rescaleMess, bool);
  CPPEXTERN_MSG1(classPtr, "smooth", smoothMess, float);
  CPPEXTERN_MSG1(classPtr, "revert", reverseMess, bool);
  CPPEXTERN_MSG1(classPtr, "material", materialMess, int);
  CPPEXTERN_MSG1(classPtr, "texture", textureMess, int);
  CPPEXTERN_MSG1(classPtr, "group", groupMess, int);
}
