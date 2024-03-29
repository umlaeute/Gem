/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Snap a pix of the render buffer and write it to a file

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_WRITER_H_
#define _INCLUDE__GEM_PIXES_PIX_WRITER_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_writer

    Writes a pix of the render buffer

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
