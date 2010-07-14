////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordV4L.h"
using namespace gem;
#include "Base/GemBase.h"
#include "Base/GemMan.h"

#include <stdlib.h>


#ifdef  HAVE_VIDEO4LINUX
REGISTER_RECORDFACTORY("V4L", recordV4L);
/////////////////////////////////////////////////////////
//
// recordV4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordV4L :: recordV4L(): 
  record()
{
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordV4L :: ~recordV4L()
{
  close();
}

void recordV4L :: close(void)
{

}

bool recordV4L :: open(const char *filename)
{

  return false;
}


/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
int recordV4L :: putFrame(imageStruct*img)
{



  return m_currentFrame;
}



/////////////////////////////////////////////////////////
// get number of codecs
//
/////////////////////////////////////////////////////////
int recordV4L :: getNumCodecs()
{
  return 1;
}
const char*recordV4L :: getCodecName(int i)
{
  if(i==0)
    return gensym("v4l")->s_name;
  return NULL;
}
const char*recordV4L :: getCodecDescription(int i)
{
  if(i==0)
    return gensym("v4l loopback")->s_name;
  return NULL;
}

/////////////////////////////////////////////////////////
// set codec by number
//
/////////////////////////////////////////////////////////
bool recordV4L :: setCodec(int num)
{
  m_codecname=NULL;
  m_codecnum=num;
  return true;
}
/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordV4L :: setCodec(const char*name)
{
  m_codecname=name;
  m_codecnum=-1;
  return true;
}
#else
recordV4L :: recordV4L(): record()
{
}
////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordV4L :: ~recordV4L()
{
}

#endif
