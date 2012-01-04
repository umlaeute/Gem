////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "modelOBJ.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "Gem/Properties.h"
#include <string>

using namespace gem::plugins;

REGISTER_MODELLOADERFACTORY("OBJ", modelOBJ);

modelOBJ :: modelOBJ(void) : 
  m_model(NULL), m_dispList(0),
  m_material(0),
  m_flags(GLM_SMOOTH | GLM_TEXTURE),
  m_group(0),
  m_rebuild(false),
  m_currentH(1.f), m_currentW(1.f),
  m_textype(GLM_TEX_DEFAULT)
{
}

modelOBJ ::~modelOBJ(void) {
  destroy();
}

bool modelOBJ :: open(const std::string&name, const gem::Properties&requestprops) {
  destroy();

#if 0
  std::vector<std::string>keys=requestprops.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    post("key[%d]=%s", i, keys[i].c_str());
  }
#endif

  m_model = glmReadOBJ(name.c_str());
  post("opening %s returned %p", name.c_str(), m_model);
  if (!m_model){
    return false;
  }

  double d=1;
  requestprops.get("rescale", d);
  if(d)glmUnitize(m_model);

  glmFacetNormals (m_model);

  gem::Properties props=requestprops;
  if(gem::Properties::UNSET==requestprops.type("smooth")) {
    props.set("smooth", 0.5);
  }
  setProperties(props);

  glmTexture(m_model, m_textype, m_currentH, m_currentW);
  m_rebuild=true;
  return true;
}

void modelOBJ :: render(void) {
  if(m_rebuild) {
    glmTexture(m_model, m_textype, m_currentH, m_currentW);
    create();
  }
  if(m_dispList)
    glCallList(m_dispList);

}
void modelOBJ :: close(void)  {
  destroy();
}

bool modelOBJ :: enumProperties(gem::Properties&readable,
                                gem::Properties&writeable) {
  readable.clear();
  writeable.clear();
  return false;
}

void modelOBJ :: setProperties(gem::Properties&props) {
  double d;

  if(props.get("smooth", d)) {
    if(d<0.)d=0.;
    if(d>1.)d=1.;
    if(m_model)
      glmVertexNormals(m_model, d*180.);
    m_rebuild=true;
  }
  if(props.get("texwidth", d)) {
    if(d!=m_currentW)
      m_rebuild=true;

    m_currentW=d;
  }
  if(props.get("texheight", d)) {
    if(d!=m_currentH)
      m_rebuild=true;

    m_currentH=d;
  }
  if(props.get("usematerials", d)) {
    int flags=GLM_SMOOTH | GLM_TEXTURE;
    if(d)
      flags |= GLM_MATERIAL;

    if(flags!=m_flags)
      m_rebuild=true;
    m_flags=flags;
  }


  std::string s;
  if(props.get("textype", s)) {
    if("UV"==s)
      m_textype= GLM_TEX_UV;
    else if("linear"==s)
      m_textype= GLM_TEX_LINEAR;
    else if("spheremap"==s)
      m_textype= GLM_TEX_SPHEREMAP;

    m_rebuild=true;
  }

  if(props.get("group", d)) {
    m_group=d;
    m_rebuild=true;
  }

  if(props.get("reverse", d)) {
    // LATER:move this to create()
    if(m_model)
      glmReverseWinding(m_model);
    m_rebuild=true;
  }
}
void modelOBJ :: getProperties(gem::Properties&props) {
}

bool modelOBJ :: create(void)  {
  if(!m_model) return false;
  if(!(GLEW_VERSION_1_1)) {
    //    verbose(1, "cannot build display-list now...do you have a window?");
    return false;
  }
  if (m_dispList) {
    glDeleteLists(m_dispList, 1);
    m_dispList=0;
  }

  if (!m_group){
    m_dispList = glmList(m_model, m_flags);
  } else {
    m_dispList = glmListGroup(m_model, m_flags, m_group);
  }
  bool res = (0 != m_dispList);
  if(res) m_rebuild=false;
  return res;
}
void modelOBJ :: destroy(void)  {
  /* LATER: check valid contexts (remove glDelete from here) */
  if (m_dispList) {
    // destroy display list
    glDeleteLists(m_dispList, 1);
    m_dispList = 0;
  }
  if(m_model) {
    glmDelete(m_model);
    m_model=NULL;
  }
}
