/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2001, FUKUCHI Kentaro
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_AGING_H_
#define _INCLUDE__GEM_PIXES_PIX_AGING_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_aging

KEYWORDS
    pix

DESCRIPTION
    Make pix look old

    this is based on EffecTV by Fukuchi Kentauro
    * AgingTV - film-aging effect.

-----------------------------------------------------------------*/
typedef struct _scratch {
  int life;
  int x;
  int dx;
  int init;
} t_scratch;

class GEM_EXTERN pix_aging : public GemPixObj
{
  CPPEXTERN_HEADER(pix_aging, GemPixObj);

public:

  //////////
  // Constructor
  pix_aging();

protected:

  //////////
  // Destructor
  virtual ~pix_aging();

  //////////
  // Do the processing
  virtual void    processImage(imageStruct &image);

  void scratchMess(int scratchlines);

  ////////
  // which FX do we want ?
  int m_coloraging;
  int m_scratching;
  int m_pits;
  int m_dusts;


  ////////
  // scratching
  int m_scratchlines;
  t_scratch *m_scratch;

  ////////
  // pits
  int m_pitinterval;
  int m_areascale;

  ////////
  // dust
  int m_dustinterval;

private:

  //////////
  // Static member callbacks
  static void colorMessCallback(void *dump, t_float state);
  static void scratchMessCallback(void *dump, t_float state);
  static void pitsMessCallback(void *dump, t_float state);
  static void dustMessCallback(void *dump, t_float state);
};

#endif  // for header file
