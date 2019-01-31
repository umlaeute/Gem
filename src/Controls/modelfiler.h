/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  read in a model file

  Copyright (c) 2019 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEOS_MODELFILER_H_
#define _INCLUDE__GEM_GEOS_MODELFILER_H_

#include "Base/CPPExtern.h"
#include "Gem/Properties.h"
#include "RTE/Outlet.h"

#include <map>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  modelfiler

  read in a model file

  DESCRIPTION

  Inlet for a list - "model"

  "open" - the RGB model to set the object to

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class modelloader;
};
};

class GEM_EXTERN modelfiler : public CPPExtern
{
  CPPEXTERN_HEADER(modelfiler, CPPExtern);

public:

  //////////
  // Constructor
  modelfiler(int argc, t_atom*argv);

protected:

  //////////
  // Destructor
  virtual ~modelfiler(void);

  //////////
  // When an open is received
  virtual void openMess(const std::string&filename);

  virtual void enumPropertyMess(void);
  virtual void clearPropertiesMess(void);
  virtual void getPropertyMess(t_symbol*s, int, t_atom*);
  virtual void setPropertyMess(t_symbol*s, int, t_atom*);
  virtual void setPropertiesMess(t_symbol*, int, t_atom*);
  virtual void applyProperties(void);

  //////////
  // Set backend to use
  virtual void  backendMess(t_symbol*s, int argc, t_atom*argv);

  gem::plugins::modelloader*m_loader;

  gem::Properties m_readprops;

  std::string m_position[3];
  std::string m_texture[2];
  std::string m_color[4];
  std::string m_normal[3];

  gem::RTE::Outlet m_infoOut;
  std::vector<std::string> m_backends;
};

#endif  // for header file
