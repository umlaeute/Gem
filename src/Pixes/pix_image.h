/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_IMAGE_H_
#define _INCLUDE__GEM_PIXES_PIX_IMAGE_H_

#include <string.h>

#include "Base/GemBase.h"
#include "Gem/Image.h"
#include "Gem/ImageIO.h"

#include "RTE/Outlet.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_image

  Loads an an image into a pix block

  KEYWORDS
  pix

  DESCRIPTION

  "open" - opens a file

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_image : public GemBase
{
  CPPEXTERN_HEADER(pix_image, GemBase);

public:

  //////////
  // Constructor
  pix_image(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~pix_image();

  //////////
  // When an open is received
  virtual void  openMess(std::string filename);

  //////////
  // Do the rendering
  virtual void  render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void  postrender(GemState *state);

  //////////
  virtual void  startRendering();

  //////////
  // Clean up the image and the pixBlock
  void          cleanImage();

  //-----------------------------------
  // GROUP:     Image data
  //-----------------------------------

  //////////
  // do we want threaded reading (default: yes);
  virtual void  threadMess(bool onoff);
  bool m_wantThread;

  //////////
  // the full filename of the image
  std::string            m_filename;
  gem::image::load::id_t m_id;

  //////////
  // The original image
  imageStruct     *m_loadedImage;

  //////////
  // The pixBlock with the current image
  pixBlock      m_pixBlock;

  void     loaded(const gem::image::load::id_t ID,
                  imageStruct*img,
                  const gem::Properties&props);


  gem::RTE::Outlet m_infoOut;

private:

  //////////
  // static member functions
  static void     loadCallback(void*data,
                               gem::image::load::id_t ID,
                               imageStruct*img,
                               const gem::Properties&props);
};

#endif  // for header file
