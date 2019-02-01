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

#include "modelfiler.h"
#include "plugins/modelloader.h"
#include <algorithm> // std::min
#include "RTE/Array.h"

CPPEXTERN_NEW_WITH_GIMME(modelfiler);


namespace {
std::string checkArrays(const std::string*tablenames, size_t count) {
  gem::RTE::Array a;
  for(size_t i=0; i<count; i++) {
    if(!a.name(tablenames[i]))
      return tablenames[i];
  }
  return std::string();
}
};

/////////////////////////////////////////////////////////
//
// modelfiler
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
modelfiler :: modelfiler(int argc, t_atom*argv)
  : m_loader(gem::plugins::modelloader::getInstance())
  , m_infoOut(gem::RTE::Outlet(this))
{

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
modelfiler :: ~modelfiler(void)
{
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
static void addProperties(gem::Properties&props, int argc, t_atom*argv)
{
  if(!argc) {
    return;
  }

  if(argv->a_type != A_SYMBOL) {
    error("no key given...");
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

void modelfiler :: setPropertyMess(t_symbol*, int argc, t_atom*argv)
{
  if(!argc) {
    error("no property specified!");
    return;
  }
  addProperties(m_readprops, argc, argv);

  if(m_loader) {
    m_loader->setProperties(m_readprops);
  }
}

void modelfiler :: getPropertyMess(t_symbol*, int argc, t_atom*argv)
{
  if(argc) {
    int i=0;
    m_readprops.clear();

    for(i=0; i<argc; i++) {
      addProperties(m_readprops, 1, argv+i);
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

void modelfiler :: enumPropertyMess()
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

void modelfiler :: setPropertiesMess(t_symbol*, int argc, t_atom*argv)
{
  addProperties(m_readprops, argc, argv);
}

void modelfiler :: applyProperties()
{
  if(m_loader) {
    m_loader->setProperties(m_readprops);
  } else {
    verbose(1, "no open model loader...remembering properties...");
  }
}
void modelfiler :: clearPropertiesMess()
{
  m_readprops.clear();
}

/////////////////////////////////////////////////////////
// backendMess
//
/////////////////////////////////////////////////////////
void modelfiler :: backendMess(t_symbol*s, int argc, t_atom*argv)
{
  m_backends.clear();
  if(argc) {
    for(int i=0; i<argc; i++) {
      if(A_SYMBOL == argv->a_type) {
        t_symbol *b=atom_getsymbol(argv+i);
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
      t_atom at;
      t_atom*ap=&at;
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
        for(int i=0; i<backends.size(); i++) {
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
void modelfiler :: openMess(const std::string&filename)
{
  gem::Properties wantProps = m_readprops;

  if(!m_loader) {
    error("no model loader backends found");
    return;
  }
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

#define COPYARRAY(name, tables, count) int name = copyArrays(#name, tables, count)
#define PRINTSUCCESS(name) if(name) do {        \
    alist.clear();                              \
    gem::any v = name;                   \
    alist.push_back(v);                         \
    m_infoOut.send(std::string(#name), alist); \
  } while(0)

  COPYARRAY(vertices, m_position, 3);
  COPYARRAY(colors, m_color, 4);
  COPYARRAY(texcoords, m_texture, 2);
  COPYARRAY(normals, m_normal, 3);

  m_loader->close();
  std::vector<gem::any> alist;
  PRINTSUCCESS(vertices);
  PRINTSUCCESS(colors);
  PRINTSUCCESS(texcoords);
  PRINTSUCCESS(normals);
}

size_t modelfiler :: copyArrays(const std::string&name, const std::string*tablenames, size_t count) {
  if((count > 0) && tablenames[0].empty())
    return 0;
  std::string failed = checkArrays(tablenames, count);
  if(!failed.empty()) {
    error("no such array '%s' for %s", failed.c_str(), name.c_str());
    return 0;
  }
  const std::vector<std::vector<float> >&data = m_loader->getVector(name);
  std::vector<gem::RTE::Array> tabs;

  size_t size = data.size();
  if(!size)
    return size;

  for(size_t i=0; i<count; i++) {
    gem::RTE::Array a(tablenames[i]);
    a.resize(size);
    tabs.push_back(a);
  }
  for(size_t i=0; i<size; i++) {
    for(size_t j=0; j<count; j++) {
      tabs[j][i] = data[i][j];
    }
  }

  return size;
}


void modelfiler :: tableMess(t_symbol*s, int argc, t_atom*argv) {
  const std::string tabletype = s->s_name;
  std::vector<std::string>extensions;
  std::string*names;

  if(tabletype == "position" || tabletype == "normal") {
    extensions.push_back("X");
    extensions.push_back("Y");
    extensions.push_back("Z");
    if("position" == tabletype)
      names = m_position;
    else
      names = m_normal;
  } else if (tabletype == "texture") {
    extensions.push_back("U");
    extensions.push_back("V");
    names = m_texture;
  } else if (tabletype == "color") {
    extensions.push_back("R");
    extensions.push_back("G");
    extensions.push_back("B");
    extensions.push_back("A");
    names = m_color;
  }

  if((argc != 1) && (argc != extensions.size()) && (argc != extensions.size() + 1)) {
    error("'%s' requires %d array names", s->s_name, extensions.size());
    return;
  }
  if(argc == extensions.size()) {
    for(int i = 0; i < argc; i++) {
      names[i] = atom_getsymbol(argv+i)->s_name;
    }
    return;
  }
  if(argc == extensions.size() + 1) {
    extensions.clear();
    for(int i = 1; i < argc; i++) {
      extensions.push_back(atom_getsymbol(argv+i)->s_name);
    }
    argc = 1;
  }
  if(argc == 1) {
    std::string basename = atom_getsymbol(argv+0)->s_name;
    for (size_t i=0; i < extensions.size(); i++) {
      names[i] = basename + extensions[i];
    }
    return;
  }
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void modelfiler :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "open", openMess, std::string);
  CPPEXTERN_MSG (classPtr, "loader", backendMess);

  CPPEXTERN_MSG (classPtr, "set", setPropertyMess);
  CPPEXTERN_MSG (classPtr, "get", getPropertyMess);
  CPPEXTERN_MSG (classPtr, "setProps", setPropertiesMess);
  CPPEXTERN_MSG0(classPtr, "enumProps", enumPropertyMess);
  CPPEXTERN_MSG0(classPtr, "clearProps", clearPropertiesMess);

  CPPEXTERN_MSG(classPtr, "position", tableMess);
  CPPEXTERN_MSG(classPtr, "color", tableMess);
  CPPEXTERN_MSG(classPtr, "texture", tableMess);
  CPPEXTERN_MSG(classPtr, "normal", tableMess);
}
