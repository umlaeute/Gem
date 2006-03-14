/*
 *  pix_share_read.cpp
 *  GEM_darwin
 *
 *  Created by lincoln on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_share_read.h"
#include <errno.h>

CPPEXTERN_NEW_WITH_GIMME(pix_share_read)


//pix_share_read :: pix_share_read(t_float f1, t_float width, t_float height, t_float color)
  pix_share_read :: pix_share_read(int argc, t_atom*argv)
{
  if(argc!=4)throw(GemException("[pix_share_read]: 4 arguments needed: <id> <width> <height> <color>"));
  if((A_FLOAT!=(argv+0)->a_type)||
     (A_FLOAT!=(argv+1)->a_type)||
     (A_FLOAT!=(argv+2)->a_type))
    throw(GemException("[pix_share_read]: <id> <width> & <height> must be integer values"));

  int fake = atom_getint(argv);
	
  int xsize = atom_getint(argv+1);
  int ysize = atom_getint(argv+2);
  int color = GL_RGBA;

  if (xsize <= 0 || ysize <= 0){
    throw(GemException("[pix_share_read]: <width> and <size> must be >0!"));
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
      throw(GemException("[pix_share_read]: <color> must be one of: 4,2,1,RGBA,YUV,Grey"));
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

  pix.image.xsize = xsize;
  pix.image.ysize = ysize;
  pix.image.setCsizeByFormat(color);
  pix.image.reallocate();

  m_size = pix.image.xsize * pix.image.ysize * pix.image.csize;
	
  post("pix_share_read: %dx%dx%d: %d",
       xsize,ysize,
       pix.image.csize,m_size);

#ifdef __WIN32__
  error("pix_share_read: no shared memory on w32!");
#else
  shm_id = shmget(fake,m_size, IPC_CREAT | 0666);

  if(shm_id>0){
    shm_addr = (unsigned char*)shmat(shm_id,NULL,0666);
	
    if (!shm_addr) error("pix_share_read: no shmaddr");
	
    shmctl(shm_id,IPC_STAT,&shm_desc);
	
    post("pix_share_read: shm:: id(%d) segsz(%d) cpid(%d)",
         shm_id, shm_desc.shm_segsz,shm_desc.shm_cpid);
  } else {
    error("pix_share_read: couldn't get shm_id: error %d", errno);
  }
#endif
}

pix_share_read :: ~pix_share_read()
{

}

void pix_share_read :: render(GemState *state)
{	
#ifndef __WIN32__
  if(shm_id>0){
    if (shm_addr) { 
      memcpy(pix.image.data,shm_addr,m_size);
      state->image = &pix;
      state->image->newimage = 1;
    }
    else{
      post("pix_share_read: no shmaddr");
    }
  }
#endif
}

void pix_share_read :: obj_setupCallback(t_class *classPtr)
{

}

