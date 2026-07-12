/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2001, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_INFO_H_
#define _INCLUDE__GEM_PIXES_PIX_INFO_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

    pix_info

        Get image information

KEYWORDS
    pix

DESCRIPTION

 outputs xsize, ysize, csize, format, data

-----------------------------------------------------------------*/
class GEM_EXTERN pix_info : public GemBase
{
  CPPEXTERN_HEADER(pix_info, GemBase);

public:

  //////////
  // Constructor
  pix_info(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_info();

  //////////
  // All we want is the pixel information, so this is a complete override.
  //      virtual void    render(GemState *state);

  //////////
  virtual void    render(GemState*);

  void showInfoRaw(pixBlock*img);
  void showInfoCooked(pixBlock*img);

  void symbolicMess(bool);

  //////////
  // The color outlet
  t_outlet        *m_x, *m_y, *m_c; // xsize; ysize; csize
  t_outlet        *m_format;        // format
  t_outlet        *m_misc;          // type, upsidedown, notowned
  t_outlet        *m_pixblock;      // newimage, newfilm
  t_outlet        *m_data;          // data

  bool m_symbolic; // use symbols for format/colorspace (in message mode)
};

#endif  // for header file
