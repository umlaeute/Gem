/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The base class for all externs written in C++

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_BASE_CPPEXTERN_H_
#define _INCLUDE__GEM_BASE_CPPEXTERN_H_

#include "Gem/ExportDef.h"

#include "Gem/RTE.h"
#include "Gem/Version.h"
#include "RTE/Atom.h"

#include <new>
#include <string>

class CPPExtern;
class Obj_header;

namespace gem
{
/* forward declaration of a generic exception handler for GemExceptions */
GEM_EXTERN void catchGemException(const char*objname, const t_object*obj);

class GEM_EXTERN CPPExtern_proxy {
private:
  class PIMPL;
  PIMPL*pimpl;
public:
  CPPExtern_proxy(t_class*cls, const char*name,
                  t_symbol*s, int argc, t_atom*argv,
                  size_t numtypes, const int*types,
                  bool wantInitmsgs);
  ~CPPExtern_proxy(void);
  int getNumArgs(void);
  void setObject(CPPExtern*);
  Obj_header*initialize(void);
};

};

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
struct GEM_EXTERN Obj_header {
  //////////
  // The obligatory object header
  t_object            pd_obj;

  //////////
  // Our data structure
  CPPExtern           *data;

  // This has a dummy arg so that NT won't complain
  void *operator new(size_t, void *location, void *dummy);
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

        CPPEXTERN_HEADER(NEW_CLASS, PARENT_CLASS);

    should be somewhere in your header file.
    One of the defines like

    CPPEXTERN_NEW(NEW_CLASS);
    CPPEXTERN_NEW_WITH_TWO_ARGS(NEW_CLASS, t_floatarg, A_FLOAT, t_floatarg, A_FLOAT);

    should be the first thing in your implementation file.
    NEW_CLASS is the name of your class and PARENT_CLASS is the
    parent of your class.

-----------------------------------------------------------------*/
class GEM_EXTERN CPPExtern
{
public:

  //////////
  // Constructor
  CPPExtern(void);

  //////////
  // The Pd header
  t_object* x_obj;

  //////////
  // Destructor
  virtual ~CPPExtern(void) = 0;

  //////////
  // Get the object's canvas
  const t_canvas* getCanvas(void) const;

  //////////
  // This is a holder - don't touch it
  static t_object* s_holder;

  //////////
  // my name
  static const char* s_holdname;

protected:

  //////////
  // Creation callback
  static void     real_obj_setupCallback(t_class *) {}

  ///////////
  // called directly before the destructor
  // normally you should not override this (use the dtor!)
  // if you do override this, make sure that you call the parent as well
  virtual void beforeDeletion();

private:
  class PIMPL;
  PIMPL*pimpl;

  //////////
  // The canvas that the object is in
  t_canvas* m_canvas;

public:
  // these call pd's print-functions, and eventually prepend the object's name
  void startpost(const char*format, ...) const;
  void post(const char*format, ...) const;
  void endpost(void) const;
  void verbose(const int level, const char*format, ...) const;
  void error(const char*format, ...) const; /* internally uses pd_error() */

