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
#ifndef __WIN32__
  : shm_id(0), shm_addr(NULL)
#endif
{
  if(argc<1){
    throw(GemException("pix_share_*: no ID given"));
  }
  int err  = getShm(argc, argv);

  switch(err){
  case 0:
    break;
  case 1:
    throw(GemException("pix_share_*: no valid size given"));
    break;
  case 2:
    throw(GemException("pix_share_*: given size < 0"));
    break;
  case 3:
    throw(GemException("pix_share_*: no valid dimensions given"));
    break;
  case 4:
    throw(GemException("pix_share_*: <color> must be one of: 4,2,1,RGBA,YUV,Grey"));
    break;
  case 5:
    throw(GemException("pix_share_*: arguments: <id> <width> <height> <color>"));
    break;
  case 6:
    throw(GemException("pix_share_*: couldn't get shared memory"));
    break;
  case 7:
    throw(GemException("pix_share_*: no ID given"));
    break;
  case 8:
    throw(GemException("pix_share_*: invalid ID..."));
    break;
   default:
    throw(GemException("pix_share_*: unknown error"));
    break;
  }
}

pix_share_write :: ~pix_share_write()
{
  freeShm();
}


void pix_share_write :: freeShm()
{
#ifndef __WIN32__
  if(shm_addr){
    if (shmdt(shm_addr) == -1) post("shmdt failed");
  }
  shm_addr=NULL;

  if(shm_id>0){
    if (shmctl(shm_id,IPC_STAT, &shm_desc) != -1){
      if(shm_desc.shm_nattch<=0){
        if (shmctl(shm_id,IPC_RMID, &shm_desc) == -1) post("shmctl failed");
      }
    }
  }
  shm_id=0;
#endif
}

