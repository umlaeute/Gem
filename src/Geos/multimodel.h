/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Load multiple images

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEOS_MULTIMODEL_H_
#define _INCLUDE__GEM_GEOS_MULTIMODEL_H_

#include "Base/GemBase.h"
#include <string.h>
#include "plugins/modelloader.h"
#include "Gem/Properties.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  multimodel

  Load multiple models

  DESCRIPTION

  Inlet for a list - "multimodel"

  "open" - the multimodel to set the object to

  -----------------------------------------------------------------*/
class GEM_EXTERN multimodel : public GemBase
{
  CPPEXTERN_HEADER(multimodel, GemBase);

    public:

  //////////
  // Constructor
  multimodel(t_symbol *filename, t_floatarg baseModel, t_floatarg topModel, t_floatarg skipRate);

 protected:

  //////////
  // Destructor
  virtual ~multimodel();

  //////////
  // When an open is received
  virtual void	openMess(const std::string&filename, float baseModel, float topModel, float skipRate);
  virtual void	open(const std::string&filename, int baseModel, int topModel, int skipRate);

  //////////
  virtual void	render(GemState *state);

  //////////
  // Change which model to display
  void	    	changeModel(int modelNum);


  void	    	cleanMultimodel(void);


  //-----------------------------------
  // GROUP:	Model data
  //-----------------------------------

  std::vector<gem::plugins::modelloader*>m_loaders;
  int 	    	m_curModel;


  virtual void  applyProperties(void);
  virtual void	rescaleMess(bool state);
  virtual void	textureMess(int state);
  virtual void	smoothMess(float state);
  float		m_currentH, m_currentW;

  gem::Properties m_properties;

 private:

  //////////
  // static member functions
  static void 	openMessCallback(void *data, t_symbol *filename, t_floatarg baseModel, t_floatarg topModel, t_floatarg skipRate);
};

#endif	// for header file
