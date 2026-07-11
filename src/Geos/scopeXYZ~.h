/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_SCOPEXYZ__H_
#define _INCLUDE__GEM_GEOS_SCOPEXYZ__H_

#include "Base/GemShape.h"
#include "Gem/VertexBuffer.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  scopeXYZ

  Creates a 3D scope

  KEYWORDS
  geo

  DESCRIPTION

  Inlet~ for signal X
  Inlet~ for signal Y
  Inlet~ for signal Z

  -----------------------------------------------------------------*/
class GEM_EXTERN scopeXYZ : public GemShape
{
  CPPEXTERN_HEADER(scopeXYZ, GemShape);

public:

  //////////
  // Constructor
  scopeXYZ(t_floatarg len=0);

protected:

  //////////
  // Destructor
  virtual ~scopeXYZ();

  //////////
  // Do the renderShapeing
  virtual void  renderShape(GemState *state);

  //////////
  // The width of the lines in line draw mode
  void          linewidthMess(float linewidth);

  //-----------------------------------
  // GROUP:     Member variables
  //-----------------------------------

  //////////
  // the number of points
  unsigned int m_blocksize;
  unsigned int m_length;
  virtual void  doLengthMess(unsigned int length);
  virtual void  lengthMess(int length);
  virtual void  setBlocksize(unsigned int length);

  virtual void  bangMess();

  int m_position;

  //////////
  // The vertices
  gem::VertexBuffer m_vertices;

  t_inlet*m_inX, *m_inY, *m_inZ;


  void perform(unsigned int count, t_sample*X, t_sample*Y, t_sample*Z);

private:

  //////////
  // Static member functions
  static void   dspCallback(void *data, t_signal** sp);
  static t_int*   perform(t_int* w);

};

#endif  // for header file
