/*
 *  pix_share_write.cpp
 *  GEM_darwin
 *
 *  Created by lincoln on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_share_write.h"
#include "Base/GemPixUtil.h"
#include <errno.h>

CPPEXTERN_NEW_WITH_GIMME(pix_share_write)

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_share_write :: pix_share_write(int argc, t_atom*argv)
{
  if(argc!=4)throw(GemException("4 arguments needed: <id> <width> <height> <color>"));
  if((A_FLOAT!=(argv+0)->a_type)||
     (A_FLOAT!=(argv+1)->a_type)||
     (A_FLOAT!=(argv+2)->a_type))
    throw(GemException("[pix_share_write]: <id> <width> & <height> must be integer values"));

  int fake = atom_getint(argv);
	
  int xsize = atom_getint(argv+1);
  int ysize = atom_getint(argv+2);
  int color = GL_RGBA;

  if (xsize <= 0 || ysize <= 0){
    throw(GemException("[pix_share_write]: <width> and <size> must be >0!"));
  }

  if(A_FLOAT==(argv+3)->a_type){
    int csize=atom_getint(argv+3);
    switch(csize){
    case 1:
      color = GL_LUMINANCE;
      break;
    case 2:
      color = GL_YUV422_GEM;
      break;
    case 4:
      color = GL_RGBA;
      break;
    default:
      throw(GemException("[pix_share_write]: <color> must be one of: 4,2,1,RGBA,YUV,Grey"));
      break;
    }
  } else { // the 4th argument is a symbol: either "RGBA", "YUV" or "Grey"
    char c=atom_getsymbol(argv+3)->s_name[0];
    switch(c){
    case 'G': case 'g':
      color = GL_LUMINANCE;
      break;
    case 'Y': case 'y':
      color = GL_YUV422_GEM;
      break;
    case 'R': case 'r':
      color = GL_RGBA;
      break;
    }
  }

  m_width = xsize;
  m_height = ysize;

  imageStruct dummy;
  dummy.setCsizeByFormat(color);

  m_color= dummy.csize;

  m_size = m_width * m_height * m_color;
	
  post("pix_share_write: %dx%dx%d: %d",
       m_width,m_height,m_color, m_size);

#ifdef __WIN32__
  error("pix_share_write: no shared memory on w32!");
#else
  shm_id = shmget(fake,m_size, IPC_CREAT | 0666);

  if(shm_id>0){
    shm_addr = (unsigned char*)shmat(shm_id,NULL,0666);
    if (!shm_addr) error("pix_share_write: no shmaddr");
    shmctl(shm_id,IPC_STAT,&shm_desc);
	
    post("pix_share_write: shm:: id(%d) segsz(%d) cpid (%d)",
         shm_id,shm_desc.shm_segsz,shm_desc.shm_cpid);
  } else {
    error("pix_share_read: couldn't get shm_id: error %d", errno);
  }
#endif
}

pix_share_write :: ~pix_share_write()
{
#ifndef __WIN32__
  if(shm_id>0){
    if (shmdt(shm_addr) == -1) post("shmdt failed");
	
    if (shmctl(shm_id,IPC_RMID, &shm_desc) == -1) post("shmctl failed");
  }
#endif
}

void pix_share_write :: render(GemState *state)
{
  if (!state || !state->image) return;

#ifndef __WIN32__
  if(shm_id>0){
    imageStruct *pix;
	
    if (!shm_addr){
      post("pix_share_write: no shmaddr");
      return;
    }

    pix = &state->image->image;

    if (pix->xsize == m_width && pix->ysize == m_height && pix->csize == m_color) {
      memcpy(shm_addr,pix->data,m_size);
    }
    else{
      post("pix_share_write: input image not the proper size: %dx%dx%d!=%dx%dx%d", 
           pix->xsize, pix->ysize, pix->csize,
           m_width, m_height, m_color
           );
    }
  }
#endif /* WIN32 */			
}

void pix_share_write :: obj_setupCallback(t_class *classPtr)
{

}

