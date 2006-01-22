/*
 *  pix_share_write.h
 *  GEM_darwin
 *
 *  Created by cgc on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_PIX_SHARE_WRITE_H
#define INCLUDE_PIX_SHARE_WRITE_H

#include "Base/GemBase.h"
#include <sys/types.h>
#ifndef __WIN32__
# include <sys/ipc.h>
# include <sys/shm.h>
#endif

class GEM_EXTERN pix_share_write : public GemBase
{
CPPEXTERN_HEADER(pix_share_write, GemBase)

	public:
		pix_share_write(t_float name, t_float width, t_float height, t_float color);
		
	protected:
		~pix_share_write();
		
		virtual void render(GemState *state);
#ifndef __WIN32__
		int	shm_id;
		unsigned char *shm_addr;
		struct shmid_ds shm_desc;
#endif
		int	m_height, m_width, m_color, m_size;
		

};

#endif

