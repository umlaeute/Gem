/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The base class for all externs written in C++

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_CPPEXTERN_H_
#define INCLUDE_CPPEXTERN_H_

#include "Base/config.h"
#include "Base/GemExportDef.h"

#include "m_pd.h"
#include "Base/GemVersion.h"

#include <new>


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemException
    
    an exception class...

DESCRIPTION
    
    this is a class, we can throw on creation, 
    to make sure that the pd-object can not be created


    
-----------------------------------------------------------------*/
class GEM_EXTERN GemException
{
 public:
  GemException() throw();
  GemException(const char*error) throw();
  virtual ~GemException() throw();
  
  virtual const char *what() const throw();
  virtual void report(const char*origin=NULL) const throw();
 private:
  const char*ErrorString;
};

class CPPExtern;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    Obj_header
    
    The obligatory object header

DESCRIPTION
    
    This is in a separate struct to assure that when PD uses the
    class, the t_object is the very first thing.  If it were the 
    first thing in CPPExtern, then there could be problems with
    the vtable.
    
-----------------------------------------------------------------*/
struct GEM_EXTERN Obj_header
{
    	//////////
    	// The obligatory object header
    	t_object    	    pd_obj;

    	//////////
    	// Our data structure
        CPPExtern           *data;
};

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    CPPExtern
    
    The base class for all externs written in C++

DESCRIPTION
    
    Each extern which is written in C++ needs to use the #defines at the
    end of this header file.  Currently, the operator new(size_t) and
    operator delete(void *) are not overridden.  This will be a problem
    when PD expects everything to fit in its memory space and control
    all memory allocation.
    
    The define
    
        CPPEXTERN_HEADER(NEW_CLASS, PARENT_CLASS)
    
    should be somewhere in your header file.
    One of the defines like
    
    CPPEXTERN_NEW(NEW_CLASS)
    CPPEXTERN_NEW_WITH_TWO_ARGS(NEW_CLASS, t_floatarg, A_FLOAT, t_floatarg, A_FLOAT)
    
    should be the first thing in your implementation file.
    NEW_CLASS is the name of your class and PARENT_CLASS is the 
    parent of your class.
        
-----------------------------------------------------------------*/
class GEM_EXTERN CPPExtern
{
    public:

        //////////
        // Constructor
    	CPPExtern();

        //////////
        // The Pd header
        t_object          *x_obj;

    	//////////
    	// Destructor
    	virtual ~CPPExtern() = 0;
    	
        //////////
        // Get the object's canvas
        t_canvas            *getCanvas()        { return(m_canvas); }

        //////////
        // This is a holder - don't touch it
        static t_object     *m_holder;

	//////////
	// my name
	static char          *m_holdname;
	t_symbol             *m_objectname;

    protected:
    	
    	//////////
    	// Creation callback
    	static void 	real_obj_setupCallback(t_class *) {}

    private:

        //////////
        // The canvas that the object is in
        t_canvas            *m_canvas;

 public:
        // these call pd's print-functions, and eventually prepend the object's name
	void            startpost(const char*format, ...);
	void            post(const char*format, ...);
	void            endpost(void);
	void            verbose(const int level, const char*format, ...);
	void            error(const char*format, ...); /* internally uses pd_error() */

 private:
	bool m_endpost; /* internal state for startpost/post/endpost */
	static bool checkGemVersion(int major, int minor);
};

// This has a dummy arg so that NT won't complain
GEM_EXTERN void *operator new(size_t, void *location, void *dummy);

////////////////////////////////////////
// This should be used in the header
////////////////////////////////////////

#define CPPEXTERN_HEADER(NEW_CLASS, PARENT_CLASS)    	    	\
public:     	    	    	    	    	    	    	\
static void obj_freeCallback(void *data)    	    	    	\
{ CPPExtern *mydata = ((Obj_header *)data)->data; delete mydata; \
  ((Obj_header *)data)->Obj_header::~Obj_header(); }   	    	\
