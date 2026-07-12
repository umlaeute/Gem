/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_WRITE_H_
#define _INCLUDE__GEM_PIXES_PIX_WRITE_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_write

    Snap a pix of the render buffer and write it to a file

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vert_size"
    Inlet for a list - "vert_pos"

    "file" - filename to write to
    "bang" - do write now
    "auto 0/1" - stop/start writing automatically

    "vert_size" - Set the size of the pix
    "vert_pos" - Set the position of the pix

-----------------------------------------------------------------*/
class GEM_EXTERN pix_write : public GemBase
{
  CPPEXTERN_HEADER(pix_write, GemBase);

public:

  //////////
  // Constructor
  pix_write(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~pix_write(void);

  //////////
  // Write to the current filename
  virtual void    doWrite(void);

  // check extensions
  virtual bool isRunnable(void);

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void    postrender(GemState *state) {};

  //////////
  // Set the filename and filetype
  virtual void    fileMess(t_symbol*, int argc, t_atom *argv);

  //////////
  // When a size message is received
  virtual void    sizeMess(int width, int height);

  //////////
  // When a position message is received
  virtual void    posMess(int x, int y);

  void autoMess(bool);
  void bangMess(void);
  void colorFormatMess(int format);

  //////////
  // Clean up the image
  void            cleanImage(void);

  //////////
  // The image read from openGL
  imageStruct      m_originalImage;
  imageStruct      m_convertedImage;


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
  std::string m_pathname;
  //////////
  // current file to write to
  char m_filename[MAXPDSTRING];

  //////////
  // current file to write to
  int             m_filetype; // 0=tiff, [1..6=jpeg]

  //////////
  // The x position
  int             m_xoff;

  //////////
  // The y position
  int             m_yoff;

  //////////
  // The width
  int             m_width;

  //////////
  // The height
  int             m_height;

  /////////
  // The color (1 = R, 3 = RGB, 4 = RGBA)
  int m_color;

private:

  //////////
  // static member functions
  static void     fileMessCallback(void *data, t_symbol* s, int argc,
                                   t_atom *argv);
  static void     autoMessCallback(void *data, t_float on);
  static void     bangMessCallback(void *data);
  static void     sizeMessCallback(void *data, t_float width,
                                   t_float height );
  static void     posMessCallback(void *data, t_float x, t_float y);
};

#endif  // for header file
