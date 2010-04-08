////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// load settings from a file (or get them via env-variables)
//
/////////////////////////////////////////////////////////

#include "Base/GemConfig.h"
#include "GemSettings.h"

#include <map>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_WORDEXP_H
# include <wordexp.h>
#endif
#ifdef _WIN32
# include <windows.h>
#endif

#define GEM_SETTINGS_FILE "gem.conf"
static const char*s_configdir[] = { 
#ifdef __linux__
  "/etc/pd",
  "~/.pd", 
#elif defined __APPLE__
  "/Library/Pd",
  "~/Library/Pd",
#elif defined  _WIN32
  "%CommonProgramFiles%\\Pd",
  "%AppData%\\Pd",
#endif
  0 /* $(pwd)/gem.conf */
};




/* this is ripped from m_imp.h */
struct _gemclass
{
  t_symbol *c_name;                   /* name (mostly for error reporting) */
  t_symbol *c_helpname;               /* name of help file */
  t_symbol *c_externdir;              /* directory extern was loaded from */
  /* ... */ /* the real t_class continues here... */
};
# define t_gemclass struct _gemclass




class GemSettingsData {
  friend class GemSettings;
 public:
  GemSettingsData(void);
  ~GemSettingsData(void);

 protected:
  // dictionary for setting values
  std::map <t_symbol*, t_atom*> data;

  virtual t_atom*get(t_symbol*name) {
    return data[name];
  }
  virtual void set(t_symbol*name, t_atom*value) {
    // LATER: we should expand envvariables
    if(value) {
      t_atom*a=(t_atom*)getbytes(sizeof(t_atom));
      memcpy(a, value, sizeof(t_atom));
      data[name]=a;
    } else {
      data.erase(name);
    }
  }

  void setEnv(t_symbol*name, const char*env);
  void set(t_symbol*name, int i) {
    t_atom a;
    SETFLOAT(&a, i);
    set(name, &a);
  }
  void set(t_symbol*name, t_float f) {
    t_atom a;
    SETFLOAT(&a, f);
    set(name, &a);
  }
  void set(t_symbol*name, t_symbol*s) {
    t_atom a;
    SETSYMBOL(&a, s);
    set(name, &a);
  }

  t_symbol*expandEnv(t_symbol*, bool bashfilename=false);


  bool open(const char*filename, const char*dirname=NULL) {
    t_binbuf*bb=binbuf_new();
    int r=0;
    if(NULL==filename)
      return false;


    if(dirname) {
      r=binbuf_read(bb, (char*)filename, expandEnv(gensym(dirname), true)->s_name, 1);
      if(0==r)verbose(1, "found Gem-settings '%s' in '%s'", filename, dirname);
    } else {
      r=binbuf_read_via_path(bb, (char*)filename, (char*)".", 1);
      if(0==r)verbose(1, "found Gem-settings '%s'", filename);
    }

    if(r){
      binbuf_free(bb);
      return false;
    }

    int ac=binbuf_getnatom(bb);
    t_atom*av=binbuf_getvec(bb);

    t_symbol*s=NULL;
    t_atom*a;
    int state=0; /* 0=(next is ID); 1=(next is value); 2=(next is ignored) */

    while(ac--) {
      if (av->a_type == A_SEMI) {
        // done
        if(NULL!=s) {
          set(s, a);
        }
        state=0;
        s=NULL;
      } else {
        switch (state) {
        case 0:
          s=atom_getsymbol(av);
          state=1;
          break;
        case 1:
          a=av;
          state=2;
          break;
        default:
          break;
        }
      }
      av++;
    }

    binbuf_free(bb);
    return true;
  }

  void print(void) {
    std::map <t_symbol*, t_atom*>::iterator it;
    for(it = data.begin(); 
        it != data.end();
        it++)
      {
        if(it->first && it->first->s_name && it->second) {
          startpost("key ['%s']: ", it->first->s_name);
          postatom(1, it->second);
          endpost();
        }
      }
  }

};


