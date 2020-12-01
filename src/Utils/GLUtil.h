/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemGLUtil.h
       - contains functions for graphics
       - part of GEM

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_UTILS_GLUTIL_H_
#define _INCLUDE__GEM_UTILS_GLUTIL_H_

#include "Gem/ExportDef.h"

/* for t_symbol/t_atom */
/* LATER get rid of that (std::string) */
struct _atom;
struct _symbol;
#include <string>

/* for GLenum */
#include "Gem/GemGL.h"
#include "Gem/Exception.h"


namespace gem
{
namespace utils
{
namespace gl
{
GEM_EXTERN extern GLenum        glReportError (bool verbose=true);
GEM_EXTERN extern int           getGLdefine(const char *name);
GEM_EXTERN extern int           getGLdefine(const struct _symbol *name);
GEM_EXTERN extern int           getGLdefine(const struct _atom *name);
GEM_EXTERN extern int           getGLbitfield(int argc,
    struct _atom *argv);
GEM_EXTERN extern void          gluLookAt(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
GEM_EXTERN extern void          gluPerspective(GLdouble, GLdouble, GLdouble, GLdouble);



/* mapping between GLSL-program IDs and float */
/* this can also return different IDs for different contexts */
class GEM_EXTERN GLuintMap
{
public:
  GLuintMap(const std::string&name);
  virtual ~GLuintMap();
  GLuint get(float); // throws GemException
  /* map a GLuint to float; if float is 0, the new mapping is created,
   * else we just update an existing one
   * updating is especially useful with multiple contexts */
  float set(GLuint, float f=0);

  void del(float);


  /* usage:
     mapper=GLuintMap("glsl_program");
     prog=glCreateProgram();
     progMapped=mapper.set(prog);
     // for multi-context, you probably want to have a single float map to multiple
     // programIDs; you add (or update) an existing mapping with:
     //    progMapped=mapper.set(prog, progMapped));
     outlet_float(m_out, mapper.get(progMapped));

     // ... somewhere else
     mapper=GLuintMap("glsl_program");
     GLuint id = mapper.get(atom_getfloat(ap));
  */

  /* multi-context:
       mapper=GLuintMap("glsl_program");
       float progF=0;
       switchContext(A);
       prog=glCreateProgram();
       progF=mapper.set(prog, progF);
       print(prog,progF); // "3" "3.1415"
       switchContext(B);
       prog=glCreateProgram();
       progF=mapper.set(prog, progF);
       print(prog,progF); // "6" "3.1415"

       // ...somewhere else
       mapper=GLuintMap("glsl_program");
       prog=mapper.get(progF);
       print(prog,progF); // "3" "3.1415"
       switchContext(B);
       prog=mapper.get(progF);
       print(prog,progF); // "6" "3.1415"
  */
private:
  struct PIMPL;
  PIMPL*m_pimpl;
  GLuintMap();
  GLuintMap&operator=(const GLuintMap&);
};
};
};
}; /* namespace */

#endif  // for header file
