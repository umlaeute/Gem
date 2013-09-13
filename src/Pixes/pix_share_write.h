/*
 *  pix_share_write.h
 *  GEM_darwin
 *
 *  Created by cgc on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_SHARE_WRITE_H_
#define _INCLUDE__GEM_PIXES_PIX_SHARE_WRITE_H_

#include "pix_share.h"

class GEM_EXTERN pix_share_write : public GemBase
{
  CPPEXTERN_HEADER(pix_share_write, GemBase);

    public:
  pix_share_write(int, t_atom*);

 protected:
  ~pix_share_write();

  void freeShm();
  int getShm(int,t_atom*);

  virtual void render(GemState *state);
  unsigned char *shm_addr;
#ifndef _WIN32
  int	shm_id;
  struct shmid_ds shm_desc;
#else
  HANDLE m_MapFile;
  char m_fileMappingName[MAXPDSTRING];
#endif
  size_t m_size;
  t_outlet *m_outlet;
  static void 	setMessCallback(void *data, t_symbol *s, int argc, t_atom *argv);

};

#endif
