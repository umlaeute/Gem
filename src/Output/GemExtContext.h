/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for an external openGL context

  Copyright (c) 2004 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/


#ifndef INCLUDE_GEMEXTCONTEXT_H_
#define INCLUDE_GEMEXTCONTEXT_H_



#ifndef EXPORT_CLASS
# ifdef _WINDOWS
#  define EXPORT_CLASS __declspec(dllexport)
# else
#  define EXPORT_CLASS
# endif
#endif


// a magic number: this is 'GemX' in ASCII
#define GEMEXTCONTEXT_MAGIC (long int)0x47656D58



class EXPORT_CLASS GemExtContext
{
 public:

  //////////
  // a mgic number to ensure that this is a valid GemExtContext
  long int magic;

  //////////
  // Constructor
  GemExtContext(){
    magic=GEMEXTCONTEXT_MAGIC;
  }

  //////////
  // Destructor
  ~GemExtContext(){ 
    magic=0;
  }

  ////////
  // make this context the current one
  virtual void makeCurrent()=0;

  ////////
  // in double buffered mode, we might want to swap Buffers
  virtual void swapBuffers()=0;

  ////////
  // is the context valid ??
  // no rendering will be done if this is not not TRUE !!
  virtual bool isValid()=0;
  
  /* the dimensions of the rendering window */
  virtual int getWidth()=0;
  virtual int getHeight()=0;

  /* returns '1 for single-buffered mode' and '2 for double-buffered mode' */
  virtual int bufferMode()=0;

};

#endif  // for header file
