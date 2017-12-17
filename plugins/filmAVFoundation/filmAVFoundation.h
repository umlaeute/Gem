/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a digital video (.mov, .mp4, etc) into a pix block on macOS 10.7+

Copyright (c) 2016 Dan Wilcox. danomatika@gmail.com

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMAVF_FILMAVF_H_
#define _INCLUDE_GEMPLUGIN__FILMAVF_FILMAVF_H_

#include "plugins/filmBase.h"
#include "Gem/Image.h"

// forward declare PIMPL types, depending on the compiler
#ifdef __OBJC__
  @class AVFMoviePlayer;
#else
  typedef struct objc_object AVFMoviePlayer;
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmAVFoundation

  Loads in a film on macOS using the AVFoundation framework

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXPORT filmAVFoundation : public filmBase {

public:

  //////////
  // Constructor
  filmAVFoundation(void);
  
  //////////
  // Destructor
  virtual ~filmAVFoundation(void);

  //////////
  // open a movie up
  virtual bool open(const std::string &filename, const gem::Properties &props);
  
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame(void);

  //////////
  // set the next frame to read;
  virtual errCode changeImage(int imgNum, int trackNum = -1);

protected:

  // PIMPL pointer to the Obj-C object behind this class
  AVFMoviePlayer* m_moviePlayer;

};};};

#endif // header guard
