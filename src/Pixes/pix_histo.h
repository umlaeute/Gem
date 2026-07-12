/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_HISTO_H_
#define _INCLUDE__GEM_PIXES_PIX_HISTO_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_histo

  KEYWORDS
  pix

  DESCRIPTION
  calc the histogramm of the pixBuf and write it to table(s)

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_histo : public GemPixObj
{
  CPPEXTERN_HEADER(pix_histo, GemPixObj);

public:

  //////////
  // Constructor
  pix_histo(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~pix_histo();

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
  virtual void  processGrayImage(imageStruct &image);
  virtual void  processYUVImage(imageStruct &image);

  //////////
  // tables to hold the curves
  t_symbol* name_R, *name_G, *name_B, *name_A;

  //////////
  // mode
  int m_mode;


  //////////
  // update graphs
  void update_graphs(void);

  //////////
  // Set new arrays
  void  setMess(int argc, t_atom *argv);

  //////////
  // the methods
  static void setMessCallback(void *data, t_symbol* s, int argc,
                              t_atom* argv);

};

#endif  // for header file
