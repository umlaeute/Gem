////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemPixUtil.h"

#include "m_pd.h"
#include <string.h>

GEM_EXTERN void copy2Image(imageStruct *to, imageStruct *from)
{
    if (!to || !from || !from->data)
    {
        error("GEM: Someone sent a bogus pointer to copy2Image");
        if (to)
			to->data = NULL;
        return;
    }

    // copy the information over
    to->xsize 	= from->xsize;
    to->ysize 	= from->ysize;
    to->csize 	= from->csize;
    to->format 	= from->format;
    to->type 	= from->type;

    to->data 	= new unsigned char[to->xsize * to->ysize * to->csize];
    // copy the data over
    memcpy(to->data, from->data, to->xsize * to->ysize * to->csize);
}

GEM_EXTERN void refreshImage(imageStruct *to, imageStruct *from)
{
    if (!to || !from || !from->data)
    {
        error("GEM: Someone sent a bogus pointer to refreshImage");
        return;
    }

    // check if we need to reallocate memory
    if (to->xsize != from->xsize ||
        to->ysize != from->ysize ||
        to->csize != from->csize ||
		!to->data)
    {
        delete [] to->data;
        copy2Image(to, from);
    }
    else
        // copy the data over
        memcpy(to->data, from->data, to->xsize * to->ysize * to->csize);
}
