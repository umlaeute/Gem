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


#ifdef __APPLE__
# define __VBO
#endif

#ifdef __VBO
# include "glVBO_ext.h"
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(vertex_draw, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// vertex_draw
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_draw :: vertex_draw(t_floatarg size)
{
   // m_drawType = GL_QUADS;
   m_drawType = GL_TRIANGLES;
   m_defaultDraw = 1;
    m_vao=1;
    m_color=1;
    m_index = 0;
    vertices = new float [4];
    m_oldsize = 0;
    m_init = 1;
    m_texcoord = 0;
#ifdef __VBO
	m_nVBOVertices = 0;
	m_nVBOColor = 0;
	m_nVBOTexCoords = 0;
	m_nVBONormals = 0;
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
   
    int size,stride;
   // int	index,vindex;
    GLuint vao;
    GLuint fences[2];
    
    if (state->VertexArray == NULL || state->VertexArraySize <= 0){
      //  post("vertex_draw: no vertex array!");
        return;
    }
    if (state->ColorArray == NULL || state->HaveColorArray == 0){
     //   post("vertex_draw: no Color array!");
       // m_color = 0;
    }
    if (state->TexCoordArray == NULL || state->HaveTexCoordArray == 0){
        post("vertex_draw: no Texture Coordinate array!");
        m_texcoord = 0;
    }
    
    if (state->drawType  && m_defaultDraw){
        m_drawType = state->drawType;
    }
    
    size = state->VertexArraySize;
    stride = state->VertexArrayStride;
    glShadeModel( GL_SMOOTH );
   
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
#ifndef __VBO
///////////////////////////////////////////  
// vertex_array_object   
///////////////////////////////////////////
#ifdef __APPLE__
//# error please replace __APPLE__ by something more specific; JMZ    
    glGenVertexArraysAPPLE(2, fences);
#endif
    
    vao = fences[0];
   //works but is slow
 //  post("vertex draw: VAO %d",vao);
   /*
   if (glIsVertexArrayAPPLE(vao)) 
        glBindVertexArrayAPPLE(vao);
        post("vertex draw: using VAO");
        }
    else{
        post("vertex draw: not using VAO");
    }*/
    
  //  glGenVertexArraysAPPLE(2, fences);
    
    //vao = fences[0];
#ifdef __APPLE__
//# error please replace __APPLE__ by something more specific; JMZ
    if (m_vao){
      if (!glIsVertexArrayAPPLE(1)) post("vertex draw: not using VAO");
      glBindVertexArrayAPPLE(1);
     }
#endif
    glDisableClientState(GL_INDEX_ARRAY);
    
    if(m_color && (state->ColorArray != NULL || state->HaveColorArray == 0) ){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4,GL_FLOAT,0,state->ColorArray);
        }else{
        glDisableClientState(GL_COLOR_ARRAY);
        }
     
   //if(m_texcoord){
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2,GL_FLOAT,0,state->TexCoordArray);
    //   }else{
    //   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //   }

    if(state->HaveNormalArray || state->NormalArray!=NULL){
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT,0,state->NormalArray);    
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4,GL_FLOAT,0,(GLfloat *)state->VertexArray);

#ifdef  GL_VERTEX_ARRAY_RANGE_APPLE
    glVertexArrayParameteriAPPLE(GL_VERTEX_ARRAY_STORAGE_HINT_APPLE, GL_STORAGE_SHARED_APPLE);
    glVertexArrayRangeAPPLE( size, (GLvoid *)state->VertexArray);
    glEnableClientState( GL_VERTEX_ARRAY_RANGE_APPLE );
    glFlushVertexArrayRangeAPPLE( size, (GLvoid *)state->VertexArray);
#endif
  
    glDrawArrays(m_drawType,0,size);
    glDisableClientState(GL_VERTEX_ARRAY);

#ifdef  GL_VERTEX_ARRAY_RANGE_APPLE   
    glDisableClientState(GL_VERTEX_ARRAY_RANGE_APPLE);
        glVertexArrayRangeAPPLE(0,0);
#endif
    if(m_color)glDisableClientState(GL_COLOR_ARRAY);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
   // if(m_texcoord)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_BLEND);
  //  glDeleteVertexArraysAPPLE(2, fences);
  
///////////////////////////////////////////  
#else	// vertex_buffer_object   
///////////////////////////////////////////

	//GLuint	m_nVBOVertices, m_nVBOColor, m_nVBOTexCoords, m_nVBONormals;
	
    if (m_vao){
	  glEnableClientState( GL_VERTEX_ARRAY );
	  if ( !m_nVBOVertices ){
		glGenBuffersARB( 1, &m_nVBOVertices );
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, state->VertexArraySize*4*sizeof(float),
						state->VertexArray, GL_STATIC_DRAW_ARB );
	  glVertexPointer( 4, GL_FLOAT,0, (char*) NULL);
	  }else{
	  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
	  glVertexPointer( 4, GL_FLOAT,0, (char*) NULL);
	  }
     }
    
    if(m_color && (state->ColorArray != NULL || state->HaveColorArray == 0) ){
        glEnableClientState(GL_COLOR_ARRAY);
		if ( !m_nVBOColor ){
			glGenBuffersARB( 1, &m_nVBOColor );
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOColor);
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, state->VertexArraySize*4*sizeof(float),
						state->ColorArray, GL_STATIC_DRAW_ARB );
        glColorPointer(4,GL_FLOAT,0,(char*) NULL);
        }else{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOColor);
			glColorPointer(4,GL_FLOAT,0,(char*) NULL);
		}
	}else{
        glDisableClientState(GL_COLOR_ARRAY);
        }
	if ( state->HaveTexCoordArray || state->TexCoordArray != NULL ){
		if ( !m_nVBOTexCoords ){
			glGenBuffersARB( 1, &m_nVBOTexCoords );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords);
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, state->VertexArraySize*2*sizeof(float),
						state->TexCoordArray, GL_DYNAMIC_DRAW_ARB );
		glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
		}else{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords);
			glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
		}
	}

    if(state->HaveNormalArray || state->NormalArray!=NULL){
		glEnableClientState(GL_NORMAL_ARRAY);
		if ( !m_nVBONormals ){
			glGenBuffersARB( 1, &m_nVBONormals );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, state->VertexArraySize*sizeof(float),
						state->NormalArray, GL_DYNAMIC_DRAW_ARB );
		glNormalPointer(GL_FLOAT,0, (char *) NULL);    
		}else{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
			glNormalPointer(GL_FLOAT,0, (char *) NULL);
		}
    }

    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer( 4, GL_FLOAT,0, (char*) NULL);
 
    glDrawArrays(m_drawType,0,size);
    glDisableClientState(GL_VERTEX_ARRAY);
    if(m_color)glDisableClientState(GL_COLOR_ARRAY);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
   // if(m_texcoord)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_BLEND);
#endif
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
{     class_addmethod(classPtr, (t_method)&vertex_draw::blendMessCallback,
    	    gensym("default"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_draw::colorMessCallback,
    	    gensym("color"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_draw::texcoordMessCallback,
    	    gensym("texcoord"), A_FLOAT, A_NULL);        
    class_addmethod(classPtr, (t_method)&vertex_draw::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);  
}

void vertex_draw :: blendMessCallback(void *data, t_floatarg size)
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
