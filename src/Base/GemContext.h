/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    a rendering context

    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMCONTEXT_H_
#define INCLUDE_GEMCONTEXT_H_

#include "Base/GemGL.h"
#include "Base/CPPExtern.h"

#include <vector>
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemContext
    
    a rendering context

DESCRIPTION
    
-----------------------------------------------------------------*/



# ifdef __APPLE__
#  define GemGlewXContext void
# elif defined _WIN32
typedef struct WGLEWContextStruct WGLEWContext;
#  define GemGlewXContext WGLEWContext
# elif defined __linux__
typedef struct GLXEWContextStruct GLXEWContext;
#  define GemGlewXContext GLXEWContext
# endif

typedef struct GLEWContextStruct GLEWContext;

class GEM_EXTERN GemContext : public CPPExtern
{
 public:
    
  //////////
  // Constructor
  GemContext(void);
    	
  //////////
  // Destructor
  virtual ~GemContext(void);

 protected:
  /* OUTPUT */

  /* an outlet to propagate information to the patch... mainly callbacks from the context */
  /* LATER think about detaching the output from the stack, so we can e.g. destroy a window from a mouse-callback */
  void info(std::vector<t_atom>);  
  void info(t_symbol*s, int, t_atom*);  
  void info(std::string);
  void info(std::string, t_float);
  void info(std::string, int i);
  void info(std::string, std::string);  

  /* tell downstream objects to render */
  void bang(void);

  /* mouse movement */
  void motion(int x, int y);
  /* mouse buttons */
  void button(int id, int state);
  /* keyboard buttons */
  //  void key(std::string id, int state);
  //void key(int id, int state);
  void key(std::string, int, int state);

  /* window resize/move */
  void dimension(unsigned int, unsigned int);
  void position (int, int);

  /* INPUT */

  /* create a new context
   * this MUST be called from the derived classes
   * as it will eventually establish a new glew-context
   * if <tt>false</tt> is returned, you should not continue
   */
  bool createContext(void);
  /* create a new window
   * make sure that this calls the parent's createContext() method
   */
  virtual bool create(void) = 0;

  /* destroy an established context */
  void destroyContext();
  /* create the current window
   * make sure that this calls the parent's destroyContext() method
   */
  virtual void destroy(void) = 0;

  /* make the object's context (window,...) the current context
   * this is virtual, so objects can add their own code
   * note however, that they should also call this (parent's) function within
   * typically implementations look like this:
   * bool <mywindow>::makeCurrent(void) {
   *    // do your own stuff
   *
   * is <tt>false</tt> is returned, do not attempt to use it (e.g. draw into it)
   */
  virtual bool makeCurrent(void) = 0;

  /* make the object's context (window,...) the current context
   * then switch the GemContext;
   * basically: { return (makeCurrent() && makeGemContextCurrent()); }
   */
  virtual bool assertCurrentContext(void);
  /*
   * make the GemContext current (reset stacks) 
   */
  bool makeGemContextCurrent(void);

  /* swap back/front buffer
   */
  virtual void swapBuffers(void) = 0;
  unsigned int m_buffer;

  /* dispatch messages from the window
   * this might get called more often than the render-cycle
   * it might also be called automatically as soon as the window
   * is create()ed (and until the window is destroy()ed)
   */
  virtual void dispatch(void);

  /* render to this window
   *  the default implementation calls:
   *    if(makeCurrent()){ bang; if(m_buffer==2)swap(); }
   * but you can override this, if you want to
   */
  virtual void render(void);

  /* set/get the dimension of the context
   * setting is done by supplying arguments to the method;
   * querying is done by supplying NO arguments
   * this should be kept throughout 
   */
  virtual void dimensionsMess(unsigned int width, unsigned int height) = 0;
  virtual void dimensionsMess(void);
  unsigned int m_width, m_height;

 public:
  static unsigned int getContextId(void);

  /* returns the last GemContext that called makeCurrent()
   * LATER: what to do if this has been invalidated (e.g. because the context was destroyed) ? 
   */
  static GLEWContext*getGlewContext(void);
  static GemGlewXContext*getGlewXContext(void);

 private:

  class PIMPL;
  PIMPL*m_pimpl;
};



#endif	// for header file
