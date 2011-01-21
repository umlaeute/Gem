////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoUNICAP.h"

using namespace gem;

// for debugging...
#include "Gem/RTE.h"

/* debugging helpers  */
#define debugPost
#define debugThread

#if 0
# undef debugPost
# define debugPost ::startpost("%s:%s[%d]", __FILE__, __FUNCTION__, __LINE__); ::post
#endif

#if 0
# undef debugThread
# define debugThread ::startpost("%s:%s[%d]", __FILE__, __FUNCTION__, __LINE__); ::post
#endif

/////////////////////////////////////////////////////////
//
// videoUNICAP
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_UNICAP

#include <sys/stat.h>

REGISTER_VIDEOFACTORY("UNICAP", videoUNICAP);

videoUNICAP :: videoUNICAP() : video("UNICAP", 0)
{
  m_capturing=false;
#warning UNICAP
  provide("analog");
}
  
////////////////////////////////////////////////////////
// Destructor
//
////////////////////////////////////////////////////////
videoUNICAP :: ~videoUNICAP()
{
  close();
}

//////////////////
// this reads the data that was captured by capturing() and returns it within a pixBlock
pixBlock *videoUNICAP :: getFrame(){
  return &m_image;
}

bool videoUNICAP :: openDevice(gem::Properties&props) {
  return false;
}
void videoUNICAP :: closeDevice() {
}



/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoUNICAP :: startTransfer()
{
  return false;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoUNICAP :: stopTransfer()
{
  return true;
}

bool videoUNICAP :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat)return -1;
  m_reqFormat=format;
  restartTransfer();
  return true;
}

std::vector<std::string> videoUNICAP::enumerate() {
  std::vector<std::string> result;
  
  return result;
}

bool videoUNICAP :: enumProperties(gem::Properties&readable,
				 gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  return true;
}
void videoUNICAP :: getProperties(gem::Properties&props) {

}
void videoUNICAP :: setProperties(gem::Properties&props) {

}


#else
videoUNICAP ::  videoUNICAP() : video("") {}
videoUNICAP :: ~videoUNICAP() {}
#endif /* HAVE_UNICAP */
