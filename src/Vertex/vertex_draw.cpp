////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_draw.h"

#include "Base/GemState.h"
#include "Base/GemCache.h"

#define __VBO

#ifdef __VBO
# include "glVBO_ext.h"
#endif

CPPEXTERN_NEW(vertex_draw)

/////////////////////////////////////////////////////////
//
// vertex_draw
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_draw :: vertex_draw():
  m_vao(1), m_color(1), m_texcoord(0),
  m_defaultDraw(1),
  m_nVBOVertices(0), m_nVBOColor(0), m_nVBOTexCoords(0), m_nVBONormals(0),
  m_drawType(GL_TRIANGLES)
{
#ifdef __VBO
  post("vertex_draw: using VBO");
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_draw :: ~vertex_draw()
{ 
#ifdef __VBO
	if (m_nVBOVertices)
		glDeleteBuffersARB( 1, &m_nVBOVertices );
	if (m_nVBOColor)
		glDeleteBuffersARB( 1, &m_nVBOColor );
	if (m_nVBOTexCoords)
		glDeleteBuffersARB( 1, &m_nVBOTexCoords );
	if (m_nVBONormals)
		glDeleteBuffersARB( 1, &m_nVBONormals );
#endif
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_draw :: render(GemState *state)
{
  bool rebuild=(state->VertexDirty);
  //if(rebuild)post("rebuild");

  if (state->VertexArray == NULL || state->VertexArraySize <= 0){
    //  post("vertex_draw: no vertex array!");
    return;
  }
  int size = state->VertexArraySize;

  int color=m_color;
  if (state->ColorArray == NULL || state->HaveColorArray == 0){
    //color = 0;
  }
  bool texcoord=m_texcoord;
  if (texcoord && (state->TexCoordArray == NULL || state->HaveTexCoordArray == 0)){
    post("vertex_draw: no Texture Coordinate array!");
    texcoord = 0;
  }
    
  GLint drawType=m_drawType;
  if (state->drawType && m_defaultDraw){
    drawType = state->drawType;
  }


  glShadeModel( GL_SMOOTH );
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  ////////////////////////////////////////  
  // vertex_array_object   
#ifndef __VBO

# ifdef __APPLE__
  if (m_vao){
    if (!glIsVertexArrayAPPLE(1)) post("vertex draw: not using VAO");
    glBindVertexArrayAPPLE(1);
  }
# endif

  glDisableClientState(GL_INDEX_ARRAY);
    
  if(color && (state->ColorArray != NULL || state->HaveColorArray == 0) ){
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4,GL_FLOAT,0,state->ColorArray);
  }else{
    glDisableClientState(GL_COLOR_ARRAY);
  }

  //if(texcoord){
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2,GL_FLOAT,16,state->TexCoordArray);
  //   }else{
  //   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  //   }
  
  if(state->HaveNormalArray || state->NormalArray!=NULL){
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,16,state->NormalArray);
  }
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4,GL_FLOAT,0,(GLfloat *)state->VertexArray);

# ifdef  GL_VERTEX_ARRAY_RANGE_APPLE
  glVertexArrayParameteriAPPLE(GL_VERTEX_ARRAY_STORAGE_HINT_APPLE, GL_STORAGE_SHARED_APPLE);
  glVertexArrayRangeAPPLE( size, (GLvoid *)state->VertexArray);
  glEnableClientState( GL_VERTEX_ARRAY_RANGE_APPLE );
  glFlushVertexArrayRangeAPPLE( size, (GLvoid *)state->VertexArray);
# endif
  
  glDrawArrays(drawType,0,size);
  glDisableClientState(GL_VERTEX_ARRAY);

# ifdef  GL_VERTEX_ARRAY_RANGE_APPLE   
  glDisableClientState(GL_VERTEX_ARRAY_RANGE_APPLE);
  glVertexArrayRangeAPPLE(0,0);
# endif

  if(color)glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  
  glDisable(GL_BLEND);

