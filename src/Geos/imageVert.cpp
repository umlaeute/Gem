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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "imageVert.h"

#ifdef __APPLE__
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW(imageVert)

  /////////////////////////////////////////////////////////
//
// imageVert
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
imageVert :: imageVert()
  : m_rebuildList(1)
{
  m_dispList = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
imageVert :: ~imageVert()
{
  // Delete our display list
  if (m_dispList) glDeleteLists(m_dispList, 1);
}

/////////////////////////////////////////////////////////
// processRGBAPix
//
/////////////////////////////////////////////////////////
void imageVert :: processRGBAPix(imageStruct &image, int texture)
{
  float red, green, blue, alpha;
  float red2, green2, blue2, alpha2;
    
  const int ySize = image.ysize;
  const int xSize = image.xsize;
  const int yStride = xSize * image.csize;
  const int xStride = image.csize;

  const float yDiff = 1.f / ySize;
  float yDown = -.5f;
  float yCurrent = yDown + yDiff;
  float yTexDown = 0.f;
  float yTex = yTexDown + yDiff;

  const float xDiff = 1.f / xSize;

  glShadeModel(GL_SMOOTH);

  glNormal3f(0.0f, 0.0f, 1.0f);

  unsigned char *data = image.data + yStride;
  if (texture)   {
    int yCount = ySize;
    while(yCount--)  {
      float xCurrent = -.5f;
      float xTex = 0.f;
      int xCount = xSize;
      glBegin(GL_QUAD_STRIP);
      while(xCount--)   {
	unsigned char *oneDown = data - yStride;
	red   = data[chRed] / 255.f;
	green = data[chGreen] / 255.f;
	blue  = data[chBlue] / 255.f;

	red2   = oneDown[chRed] / 255.f;
	green2 = oneDown[chGreen] / 255.f;
	blue2  = oneDown[chBlue] / 255.f;
    		        
	glTexCoord2f(xTex, yTexDown);
	glVertex3f(xCurrent, yDown, red2 + green2 + blue2);
	glTexCoord2f(xTex, yTex);
	glVertex3f(xCurrent, yCurrent, red + green + blue);

	xCurrent += xDiff;
	xTex += xDiff;
	data += xStride;
      }
      glEnd();
      yDown = yCurrent;
      yCurrent += yDiff;
      yTexDown = yTex;
      yTex += yDiff;
    }
  } else {
    int yCount = ySize;
    while(yCount--) {
      int xCount = xSize;
      float xCurrent = -.5f;
    	    
      glBegin(GL_QUAD_STRIP);
      while(xCount--) {
	unsigned char *oneDown = data - yStride;
	red   = data[chRed] / 255.f;
	green = data[chGreen] / 255.f;
	blue  = data[chBlue] / 255.f;
	alpha = data[chAlpha] / 255.f;

	red2   = oneDown[chRed] / 255.f;
	green2 = oneDown[chGreen] / 255.f;
	blue2  = oneDown[chBlue] / 255.f;
	alpha2 = oneDown[chAlpha] / 255.f;
    		        
	glColor4f(red2, green2, blue2, alpha2);
	glVertex3f(xCurrent, yDown, red2 + green2 + blue2);
	glColor4f(red, green, blue, alpha);
	glVertex3f(xCurrent, yCurrent, red + green + blue);

	xCurrent += xDiff;
	data += xStride;
      }
      glEnd();
      yDown = yCurrent;
      yCurrent += yDiff;
    }
  }
}

/////////////////////////////////////////////////////////
// processGrayPix
//
/////////////////////////////////////////////////////////
void imageVert :: processGrayPix(imageStruct &image, int texture)
{
  float gray, gray2;
    
  const int ySize = image.ysize;
  const int xSize = image.xsize;
  const int yStride = xSize * image.csize;
  const int xStride = image.csize;

  const float yDiff = 1.f / ySize;
  float yDown = -.5f;
  float yCurrent = yDown + yDiff;
  float yTexDown = 0.f;
  float yTex = yTexDown + yDiff;

  const float xDiff = 1.f / xSize;

  glShadeModel(GL_SMOOTH);

  glNormal3f(0.0f, 0.0f, 1.0f);

  unsigned char *data = image.data + yStride;
  if (texture) {
    int yCount = ySize;
    while(yCount--) {
      float xCurrent = -.5f;
      float xTex = 0.f;
      int xCount = xSize;
      glBegin(GL_QUAD_STRIP);
                
      while(xCount--) {
	unsigned char *oneDown = data - yStride;
	gray   = data[chGray] / 255.f;
	gray2  = oneDown[chGray] / 255.f;
		    
	glTexCoord2f(xTex, yTexDown);
	glVertex3f(xCurrent, yDown, gray2 + gray2 + gray2);
	glTexCoord2f(xTex, yTex);
	glVertex3f(xCurrent, yCurrent, gray + gray + gray);

	xCurrent += xDiff;
	xTex += xDiff;
	data += xStride;
      }
      glEnd();
      yDown = yCurrent;
      yCurrent += yDiff;
      yTexDown = yTex;
      yTex += yDiff;
    }
  } else {
    int yCount = ySize;
    while(yCount--)  {
      int xCount = xSize;
      float xCurrent = -.5f;
	
      glBegin(GL_QUAD_STRIP);
      while(xCount--)  {
	unsigned char *oneDown = data - yStride;
	gray   = data[chGray] / 255.f;
	gray2  = oneDown[chGray] / 255.f;
    		        
	glColor4f(gray2, gray2, gray2, 1.0f);
	glVertex3f(xCurrent, yDown, gray2 + gray2 + gray2);
	glColor4f(gray, gray, gray, 1.0f);
	glVertex3f(xCurrent, yCurrent, gray + gray + gray);

	xCurrent += xDiff;
	data += xStride;
      }
      glEnd();
      yDown = yCurrent;
      yCurrent += yDiff;
    }
  }
}

/////////////////////////////////////////////////////////
// processYUVPix
//
/////////////////////////////////////////////////////////
void imageVert :: processYUVPix(imageStruct &image, int texture)
{
  post("GEM:imageVert: YUV not yet implemented :-(");
/*  float Y, Y2, U, U2, V, V2;
    
  const int ySize = image.ysize;
  const int xSize = image.xsize;
  const int yStride = xSize * image.csize;
  const int xStride = image.csize;

  const float yDiff = 1.f / ySize;
  float yDown = -.5f;
  float yCurrent = yDown + yDiff;
  float yTexDown = 0.f;
  float yTex = yTexDown + yDiff;

  const float xDiff = 1.f / xSize;

  glShadeModel(GL_SMOOTH);

  glNormal3f(0.0f, 0.0f, 1.0f);

  unsigned char *data = image.data + yStride;
  if (texture)   {
    int yCount = ySize;
    while(yCount--)  {
      float xCurrent = -.5f;
      float xTex = 0.f;
      int xCount = xSize;
      glBegin(GL_QUAD_STRIP);
      while(xCount--)   {
	unsigned char *oneDown = data - yStride;
	Y   = data[chY] / 255.f;
	U = data[chU] / 255.f;
	V  = data[chV] / 255.f;

	Y2   = oneDown[chY] / 255.f;
	U2 = oneDown[chU] / 255.f;
	V2  = oneDown[chV] / 255.f;
    		        
	glTexCoord2f(xTex, yTexDown);
	glVertex3f(xCurrent, yDown, Y2 + U2 + V2);
	glTexCoord2f(xTex, yTex);
	glVertex3f(xCurrent, yCurrent, Y + U + V);

	xCurrent += xDiff;
	xTex += xDiff;
	data += xStride;
      }
      glEnd();
      yDown = yCurrent;
      yCurrent += yDiff;
      yTexDown = yTex;
      yTex += yDiff;
    }
  } else {
    int yCount = ySize;
    while(yCount--) {
      int xCount = xSize;
      float xCurrent = -.5f;
    	    
      glBegin(GL_QUAD_STRIP);
      while(xCount--) {
	unsigned char *oneDown = data - yStride;
	Y   = data[chY] / 255.f;
	U = data[chU] / 255.f;
        V = data[chV] / 255.f;

	Y2   = oneDown[chY] / 255.f;
	U2 = oneDown[chU] / 255.f;
	V2  = oneDown[chV] / 255.f;
    		        
	glColor3f(Y2, U2, V2);
	glVertex3f(xCurrent, yDown, Y2 + U2 + V2);
	glColor3f(Y, U, V);
	glVertex3f(xCurrent, yCurrent, Y + U + V);

	xCurrent += xDiff;
	data += xStride;
      }
      glEnd();
      yDown = yCurrent;
      yCurrent += yDiff;
    }
  }
*/
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void imageVert :: render(GemState *state)
{
  // always want to render
  if (!state->image) return;

  if (state->image->newimage) m_rebuildList = 1;

  if (!m_dispList){
    m_dispList=glGenLists(1);
    m_rebuildList=1;
  }

  // can we build a display list?
  if (!state->inDisplayList && m_rebuildList)
    {
      glNewList(m_dispList, GL_COMPILE_AND_EXECUTE);
      if (state->image->image.format == GL_RGBA || state->image->image.format == GL_BGRA_EXT)	//tigital
	processRGBAPix(state->image->image, state->texture);
      else
	processGrayPix(state->image->image, state->texture);
      glEndList();
      m_rebuildList = 0;
    }
  // nope, but our current one isn't valid
  else if (m_rebuildList) {
    if (state->image->image.format == GL_RGBA || state->image->image.format == GL_BGRA_EXT)	//tigital
      processRGBAPix(state->image->image, state->texture);
    else
      processGrayPix(state->image->image, state->texture);
  }
  // the display list has already been built
  else glCallList(m_dispList);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void imageVert :: obj_setupCallback(t_class *)
{ }
