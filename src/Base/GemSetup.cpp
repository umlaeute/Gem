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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/* -------------------------- setup function ------------------------------ */

#include "GemMan.h"
#include "GemVersion.h"

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
  "Guenter Geiger, Daniel Heckenberg, James Tittle, Hans-Christop Steiner, et al.";


extern "C" {
#define GEM_ADDOWNPATH
#ifdef GEM_ADDOWNPATH
# include "s_stuff.h"
# include <string.h>

  /* this is ripped from m_imp.h */
  struct _gemclass
  {
    t_symbol *c_name;                   /* name (mostly for error reporting) */
    t_symbol *c_helpname;               /* name of help file */
    t_symbol *c_externdir;              /* directory extern was loaded from */
    /* ... */ /* the real t_class continues here... */
  };
# define t_gemclass struct _gemclass

  static void Gem_addownpath(void) {
    char buf[MAXPDSTRING];
    char*bufptr=NULL;
    int fd=-1;

    int flags=O_RDONLY;
# ifdef _WIN32
    flags |= _O_BINARY;
# endif

    /* check whether we can find the abstractions (because they are already in Pd's path) */
    if ((fd=open_via_path(".", "hsv2rgb", ".pd", buf, &bufptr, MAXPDSTRING, 1))>=0){
      close(fd);
      return;
    }

    char*mypath=0;
    t_gemclass *c = (t_gemclass*)class_new(gensym("Gem"), 0, 0, 0, 0, A_NULL);
    mypath=c->c_externdir->s_name;

    /* check whether we can find the abstractions in Gem's own path */
    snprintf(buf, MAXPDSTRING-1, "%s/%s%s", mypath, "hsv2rgb", ".pd");
    buf[MAXPDSTRING-1]=0;
    if (fd=open(buf, flags)>=0){
      close(fd);
    } else {
      // can't find this abstraction...giving up
      return;
    }

    verbose(1, "eventually adding Gem path '%s' to search-paths", mypath);
# ifndef _WIN32
    sys_searchpath = namelist_append(sys_searchpath, mypath, 0);
# endif
  }
#else
  static void Gem_addownpath(void) {  }
#endif

  GEM_EXTERN void Gem_setup()
  {
    Gem_addownpath();
    
    // startup GEM

    post("GEM: Graphics Environment for Multimedia");
    post("GEM: ver: %s", GemVersion::versionString());
    post("GEM: compiled: " __DATE__);
    post("GEM: maintained by %s", GEM_MAINTAINER);
    post("GEM: Authors :\tMark Danks (original version)");
    for(unsigned int i=0; i<sizeof(GEM_AUTHORS)/sizeof(*GEM_AUTHORS); i++) {
      post("GEM:\t\t%s", GEM_AUTHORS[i]);
    }  
    post("GEM: with help by %s", GEM_OTHERAUTHORS);
    post("GEM: found a bug? miss a feature? please report it:");
    post("GEM: \thomepage http://gem.iem.at/");
    post("GEM: \tbug-tracker http://sourceforge.net/projects/pd-gem/");
    post("GEM: \tmailing-list http://lists.puredata.info/listinfo/gem-dev/");

    GemMan::initGem();
  }

  GEM_EXTERN void gem_setup()
  {
    Gem_setup();
  }

  GEM_EXTERN void GEM_setup()
  {
    Gem_setup();
  }

}   // for extern "C"