static void real_obj_setupCallback(t_class *classPtr)  	    	\
{ PARENT_CLASS::real_obj_setupCallback(classPtr);    	    	\
  NEW_CLASS::obj_setupCallback(classPtr); }  	    	    	\
private:    	    	    	    	    	    	    	\
static inline NEW_CLASS *GetMyClass(void *data) {return((NEW_CLASS *)((Obj_header *)data)->data);} \
static void obj_setupCallback(t_class *classPtr);


////////////////////////////////////////
// This should be the first thing in the implementation file
////////////////////////////////////////

//
// NO ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW(NEW_CLASS)    	    	    	    	\
    REAL_NEW(NEW_CLASS)
//
// ONE ARGUMENT
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_WITH_ONE_ARG(NEW_CLASS, TYPE, PD_TYPE)    \
    REAL_NEW_WITH_ARG(NEW_CLASS, TYPE, PD_TYPE)
//
// GIMME ARGUMENT
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_WITH_GIMME(NEW_CLASS)  	    	    	\
    REAL_NEW_WITH_GIMME(NEW_CLASS)
//
// TWO ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_WITH_TWO_ARGS(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO)	\
    REAL_NEW_WITH_ARG_ARG(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO)
//
// THREE ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_WITH_THREE_ARGS(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO, TTHREE, PD_THREE)	\
    REAL_NEW_WITH_ARG_ARG_ARG(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO, TTHREE, PD_THREE)
//
// FOUR ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_WITH_FOUR_ARGS(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO, TTHREE, PD_THREE, TFOUR, PD_FOUR) \
    REAL_NEW_WITH_ARG_ARG_ARG_ARG(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO, TTHREE, PD_THREE, TFOUR, PD_FOUR)
//
// FIVE ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_WITH_FIVE_ARGS(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO, TTHREE, PD_THREE, TFOUR, PD_FOUR, TFIVE, PD_FIVE) \
    REAL_NEW_WITH_ARG_ARG_ARG_ARG_ARG(NEW_CLASS, TYPE, PD_TYPE, TTWO, PD_TWO, TTHREE, PD_THREE, TFOUR, PD_FOUR, TFIVE, PD_FIVE)
	


//////////////////////////////////////////////////////////////////////////////
// These should never be called or used directly!!!
//
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// static class:
//   by default classes are declared static
//   however, sometimes we need classes not-static, so we can refer to them
//   from other classes
///////////////////////////////////////////////////////////////////////////////
#ifdef NO_STATIC_CLASS
# define STATIC_CLASS
#else
# define STATIC_CLASS static
#endif

///////////////////////////////////////////////////////////////////////////////
// auto registering a class
// this creates a dummy class, whose constructor calls the setup-function 
// (registering the class with pd)
// a static copy of this class is created at runtime, to actually do the setup-call
///////////////////////////////////////////////////////////////////////////////
#ifdef NO_AUTO_REGISTER_CLASS
// if NO_AUTO_REGISTER_CLASS is defined, we will not register the class
# define AUTO_REGISTER_CLASS(NEW_CLASS)
#else
// for debugging we can show the which classes are auto-registering
# if 0
#  define POST_AUTOREGISTER(NEW_CLASS) post("auto-registering: "#NEW_CLASS)
# else
#  define POST_AUTOREGISTER(NEW_CLASS)
# endif
# define AUTO_REGISTER_CLASS(NEW_CLASS)			\
  class NEW_CLASS ## _cppclass {					\
  public:								\
  NEW_CLASS ## _cppclass() {POST_AUTOREGISTER(NEW_CLASS); NEW_CLASS ## _setup(); } \
};									\
  static NEW_CLASS ## _cppclass NEW_CLASS ## _instance;
#endif

///////////////////////////////////////////////////////////////////////////////
// setting the help-symbol
///////////////////////////////////////////////////////////////////////////////
#if defined HELPSYMBOL_BASE || defined HELPSYMBOL
# ifndef HELPSYMBOL_BASE
#  define HELPSYMBOL_BASE ""
# endif

