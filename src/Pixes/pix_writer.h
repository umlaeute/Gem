/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2009, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_WRITER_H_
#define _INCLUDE__GEM_PIXES_PIX_WRITER_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_writer

    Write a pix to a file

KEYWORDS
    pix

DESCRIPTION

    "file" - filename to write to
    "bang" - do write now
    "auto 0/1" - stop/start writing automatically

-----------------------------------------------------------------*/
class GEM_EXTERN pix_writer : public GemPixObj
{
  CPPEXTERN_HEADER(pix_writer, GemBase);

public:

  //////////
  // Constructor
  pix_writer(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~pix_writer();

  //////////
  // Do the rendering
  virtual void    processImage(imageStruct&image);

  //////////
  // Set the filename and filetype
  virtual void    fileMess(int argc, t_atom *argv);

  //////////
  // Manual writing
  bool            m_banged;

  //////////
  // Automatic writing
  bool            m_automatic;

  //////////
  // Counter for automatic writing
  int             m_autocount;

  //////////
  // path to write to
  char            m_pathname[MAXPDSTRING];
  //////////
  // current file to write to
  char            m_filename[MAXPDSTRING+10];

  //////////
  // current file to write to
  int             m_filetype; // 0=tiff, [1..6=jpeg]

private:

  //////////
  // static member functions
  static void     fileMessCallback(void *data, t_symbol* s, int argc,
                                   t_atom *argv);
  static void     autoMessCallback(void *data, t_float on);
  static void     bangMessCallback(void *data);
};

#endif  // for header file
