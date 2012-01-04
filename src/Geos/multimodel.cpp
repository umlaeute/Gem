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
#include "multimodel.h"
#include "Gem/State.h"
#include <stdio.h>

CPPEXTERN_NEW_WITH_FOUR_ARGS(multimodel, t_symbol *, A_DEFSYM, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// multimodel
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
multimodel :: multimodel(t_symbol *filename, t_floatarg baseModel,
                         t_floatarg topModel, t_floatarg skipRate)
  : m_curModel(-1),
    m_currentH(1.f), m_currentW(1.f)
{
  inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mdl_num"));
  
  // make sure that there are some characters
  if (filename->s_name[0]) {
    int skipRatei=static_cast<int>(skipRate);
    int topModeli=static_cast<int>(topModel);
    int baseModeli=static_cast<int>(baseModel);
    if (skipRatei == 0) {
      if (topModeli == 0)
        openMess(filename->s_name, 0, baseModeli, 1);
      else
        openMess(filename->s_name, baseModeli, topModeli, 1);
    }
    else openMess(filename->s_name, baseModeli, topModeli, skipRatei);
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
multimodel :: ~multimodel()
{
}

/////////////////////////////////////////////////////////
// cleanMultimodel
//
/////////////////////////////////////////////////////////
void multimodel :: cleanMultimodel(void)
{
  unsigned int i;
  for(i=0; i<m_loaders.size(); i++) {
    if (m_loaders[i])
      delete m_loaders[i];
    m_loaders[i]=NULL;
  }
  m_loaders.clear();
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void multimodel :: openMess(const std::string&filename, int baseModel, int topModel, int skipRate)
{
  std::vector<gem::plugins::modelloader*>loaders;

  if (!topModel) {
    error("requires an int for number of models");
    return;
  }
  if (baseModel > topModel) {
    error("top range less than base model");
    return;
  }
  if (skipRate < 1) skipRate = 1;
  char preName[256];
  char postName[256];

  int i = 0;
  const char *strPtr = filename.c_str();
  while (strPtr[i] && strPtr[i] != '*') {
    preName[i] = strPtr[i];
    i++;
  }

  if (!strPtr[i]) {
    error("unable to find * in file name");
    return;
  }

  preName[i] = '\0';
  strcpy(postName, &(strPtr[i+1]));

  // need to figure out how many filenames there are to load
  int numModels = (topModel + 1 - baseModel) / skipRate;

  int realNum = baseModel;
  char bufName[MAXPDSTRING];
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()), preName, bufName, MAXPDSTRING);

  for (i = 0; i < numModels; i++, realNum += skipRate) {
    char newName[256];
    sprintf(newName, "%s%d%s", bufName, realNum, postName);

    loaders.push_back(gem::plugins::modelloader::getInstance());
    if(!loaders[i])
      break;
    if(!loaders[i]->open(newName, m_properties))
      break;
  }

  if(loaders.size()!=numModels) {
    /* outch, something went wrong! */
    error("failed to load model#%d of %d...resetting to original models", i, numModels);

    for(i=0; i<loaders.size(); i++) {
      if(loaders[i])
        delete loaders[i];
      loaders[i]=NULL;
    }
    loaders.clear();
    return;
  }

  cleanMultimodel();
  m_loaders=loaders;
  m_curModel = 0;

  post("loaded models: %s %s from %d to %d skipping %d",
       bufName, postName, baseModel, topModel, skipRate);
  setModified();
}


void multimodel :: applyProperties(void)
{
  unsigned int i;
  for(i=0; i<m_loaders.size(); i++) {
    if(m_loaders[i])
      m_loaders[i]->setProperties(m_properties);
  }
}


/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void multimodel :: textureMess(int state)
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
void multimodel :: smoothMess(float fsmooth)
{  
  m_properties.set("smooth", fsmooth);
  applyProperties();
}

/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void multimodel :: rescaleMess(bool state)
{
  gem::any value=state;
  m_properties.set("rescale", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void multimodel :: render(GemState *state)
{
  if (0==m_loaders.size() || m_curModel<0 || m_curModel>=m_loaders.size())
    return;

  if (state && (m_currentH != state->texCoordX(2) || m_currentW != state->texCoordY(2))) {
    m_currentH = state->texCoordX(2);
    m_currentW = state->texCoordY(2);

    m_properties.set("texwidth", m_currentW);
    m_properties.set("texheight", m_currentH);
    applyProperties();
  }

  m_loaders[m_curModel]->render();
}

/////////////////////////////////////////////////////////
// changeModel
//
/////////////////////////////////////////////////////////
void multimodel :: changeModel(int modelNum)
{
  if (modelNum < 0 || modelNum >= m_loaders.size()) {
    error("selection %d out of range: 0..%d", modelNum, m_loaders.size()-1);
    return;
  }
  m_curModel = modelNum;
  //  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void multimodel :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, reinterpret_cast<t_method>(&multimodel::openMessCallback),
                  gensym("open"), A_SYMBOL, A_FLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&multimodel::changeModelCallback),
                  gensym("mdl_num"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&multimodel::rescaleMessCallback),
                  gensym("rescale"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&multimodel::textureMessCallback),
                  gensym("texture"), A_FLOAT, A_NULL);
}
void multimodel :: openMessCallback(void *data, t_symbol *filesymbol, t_floatarg baseModel,
                                    t_floatarg topModel, t_floatarg skipRate)
{
  int skipRatei=static_cast<int>(skipRate);
  int topModeli=static_cast<int>(topModel);
  int baseModeli=static_cast<int>(baseModel);
  std::string filename=filesymbol->s_name;

  if (skipRatei == 0)
    {
      if (topModeli == 0)
        GetMyClass(data)->openMess(filename, 0, topModeli, 0);
      else
        GetMyClass(data)->openMess(filename, baseModeli, topModeli, 0);
    }
  else
    GetMyClass(data)->openMess(filename, baseModeli, topModeli, skipRatei);
}
void multimodel :: changeModelCallback(void *data, t_floatarg modelNum)
{
  GetMyClass(data)->changeModel(static_cast<int>(modelNum));
}
void multimodel :: rescaleMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->rescaleMess(static_cast<int>(state));
}
void multimodel :: textureMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->textureMess(static_cast<int>(state));
}