# ifndef HELPSYMBOL
#  define SET_HELPSYMBOL(NEW_CLASS)                                     \
  class_sethelpsymbol(NEW_CLASS ## _class, gensym(HELPSYMBOL_BASE #NEW_CLASS))
# else
#  define SET_HELPSYMBOL(NEW_CLASS)				\
    class_sethelpsymbol(NEW_CLASS ## _class, gensym(HELPSYMBOL_BASE HELPSYMBOL))
# endif

#else 
# define SET_HELPSYMBOL(NEW_CLASS)
#endif /* HELPSYMBOL */


///////////////////////////////////////////////////////////////////////////////
// setting the class-flags
///////////////////////////////////////////////////////////////////////////////
#ifndef GEM_CLASSFLAGS
# define GEM_CLASSFLAGS 0
#endif
///////////////////////////////////////////////////////////////////////////////
// no args
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW(NEW_CLASS)        \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS ()                              \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)#NEW_CLASS;                    \
    obj->data = new NEW_CLASS;                                  \
    CPPExtern::m_holder = NULL;                                 \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(CPPExtern::m_holdname); return NULL;} \
}                                                               \
  extern "C" {                                                  \
    void NEW_CLASS ## _setup()                          \
    {                                                           \
      static bool firsttime=true; if(!firsttime)return; firsttime=false; \
      NEW_CLASS ## _class = class_new(                     \
                                           gensym(#NEW_CLASS),  \
                                           (t_newmethod)create_ ## NEW_CLASS, \
                                           (t_method)&NEW_CLASS::obj_freeCallback, \
                                           sizeof(Obj_header), GEM_CLASSFLAGS, \
                                           A_NULL);                     \
      SET_HELPSYMBOL(NEW_CLASS);                           \
      NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);      \
    }                                                                   \
  }                                                                     \
  AUTO_REGISTER_CLASS(NEW_CLASS);


///////////////////////////////////////////////////////////////////////////////
// one arg
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW_WITH_ARG(NEW_CLASS, VAR_TYPE, PD_TYPE) \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS (VAR_TYPE arg)                  \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)#NEW_CLASS;                    \
    obj->data = new NEW_CLASS(arg);                             \
    CPPExtern::m_holder = NULL;                                 \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(); return NULL;}           \
}   	    	    	    	    	    	    	    	\
extern "C" {	    	    	    	    	    	    	\
void NEW_CLASS ## _setup()    	    	    	    	\
{   	    	    	    	    	    	    	    	\
    NEW_CLASS ## _class = class_new(                       \
    	     	gensym(#NEW_CLASS), 	    	    	    	\
    	    	(t_newmethod)create_ ## NEW_CLASS,	    	\
    	    	(t_method)&NEW_CLASS::obj_freeCallback,         \
    	     	sizeof(Obj_header), GEM_CLASSFLAGS,             \
    	     	PD_TYPE,                                        \
    	     	A_NULL);      	    	    	    	    	\
    SET_HELPSYMBOL(NEW_CLASS);				\
    NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);	\
 }									\
}									\
  AUTO_REGISTER_CLASS(NEW_CLASS);

///////////////////////////////////////////////////////////////////////////////
// gimme arg
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW_WITH_GIMME(NEW_CLASS) \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS (t_symbol *s, int argc, t_atom *argv) \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)s->s_name;                     \
    obj->data = new NEW_CLASS(argc, argv);                      \
    CPPExtern::m_holder=NULL;                                   \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(); return NULL;}           \
}   	    	    	    	    	    	    	    	\
extern "C" {	    	    	    	    	    	    	\
void NEW_CLASS ## _setup()    	    	    	    	\
{   	    	    	    	    	    	    	    	\
    NEW_CLASS ## _class = class_new(                       \
    	     	gensym(#NEW_CLASS), 	    	    	    	\
    	    	(t_newmethod)create_ ## NEW_CLASS,	    	\
    	    	(t_method)&NEW_CLASS::obj_freeCallback,         \
    	     	sizeof(Obj_header), GEM_CLASSFLAGS,             \
    	     	A_GIMME,                                        \
    	     	A_NULL);      	    	    	    	    	\
    SET_HELPSYMBOL(NEW_CLASS);				\
    NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);	\
 }									\
}									\
  AUTO_REGISTER_CLASS(NEW_CLASS);

