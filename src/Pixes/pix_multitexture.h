/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    load texture ID's to specific texture units

  Copyright (c) 2005 James Tittle II <tigita AT mac.com>
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_MULTITEXTURE_H_
#define _INCLUDE__GEM_PIXES_PIX_MULTITEXTURE_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"
#include "Gem/State.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_multitexture

    Assigns texID's to texUnits for later combining or shading

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/
#define MAX_MULTITEX_ID 32
class GEM_EXTERN pix_multitexture : public GemBase
{
  CPPEXTERN_HEADER(pix_multitexture, GemBase);
public:

  //////////
  // Constructor
  pix_multitexture(t_floatarg);

protected:

  //////////
  // Destructor
  virtual ~pix_multitexture();

  //////////
  // extension checks
  //
  virtual bool isRunnable(void);

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Turn back off texture mapping
  virtual void    postrender(GemState *state);

  t_inlet**m_inlet;
  unsigned int m_numInlets;

  int
  m_reqTexUnits; // requested # of texture Units, defaults to m_max
  GLint
  m_max; // maximum # of texture units supported by the specific card
  GLint                   m_texID[MAX_MULTITEX_ID];
  GLint                   m_textureType;
  int                     m_mode; // 1=TEXTURE_RECTANGLE_EXT, 0=TEXTURE_2D

  //////////
  // The texture coordinates
  TexCoord        m_coords[4];
  float                   m_xRatio;
  float                   m_yRatio;
  GLboolean               upsidedown;
  int                             m_texSizeX;
  int                             m_texSizeY;

  //////////
  // this is what we get from upstream
  TexCoord       *m_oldTexCoords;
  int             m_oldNumCoords;
  int             m_oldTexture;


  void texUnitMess(int n, int texID);
  void dimenMess(int, int);
  void rectangleMess(bool mode);
private:

  //////////
  // static member functions
  static void             parmCallback(void*,t_symbol*,int, t_atom*);
};

#endif  // for header file
