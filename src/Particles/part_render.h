/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Draw a part_render group

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_RENDER_H_
#define INCLUDE_PART_RENDER_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_render
    
	Draw a part_render group

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_render : public GemBase
{
  CPPEXTERN_HEADER(part_render, GemBase)

    public:

  //////////
  // Constructor
  part_render();
    	
  //////////
  virtual void 	render(GemState *state);
  virtual void 	postrender(GemState *state);

 protected:
    	
  //////////
  // Destructor
  virtual ~part_render();

  // How the object should be drawn
  float        *m_pos;
  void		colorMess(int state);
  bool		m_colorize;
  float        *m_colors;
  void		sizeMess(int state);
  bool		m_sizing;
  float        *m_sizes;

  int           m_number;

 private:
  //////////
  // static member functions
  static void 	colorMessCallback(void *data,  t_floatarg state);
  static void 	sizeMessCallback(void *data,  t_floatarg state);
};

#endif	// for header file