///////////////////////////////////////////////////////////////////////////////
// two args
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW_WITH_ARG_ARG(NEW_CLASS, ONE_VAR_TYPE, ONE_PD_TYPE, TWO_VAR_TYPE, TWO_PD_TYPE) \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS (ONE_VAR_TYPE arg, TWO_VAR_TYPE argtwo) \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)#NEW_CLASS;                    \
    obj->data = new NEW_CLASS(arg, argtwo);                     \
    CPPExtern::m_holder = NULL;                                 \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(); return NULL;}           \
}   	    	    	    	    	    	    	    	\
extern "C" {	    	    	    	    	    	    	\
void NEW_CLASS ## _setup()    	    	    	    	\
{   	    	    	    	    	    	    	    	\
    NEW_CLASS ## _class = class_new(                       \
    	     	gensym(#NEW_CLASS), 	    	    	    	\
    	    	(t_newmethod)create_ ## NEW_CLASS,	    	\
    	    	(t_method)&NEW_CLASS::obj_freeCallback,         \
    	     	sizeof(Obj_header), GEM_CLASSFLAGS,             \
    	     	ONE_PD_TYPE, TWO_PD_TYPE,                       \
    	     	A_NULL);      	    	    	    	    	\
    SET_HELPSYMBOL(NEW_CLASS);				\
    NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);	\
 }									\
}									\
  AUTO_REGISTER_CLASS(NEW_CLASS);

///////////////////////////////////////////////////////////////////////////////
// three args
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW_WITH_ARG_ARG_ARG(NEW_CLASS, ONE_VAR_TYPE, ONE_PD_TYPE, TWO_VAR_TYPE, TWO_PD_TYPE, THREE_VAR_TYPE, THREE_PD_TYPE) \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS (ONE_VAR_TYPE arg, TWO_VAR_TYPE argtwo, THREE_VAR_TYPE argthree) \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)#NEW_CLASS;                    \
    obj->data = new NEW_CLASS(arg, argtwo, argthree);           \
    CPPExtern::m_holder = NULL;                                 \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(); return NULL;}           \
}   	    	    	    	    	    	    	    	\
extern "C" {	    	    	    	    	    	    	\
void NEW_CLASS ## _setup()    	    	    	    	\
{   	    	    	    	    	    	    	    	\
    NEW_CLASS ## _class = class_new(                       \
    	     	gensym(#NEW_CLASS), 	    	    	    	\
    	    	(t_newmethod)create_ ## NEW_CLASS,	    	\
    	    	(t_method)&NEW_CLASS::obj_freeCallback,         \
    	     	sizeof(Obj_header), GEM_CLASSFLAGS,             \
    	     	ONE_PD_TYPE, TWO_PD_TYPE, THREE_PD_TYPE,        \
    	     	A_NULL);      	    	    	    	    	\
    SET_HELPSYMBOL(NEW_CLASS);				\
    NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);	\
 }									\
}									\
  AUTO_REGISTER_CLASS(NEW_CLASS);

