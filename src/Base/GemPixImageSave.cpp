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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "Base/GemConfig.h"

#include "GemPixImageSave.h"
#include "Gem/RTE.h"

#include "GemPixUtil.h"
#include "plugins/imagesaver.h"

namespace gem {
  class PixImageSaver : public gem::plugins::imagesaver {
  private:
    static PixImageSaver*s_instance;
    std::vector<gem::plugins::imagesaver*>m_savers;
    std::vector<std::string>m_ids;

    PixImageSaver(void) {
      gem::PluginFactory<gem::plugins::imagesaver>::loadPlugins("image");
      std::vector<std::string>available_ids=gem::PluginFactory<gem::plugins::imagesaver>::getIDs();
      if(available_ids.size()>0) {
	startpost("Image saving support:");
	int i;
	for(i=0; i<available_ids.size(); i++) {
	  startpost(" %s", available_ids[i].c_str());
	}
	endpost();
      }

      addSaver(available_ids, "magick");
      addSaver(available_ids);
    }
    bool addSaver( std::vector<std::string>available, std::string ID=std::string(""))
    {
      int i=0;
      int count=0;

      std::vector<std::string>id;
      if(!ID.empty()) {
	// if requested 'cid' is in 'available' add it to the list of 'id's
	if(std::find(available.begin(), available.end(), ID)!=available.end()) {
	  id.push_back(ID);
	} else {
	  // request for an unavailable ID
	  verbose(2, "backend '%s' unavailable", ID.c_str());
	  return false;
	}
      } else {
	// no 'ID' given: add all available IDs
	id=available;
      }

      for(i=0; i<id.size(); i++) {
	std::string key=id[i];
	verbose(2, "trying to add '%s' as backend", key.c_str());
	if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
	  // not yet added, do so now!
	  gem::plugins::imagesaver*saver=
	    gem::PluginFactory<gem::plugins::imagesaver>::getInstance(key); 
	  if(NULL==saver)break;
	  m_ids.push_back(key);
	  m_savers.push_back(saver);
	  count++;
	  verbose(2, "added backend#%d '%s' @ 0x%x", m_savers.size()-1, key.c_str(), saver);
	}
      }
      return (count>0);
    }

  public:
    virtual ~PixImageSaver(void) {
      int i;
      for(i=0; i<m_savers.size(); i++) {
	delete m_savers[i];
	m_savers[i]=NULL;
      }
    }

    virtual bool save(std::string filename, const imageStruct&result) {
      int i;
      for(i=0; i<m_savers.size(); i++) {
	verbose(2, "trying saver[%d]=%s", i, m_ids[i].c_str());
	if(m_savers[i]->save(filename, result))
	  return true;
      }
      return false;
    }

    static PixImageSaver*getInstance(void) {
      if(NULL==s_instance) {
	s_instance=new PixImageSaver();
      }
      return s_instance;
    }

}; };

gem::PixImageSaver*gem::PixImageSaver::s_instance=NULL;

/***************************************************************************
 *
 * mem2image - Save an image to a file
 *
 ***************************************************************************/
GEM_EXTERN int mem2image(imageStruct* image, const char *filename, const int type)
{
   gem::PixImageSaver*piximagesaver=gem::PixImageSaver::getInstance();
   if(piximagesaver) {
     std::string fname=filename;
     if(piximagesaver->save(filename, *image)) {
       return (1);
     }
   }
   error("GEM: Unable to save image to '%s'", filename);
   return (0);
}
