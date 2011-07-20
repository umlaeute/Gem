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
#include "Gem/RTE.h"

#include <stdlib.h>

using namespace gem::plugins;

class record :: PIMPL {
public:
  bool running;
  PIMPL(void) :
	running(false)
  {}
};


/////////////////////////////////////////////////////////
//
// record
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

record :: record() : m_pimpl(new PIMPL())
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
record :: ~record()
{
  if(m_pimpl->running) {
	error("record: implementation forgot to call close() - please report a bug!");
  }
  delete m_pimpl;
  m_pimpl=NULL;
}

void record :: close(void)
{}

/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////
bool record :: start(const std::string filename, gem::Properties&props)
{
  if(m_pimpl->running)close();
  m_pimpl->running=false;
  m_props=props;

  m_pimpl->running=open(filename);

  return m_pimpl->running;
}
void record :: stop()
{
  if(m_pimpl->running)
    close();
  m_pimpl->running=false;
}

bool record::write(imageStruct*img) {
  if(!m_pimpl->running)
    return false;
  if(!img) {
    return true;
  }
  m_pimpl->running=putFrame(img);
  return m_pimpl->running;
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
