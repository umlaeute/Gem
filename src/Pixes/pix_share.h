/*
 *  pix_share_write.h
 *  GEM_darwin
 *
 *  Created by cgc on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_SHARE_H_
#define _INCLUDE__GEM_PIXES_PIX_SHARE_H_

#include <Gem/GemConfig.h>
#if (defined HAVE_SYS_IPC_H) && (defined HAVE_SYS_SHM_H)
# define USE_SHM 1
#endif

#include "Base/GemBase.h"
#include <sys/types.h>
#if USE_SHM
# include <sys/ipc.h>
# include <sys/shm.h>
#elif defined _WIN32
# include <windows.h>
# include <stdio.h>
# include <conio.h>
# include <tchar.h>
#endif

// this is the header of the shared-memory segment
typedef struct _pixshare_header {
  size_t     size;    // total size of the shared-memory segment (without header)
  int      xsize;     // width of the image in the shm-segment
  int      ysize;     // height of the image in the shm-segment
  unsigned int format;    // format of the image (calculate csize,... from that)
  bool upsidedown;    // is the stored image swapped?
} t_pixshare_header;

#endif
