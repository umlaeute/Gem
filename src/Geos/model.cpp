////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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

#include <algorithm> // std::min

namespace
{
static char mytolower(char in)
{
  if(in<='Z' && in>='A') {
    return in-('Z'-'z');
  }
  return in;
}
};

CPPEXTERN_NEW_WITH_ONE_ARG(model, t_symbol*, A_DEFSYMBOL);

model :: modelmesh :: modelmesh(gem::plugins::modelloader::mesh*m)
  : mesh(m)
  , vertices(GL_VERTEX_ARRAY)
  , normals(GL_NORMAL_ARRAY)
  , colors(GL_COLOR_ARRAY)
  , texcoords(GL_TEXTURE_COORD_ARRAY)
{
}
void model :: modelmesh :: update(void)
{
  vertices.update(mesh->size, mesh->vertices);
  normals.update(mesh->size, mesh->normals);
  colors.update(mesh->size, mesh->colors);
  texcoords.update(mesh->size, mesh->texcoords);
}
void model :: modelmesh :: render(GLenum drawType) const
{
  std::vector<size_t>sizes;
  size_t sizeV=0, sizeN=0, sizeC=0, sizeT=0;
  sizeV = vertices.render();
  if(sizeV>0)sizes.push_back(sizeV);
  sizeN = normals.render();
  if(sizeN>0)sizes.push_back(sizeN);
  sizeC = colors.render();
  if(sizeC>0)sizes.push_back(sizeC);
  sizeT = texcoords.render();
  if(sizeT>0)sizes.push_back(sizeT);

  if ( sizes.size() > 0 ) {
    unsigned int npoints = *std::min_element(sizes.begin(),sizes.end());
    glDrawArrays(drawType, 0, npoints);
  }

  if(sizeV)glDisableClientState(GL_VERTEX_ARRAY);
  if(sizeN)glDisableClientState(GL_NORMAL_ARRAY);
  if(sizeC)glDisableClientState(GL_COLOR_ARRAY);
  if(sizeT)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

/////////////////////////////////////////////////////////
//
// model
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
model :: model(t_symbol* filename)
  : m_loader(gem::plugins::modelloader::getInstance())
  , m_loaded(false)
  , m_size_change_flag(false)
  , m_position(0,3)
  , m_texture (0,2)
  , m_color   (0,4)
  , m_normal  (0,3)
  , m_infoOut(gem::RTE::Outlet(this))
  , m_drawType(GL_TRIANGLES)
  , m_blend(false)
  , m_linewidth(1.0)
{
  m_drawTypes.clear();
  m_drawTypes["default"]=m_drawType;
  m_drawTypes["point"]=GL_POINTS;
  m_drawTypes["points"]=GL_POINTS;
  m_drawTypes["line"]=GL_LINES;
  m_drawTypes["lines"]=GL_LINES;
  m_drawTypes["fill"]=GL_TRIANGLES;
  m_texscale[0] = m_texscale[1] = 1.;

  // make sure that there are some characters
  if (filename&&filename->s_name&&*filename->s_name) {
    openMess(filename->s_name);
  }
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

static gem::any atom2any(t_atom*ap)
{
  gem::any result;
  if(ap) {
    switch(ap->a_type) {
    case A_FLOAT:
      result=atom_getfloat(ap);
      break;
    case A_SYMBOL:
      result=std::string(atom_getsymbol(ap)->s_name);
      break;
    default:
      result=ap->a_w.w_gpointer;
    }
  }
  return result;
}
static void addProperties(CPPExtern*obj, gem::Properties&props, int argc, t_atom*argv)
{
  if(!argc) {
    return;
  }

  if(argv->a_type != A_SYMBOL) {
    pd_error(obj, "no key given...");
    return;
  }
  std::string key=std::string(atom_getsymbol(argv)->s_name);
  std::vector<gem::any> values;
  argc--;
  argv++;
  while(argc-->0) {
    values.push_back(atom2any(argv++));
  }
  switch(values.size()) {
  default:
    props.set(key, values);
    break;
  case 1:
    props.set(key, values[0]);
    break;
  case 0: {
    gem::any dummy;
    props.set(key, dummy);
  }
  break;
  }
}

void model :: setPropertyMess(t_symbol*, int argc, t_atom*argv)
{
  if(!argc) {
    error("no property specified!");
    return;
  }
  addProperties(this, m_writeprops, argc, argv);

  if(m_loader) {
    m_loader->setProperties(m_writeprops);
  }
}

void model :: getPropertyMess(t_symbol*, int argc, t_atom*argv)
{
  if(argc) {
    int i=0;
    m_readprops.clear();

    for(i=0; i<argc; i++) {
      addProperties(this, m_readprops, 1, argv+i);
    }

  } else {
    /* LATER: read all properties */
  }

  if(m_loader) {
    m_loader->getProperties(m_readprops);
    std::vector<std::string>keys=m_readprops.keys();
    unsigned int i=0;
    for(i=0; i<keys.size(); i++) {
      std::vector<gem::any>atoms;
      gem::any value;
      std::string key=keys[i];
      atoms.push_back(value=key);
      switch(m_readprops.type(key)) {
      default:
      case gem::Properties::UNSET:
        post("oops: %s", key.c_str());
        continue;
      case gem::Properties::NONE:
        break;
      case gem::Properties::DOUBLE:
        do {
          double d=0;
          if(m_readprops.get(key, d)) {
            atoms.push_back(value=d);
          }
        } while(0);
        break;
      case gem::Properties::STRING:
        do {
          std::string s;
          if(m_readprops.get(key, s)) {
            atoms.push_back(value=s);
          }
        } while(0);
        break;
      }
      m_infoOut.send("prop", atoms);
    }
  } else {
    verbose(1, "no open model loader...remembering properties...");
  }
}

void model :: enumPropertyMess()
{
  if(m_loader) {
    gem::Properties readable, writeable;
    std::vector<std::string>readkeys, writekeys;
    std::vector<gem::any>data;
    gem::any value;

    m_loader->enumProperties(readable, writeable);

    readkeys=readable.keys();

    data.clear();
    data.push_back(value=std::string("numread"));
    data.push_back(value=(int)readkeys.size());
    m_infoOut.send("proplist", data);

    unsigned int i=0;
    for(i=0; i<readkeys.size(); i++) {
      std::string key=readkeys[i];
      data.clear();
      data.push_back(value=std::string("read"));
      data.push_back(key);
      switch(readable.type(key)) {
      case gem::Properties::NONE:
        data.push_back(value=std::string("bang"));
        break;
      case gem::Properties::DOUBLE: {
        double d=-1;
        data.push_back(value=std::string("float"));
        /* LATER: get and show ranges */
        if(readable.get(key, d)) {
          data.push_back(value=d);
        }
      }
      break;
      case gem::Properties::STRING: {
        data.push_back(value=std::string("symbol"));
        std::string s;
        if(readable.get(key, s)) {
          data.push_back(value=s);
        }
      }
      break;
      default:
        data.push_back(value=std::string("unknown"));
        break;
      }
      m_infoOut.send("proplist", data);
    }

    writekeys=writeable.keys();

    data.clear();
    data.push_back(value=std::string("numwrite"));
    data.push_back(value=(int)writekeys.size());
    m_infoOut.send("proplist", data);

    for(i=0; i<writekeys.size(); i++) {
      data.clear();
      data.push_back(value=std::string("write"));
      std::string key=writekeys[i];
      data.push_back(value=key);
      switch(writeable.type(key)) {
      case gem::Properties::NONE:
        data.push_back(value=std::string("bang"));
        break;
      case gem::Properties::DOUBLE: {
        double d=-1;
        data.push_back(value=std::string("float"));
        /* LATER: get and show ranges */
        if(writeable.get(key, d)) {
          data.push_back(value=d);
        }
      }
      break;
      case gem::Properties::STRING: {
        data.push_back(value=std::string("symbol"));
        std::string s;
        if(writeable.get(key, s)) {
          data.push_back(value=s);
        }
      }
      break;
      default:
        data.push_back(value=std::string("unknown"));
        break;
      }
      m_infoOut.send("proplist", data);
    }
  } else {
    error("cannot enumerate properties without a valid model loader");
  }
}

void model :: setPropertiesMess(t_symbol*, int argc, t_atom*argv)
{
  addProperties(this, m_writeprops, argc, argv);
}

void model :: applyProperties()
{
  if(m_loader) {
    m_loader->setProperties(m_writeprops);
  } else {
    verbose(1, "no open model loader...remembering properties...");
  }
}

void model :: clearPropertiesMess()
{
  m_writeprops.clear();
}




/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: materialMess(int material)
{
  gem::any value=material;
  m_writeprops.set("usematerials", value);
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
    m_writeprops.erase("textype");
  } else {
    gem::any value=textype;
    m_writeprops.set("textype", value);
  }
  applyProperties();
}

/////////////////////////////////////////////////////////
// smoothMess
//
/////////////////////////////////////////////////////////
void model :: smoothMess(t_float fsmooth)
{
  m_writeprops.set("smooth", fsmooth);
  applyProperties();
}

/////////////////////////////////////////////////////////
// rescaleMess
//
/////////////////////////////////////////////////////////
void model :: reverseMess(bool reverse)
{
  gem::any value=(double)reverse;
  m_writeprops.set("reverse", value);
  applyProperties();
}
/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void model :: rescaleMess(bool state)
{
  gem::any value=(double)state;
  m_writeprops.set("rescale", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// groupMess
//
/////////////////////////////////////////////////////////
void model :: groupMess(int state)
{
  gem::any value=state;
  m_writeprops.set("group", value);
  applyProperties();
}

void model :: blendMess(bool mode)
{
  m_blend = mode;
  setModified();
}
void model :: linewidthMess(t_float w)
{
  m_linewidth = w;
  setModified();
}


/////////////////////////////////////////////////////////
// drawStyle
//
/////////////////////////////////////////////////////////
void model :: drawMess(int type)
{
  /* raw */
  m_drawType = type;
}
void model :: drawMess(std::string name)
{
  if(0==m_drawTypes.size()) {
    error("unable to change drawstyle");
    return;
  }

  std::transform(name.begin(), name.end(), name.begin(), mytolower);

  std::map<std::string, GLenum>::iterator it=m_drawTypes.find(name);
  if(m_drawTypes.end() == it) {
    error ("unknown draw style '%s'... possible values are:", name.c_str());
    it=m_drawTypes.begin();
    while(m_drawTypes.end() != it) {
      error("\t %s", it->first.c_str());
      ++it;
    }
    return;
  }
  m_drawType=it->second;
}


/////////////////////////////////////////////////////////
// backendMess
//
/////////////////////////////////////////////////////////
void model :: backendMess(t_symbol*s, int argc, t_atom*argv)
{
#if 0
  gem::any value=ids;
  m_writeprops.set("backends", value);
  applyProperties();
#endif
  int i;

  m_backends.clear();
  if(argc) {
    for(i=0; i<argc; i++) {
      if(A_SYMBOL == argv->a_type) {
        t_symbol* b=atom_getsymbol(argv+i);
        m_backends.push_back(b->s_name);
      } else {
        error("%s must be symbolic", s->s_name);
      }
    }
  } else {
    /* no backend requested, just enumerate them */
    if(m_loader) {
      std::vector<gem::any>atoms;
      gem::any value;
      gem::Properties props;
      std::vector<std::string> backends;
      props.set("backends", value);
      m_loader->getProperties(props);
      if(props.type("backends")!=gem::Properties::UNSET) {
        props.get("backends", backends);
      }
      atoms.clear();
      atoms.push_back(value=(int)(backends.size()));
      m_infoOut.send("loaders", atoms);
      if(!backends.empty()) {
        for(i=0; i<backends.size(); i++) {
          atoms.clear();
          atoms.push_back(value=backends[i]);
          post("loader[%d] %s", i, backends[i].c_str());
          m_infoOut.send("loader", atoms);
        }
      } else {
        post("no model-loading backends found!");
      }
    }
  }
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void model :: openMess(const std::string&filename)
{
  gem::Properties wantProps = m_writeprops;

  if(!m_loader) {
    error("no model loader backends found");
    return;
  }
  m_loader->close();
  m_loaded=false;

  if(!m_backends.empty()) {
    wantProps.set("backends", m_backends);
  }

  char buf[MAXPDSTRING];
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()),
                      const_cast<char*>(filename.c_str()), buf, MAXPDSTRING);
  if(!m_loader->open(buf, wantProps)) {
    error("unable to read model '%s'", buf);
    return;
  }

  m_loaded=true;

  m_mesh.clear();
  const size_t nummeshes = m_loader->getNumMeshes();
  for(size_t n=0; n < nummeshes; n++) {
    gem::plugins::modelloader::mesh*mesh = m_loader->getMesh(n);
    if(mesh) {
      m_mesh.push_back(modelmesh(mesh));
    }
  }

}

void model :: startRendering()
{
  for(auto m : m_mesh) {
    m.update();
  }
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void model :: render(GemState *state)
{
  bool blend = m_blend;
  bool setwidth = false;

  if(!m_loaded) {
    return;
  }

  int texType = 0;
  state->get(GemState::_GL_TEX_TYPE, texType);
  if(texType) {
    bool rebuild = false;
    TexCoord*texCoords = 0;
    int texNum = 0;
    state->get(GemState::_GL_TEX_COORDS, texCoords);
    state->get(GemState::_GL_TEX_NUMCOORDS, texNum);
    if(texNum>1 && texCoords) {
      if(texCoords[1].s != m_texscale[0])
        rebuild = true;
      if(texCoords[1].t != m_texscale[1])
        rebuild = true;
      m_texscale[0] = texCoords[1].s;
      m_texscale[1] = texCoords[1].t;
    }
    if(rebuild) {
      gem::Properties props = gem::Properties(m_writeprops);
      props.set("_texwidth", m_texscale[0]);
      props.set("_texheight", m_texscale[1]);
      m_loader->setProperties(props);
    }
  }

  switch(m_drawType) {
  case GL_LINE_LOOP:
  case GL_LINE_STRIP:
  case GL_LINES:
  case GL_LINE:
    setwidth = true;
    break;
  default:
    setwidth = false;
    break;
  }

#if 0
  if(m_loader->updateMeshes()) {
    if(GLEW_VERSION_1_5) {
      /* update the VBOs */
      for (auto m: m_mesh) {
        m.update();
      }
    }
  }
#endif

  if(setwidth) {
    glLineWidth(m_linewidth);
  }
  if(blend) {
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
  }
  if(!GLEW_VERSION_1_5) {
    for (auto m : m_mesh) {
      size_t size = m.mesh->size;
      float*positions = size?m.mesh->vertices:0;
      float*textures = size?m.mesh->texcoords:0;
      float*colors = size?m.mesh->colors:0;
      float*normals = size?m.mesh->normals:0;


      glBegin(m_drawType);
      for (unsigned int i=0; i<size; i++) {
        if(normals) {
          glNormal3fv(normals);
          normals += m_normal.dimen;
        }
        if(textures) {
          glTexCoord2fv(textures);
          textures += m_texture.dimen;
        }
        if(colors) {
          glColor4fv(colors);
          colors += m_color.dimen;
        }
        if(positions) {
          glVertex3fv(positions);
          positions += m_position.dimen;
        }
      }
      glEnd();
    }
  } else { /* openGL-2+ */
    for (auto m: m_mesh) {
      m.update();
      m.render(m_drawType);
    }
  }
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
  CPPEXTERN_MSG (classPtr, "loader", backendMess);

  CPPEXTERN_MSG (classPtr, "set", setPropertyMess);
  CPPEXTERN_MSG (classPtr, "get", getPropertyMess);
  CPPEXTERN_MSG (classPtr, "setProps", setPropertiesMess);
  CPPEXTERN_MSG0(classPtr, "enumProps", enumPropertyMess);
  CPPEXTERN_MSG0(classPtr, "clearProps", clearPropertiesMess);
  CPPEXTERN_MSG0(classPtr, "applyProps", applyProperties);

  CPPEXTERN_MSG1(classPtr, "draw", drawMess, std::string);
  CPPEXTERN_MSG1(classPtr, "type", drawMess, int);
  CPPEXTERN_MSG1(classPtr, "width", linewidthMess, t_float);
  CPPEXTERN_MSG1(classPtr, "blend", blendMess, bool);
}