int pix_share_write :: getShm(int argc,t_atom*argv)
{
  int fake = 0;

  size_t size=0;
  int    xsize=1;
  int    ysize=1;
  GLenum color=GL_RGBA;

  if(argc<1)return 7;
#ifndef __WIN32__
  if(shm_id>0)freeShm();
#endif
  if(A_FLOAT==argv->a_type){
    fake=atom_getint(argv);
  } else if(A_SYMBOL==argv->a_type){
    char*s=atom_getsymbol(argv)->s_name;
    char c=0;
    while((c=*s++))
      {
        int x=0;
        if(c>96)x=c-96;
        else if(c>64)x=c-64;
        else if(c>48)x=c-48;
        if(x<0)x=0;
        if(x>26)x=26;
        
        fake*=26;
        fake+=x;
      }
  }
  if(fake<=0)return 8;

  argc--; argv++;

  switch(argc)
    {
    case 1: /* just the size */
      {
        if(A_FLOAT!=argv->a_type)return 1;
        size=atom_getint(argv);
        if(size<0)return 2;
      }
      break;
    case 2: /* x*y; assume GL_RGBA */
      {
        if((A_FLOAT!=(argv+0)->a_type)||(A_FLOAT!=(argv+1)->a_type))return 3;
        xsize=atom_getint(argv);
        ysize=atom_getint(argv+1);
      }
      break;
    case 3:
      {
        if((A_FLOAT!=(argv+0)->a_type)||(A_FLOAT!=(argv+1)->a_type))return 3;
        xsize=atom_getint(argv);
        ysize=atom_getint(argv+1);
        if(A_FLOAT==(argv+2)->a_type)
          {
            int csize=atom_getint(argv+2);
            switch(csize)
              {
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
                return 4;
                break;
              }
          } else { // the 4th argument is a symbol: either "RGBA", "YUV" or "Grey"
          char c=atom_getsymbol(argv+2)->s_name[0];
          switch(c)
            {
            case 'G': case 'g':
              color = GL_LUMINANCE;
              break;
            case 'Y': case 'y':
              color = GL_YUV422_GEM;
              break;
            case 'R': case 'r':
              color = GL_RGBA;
              break;
            default:
              return 4;
              break;
            }
        }
      }
      break;
    default:
      return 5;
      throw(GemException("pix_share_* arguments: <id> <width> <height> <color>"));
      break;
    }
  
  if (xsize <= 0 || ysize <= 0){
    return 3;
  }

  imageStruct dummy;
  dummy.setCsizeByFormat(color);

  m_size = (size)?(size):(xsize * ysize * dummy.csize);
	
  post("pix_share_*: %dx%dx%d: %d",
       xsize,ysize,dummy.csize, m_size);

#ifdef __WIN32__
  error("pix_share_*: no shared memory on w32!");
#else
  /* get a new segment with the size specified by the user
   * OR an old segment with the size specified in its header
   */
  errno=0;
  shm_id = shmget(fake,m_size+sizeof(t_pixshare_header), IPC_CREAT | 0666);

  if((shm_id<0) && (EINVAL==errno)){
    errno=0;
    // the segment already exists, but is smaller than we thought!
    int id = shmget(fake,sizeof(t_pixshare_header),0666);
    if(id>0){ /* yea, we got it! */
      t_pixshare_header*h=(t_pixshare_header*)shmat(id,NULL,0666);
      /* read the size of the blob from the shared segment */
      if(h->size){
        post("pix_share_*: someone was faster: only got %d bytes instead of %d",
             h->size, m_size);
        m_size=h->size;
        shm_id = shmget(fake,m_size+sizeof(t_pixshare_header), IPC_CREAT | 0666);
      }
    }
  }
  if(shm_id>0){
    shm_addr = (unsigned char*)shmat(shm_id,NULL,0666);

    if (!shm_addr) return 6;
    shmctl(shm_id,IPC_STAT,&shm_desc);
    /* write the size into the shm-segment */
    t_pixshare_header *h=(t_pixshare_header *)shm_addr;
    h->size = (shm_desc.shm_segsz-sizeof(t_pixshare_header));
    
    post("pix_share_*: shm:: id(%d) segsz(%d) cpid (%d) mem(0x%X)",
         shm_id,shm_desc.shm_segsz,shm_desc.shm_cpid, shm_addr);
  } else {
    error("pix_share_*: couldn't get shm_id: error %d", errno);
  }
#endif /* __WIN32__ */
  return 0;
}


void pix_share_write :: render(GemState *state)
{
  if (!state || !state->image) return;

#ifndef __WIN32__
  if(shm_id>0){
    imageStruct *pix = &state->image->image;
    int size=pix->xsize*pix->ysize*pix->csize;
	
    if (!shm_addr){
      post("pix_share_write: no shmaddr");
      return;
    }

    if (size<=m_size) {
      t_pixshare_header *h=(t_pixshare_header *)shm_addr;
      h->size =m_size;
      h->xsize=pix->xsize;
      h->ysize=pix->ysize;
      h->format=pix->format;
      h->upsidedown=pix->upsidedown;
      memcpy(shm_addr+sizeof(t_pixshare_header),pix->data,size);
    }
    else{
      post("pix_share_write: input image too large: %dx%dx%d=%d>%d", 
           pix->xsize, pix->ysize, pix->csize, 
           pix->xsize*pix->ysize*pix->csize, 
           m_size);
    }
  }
#endif /* WIN32 */			
}

void pix_share_write :: obj_setupCallback(t_class *classPtr)
{
   class_addmethod(classPtr, (t_method)&pix_share_write::setMessCallback,
		  gensym("set"), A_GIMME, A_NULL);
}

void pix_share_write :: setMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if(argc){
    int err  = 0;
    //GetMyClass(data)->freeShm();
    err = GetMyClass(data)->getShm(argc, argv);
    if(err)error("pix_share_*: couldn't get new shared memory block! %d", err);
  } else
    error("pix_share_*:no args given!");
}
