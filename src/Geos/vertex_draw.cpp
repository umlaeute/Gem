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
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_draw :: ~vertex_draw()
{ }

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

#ifdef __APPLE__
# error please replace __APPLE__ by something more specific; JMZ    
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
    
    vao = fences[0];
#ifdef __APPLE__
# error please replace __APPLE__ by something more specific; JMZ
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
     
   // if(m_texcoord){
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2,GL_FLOAT,0,state->TexCoordArray);
    //   }else{
    //   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //   }
    
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,state->NormalArray);    
        
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4,GL_FLOAT,0,(GLfloat *)state->VertexArray);
    #ifdef __APPLE__
    glVertexArrayParameteriAPPLE(GL_VERTEX_ARRAY_STORAGE_HINT_APPLE, GL_STORAGE_SHARED_APPLE);
    glVertexArrayRangeAPPLE( size, (GLvoid *)state->VertexArray);
#ifdef  GL_VERTEX_ARRAY_RANGE_APPLE
    glEnableClientState( GL_VERTEX_ARRAY_RANGE_APPLE );
    glFlushVertexArrayRangeAPPLE( size, (GLvoid *)state->VertexArray);
#endif
    #endif //__APPLE__
  
  
 
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
