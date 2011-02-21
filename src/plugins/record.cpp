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

#include "plugins/record.h"

using namespace gem;

#include <stdlib.h>

/////////////////////////////////////////////////////////
//
// record
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

record :: record()
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
record :: ~record()
{
  close();
}

void record :: close(void)
{}

/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////
bool record :: start(const std::string filename, gem::Properties&props)
{
  if(m_running)close();
  m_running=false;
  m_props=props;

  m_running=open(filename);

  return m_running;
}
void record :: stop()
{
  if(m_running)
    close();
  m_running=false;
}

bool record::write(imageStruct*img) {
  if(!m_running)
    return false;
  if(!img) {
    return true;
  }
  m_running=putFrame(img);
  return m_running;
}

bool record :: open(const std::string filename)
{
  return false;
}

/////////////////////////////////////////////////////////
// set the codec
//
/////////////////////////////////////////////////////////
bool record :: dialog()
{
  return false;
}

/////////////////////////////////////////////////////////
// get number of codecs
//
/////////////////////////////////////////////////////////
std::vector<std::string>record :: getCodecs()
{
  std::vector<std::string>result;
  m_codecdescriptions.clear();
  return result;
}
const std::string record :: getCodecDescription(const std::string name)
{
  std::map<std::string,std::string>::iterator it = m_codecdescriptions.find(name);

  if(it==m_codecdescriptions.end()) {
    return name;
  }

  return it->second;
}


/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool record :: setCodec(const std::string name)
{
  return false;
}

bool record :: enumProperties(gem::Properties&props) 
{
  props.clear();
  return false;
}

INIT_RECORDFACTORY();
