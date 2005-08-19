////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "text2d.h"

#ifdef FTGL
#include "FTGLPixmapFont.h"
#include "FTGLBitmapFont.h"
#endif

CPPEXTERN_NEW_WITH_GIMME(text2d)

/////////////////////////////////////////////////////////
//
// text2d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef FTGL
text2d :: text2d(int argc, t_atom *argv)
  : TextBase(argc, argv), m_antialias(1), m_afont(NULL) {
  fontNameMess(DEFAULT_FONT);
} 
text2d :: ~text2d() {
  if(m_font) delete m_font; m_font=NULL;
  if(m_afont)delete m_afont;m_afont=NULL;
}
FTFont *text2d :: makeFont(const char*fontfile){
  if(m_font) delete m_font;  m_font=NULL;
  if(m_afont)delete m_afont; m_afont=NULL;

  m_font =  new FTGLBitmapFont(fontfile);
  if (m_font->Error()){
    delete m_font;
    m_font = NULL;
  }
  m_afont =  new FTGLPixmapFont(fontfile);
  if (m_afont->Error()){
    delete m_afont;
    m_afont = NULL;
  }
  
  return m_font;
}
/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void text2d :: setFontSize(t_float size){
  m_fontSize = size;
  if (m_font)if (! m_font->FaceSize((int)m_fontSize) ) {
    error("GEMtext: unable set fontsize !");
  }
  if (m_afont)if (! m_afont->FaceSize((int)m_fontSize) ) {
    error("GEMtext: unable set fontsize !");
  }
  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text2d :: render(GemState *)
{
  if (!m_theString || !(m_afont || m_font))return;
  if (m_antialias && !m_afont)m_antialias=0;
  if (!m_antialias && !m_font)m_antialias=1;
  float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;
  // compute the offset due to the justification
  if(m_antialias && m_afont){
    m_afont->BBox( m_theString, x1, y1, z1, x2, y2, z2); // FTGL
    float width  = 0.f;
    float height = 0.f;
    float depth  = 0.f;

    if (m_widthJus == LEFT)       width = x1;
    else if (m_widthJus == RIGHT) width = x2-x1;
    else if (m_widthJus == CENTER)width = x2 / 2.f;

    if (m_heightJus == BOTTOM)     height = y1;
    else if (m_heightJus == TOP)   height = y2-y1;
    else if (m_heightJus == MIDDLE)height = y2 / 2.f;
    
    if (m_depthJus == FRONT)       depth = z1;
    else if (m_depthJus == BACK)   depth = z2-z1;
    else if (m_depthJus == HALFWAY)depth = z2 / 2.f;

    glPushMatrix();

    glRasterPos2i(0,0);
    glBitmap(0,0,0.0,0.0,-width,-height, NULL);
    justifyFont(x1, y1, z1, x2, y2, z2);
    m_afont->Render(m_theString);
    glPopMatrix();
  } else if (m_font) {
    m_font->BBox( m_theString, x1, y1, z1, x2, y2, z2); // FTGL
    float width  = 0.f;
    float height = 0.f;
    float depth  = 0.f;

    if (m_widthJus == LEFT)       width = x1;
    else if (m_widthJus == RIGHT) width = x2-x1;
    else if (m_widthJus == CENTER)width = x2 / 2.f;

    if (m_heightJus == BOTTOM)     height = y1;
    else if (m_heightJus == TOP)   height = y2-y1;
    else if (m_heightJus == MIDDLE)height = y2 / 2.f;
    
    if (m_depthJus == FRONT)       depth = z1;
    else if (m_depthJus == BACK)   depth = z2-z1;
    else if (m_depthJus == HALFWAY)depth = z2 / 2.f;

    glPushMatrix();

    glRasterPos2i(0,0);
    glBitmap(0,0,0.0,0.0,-width,-height, NULL);
    m_font->Render(m_theString);
    glPopMatrix();
  }
}

#elif defined GLTT
text2d :: text2d(int argc, t_atom *argv)
  : TextBase(argc, argv)
  , m_font(NULL)
#if defined __linux__ || defined __APPLE__
  , m_afont(NULL)
#endif
{
#ifdef __APPLE__
  if (!HaveValidContext ()) {post("GEM: geo: text2d - need window to load font");return;}
#endif
  fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text2d :: ~text2d()
{
  if(m_font)delete m_font;m_font=NULL;
#if defined __linux__ || defined __APPLE__
  if(m_afont)delete m_afont;m_afont=NULL;
#endif
  if(m_face)delete m_face;m_face=NULL;
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
void text2d :: destroyFont() { // fonts have to be destroyed before face
  if(m_font)delete m_font; m_font=NULL;
  if(m_afont)delete m_afont; m_afont=NULL;
}
int text2d :: makeFontFromFace()
{
  if(m_font)delete m_font;m_font=NULL;
#if defined __linux__ || defined __APPLE__
  if(m_afont)delete m_afont;m_afont=NULL;
#endif

  if (!m_face)    {
    error("GEM: text2d: True type font doesn't exist");
    return(0);
  }
  m_font = new GLTTBitmapFont(m_face);
  //m_font->setPrecision((double)m_precision);
  if( ! m_font->create((int)m_fontSize) ) {
    error("GEM: text2d: unable to create bitmap'ed font");
    delete m_font; m_font = NULL;
    //    return(0);
  }
#if defined __linux__ || defined __APPLE__
  m_afont = new GLTTPixmapFont(m_face);
  //  m_afont->setPrecision((double)m_precision);
  if( ! m_afont->create((int)m_fontSize) ) {
    error("GEM: text2d: unable to create pixmap'ed font");
    delete m_afont; m_afont = NULL;
    //    return(0);
  }
  if (!m_font && !m_afont)return 0;
#else
  if (!m_font)return 0;
#endif
  return(1);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text2d :: render(GemState *)
{
  if (m_valid && m_theString) {
    glPushMatrix();

    // compute the offset due to the justification
    float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;

#if defined __linux__ || defined __APPLE__
    if (m_antialias && !m_afont)m_antialias=0;
    if (!m_antialias && !m_font)m_antialias=1;
    // pixmap'ed fonts are not supported under GLTT/NT
    if (m_antialias){
      if(!m_afont)return;
      x2=m_afont->getWidth (m_theString);
      y2=m_afont->getHeight();

      float width  = 0.f;
      float height = 0.f;
      float depth  = 0.f;

      if (m_widthJus == LEFT)       width = x1;
      else if (m_widthJus == RIGHT) width = x2-x1;
      else if (m_widthJus == CENTER)width = x2 / 2.f;

      if (m_heightJus == BOTTOM)     height = y1;
      else if (m_heightJus == TOP)   height = y2-y1;
      else if (m_heightJus == MIDDLE)height = y2 / 2.f;
    
      if (m_depthJus == FRONT)       depth = z1;
      else if (m_depthJus == BACK)   depth = z2-z1;
      else if (m_depthJus == HALFWAY)depth = z2 / 2.f;

      m_afont->output((int)-width, (int)-height, m_theString);
    }else
#endif
      {
	if(!m_font)return;

	x2=m_font->getWidth (m_theString);
	y2=m_font->getHeight();

	float width  = 0.f;
	float height = 0.f;
	float depth  = 0.f;

	if (m_widthJus == LEFT)       width = x1;
	else if (m_widthJus == RIGHT) width = x2-x1;
	else if (m_widthJus == CENTER)width = x2 / 2.f;

	if (m_heightJus == BOTTOM)     height = y1;
	else if (m_heightJus == TOP)   height = y2-y1;
	else if (m_heightJus == MIDDLE)height = y2 / 2.f;
	
	if (m_depthJus == FRONT)       depth = z1;
	else if (m_depthJus == BACK)   depth = z2-z1;
	else if (m_depthJus == HALFWAY)depth = z2 / 2.f;

	m_font->output((int)-width, (int)-height, m_theString);
      }
    glPopMatrix();
  }
}


#else /* !FTGL && !GLTT */

text2d :: text2d(int argc, t_atom *argv)
  : TextBase(argc, argv)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text2d :: ~text2d()
{}

void text2d :: render(GemState*){}

#endif /* !GLTT && !FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text2d :: obj_setupCallback(t_class *classPtr )
{ 
  class_addmethod(classPtr, (t_method)&text2d::aliasMessCallback,
		  gensym("alias"), A_FLOAT, A_NULL);
}

void text2d :: aliasMess(int size)
{
  m_antialias = (int)size;
}
void text2d :: aliasMessCallback(void *data, t_floatarg tog)
{
  GetMyClass(data)->aliasMess((int)tog);
}