#else	/* YES, we want VBO ! */

  // set-up the VertexArray
  if (m_vao){
    glEnableClientState( GL_VERTEX_ARRAY );
    if (rebuild || !m_nVBOVertices ){
      if(!m_nVBOVertices) glGenBuffersARB( 1, &m_nVBOVertices );
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*4*sizeof(float),
		       state->VertexArray, GL_DYNAMIC_DRAW_ARB );
      glVertexPointer( 4, GL_FLOAT,0, (char*) NULL);
    }else{
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
      glVertexPointer( 4, GL_FLOAT,0, (char*) NULL);
    }
  }
    
  // setup the ColorArray
  if(color && (state->ColorArray != NULL || state->HaveColorArray == 0) ){
    glEnableClientState(GL_COLOR_ARRAY);
    if (rebuild || !m_nVBOColor ){
      if(!m_nVBOColor)glGenBuffersARB( 1, &m_nVBOColor );
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOColor);
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*4*sizeof(int),
		       state->ColorArray, GL_DYNAMIC_DRAW_ARB );
      glColorPointer(4,GL_FLOAT,0,(char*) NULL);
    }else{
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOColor);
      glColorPointer(4,GL_FLOAT,0,(char*) NULL);
    }
  }else{
    glDisableClientState(GL_COLOR_ARRAY);
  }

  // setup the TexCoordArray
  if ( state->HaveTexCoordArray || state->TexCoordArray != NULL ){
    if (rebuild || !m_nVBOTexCoords ){
      if(!m_nVBOTexCoords)glGenBuffersARB( 1, &m_nVBOTexCoords );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords);
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*4*sizeof(float),
		       state->TexCoordArray, GL_DYNAMIC_DRAW_ARB );
      glTexCoordPointer(4, GL_FLOAT, 16, (char *) NULL);
    }else{
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords);
      glTexCoordPointer(4, GL_FLOAT, 16, (char *) NULL);
    }
  }

  // setup the NormalArray
  if(state->HaveNormalArray || state->NormalArray!=NULL){
    glEnableClientState(GL_NORMAL_ARRAY);
    if (rebuild || !m_nVBONormals ){
      if(!m_nVBONormals)glGenBuffersARB( 1, &m_nVBONormals );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*4*sizeof(float),
		       state->NormalArray, GL_DYNAMIC_DRAW_ARB );
      glNormalPointer(GL_FLOAT,16, (char *) NULL);    
    }else{
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
      glNormalPointer(GL_FLOAT,16, (char *) NULL);
    }
  }


  // setup the TextureArray
  if(state->HaveTexCoordArray || state->TexCoordArray!=NULL){
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (rebuild || !m_nVBOTexCoords ){
      if(!m_nVBOTexCoords)glGenBuffersARB( 1, &m_nVBOTexCoords );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*4*sizeof(float),
		       state->TexCoordArray, GL_DYNAMIC_DRAW_ARB );
      glTexCoordPointer(2,GL_FLOAT,16, (char *) NULL);    
    }else{
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
      glTexCoordPointer(2,GL_FLOAT,16, (char *) NULL);
    }
  }


  
  //glEnableClientState(GL_VERTEX_ARRAY);
  //glVertexPointer( 4, GL_FLOAT,0, (char*) NULL);
  
  glDrawArrays(drawType,0,size);
  glDisableClientState(GL_VERTEX_ARRAY);
  if(color)glDisableClientState(GL_COLOR_ARRAY);
  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  // if(texcoord)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_BLEND);
# endif /* VBO */
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void vertex_draw :: typeMess(t_symbol *type)
{
  char c=*type->s_name;
  switch (c){
  case 'L': case 'l': // line
    m_drawType = GL_LINE_LOOP;
    break;
  case 'F': case 'f': // fill
    m_drawType = GL_POLYGON;
    break;
  case 'Q': case 'q': // fill
    m_drawType = GL_QUADS;
    break;
  case 'P': case 'p': // point
    m_drawType = GL_POINTS;
    break;
  case 'T': case 't': // triangles
    m_drawType = GL_TRIANGLES;
    break;
  case 'S': case 's': // tri-strip
    m_drawType = GL_TRIANGLE_STRIP;
    break;  
    
  default:
    error ("GEM: square draw style");
    return;
  }
  setModified();
}

 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_draw :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&vertex_draw::defaultMessCallback,
    	    gensym("default"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_draw::colorMessCallback,
    	    gensym("color"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_draw::texcoordMessCallback,
    	    gensym("texcoord"), A_FLOAT, A_NULL);        
    class_addmethod(classPtr, (t_method)&vertex_draw::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);  
}

void vertex_draw :: defaultMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->m_defaultDraw=((int)size);
}

void vertex_draw :: colorMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->m_color=((int)size);
}

void vertex_draw :: texcoordMessCallback(void *data, t_floatarg t)
{
    GetMyClass(data)->m_texcoord=((int)t);
}
void vertex_draw :: typeMessCallback(void *data, t_symbol *type)
{
    GetMyClass(data)->typeMess(type);
}