  // searches for a file based on the parent abstraction's path
  // wraps open_via_path() and canvas_makefilename()
  // the full filename is returned
  // if the file does not exist, it is constructed
  std::string findFile(const std::string&filename,
                       const std::string&ext) const;
  std::string findFile(const std::string&filename) const;

private:
  static bool checkGemVersion(const int major, const int minor);
  CPPExtern(const CPPExtern&);
  virtual CPPExtern&operator=(const CPPExtern&);
};

////////////////////////////////////////
// This should be used in the header
////////////////////////////////////////

#define CPPEXTERN_HEADER(NEW_CLASS, PARENT_CLASS)               \
public:                                                         \
static void obj_freeCallback(void *data)                        \
{ CPPExtern *mydata = ((Obj_header *)data)->data;               \
  GetMyClass(data)->beforeDeletion();                           \
  delete mydata;                                                \
  ((Obj_header *)data)->Obj_header::~Obj_header(); }            \
static void real_obj_setupCallback(t_class *classPtr)           \
{ PARENT_CLASS::real_obj_setupCallback(classPtr);               \
  NEW_CLASS::obj_setupCallback(classPtr); }                     \
private:                                                        \
static inline NEW_CLASS *GetMyClass(void *data) {return((NEW_CLASS *)((Obj_header *)data)->data);} \
static void obj_setupCallback(t_class *classPtr);


////////////////////////////////////////
// This should be the first thing in the implementation file
////////////////////////////////////////

//
// NO ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED(NEW_CLASS, CLASSNAME)                       \
  REAL_NEW__CLASS(NEW_CLASS);                                           \
  const int typespecs[] = {};                                           \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                              \
  REAL_NEW__CREATE1(NEW_CLASS);                                         \
  REAL_NEW__SETOBJ(new NEW_CLASS());                                    \
  REAL_NEW__CREATE2(NEW_CLASS);                                         \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW(NEW_CLASS)                \
  CPPEXTERN_NEW_NAMED(NEW_CLASS, #NEW_CLASS)

//
// GIMME ARGUMENT
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED_WITH_GIMME(NEW_CLASS, CLASSNAME)            \
  REAL_NEW__CLASS(NEW_CLASS);                                           \
  const int typespecs[] = {};                                           \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                               \
  REAL_NEW__CREATE1(NEW_CLASS);                                         \
  REAL_NEW__SETOBJ(new NEW_CLASS(argc,argv));                           \
  REAL_NEW__CREATE2(NEW_CLASS);                                         \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW_WITH_GIMME(NEW_CLASS) \
  CPPEXTERN_NEW_NAMED_WITH_GIMME(NEW_CLASS, #NEW_CLASS)

//
// ONE ARGUMENT
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED_WITH_ONE_ARG(NEW_CLASS, CLASSNAME, TYPE1, PD_TYPE1)  \
  REAL_NEW__CLASS(NEW_CLASS);                                   \
  const int typespecs[] = {PD_TYPE1};                           \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                      \
  REAL_NEW__CREATE1(NEW_CLASS);                                 \
  REAL_NEW__MAKEVAR(1, TYPE1);                                  \
  REAL_NEW__SETOBJ(new NEW_CLASS(arg1));                        \
  REAL_NEW__CREATE2(NEW_CLASS);                                 \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW_WITH_ONE_ARG(NEW_CLASS, TYPE1, PD_TYPE1)  \
  CPPEXTERN_NEW_NAMED_WITH_ONE_ARG(NEW_CLASS, #NEW_CLASS, TYPE1, PD_TYPE1)

//
// TWO ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED_WITH_TWO_ARGS(NEW_CLASS, CLASSNAME, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2) \
  REAL_NEW__CLASS(NEW_CLASS);                                           \
  const int typespecs[] = {PD_TYPE1, PD_TYPE2};                         \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                              \
  REAL_NEW__CREATE1(NEW_CLASS);                                         \
  REAL_NEW__MAKEVAR(1, TYPE1);                                          \
  REAL_NEW__MAKEVAR(2, TYPE2);                                          \
  REAL_NEW__SETOBJ(new NEW_CLASS(arg1, arg2));                          \
  REAL_NEW__CREATE2(NEW_CLASS);                                         \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW_WITH_TWO_ARGS(NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2) \
  CPPEXTERN_NEW_NAMED_WITH_TWO_ARGS(NEW_CLASS, #NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2)
//
// THREE ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED_WITH_THREE_ARGS(NEW_CLASS, CLASSNAME, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3) \
  REAL_NEW__CLASS(NEW_CLASS);                                           \
  const int typespecs[] = {PD_TYPE1, PD_TYPE2, PD_TYPE3};               \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                              \
  REAL_NEW__CREATE1(NEW_CLASS);                                         \
  REAL_NEW__MAKEVAR(1, TYPE1);                                          \
  REAL_NEW__MAKEVAR(2, TYPE2);                                          \
  REAL_NEW__MAKEVAR(3, TYPE3);                                          \
  REAL_NEW__SETOBJ(new NEW_CLASS(arg1, arg2, arg3));                    \
  REAL_NEW__CREATE2(NEW_CLASS);                                         \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW_WITH_THREE_ARGS(NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3) \
  CPPEXTERN_NEW_NAMED_WITH_THREE_ARGS(NEW_CLASS, #NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3)
//
// FOUR ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED_WITH_FOUR_ARGS(NEW_CLASS, CLASSNAME, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3, TYPE4, PD_TYPE4) \
  REAL_NEW__CLASS(NEW_CLASS);                                           \
  const int typespecs[] = {PD_TYPE1, PD_TYPE2, PD_TYPE3, PD_TYPE4};     \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                              \
  REAL_NEW__CREATE1(NEW_CLASS);                                         \
  REAL_NEW__MAKEVAR(1, TYPE1);                                          \
  REAL_NEW__MAKEVAR(2, TYPE2);                                          \
  REAL_NEW__MAKEVAR(3, TYPE3);                                          \
  REAL_NEW__MAKEVAR(4, TYPE4);                                          \
  REAL_NEW__SETOBJ(new NEW_CLASS(arg1, arg2, arg3, arg4));              \
  REAL_NEW__CREATE2(NEW_CLASS);                                         \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW_WITH_FOUR_ARGS(NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3, TYPE4, PD_TYPE4) \
  CPPEXTERN_NEW_NAMED_WITH_FOUR_ARGS(NEW_CLASS, #NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3, TYPE4, PD_TYPE4)

//
// FIVE ARGUMENTS
/////////////////////////////////////////////////
#define CPPEXTERN_NEW_NAMED_WITH_FIVE_ARGS(NEW_CLASS, CLASSNAME, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3, TYPE4, PD_TYPE4, TYPE5, PD_TYPE5) \
  REAL_NEW__CLASS(NEW_CLASS);                                           \
  const int typespecs[] = {PD_TYPE1, PD_TYPE2, PD_TYPE3, PD_TYPE4, PD_TYPE5}; \
  REAL_NEW__CREATE0(NEW_CLASS, typespecs);                              \
  REAL_NEW__CREATE1(NEW_CLASS);                                         \
  REAL_NEW__MAKEVAR(1, TYPE1);                                          \
  REAL_NEW__MAKEVAR(2, TYPE2);                                          \
  REAL_NEW__MAKEVAR(3, TYPE3);                                          \
  REAL_NEW__MAKEVAR(4, TYPE4);                                          \
  REAL_NEW__MAKEVAR(5, TYPE5);                                          \
  REAL_NEW__SETOBJ(new NEW_CLASS(arg1, arg2, arg3, arg4, arg5));        \
  REAL_NEW__CREATE2(NEW_CLASS);                                         \
  REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)
#define CPPEXTERN_NEW_WITH_FIVE_ARGS(NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3, TYPE4, PD_TYPE4, TYPE5, PD_TYPE5) \
  CPPEXTERN_NEW_NAMED_WITH_FIVE_ARGS(NEW_CLASS, #NEW_CLASS, TYPE1, PD_TYPE1, TYPE2, PD_TYPE2, TYPE3, PD_TYPE3, TYPE4, PD_TYPE4, TYPE5, PD_TYPE5)


//////////////////////////////////////////////////////////////////////////////
// These should never be called or used directly!!!
//
//
///////////////////////////////////////////////////////////////////////////////

#define REAL_NEW__CLASS(NEW_CLASS)                                      \
  STATIC_CLASS t_class * NEW_CLASS ## _class;                           \
  static void* create_ ## NEW_CLASS (t_symbol*s, int argc, t_atom*argv) { \

#define REAL_NEW__CREATE0(NEW_CLASS, types)                             \
  const unsigned int num##types = sizeof(types)/sizeof(*types);         \
  try {                                                                 \
    gem::CPPExtern_proxy proxy(NEW_CLASS ## _class, #NEW_CLASS, s, argc, argv, num##types, types, GEM_ARGMESSAGES)

#define REAL_NEW__CREATE1(NEW_CLASS)                                    \
    argc = proxy.getNumArgs()

#define REAL_NEW__CREATE2(NEW_CLASS)                                    \
  return proxy.initialize();                                            \
  } catch (...) {gem::catchGemException(CPPExtern::s_holdname, CPPExtern::s_holder); return NULL;} \
  }

#define REAL_NEW__SETUP(NEW_CLASS, CLASSNAME)                           \
  extern "C" {                                                          \
  GEM_EXPORT void NEW_CLASS ## _setup(void)                             \
  {                                                                     \
  static int recalled=0; if(recalled)return; recalled=1;                \
  NEW_CLASS ## _class = class_new(                                      \
    gensym(CLASSNAME),                                                 \
    (t_newmethod)create_ ## NEW_CLASS,                                  \
    (t_method)&NEW_CLASS::obj_freeCallback,                             \
    sizeof(Obj_header), GEM_CLASSFLAGS,                                 \
    A_GIMME, A_NULL);                                                   \
  SET_HELPSYMBOL(NEW_CLASS, CLASSNAME);                                 \
  NEW_CLASS::real_obj_setupCallback(NEW_CLASS ## _class);               \
  }                                                                     \
   }                                                                    \
  AUTO_REGISTER_CLASS(NEW_CLASS)

#define REAL_NEW__MAKEVAR(num, type)                                    \
  type arg##num = (num>argc)?gem::RTE::Atom():gem::RTE::Atom(argv+num-1)

#define REAL_NEW__SETOBJ(obj)                   \
  proxy.setObject(obj)

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
typedef void (*t_class_setup)(void);
extern "C" {
  GEM_EXTERN void gem_register_class_setup(const char*name, t_class_setup);
}

#ifdef NO_AUTO_REGISTER_CLASS
// if NO_AUTO_REGISTER_CLASS is defined, we will not register the class
# define AUTO_REGISTER_CLASS(NEW_CLASS) \
  static int NEW_CLASS ## _dummyinstance
#else
// for debugging we can show which classes are auto-registering
# define AUTO_REGISTER_CLASS(NEW_CLASS)                                 \
  class NEW_CLASS ## _cppclass {                                        \
  public:                                                               \
  NEW_CLASS ## _cppclass(void) {gem_register_class_setup(#NEW_CLASS, & NEW_CLASS ## _setup); } \
};                                                                      \
  static NEW_CLASS ## _cppclass NEW_CLASS ## _instance
#endif

///////////////////////////////////////////////////////////////////////////////
// setting the help-symbol
///////////////////////////////////////////////////////////////////////////////
#if defined HELPSYMBOL_BASE || defined HELPSYMBOL
# ifndef HELPSYMBOL_BASE
#  define HELPSYMBOL_BASE ""
# endif

# ifndef HELPSYMBOL
#  define SET_HELPSYMBOL(NEW_CLASS, CLASSNAME)                          \
  class_sethelpsymbol(NEW_CLASS ## _class, gensym(HELPSYMBOL_BASE CLASSNAME))
# else
#  define SET_HELPSYMBOL(NEW_CLASS, CLASSNAME)                          \
    class_sethelpsymbol(NEW_CLASS ## _class, gensym(HELPSYMBOL_BASE HELPSYMBOL))
# endif

#else
# define SET_HELPSYMBOL(NEW_CLASS, CLASSNAME)
#endif /* HELPSYMBOL */

///////////////////////////////////////////////////////////////////////////////
// setting the class-flags
///////////////////////////////////////////////////////////////////////////////
#ifndef GEM_CLASSFLAGS
# define GEM_CLASSFLAGS 0
#endif
#ifndef GEM_ARGMESSAGES
# define GEM_ARGMESSAGES 1
#endif

// macros for boilerplate code to object messages
#include "RTE/MessageCallbacks.h"

#endif  // for header file
