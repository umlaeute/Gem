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

CPPEXTERN_NEW_WITH_FOUR_ARGS(pix_share_write,t_float, A_FLOAT,t_float, A_FLOAT,t_float, A_FLOAT,t_float, A_FLOAT)

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_share_write :: pix_share_write(t_float f,t_float width, t_float height, t_float color)
{
#ifndef __WIN32__
	int fake;
	//int size;
	
	m_height = (int)height;
	m_width = (int)width;
	m_color = (int)color;
	if (m_height <= 0 || m_width <= 0){
	
		m_height = 320;
		m_width = 240;
	
	}
	
	if (m_color != 2 && m_color != 4) m_color = 4;
	
	//size = 640 * 480 * 2;
	m_size = m_width * m_height * m_color;
	
	post("pix_share_write: height %d width %d color %d size %d",m_height,m_width,m_color,m_size);
	
	fake = (int)f;
	
	shm_id = shmget(fake,m_size, IPC_CREAT | 0666);
	post("pix_share_write: shm_id %d",shm_id);
	
	shm_addr = (unsigned char*)shmat(shm_id,NULL,0666);
	
	if (!shm_addr) post("pix_share_write: no shmaddr");
	
	shmctl(shm_id,IPC_STAT,&shm_desc);
	
	post("pix_share_write: shm_segsz %d shm_cpid %d",shm_desc.shm_segsz,shm_desc.shm_cpid);
#endif /* WIN32 */	
}

pix_share_write :: ~pix_share_write()
{
#ifndef __WIN32__
	if (shmdt(shm_addr) == -1) post("shmdt failed");
	
	if (shmctl(shm_id,IPC_RMID, &shm_desc) == -1) post("shmctl failed");
#endif
}

void pix_share_write :: render(GemState *state)
{
#ifndef __WIN32__
	imageStruct pix;
	//int size;
	//size = 640 * 480 * 2;
	
	if (!state || !state->image) return;

	if (!shm_addr){
		post("pix_share_write: no shmaddr");
		return;
	 }
	 	 
	 pix = state->image->image;
	 
	 if (pix.xsize == m_width && pix.ysize == m_height && pix.csize == m_color) {
		memcpy(shm_addr,pix.data,m_size);
	 
		}
		else{
			post("pix_share_write: input image not the proper size");
		}
#endif /* WIN32 */			
}

void pix_share_write :: obj_setupCallback(t_class *classPtr)
{

}
