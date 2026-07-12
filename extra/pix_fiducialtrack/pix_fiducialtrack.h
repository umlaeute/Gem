/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef INCLUDE_PIX_FIDUCIALTRACK_H_
#define INCLUDE_PIX_FIDUCIALTRACK_H_

#include "Base/GemPixObj.h"

#include <vector>

#include "fidtrackX.h"
#define MAX_FIDUCIAL_COUNT 512
#include "segment.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_fiducialtrack

  Track fiducials

  KEYWORDS
  pix

  DESCRIPTION

  Inlet for a list - "vec_thresh"
  Inlet for a float - "ft1"

  "vec_thresh" - The fiducialtrack vector
  "ft1" - Set all fiducialtracks to one value

  -----------------------------------------------------------------*/
class GEM_EXPORT pix_fiducialtrack : public GemPixObj
{
  CPPEXTERN_HEADER(pix_fiducialtrack, GemPixObj);

public:

  //////////
  // Constructor
  pix_fiducialtrack(t_symbol*);

protected:

  //////////
  // Destructor
  virtual ~pix_fiducialtrack();

  //////////
  virtual void  processGrayImage(imageStruct &image);

  int m_width,m_height;

  virtual void  deinit_segmenter();
  bool initialized;

  Segmenter segmenter;
  void treeMess(t_symbol*s);
  char m_treefile[MAXPDSTRING];
  void addMess(t_symbol*s);
  TreeIdMap treeidmap;

  FidtrackerX fidtrackerx;
  FiducialX fiducials[ 1024 ];

private:

  t_outlet*m_infoOut;
  t_atom   m_outlist[4];

  //////////
  // Static member functions
  static void 	treeMessCallback(void *data, t_symbol*s);
  static void 	addMessCallback(void *data, t_symbol*s);
};

#endif	// for header file
