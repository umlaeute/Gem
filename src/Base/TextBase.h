/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  A text

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_TEXTBASE_H_
#define INCLUDE_TEXTBASE_H_

#include "Base/GemBase.h"
#ifdef __linux__
#include "Base/config.h"
#endif /* __linux__ */

#ifdef _WINDOWS
# define GLTT
#elif defined MACOSX
# define FTGL
#elif defined __linux__
# ifdef HAVE_LIBFTGL
#  define FTGL
# elif defined HAVE_LIBGLTT
#  define GLTT
# endif
#endif

#ifdef FTGL
#define FONT_SCALE 0.2/3.0
#include "FTFont.h"
#elif defined GLTT
#define FONT_SCALE 0.05
#include "FTFace.h"
#else
#define FONT_SCALE 1.0
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  TextBase
    
  Base class for text objects

  DESCRIPTION

  "ft1" - The font size
  "text" - The text to draw
  
  -----------------------------------------------------------------*/
class GEM_EXTERN TextBase : public GemBase
{
  CPPEXTERN_HEADER(TextBase, GemBase)

    public:

  //////////
  // Constructor
  TextBase(int argc, t_atom *argv);
    	
 protected:
  
  //////////
  // Destructor
  virtual ~TextBase();

  //////////
  // Set the text string
  void	    	textMess(int argc, t_atom *argv);

  //////////
  // The font to use
  virtual void  fontNameMess(const char *filename);

  //////////
  // Set the font size
  virtual void	setFontSize(int size);

  //////////
  // Set the precision for rendering
  virtual void	setPrecision(float prec);

  //////////
  // The different types of justification
  enum JustifyWidth { LEFT, RIGHT, CENTER };
  enum JustifyHeight { BOTTOM, TOP, MIDDLE };
  enum JustifyDepth { FRONT, BACK, HALFWAY };
	
  //////////
  // Set the justification
  void	setJustification(JustifyWidth wType);
  void	setJustification(JustifyWidth wType, JustifyHeight hType);
  void	setJustification(JustifyWidth wType, JustifyHeight hType, JustifyDepth dType);

  //////////
  // do the justification
  // x1,...,z2 just defines the bounding box of the rendered string.
  void justifyFont(float x1, float y1, float z1, float x2, float y2, float z2);

#ifdef GLTT
  //////////
  // make the actual font (for GLTT)
  // return values: 1==success; 0==failure
  virtual int makeFontFromFace(void) = 0;
#endif

  //-----------------------------------
  // GROUP:	Member variables
  //-----------------------------------
    
  //////////
  // Do we have a valid font?
  int	m_valid;

  //////////
  // The string to display
  char	    	*m_theString;
    	
  //////////
  // The maximum memory currently allocated for the string
  // This includes the terminator /0 !!!
  int	m_theMaxStringSize;
    	
  //////////
  // The font fize
  int		m_fontSize;

  //////////
  // The font depth (only for extruded fonts)
  float		m_fontDepth;

    	
  //////////
  // The rendering precision
  float		m_precision;

  //////////
  // The width justification
  JustifyWidth	m_widthJus;

  //////////
  // The height justification
  JustifyHeight	m_heightJus;

  //////////
  // The depth justification
  JustifyDepth	m_depthJus;

  //////////
  // The inlet
  t_inlet         *m_inlet;

  //////////
  // The default font name
  static char *DEFAULT_FONT;

  //////////
  // The font structure
#ifdef FTGL
  FTFont		*m_font;
#endif
#if defined GLTT || defined FTGL
  FTFace 		*m_face;
#endif

  //-----------------------------------
  // GROUP:	Setup functions
  //-----------------------------------
    
 private:
    	    
  //////////
  // Static member functions
  static void 	textMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void 	fontSizeMessCallback(void *data, t_floatarg size);
  static void 	precisionMessCallback(void *data, t_floatarg prec);
  static void 	fontNameMessCallback(void *data, t_symbol *s);
  static void 	justifyMessCallback(void *data, t_symbol *, int, t_atom*);
};

#endif	// for header file
