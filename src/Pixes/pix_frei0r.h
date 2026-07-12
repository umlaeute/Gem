/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_FREI_R_H_
#define _INCLUDE__GEM_PIXES_PIX_FREI_R_H_

#include "Base/GemPixObj.h"

#ifndef DONT_WANT_FREI0R

#include "frei0r.h"

#endif /* DONT_WANT_FREI0R */



/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_frei0r

    load a Frei0r-plugin and use it do process the image-data

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/

class GEM_EXTERN pix_frei0r : public GemPixObj
{
  CPPEXTERN_HEADER(pix_frei0r, GemPixObj);

public:

  //////////
  // Constructor
  pix_frei0r(t_symbol*s);

protected:

  //////////
  // Destructor
  virtual ~pix_frei0r();

  //////////
  // Do the processing
  void        processRGBAImage(imageStruct &image);

  imageStruct m_image;
  imageStruct m_converterImage;
  class F0RPlugin;
  F0RPlugin*m_plugin;

  bool m_canopen;
  std::vector<t_inlet*>m_inlet;

  void parmMess(int param, int, t_atom*);
  void parmMess(const std::string&param, int, t_atom*);

  void openMess(t_symbol*name);
  void closeMess(void);

private:
  static void parmCallback(void *data, t_symbol*s, int argc, t_atom*argv);

public:
  static bool loader(const t_canvas *canvas, const std::string&classname,
                     const std::string&path, bool legacy);
};

#endif  // for header file
