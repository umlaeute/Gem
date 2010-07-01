////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt" 
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoAVT.h"
#include <sstream>
using namespace gem;

#include "Gem/RTE.h"

#if 0
# define debug ::post
#else
# define debug
#endif

/////////////////////////////////////////////////////////
//
// videoAVT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_AVT

REGISTER_VIDEOFACTORY("avt", videoAVT);


class PvApiClass {
private:
 PvApiClass(void) {
     std::cerr << "PvApi init"<<std::endl;
 //  PvInitialize();
 }

 }
 virtual ~PvApiClass(void) {
     std::cerr << "PvApi deinit"<<std::endl;
 }


public:
 static bool initialize(void) {
   static PvApiClass*holder=NULL;
   if(NULL==holder) {
     holder=new PvApiClass();
     std::cerr << "initializing new PvApi: "<<(void*)holder<<std::endl;
   } else {
     std::cerr << "PvApi already initialized: "<<(void*)holder<<std::endl;
   }
   return true;
 }
};


videoAVT :: videoAVT() : video("avt"),
                               m_grabber(NULL)
{
  m_width=0;
  m_height=0;

  if(!PvApiClass::initialize())throw(new GemException("unable to initialization PvAPI"));

  provide("gige");
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoAVT :: ~videoAVT()
{
  close();
}


////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
bool videoAVT :: grabFrame() {
  return true;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoAVT :: openDevice()
{
  if(m_grabber)closeDevice();

  //return true;
  return false;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoAVT :: closeDevice() {
  if(m_grabber)delete m_grabber;
  m_grabber=NULL;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoAVT :: startTransfer()
{
  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoAVT :: stopTransfer()
{
  return true;
}

std::vector<std::string> videoAVT::enumerate() {
  std::vector<std::string> result;
  return result;
}




#else
videoAVT :: videoAVT() : video("")
{ }
videoAVT :: ~videoAVT()
{ }
#endif /* HAVE_AVT */
