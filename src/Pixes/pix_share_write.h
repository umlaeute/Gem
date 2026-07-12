/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, Chris Clepper and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
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
#if USE_SHM
  int   shm_id;
  struct shmid_ds shm_desc;
#elif defined _WIN32
  HANDLE m_MapFile;
  char m_fileMappingName[MAXPDSTRING];
#endif
  size_t m_size;
  t_outlet *m_outlet;
  static void   setMessCallback(void *data, t_symbol* s, int argc,
                                t_atom *argv);

};

#endif
