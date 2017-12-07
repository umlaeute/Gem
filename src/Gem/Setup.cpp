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

/* OLDNAMES.lib pd.lib opengl32.lib glu32.lib freetype235mt.lib FTGL_static.lib libcpmt.lib msvcrt.lib msvcprt.lib ws2_32.lib pthreadVC.lib
 * pd.lib freetype235mt.lib FTGL_static.lib opengl32.lib glu32.lib ws2_32.lib pthreadVC.lib (?)

 * OLDNAMES.lib: _close, _open, _strdup
 * pd.lib
 * opengl32.lib
 * glu32.lib: gluFunctions (sphere, cylinder, lookat,...)
 * freetype235mt.lib FTGL_static.lib
 * libcpmt.lib: std::cerr,...
 * msvcrt.lib: typeinfo,...
 * ws2_32.lib
 * pthreadVC.lib

*/


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

#include <fcntl.h>

#ifndef _MSC_VER
# define _open open
# define _close close
#endif


static const char GEM_MAINTAINER[] = "IOhannes m zmoelnig";

static const char *GEM_AUTHORS[] = {
  "Chris Clepper",
  "Cyrille Henry",
  "IOhannes m zmoelnig"
};

static const char GEM_OTHERAUTHORS[] =
  "Guenter Geiger, Daniel Heckenberg, James Tittle, Hans-Christoph Steiner, et al.";

# include "m_pd.h"
# include "RTE/RTE.h"

#if defined HAVE_M_IMP_H
extern "C" {
# include "m_imp.h"
} // for extern "C"
#endif /* HAVE_M_IMP_H */



namespace
{
static bool checkVersion(const char*dirname, const char*filename,
                         int flags)
{
  t_binbuf*bb=binbuf_new();
  if(binbuf_read(bb, const_cast<char*>(filename), const_cast<char*>(dirname),
                 flags)) {
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
    if(A_SYMBOL!=argv[i].a_type) {
      continue;
    }
    if((_X==atom_getsymbol(argv+i)) && (i+6<argc)) {
      t_atom*ap=argv+i+1;
      if(_text   ==atom_getsymbol(ap+0) &&
          _version==atom_getsymbol(ap+3) &&
          A_FLOAT == ap[1].a_type &&
          A_FLOAT == ap[2].a_type
        ) {

        if(A_SYMBOL==ap[4].a_type) {
          gotversion=std::string(atom_getsymbol(ap+4)->s_name);
        } else {
          char buf[MAXPDSTRING];
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
    error("GEM: binary/abstractions version mismatch!");
    error("GEM:   continue at your own risk...");
    verbose(0, "GEM: binary is %d.%d, but Gem abstractions are %s",
            GEM_VERSION_MAJOR, GEM_VERSION_MINOR, gotversion.c_str());
    verbose(0,
            "GEM: This usually means that you have a path to another version of Gem stored in your startup preferences");
    verbose(0, "GEM: Consider removing the wrong path!");
  }

  return result;
}

static void addownpath(const char*filename)
{
  char buf[MAXPDSTRING];
  char*bufptr=NULL;
  int fd=-1;

  int flags=O_RDONLY;
#ifdef _WIN32
  flags |= _O_BINARY;
#endif

  /* check whether we can find the abstractions (because they are already in Pd's path) */
  if ((fd=canvas_open(NULL, filename, "", buf, &bufptr, MAXPDSTRING,
                      1))>=0) {
    gem::files::close(fd);
    checkVersion(buf, filename, flags);
    return;
  }

  char*mypath=0;
#ifdef HAVE_M_IMP_H
  t_class *c = (t_class*)class_new(gensym("Gem"), 0, 0, 0, 0, A_NULL);
  mypath=c->c_externdir->s_name;
#endif /* HAVE_S_STUFF_H */

  int success = 0;

  if (mypath) {
    /* check whether we can find the abstractions in Gem's own path */
    snprintf(buf, MAXPDSTRING-1, "%s/%s", mypath, filename);
    buf[MAXPDSTRING-1]=0;
    if ((fd=_open(buf, flags))>=0) {
      _close(fd);
      verbose(1, "GEM: trying to add Gem path '%s' to search-paths", mypath);
      gem::RTE::RTE*rte=gem::RTE::RTE::getRuntimeEnvironment();
      if(rte) {
        success = rte->addSearchPath(mypath, 0);
      }
    }
  }
  if(!success) {
    // can't find this abstraction...giving up
    std::string qpath = std::string("");
    if (mypath) {
      qpath += " '";
      qpath += mypath;
      qpath += "'";
    } else {
      error("GEM: unable to find Gem's abstractions");
    }
    error("GEM: please manually add Gem path%s to Pd's search path",
          qpath.c_str());
  }

  checkVersion(mypath, filename, flags);
}
}; // namespace

namespace gem
{
namespace plugins
{
void init(void);
};
namespace Settings
{
void init(void);
};
};

#ifndef BUILD_DATE
# define BUILD_DATE " on " __DATE__
#endif


namespace Gem
{
void setup()
{
  // startup GEM
  post("GEM: Graphics Environment for Multimedia");
  verbose(-1, "GEM: ver: %s", GemVersion::versionString());
  verbose(-1, "GEM: compiled " BUILD_DATE );
  verbose(-1, "GEM: maintained by %s", GEM_MAINTAINER);
  verbose(-1, "GEM: Authors :\tMark Danks (original version)");
  for(unsigned int i=0; i<sizeof(GEM_AUTHORS)/sizeof(*GEM_AUTHORS); i++) {
    verbose(-1, "GEM:\t\t%s", GEM_AUTHORS[i]);
  }
  verbose(-1, "GEM: with help by %s", GEM_OTHERAUTHORS);
  verbose(-1, "GEM: found a bug? miss a feature? please report it:");
  verbose(-1, "GEM: \thomepage https://gem.iem.at/");
  verbose(-1, "GEM: \tbug-tracker https://bugs.gem.iem.at/");
  verbose(-1,
          "GEM: \tmailing-list https://lists.puredata.info/listinfo/gem-dev/");

  gem::Settings::init();
  addownpath("Gem-meta.pd");
  GemMan::initGem();


  // initialize some plugins
  gem::plugins::init();
}
}; // namespace

namespace
{
void caseinsensitive_error(const char*gem)
{
  /* traditionally Gem can be loaded with wrong spelling on a case-insenstive platform
   * starting with 0.94 we issue a fat warning.
   * however, much of Gem's loading is done via CTORs and the Gem::setup() only finishes
   * the init phase; so we probably can never get rid of wrongly-spelled libraries ever.
   */
  error("GEM: rejecting incorrect spelling '%s' for cross-platform reasons: use 'Gem'!",
        gem);
}
};

extern "C" {
  GEM_EXTERN void Gem_setup()
  {
    Gem::setup();
  }

  GEM_EXTERN void gem_setup()
  {
    caseinsensitive_error("gem");
  }

  GEM_EXTERN void GEM_setup()
  {
    caseinsensitive_error("GEM");
  }

}   // for extern"C"
