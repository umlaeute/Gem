/*
 *  pix_share_read.cpp
 *  GEM_darwin
 *
 *  Created by lincoln on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_share_read.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS(pix_share_read,t_float,A_FLOAT,t_float,A_FLOAT,t_float,A_FLOAT,t_float,A_FLOAT)

pix_share_read :: pix_share_read(t_float f1, t_float width, t_float height, t_float color)
{
	int fake;
//	int size;
	
	//size = 640 * 480 * 2;
	m_height = (int)height;
	m_width = (int)width;
	m_color = (int)color;
	if (m_height <= 0 || m_width <= 0){
	
		m_height = 320;
		m_width = 240;
	
	}
	
	if (m_color != 2 && m_color != 4) m_color = 4;
	
	m_size = m_width * m_height * m_color;
	
	post("pix_share_read: height %d width %d color %d size %d",m_height,m_width,m_color,m_size);
	
	fake = (int)f1;
	
	shm_id = shmget(fake,m_size, IPC_CREAT | 0666);
	post("pix_share: shm_id %d",shm_id);
	
	shm_addr = (unsigned char*)shmat(shm_id,NULL,0666);
	
	if (!shm_addr) post("pix_share: no shmaddr");
	
	shmctl(shm_id,IPC_STAT,&shm_desc);
	
	post("pix_share: shm_segsz %d shm_cpid %d",shm_desc.shm_segsz,shm_desc.shm_cpid);
	
	pix.image.xsize = m_width;
	pix.image.ysize = m_height;
	pix.image.csize = m_color;
	pix.image.format = GL_YCBCR_422_APPLE;
	pix.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
	//pix.allocate();
	pix.image.data = new unsigned char [m_size];
}

pix_share_read :: ~pix_share_read()
{

}

void pix_share_read :: render(GemState *state)
{	
	//int size;
	//size = 640 * 480 * 2;
	//unsigned char *pixels;
	
	//if (pixels) delete [] pixels;
	//pixels = new unsigned char [size];
	
	if (shm_addr) { 
		memcpy(pix.image.data,shm_addr,m_size);
		state->image = &pix;
		state->image->newimage = 1;
	}
	else{
		post("pix_share_read: no shmaddr");
	}
	//post("pix_share_read: shm_addr[0] %d shm_addr[1] %d",shm_addr[0],shm_addr[1]);
}

void pix_share_read :: obj_setupCallback(t_class *classPtr)
{

}