/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  test images

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_TEST_H_
#define _INCLUDE__GEM_PIXES_PIX_TEST_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_test

  Change pix to greyscale

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_test : public GemPixObj
{
  CPPEXTERN_HEADER(pix_test, GemPixObj);

public:

  //////////
  // Constructor
  pix_test();
  pix_test(int argc, t_atom*argv);

protected:

  //////////
  // Destructor
  virtual ~pix_test();

  //////////
  // Do the processing
  virtual void  render(GemState*);
  virtual void  postrender(GemState*);

  pixBlock    m_pixBlock;

  void dimenMess(unsigned int, unsigned int);
  void csMess(std::string);
  void noiseMess(bool);

  bool m_noise;
};

#endif  // for header file