GemSettingsData::GemSettingsData(void)
{
  int i=0;
#ifdef GEM_DEFAULT_FONT
  set(gensym("font.face"), gensym(GEM_DEFAULT_FONT));
#endif

  setEnv(gensym("settings.file"), "GEM_SETTINGS");
  t_atom*a=get(gensym("settings.file"));
  if(a) {
    t_symbol*s=atom_getsymbol(a);
    open(expandEnv(s, true)->s_name, ".");
  } else {
    while(s_configdir[i]) {
      open(GEM_SETTINGS_FILE, s_configdir[i]);
      i++;
    }
    open(GEM_SETTINGS_FILE, ".");
  }

  /* legacy settings via environmental variables */
  setEnv(gensym("texture.rectangle"), "GEM_RECTANGLE_TEXTURE");
  setEnv(gensym("singlecontext"), "GEM_SINGLE_CONTEXT"); // hmm, what's a better new name for this?
  setEnv(gensym("font.face"), "GEM_DEFAULT_FONT");


  t_gemclass *c = (t_gemclass*)class_new(gensym("Gem"), 0, 0, 0, 0, A_NULL);
  set(gensym("gem.path"), c->c_externdir);


  //  print();
}

GemSettingsData::~GemSettingsData(void) {

}

void GemSettingsData::setEnv(t_symbol*key, const char*env) {
  if(NULL==env)return;
  if(NULL==key)return;

  char*result=getenv(env);
  if(NULL==result) {
    return;
  }

  t_atom a;
  errno=0;

  /* try integer */
  long l=strtol(result, NULL, 0);
  if(0==errno) {
    SETFLOAT(&a, l);
    set(key, &a);
  }

  /* try float */
  double d=strtod(result, NULL);
  if(0==errno) {
    SETFLOAT(&a, d);
    set(key, &a);
  }

  /* try symbol */
  SETSYMBOL(&a, gensym(result));
  set(key, &a);

  // we ignore lists and other complex things for now
}

t_symbol*GemSettingsData::expandEnv(t_symbol*value, bool bashfilename) {
  if(NULL==value)
    return NULL;
  verbose(2, "expanding '%s'", value->s_name);

  if(bashfilename) {
    char bashBuffer[MAXPDSTRING];
    sys_bashfilename(value->s_name, bashBuffer);
    value=gensym(bashBuffer);
  }

#ifdef HAVE_WORDEXP_H
  wordexp_t pwordexp;

  if(0==wordexp(value->s_name, &pwordexp, 0)) {
    if(pwordexp.we_wordc) {
      // we only take the first match into account 
      value=gensym(pwordexp.we_wordv[0]);
    }
# ifdef __APPLE__
/* wordfree() broken on apple: keeps deallocating non-aligned memory */
#  warning wordfree() not called
# else
    wordfree(&pwordexp);
# endif
  }
#endif
#ifdef _WIN32
  char envVarBuffer[MAXPDSTRING];
  ExpandEnvironmentStrings(value->s_name, envVarBuffer, MAX_PATH - 2);
  value=gensym(envVarBuffer);
#endif

  verbose(2, "expanded '%s'", value->s_name);
  return value;
}

/* GemSettings: the public API */

GemSettingsData*GemSettings::settings=NULL;

/* public static functions */
void GemSettings::init() {
  if(settings)return;
  settings=new GemSettingsData(); 
}
void GemSettings::print() {
  if(!settings)return;
  settings->print();
}
void GemSettings::save() {
  if(!settings)return;
  post("GemSettings: save not yet implemented!");
}



t_atom*GemSettings::get(t_symbol*s) {
  if(NULL==settings) init();
  return settings->get(s);
}
t_atom*GemSettings::get(const char*s) {
  return get(gensym(s));
}

void GemSettings::set(t_symbol*s, t_atom*v) {
  if(NULL==settings) init();
  settings->set(s, v);
}
void GemSettings::set(const char*s, t_atom*v) {
  set(gensym(s), v);
}


void GemSettings::get(const char*key, int&value) {
  t_atom*a=get(key);
  if(a && A_FLOAT==a->a_type) {
    value=atom_getint(a);
  }
}
void GemSettings::get(const char*key, t_float&value) {
  t_atom*a=get(key);
  if(a && A_FLOAT==a->a_type) {
    value=atom_getfloat(a);
  }
}

void GemSettings::get(const char*key, t_symbol*&value) {
  t_atom*a=get(key);
  if(a) {
    value=atom_getsymbol(a);
  }
}

void GemSettings::get(const char*key, char*&value) {
  t_atom*a=get(key);
  if(a) {
    value=atom_getsymbol(a)->s_name;
  }
}
