/*
 *  pix_share_read.h
 *  GEM_darwin
 *
 *  Created by lincoln on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef INCLUDE_PIX_SHARE_READ_H
#define INCLUDE_PIX_SHARE_READ_H

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include <sys/types.h>
#ifndef __WIN32__
# include <sys/ipc.h>
# include <sys/shm.h>
#endif

class GEM_EXTERN pix_share_read : public GemBase
{
CPPEXTERN_HEADER(pix_share_read, GemBase)

	public:
		pix_share_read(t_float f1,t_float width ,t_float height, t_float color);
		
	protected:
		~pix_share_read();
		
		virtual void render(GemState *state);
		
		int	shm_id;
#ifndef __WIN32__
		unsigned char *shm_addr;
		struct shmid_ds shm_desc;
#endif
		int	m_height, m_width, m_color, m_size;
		
		pixBlock	pix;

};

#endif