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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"
/* -------------------------- setup function ------------------------------ */

#include "Gem/Manager.h"
#include "Gem/Version.h"
#include "Gem/Files.h"

#include <stdio.h>

#ifdef _WIN32
# include <io.h>
# include <windows.h>
# define snprintf _snprintf
#else
# include <unistd.h>
#endif

# include <fcntl.h>


static const char GEM_MAINTAINER[] = "IOhannes m zmoelnig";

static const char *GEM_AUTHORS[] = {
  "Chris Clepper",
  "Cyrille Henry",
  "IOhannes m zmoelnig"
};

static const char GEM_OTHERAUTHORS[] =
  "Guenter Geiger, Daniel Heckenberg, James Tittle, Hans-Christoph Steiner, et al.";

extern "C" {
#if defined HAVE_S_STUFF_H
# include "s_stuff.h"

# ifndef _WIN32
  /* MSVC/MinGW cannot really handle these exported symbols */
#  define GEM_ADDPATH
# endif
  
#endif /* HAVE_S_STUFF_H */
  
  /* this is ripped from m_imp.h */
  struct _gemclass
  {
    t_symbol *c_name;                   /* name (mostly for error reporting) */
    t_symbol *c_helpname;               /* name of help file */
    t_symbol *c_externdir;              /* directory extern was loaded from */
    /* ... */ /* the real t_class continues here... */
  };
# define t_gemclass struct _gemclass
} // for extern "C"



namespace Gem {
  static bool checkVersion(const char*dirname, const char*filename, int flags) {
    t_binbuf*bb=binbuf_new();
    if(binbuf_read(bb, const_cast<char*>(filename), const_cast<char*>(dirname), flags)) {
      /* couldn't find the file */
      return true;
    }

    int argc = binbuf_getnatom(bb);
    t_atom*argv=binbuf_getvec(bb);

    const t_symbol* _X=gensym("#X");
    const t_symbol* _text=gensym("text");
    const t_symbol* _version=gensym("VERSION");

    std::string gotversion;

     int i;
    /* search for: "#X text <num> <num> VERSION <string>;" */
    //              #X text 10 30 VERSION 0.93 \;

    for(i=0; i<argc; i++) {
      if(A_SYMBOL!=argv[i].a_type)continue;
      if((_X==atom_getsymbol(argv+i)) && (i+6<argc)) {
        t_atom*ap=argv+i+1;     
        if(_text   ==atom_getsymbol(ap+0) &&
           _version==atom_getsymbol(ap+3) &&
           A_FLOAT == ap[1].a_type &&
           A_FLOAT == ap[2].a_type
           ) {
          char buf[MAXPDSTRING];
    
          if(A_SYMBOL==ap[4].a_type) {
            gotversion=std::string(atom_getsymbol(ap+4)->s_name);
          } else {
            snprintf(buf, MAXPDSTRING-1, "%g", atom_getfloat(ap+4));
            gotversion=std::string(buf);
          }
          break;
        }
      }
    }
    binbuf_free(bb);

    int major, minor;
    sscanf(gotversion.c_str(), "%d.%d", &major, &minor);

    bool result=gem::Version::versionCheck(major,minor);
    if(!result) {
      error("Gem binary/abstractions version mismatch!");
      verbose(0, "Gem binary is %d.%d, but Gem abstractions are %s", GEM_VERSION_MAJOR, GEM_VERSION_MINOR, gotversion.c_str());
      verbose(0, "This usually means that you have a path to another version of Gem stored in your startup preferences");
      verbose(0, "Consider removing the wrong path!");
    }
    
    return result;
  }


  static void addownpath(const char*filename) {
    char buf[MAXPDSTRING];
    char*bufptr=NULL;
    int fd=-1;

    int flags=O_RDONLY;
#ifdef _WIN32
    flags |= _O_BINARY;
#endif

    /* check whether we can find the abstractions (because they are already in Pd's path) */
    if ((fd=canvas_open(NULL, filename, "", buf, &bufptr, MAXPDSTRING, 1))>=0){
      gem::files::close(fd);
      checkVersion(buf, filename, flags);
      return;
    }

    char*mypath=0;
    t_gemclass *c = (t_gemclass*)class_new(gensym("Gem"), 0, 0, 0, 0, A_NULL);
    mypath=c->c_externdir->s_name;

    /* check whether we can find the abstractions in Gem's own path */
    snprintf(buf, MAXPDSTRING-1, "%s/%s", mypath, filename);
    buf[MAXPDSTRING-1]=0;
    if ((fd=open(buf, flags))>=0){
      close(fd);
    } else {
      // can't find this abstraction...giving up
      verbose(0, "please add path to '%s' to your search-path!", filename);
      return;
    }

#ifdef GEM_ADDPATH
    verbose(1, "eventually adding Gem path '%s' to search-paths", mypath);
    sys_searchpath = namelist_append(sys_searchpath, mypath, 0);
#else
    verbose(0, "please manually add '%s' to your search-path!", mypath);
#endif

    checkVersion(mypath, filename, flags);
  }
}; // namespace

namespace GemSettings { void init(void); };
namespace Gem {
  void setup()
  {
    // startup GEM
    post("GEM: Graphics Environment for Multimedia");
    verbose(-1, "GEM: ver: %s", GemVersion::versionString());
    verbose(-1, "GEM: compiled: " __DATE__);
    verbose(-1, "GEM: maintained by %s", GEM_MAINTAINER);
    verbose(-1, "GEM: Authors :\tMark Danks (original version)");
    for(unsigned int i=0; i<sizeof(GEM_AUTHORS)/sizeof(*GEM_AUTHORS); i++) {
      verbose(-1, "GEM:\t\t%s", GEM_AUTHORS[i]);
    }
    verbose(-1, "GEM: with help by %s", GEM_OTHERAUTHORS);
    verbose(-1, "GEM: found a bug? miss a feature? please report it:");
    verbose(-1, "GEM: \thomepage http://gem.iem.at/");
    verbose(-1, "GEM: \tbug-tracker http://sourceforge.net/projects/pd-gem/");
    verbose(-1, "GEM: \tmailing-list http://lists.puredata.info/listinfo/gem-dev/");

    GemSettings::init();
    addownpath("Gem-meta.pd");
    GemMan::initGem();
  }
}; // namespace

extern "C" {
  GEM_EXTERN void Gem_setup()
  {
    Gem::setup();
  }

  GEM_EXTERN void gem_setup()
  {
    Gem_setup();
  }

  GEM_EXTERN void GEM_setup()
  {
    Gem_setup();
  }

}   // for extern"C"
