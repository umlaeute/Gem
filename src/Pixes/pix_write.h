/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Snap a pix of the render buffer and write it to a file

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_WRITE_H_
#define INCLUDE_PIX_WRITE_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "Base/GemPixImageSave.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_write
    
    Writes a pix of the render buffer
    
KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vert_size"
    Inlet for a list - "vert_pos"

    "file" - filename to write to
    "bang" - do write now
    "auto 0/1" - stop/start writing automatically

    "vert_size" - Set the size of the pix
    "vert_pos" - Set the position of the pix
    
-----------------------------------------------------------------*/
class GEM_EXTERN pix_write : public GemBase
{
    CPPEXTERN_HEADER(pix_write, GemBase)

    public:

        //////////
        // Constructor
    	pix_write(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_write();

    	//////////
    	// Write to the current filename
    	virtual void	doWrite();
    	
    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state) {};

    	//////////
    	// Set the filename and filetype
    	virtual void	fileMess(int argc, t_atom *argv);
 
    	//////////
    	// When a size message is received
    	virtual void	sizeMess(int width, int height);
    	
    	//////////
    	// When a position message is received
    	virtual void	posMess(int x, int y);
    	
    	//////////
    	// Clean up the image
    	void	    	cleanImage();
    	
    	//////////
    	// The original pix_write
    	imageStruct 	*m_originalImage;


	//////////
	// Manual writing
	bool            m_banged;
    	
	//////////
	// Automatic writing
	bool            m_automatic;

	//////////
	// Counter for automatic writing
	int             m_autocount;
    	
    	//////////
	// path to write to
    	char	    	m_pathname[80];
    	//////////
	// current file to write to
    	char	    	m_filename[80];

    	//////////
	// current file to write to
    	int	    	m_filetype; // 0=tiff, [1..6=jpeg]

    	//////////
    	// The x position
    	int     	m_xoff;
    	
    	//////////
    	// The y position
    	int     	m_yoff;
    	
    	//////////
    	// The width
    	int     	m_width;
    	
    	//////////
    	// The height
    	int     	m_height;
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv);
    	static void 	autoMessCallback(void *data, t_floatarg on);
    	static void 	bangMessCallback(void *data);
    	static void 	sizeMessCallback(void *data, t_floatarg width, t_floatarg height );
    	static void 	posMessCallback(void *data, t_floatarg x, t_floatarg y);
};

#endif	// for header file