///////////////////////////////////////////////////////////////////////////////
// four args
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW_WITH_ARG_ARG_ARG_ARG(NEW_CLASS, ONE_VAR_TYPE, ONE_PD_TYPE, TWO_VAR_TYPE, TWO_PD_TYPE, THREE_VAR_TYPE, THREE_PD_TYPE, FOUR_VAR_TYPE, FOUR_PD_TYPE) \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS (ONE_VAR_TYPE arg, TWO_VAR_TYPE argtwo, THREE_VAR_TYPE argthree, FOUR_VAR_TYPE argfour) \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)#NEW_CLASS;                    \
    obj->data = new NEW_CLASS(arg, argtwo, argthree, argfour);  \
    CPPExtern::m_holder = NULL;                                 \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(); return NULL;}           \
}   	    	    	    	    	    	    	    	\
extern "C" {	    	    	    	    	    	    	\
void NEW_CLASS ## _setup()    	    	    	    	\
{   	    	    	    	    	    	    	    	\
    NEW_CLASS ## _class = class_new(                       \
    	     	gensym(#NEW_CLASS), 	    	    	    	\
    	    	(t_newmethod)create_ ## NEW_CLASS,	    	\
    	    	(t_method)&NEW_CLASS::obj_freeCallback,         \
    	     	sizeof(Obj_header), GEM_CLASSFLAGS,             \
    	     	ONE_PD_TYPE, TWO_PD_TYPE, THREE_PD_TYPE, FOUR_PD_TYPE,	\
    	     	A_NULL);						\
    SET_HELPSYMBOL(NEW_CLASS);				\
    NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);	\
 }									\
}									\
  AUTO_REGISTER_CLASS(NEW_CLASS);

///////////////////////////////////////////////////////////////////////////////
// five args
///////////////////////////////////////////////////////////////////////////////
#define REAL_NEW_WITH_ARG_ARG_ARG_ARG_ARG(NEW_CLASS, ONE_VAR_TYPE, ONE_PD_TYPE, TWO_VAR_TYPE, TWO_PD_TYPE, THREE_VAR_TYPE, THREE_PD_TYPE, FOUR_VAR_TYPE, FOUR_PD_TYPE, FIVE_VAR_TYPE, FIVE_PD_TYPE) \
STATIC_CLASS t_class * NEW_CLASS ## _class;    	    	    	\
static void* create_ ## NEW_CLASS (ONE_VAR_TYPE arg, TWO_VAR_TYPE argtwo, THREE_VAR_TYPE argthree, FOUR_VAR_TYPE argfour, FIVE_VAR_TYPE argfive) \
{     	    	    	    	    	    	    	    	\
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(NEW_CLASS ## _class),(void *)NULL) Obj_header; \
    CPPExtern::m_holder = &obj->pd_obj;                         \
    CPPExtern::m_holdname=(char*)#NEW_CLASS;                            \
    obj->data = new NEW_CLASS(arg, argtwo, argthree, argfour, argfive);  \
    CPPExtern::m_holder = NULL;                                 \
    CPPExtern::m_holdname=NULL;                                 \
    return(obj);                                                \
  } catch (GemException e) {e.report(); return NULL;}           \
}   	    	    	    	    	    	    	    	\
extern "C" {	    	    	    	    	    	    	\
void NEW_CLASS ## _setup()    	    	    	    	\
{   	    	    	    	    	    	    	    	\
  if(!checkGemVersion(GEM_VERSION_MAJOR, GEM_VERSION_MINOR)) { \
    ::error("[%s] will not be available", #NEW_CLASS);	       \
    return;}						       \
    NEW_CLASS ## _class = class_new(                       \
    	     	gensym(#NEW_CLASS), 	    	    	    	\
    	    	(t_newmethod)create_ ## NEW_CLASS,	    	\
    	    	(t_method)&NEW_CLASS::obj_freeCallback,         \
    	     	sizeof(Obj_header), GEM_CLASSFLAGS,             \
    	     	ONE_PD_TYPE, TWO_PD_TYPE, THREE_PD_TYPE, FOUR_PD_TYPE, FIVE_PD_TYPE,	\
    	     	A_NULL);						\
    SET_HELPSYMBOL(NEW_CLASS);				\
    NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);	\
 }									\
}									\
  AUTO_REGISTER_CLASS(NEW_CLASS);

#endif	// for header file
