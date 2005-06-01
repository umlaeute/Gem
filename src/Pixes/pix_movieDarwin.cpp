////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com & cgc@humboldtblvd.com
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) 2002 James Tittle & Chris Clepper
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#ifdef __APPLE__
/* i think this is APPLE only...JMZ */

#include "pix_movieDarwin.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_movieDarwin, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_movie
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_movieDarwin :: pix_movieDarwin(t_symbol *filename) 

{
 // m_film=false;
 m_colorspace = GL_YCBCR_422_GEM;
  m_hiquality = 1;
  m_play = 0;
  m_rate = 1;
  prevTime = 0;
  curTime = 0; 
  m_rectangle = 1;
  
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_movieDarwin :: ~pix_movieDarwin()
{
  // Clean up the movie
  closeMess();
  deleteBuffer();
}

void pix_movieDarwin :: deleteBuffer()
{
  //post("deleting buffer %x", m_data);
  if (m_data){
    delete [] m_data;
    //post("deleted");
  }
  
  m_pixBlock.image.data=NULL;
  m_frame=m_data=NULL;
  m_pixBlock.image.xsize=m_pixBlock.image.ysize=m_pixBlock.image.csize=0;
}

void pix_movieDarwin :: createBuffer()
{
  const int neededXSize = m_xsize;
  const int neededYSize = m_ysize;
  int	oldx, oldy;
  
oldx = 0;
oldy = 0;

    if (neededXSize != oldx || neededYSize != oldy)
    {
deleteBuffer();

  m_pixBlock.image.xsize = neededXSize;
  m_pixBlock.image.ysize = neededYSize;
  m_pixBlock.image.csize = m_csize;
  m_pixBlock.image.format= m_format;
  
  int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize * m_pixBlock.image.csize;

  m_data = new unsigned char[dataSize];

  m_pixBlock.image.data = m_data;
  m_frame =  m_data;

  m_pixBlock.image.csize = m_csize;
  m_pixBlock.image.format= m_format;

    oldx = m_pixBlock.image.xsize;
    oldy = m_pixBlock.image.ysize;
}
  //post("created buffer @ %x", m_data);
}


void pix_movieDarwin :: closeMess(void)
{
    switch (m_haveMovie) {
        case GEM_MOVIE_MOV:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_AVI:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_MPG:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_NONE:
        default:
            break;
  }
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////

void pix_movieDarwin :: openMess(t_symbol *filename, int format)
{
  //  if (filename==x_filename)return;
  x_filename=filename;
  if (format)m_colorspace=format;

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  // Clean up any open files
  closeMess();

  m_haveMovie = GEM_MOVIE_NONE;
  realOpen(buf);
  if (m_haveMovie == GEM_MOVIE_NONE)return;

  t_atom ap[3];
  SETFLOAT(ap, m_numFrames);
  SETFLOAT(ap+1, m_xsize);
  SETFLOAT(ap+2, m_ysize);

    m_newFilm = 1;
  //outlet_float(m_outNumFrames, (float)m_numFrames);
  post("GEM: pix_film: Loaded file: %s with %d frames (%dx%d)", buf, m_numFrames, m_xsize, m_ysize);
  outlet_list(m_outNumFrames, 0, 3, ap);
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: realOpen(char *filename)
{
    FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
    
    long		m_rowBytes;

    if (!filename[0]) {
        post("pix_filmDarwin:  no filename passed");
    } else {            
        err = ::FSPathMakeRef((UInt8*)filename, &ref, NULL);
        err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
            
        if (err) {
            error("GEM: pix_movie: Unable to find file: %#s", theFSSpec.name);
            return;
        }
        m_haveMovie = GEM_MOVIE_MOV;
    }
    
    short	refnum = 0;
    err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
    if (err) {
        error("GEM: pix_movie: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
        if (refnum) ::CloseMovieFile(refnum);
        return;
    }

	::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
	if (refnum) ::CloseMovieFile(refnum);

	m_reqFrame = 0;
	m_curFrame = -1;
        m_numTracks = (int)GetMovieTrackCount(m_movie);

	// Get the length of the movie
        
        movieDur = (long)GetMovieDuration(m_movie);
        movieScale = (long)GetMovieTimeScale(m_movie);
        
                                            
	OSType		whichMediaType = VisualMediaCharacteristic;
	short		flags = nextTimeMediaSample + nextTimeEdgeOK;
        
        TimeValue	theTime = 0;
	
	m_numFrames = -1;
	while (theTime >= 0) {
		m_numFrames++;
		::GetMovieNextInterestingTime(m_movie,
                                            flags,
                                            1,
                                            &whichMediaType,
                                            theTime,
                                            0,
                                            &theTime,
                                            &duration);
		// after the first interesting time, don't include the time we
		//  are currently at.
		flags = nextTimeMediaSample;
	}

        durationf = (float)movieDur/(float)m_numFrames;

	// Get the bounds for the movie
	::GetMovieBox(m_movie, &m_srcRect);
        OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
	SetMovieBox(m_movie, &m_srcRect);	
	m_xsize = m_srcRect.right - m_srcRect.left;
	m_ysize = m_srcRect.bottom - m_srcRect.top;
       
        if (m_colorspace == GL_BGRA_EXT){
			m_csize = 4;
            m_format = GL_BGRA_EXT;
            m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
            
            createBuffer();
            prepareTexture();
            m_rowBytes = m_xsize * 4;
            if (m_hiquality) SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
            err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                                            k32ARGBPixelFormat,	
                                            &m_srcRect, 
                                            NULL, 
                                            NULL, 
                                            0, 
                                            m_pixBlock.image.data, 
                                            m_rowBytes);
                                        
        }else{
            m_csize = 2;
            m_format = GL_YCBCR_422_APPLE;
            m_pixBlock.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
            
            createBuffer();
            prepareTexture();
            m_rowBytes = m_xsize * 2;
            if (m_hiquality) SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
            err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                                            k422YpCbCr8CodecType,	
                                            &m_srcRect, 
                                            NULL, 
                                            NULL, 
                                            0, 
                                            m_pixBlock.image.data, 
                                            m_rowBytes);
        }
	if (err) {
		error("GEM: pix_filmDarwin: Couldn't make QTNewGWorldFromPtr %d", err);
		m_haveMovie = 0;
		return;
	}
             
        /* movies task method */
        m_movieTime = GetMovieTime(m_movie,nil);
        playRate = GetMoviePreferredRate(m_movie);
    
	// *** set the graphics world for displaying the movie ***
	::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
        
        if (m_auto) { 
            SetMovieRate(m_movie,X2Fix(m_rate));
           // SetMovieRate(m_movie,X2Fix(1.0));
            m_play = 1;
        }
        else {
            SetMovieRate(m_movie,X2Fix(0.0));
        }
        
	::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
        curTime = GetMovieTime(m_movie,NULL);
        prevTime = 0;
        newImage = 1;
        m_newFilm = 1;
        //StartMovie(m_movie);
        
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: getFrame()
{	
    short 	flags = nextTimeStep;
    OSType	whichMediaType = VisualMediaCharacteristic;
    if (!m_haveMovie) return;

    if (m_curFrame >= m_numFrames) m_curFrame = 0;
    
    //************************************
    //
    //what follows is some of the worst hack work i've ever done to get QT to 'work'
    //
    //the problem is that QT is very good a playing media if it manages everything itself internally.
    //however, that doesn't fit well with GEM because GEM has it's own internal tasking callbacks, so
    //in order to get the two to play nice, a bunch of ugly, shit code has to be done.   below is a way to
    //track the internal state of QT MoviesTask() and figure out which frame it is currently processing.
    //this avoids the frame being processed twice by the GEM render chain by managing the newImage flag
    //
    //note all of the crap to check for the direction of the playback and loop points.
    //
    // THERE MUST BE A BETTER WAY!!!!!!!!!!!!!!
    //
    //************************************
    if (m_auto){
        //play the startmovie() way
        if (!m_play) SetMovieRate(m_movie,X2Fix(m_rate));
        m_play = 1;
        
        if (m_rate > 0.f) {
            if (IsMovieDone(m_movie)) {
                GoToBeginningOfMovie(m_movie);
           	prevTime = 0;
                flags |= nextTimeEdgeOK;
                }
                
                MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld *** 
        curTime = GetMovieTime(m_movie,NULL);
                
                //check to see if the current position is past our next frame
                if (prevTime < curTime){
                    newImage = 1;
                    prevTime = curTime;
                    
                    //find next frame bounds using GetMovieNextIntertestingTime()
                    GetMovieNextInterestingTime(m_movie,
                                        flags,
                                        1,
                                        &whichMediaType,
                                        prevTime,
                                        0,
                                        &prevTime,
                                         nil);
    
                }
                else{
                    //if it's still the same frame then don't process
                    newImage = 0;
                    }
                
            }
        else {
                
                if (GetMovieTime(m_movie,nil) <= 0) {
                    GoToEndOfMovie(m_movie);
                    prevTime = GetMovieTime(m_movie,NULL);
                    curTime = prevTime;
                   // get the frame prior to the last frame
                    GetMovieNextInterestingTime(m_movie,
                                        flags,
                                        1,
                                        &whichMediaType,
                                        prevTime,
                                        -1,
                                        &prevTime,
                                         NULL);
                                         
                    
                    }else{
                    
                    MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld *** 
                    curTime = GetMovieTime(m_movie,NULL);
                    
                    if (prevTime >= curTime){
                        newImage = 1;
                        prevTime = curTime;
                        //find next frame bounds using GetMovieNextIntertestingTime()
                        GetMovieNextInterestingTime(m_movie,
                                            flags,
                                            1,
                                        &whichMediaType,
                                        prevTime,
                                        -1,
                                        &prevTime,
                                         NULL);
    
                    }
                    else{
                        newImage = 0;
                        }
                }
                }
            if (m_newFilm){
                        newImage = 1;
                        //MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld *** 
                       // curTime = GetMovieTime(m_movie,NULL);
                    }
                            
    }
    else
    {
    //play the manual way
        if (m_play) {
            SetMovieRate(m_movie,X2Fix(0.0));
            m_play = 0; //turn off play
            return;
            }
        
        //m_movieTime = m_reqFrame * duration;
        m_movieTime = (long)((float)m_reqFrame * durationf);
        
         m_movieTime-=9; //total hack!! subtract an arbitrary amount and have nextinterestingtime find the exact place
        ::GetMovieNextInterestingTime(	m_movie,
                                        flags,
                                        1,
                                        &whichMediaType,
                                        m_movieTime,
                                        0,
                                        &m_movieTime,
                                         NULL);
                                       // &durationf);
                                         
        SetMovieTimeValue(m_movie, m_movieTime);
        MoviesTask(m_movie, 0);
        
    }

}

void pix_movieDarwin :: render(GemState *state)
{

  //  m_rate = 1;
    //m_auto = 1;
   // getFrame();
    
    texFrame(state,1);
    getFrame();
   // MoviesTask(m_movie,0);
    //post("pix__movieDarwin: this render function is being called");

}


/////////////////////////////////////////////////////////
// on opening a file, prepare for texturing
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: prepareTexture()
{
    float x2,y2;
    float xratio, yratio;
    
    x2 = powerOfTwo(m_pixBlock.image.xsize);
    y2 = powerOfTwo(m_pixBlock.image.ysize);
    xratio = x2 / m_pixBlock.image.xsize;
    yratio = y2 / m_pixBlock.image.ysize;
    
    if (m_rectangle) {
    
        m_coords[3].s = 0.f;
        m_coords[3].t = 0.f;
    
        m_coords[2].s = m_pixBlock.image.xsize;
        m_coords[2].t = 0.f;
    
        m_coords[1].s = m_pixBlock.image.xsize;
        m_coords[1].t = m_pixBlock.image.ysize;
    
        m_coords[0].s = 0.f;
        m_coords[0].t = m_pixBlock.image.ysize;
        
    }
    else
    {
        m_coords[3].s = 0.f;
        m_coords[3].t = 0.f;
    
        m_coords[2].s = xratio;
        m_coords[2].t = 0.f;
    
        m_coords[1].s = xratio;
        m_coords[1].t = yratio;
    
        m_coords[0].s = 0.f;
        m_coords[0].t = yratio;
    }

/*
    if (!GemMan::texture_rectangle_supported)
    {
        int neededXSize = m_pixBlock.image.xsize;
        int neededYSize = m_pixBlock.image.ysize;
        post("pix_movie: prepareTexture: x : %d, y : %d", neededXSize, neededYSize );

        // ratio for the texture map coordinates
        m_xRatio = (float)m_xsize / (float)neededXSize;
        m_yRatio = (float)m_ysize / (float)neededYSize;

        m_coords[3].s = 0.f;
        m_coords[3].t = 0.f;
    
        m_coords[2].s = m_xRatio;
        m_coords[2].t = 0.f;
    
        m_coords[1].s = m_xRatio;
        m_coords[1].t = m_yRatio;
    
        m_coords[0].s = 0.f;
        m_coords[0].t = m_yRatio;
    } else {

        m_coords[3].s = 0.f;
        m_coords[3].t = 0.f;
    
        m_coords[2].s = m_pixBlock.image.xsize;
        m_coords[2].t = 0.f;
    
        m_coords[1].s = m_pixBlock.image.xsize;
        m_coords[1].t = m_pixBlock.image.ysize;
    
        m_coords[0].s = 0.f;
        m_coords[0].t = m_pixBlock.image.ysize;
    }
    */
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: setUpTextureState()
{
    if (m_rectangle) {
        post("pix__movieDarwin: using rectangle textures");
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_PRIORITY, 0.0);

        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
        
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        post("pix__movieDarwin: using poer of two textures");
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0.0);

        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

/*
    if ( !GemMan::texture_rectangle_supported )				//tigital
        {
        post("pix__movieDarwin: using rectangle textures");
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_PRIORITY, 0.0);

        if (GemMan::client_storage_supported)
            glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
        else
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else
    {
        post("pix__movieDarwin: using 2^n textures");
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } 
    */
}



/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: texFrame(GemState *state, int doit)
{
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;
  
  if (!m_rectangle){
        glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
 
  if (m_pixBlock.image.csize != m_dataSize[0] ||
	m_pixBlock.image.xsize != m_dataSize[1] ||
	m_pixBlock.image.ysize != m_dataSize[2]) {
        m_dataSize[0] = m_pixBlock.image.csize;
        m_dataSize[1] = m_pixBlock.image.xsize;
        m_dataSize[2] = m_pixBlock.image.ysize;
	glTexImage2D(GL_TEXTURE_2D, 0,
		     GL_RGBA,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     //m_pixBlock.image.format,
		     //m_pixBlock.image.type,
                     GL_YCBCR_422_APPLE,
                     GL_UNSIGNED_SHORT_8_8_REV_APPLE,
		     m_pixBlock.image.data);
            
    post("pix__movieDarwin: new power-of-two texture size - glTexImage2D");
    }

    

    glTexSubImage2D(GL_TEXTURE_2D, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
                    GL_YCBCR_422_APPLE,
                    GL_UNSIGNED_SHORT_8_8_REV_APPLE,
		  //  m_pixBlock.image.format,	// the format
		  //  m_pixBlock.image.type,	// the type
                   //  m_pixBlock.image.data);
		    m_frame);		
  }
  else{
  
  glEnable(GL_TEXTURE_RECTANGLE_EXT);
  glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
 
  if (m_pixBlock.image.csize != m_dataSize[0] ||
	m_pixBlock.image.xsize != m_dataSize[1] ||
	m_pixBlock.image.ysize != m_dataSize[2]) {
        m_dataSize[0] = m_pixBlock.image.csize;
        m_dataSize[1] = m_pixBlock.image.xsize;
        m_dataSize[2] = m_pixBlock.image.ysize;
	glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		     GL_RGBA,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     //m_pixBlock.image.format,
		     //m_pixBlock.image.type,
                     GL_YCBCR_422_APPLE,
                     GL_UNSIGNED_SHORT_8_8_REV_APPLE,
		     m_pixBlock.image.data);
            
    post("pix__movieDarwin: new rectangle texture size - glTexImage2D");
    }

    if (m_newFilm ){
            glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		     GL_RGBA,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     //m_pixBlock.image.format,
		     //m_pixBlock.image.type,
                     GL_YCBCR_422_APPLE,
                     GL_UNSIGNED_SHORT_8_8_REV_APPLE,
		     m_pixBlock.image.data);
	    post("pix_texture: new film");
            m_newFilm = 0; //just to be sure
      } 

    glTexSubImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
                    GL_YCBCR_422_APPLE,
                    GL_UNSIGNED_SHORT_8_8_REV_APPLE,
		  //  m_pixBlock.image.format,	// the format
		  //  m_pixBlock.image.type,	// the type
                   //  m_pixBlock.image.data);
		    m_frame);		// the data + header offset

    }

/*
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;
  // enable to texture binding
  if (!GemMan::texture_rectangle_supported)		//tigital
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
  } else {
    glEnable(GL_TEXTURE_RECTANGLE_EXT);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
  }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
  
  if (doit) {
    // if the size changed, then reset the texture
    if (m_pixBlock.image.csize != m_dataSize[0] ||
	m_pixBlock.image.xsize != m_dataSize[1] ||
	m_pixBlock.image.ysize != m_dataSize[2]) {
      m_dataSize[0] = m_pixBlock.image.csize;
      m_dataSize[1] = m_pixBlock.image.xsize;
      m_dataSize[2] = m_pixBlock.image.ysize;
      if (GemMan::texture_rectangle_supported)	//tigital
	glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		     m_pixBlock.image.csize,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     m_pixBlock.image.format,
		     m_pixBlock.image.type,
		     m_pixBlock.image.data);
      else
	glTexImage2D(GL_TEXTURE_2D, 0,
		     m_pixBlock.image.csize,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     m_pixBlock.image.format,
		     m_pixBlock.image.type,
		     m_pixBlock.image.data);
    }
    // okay, load in the actual pixel data
    if ( !GemMan::texture_rectangle_supported)
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
		    m_pixBlock.image.format,	// the format
		    m_pixBlock.image.type,	// the type
                     m_pixBlock.image.data);
		   // m_frame);		// the data + header offset
    else
        glTexSubImage2D(GL_TEXTURE_2D, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
		    m_pixBlock.image.format,	// the format
		    m_pixBlock.image.type,	// the type
                    m_pixBlock.image.data);
		   // m_frame);		// the data + header offset
  }
  */
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
  state->texture = 0;
  if (m_numFrames>0 && m_reqFrame>m_numFrames){
    m_reqFrame = m_numFrames;
    //outlet_bang(m_outEnd);
  }
#ifdef GL_TEXTURE_RECTANGLE_EXT
  if ( !GemMan::texture_rectangle_supported )
    glDisable(GL_TEXTURE_2D);
  else
    glDisable(GL_TEXTURE_RECTANGLE_EXT);
#else
    glDisable(GL_TEXTURE_2D);
#endif
}


/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: startRendering()
{
    glGenTextures(1, &m_textureObj);

    if ( ! m_rectangle )
        glBindTexture(GL_TEXTURE_2D, m_textureObj);
    else
        glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);

//    glBindTexture(GL_TEXTURE_2D, m_textureObj);

  
    setUpTextureState();
    
    m_pixBlock.newimage = 1;
    m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: stopRendering()
{
  if (m_textureObj) glDeleteTextures(1, &m_textureObj);
  m_textureObj = 0;
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = 0;
}


void pix_movieDarwin :: MovRate(float rate)
{
    m_rate = (float)rate;
    if (m_auto) {
    SetMovieRate(m_movie,X2Fix((double)m_rate));
    }
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
   // error("GEM: pix_film: selection number must be > 0");
    imgNum=0;
  }
  if (trackNum < 0){
    error("GEM: pix_film: track number must be > 0");
    trackNum=0;
  }

  if (m_haveMovie){
        if (imgNum > m_numFrames) {
            if (m_numFrames<0) m_reqFrame = imgNum;
                else m_reqFrame=m_numFrames;
      //      else error("GEM: pix_film: frame %d exceeds max (%d)", imgNum, m_numFrames);
      //m_reqFrame = imgNum;
            return;
        } else m_reqFrame = imgNum;
  }

/*
  switch (m_haveMovie){
  case GEM_MOVIE_MPG:
#ifdef HAVE_LIBMPEG3
#else
#ifdef HAVE_LIBMPEG
    m_reqFrame=(imgNum)?(m_curFrame==1)?2:1:0;
    break;
#endif
#endif
  case GEM_MOVIE_MOV:
    if (trackNum > m_numTracks-1) error("GEM: pix_film: track %d number too high (max %d) ", trackNum, m_numTracks-1);
    else m_track = trackNum;
  case GEM_MOVIE_AVI:
  default:
    if (imgNum > m_numFrames) {
      if (m_numFrames<0) m_reqFrame = imgNum;
      else m_reqFrame=m_numFrames;
      //      else error("GEM: pix_film: frame %d exceeds max (%d)", imgNum, m_numFrames);
      //m_reqFrame = imgNum;
      return;
    } else m_reqFrame = imgNum;
  }*/
}

void pix_movieDarwin :: LoadRam()
{
    TimeValue	length;
    OSErr err;
    if (m_haveMovie){      
        m_movieTime = 0;
        length = GetMovieDuration(m_movie);
        err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
        if (err)
        {
            post("pix_movie: LoadMovieIntoRam failed miserably");
        }
    }else{
        post("pix_movie: no movie to load into RAM!");
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_movieDarwin :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_movieDarwin,gensym("pix_movie"),A_DEFSYM,A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::rateCallback,
		  gensym("rate"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::ramCallback,
		  gensym("ram"),  A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::hiqualityCallback,
		  gensym("hiquality"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieDarwin::rectangleCallback,
		  gensym("rectangle"), A_DEFFLOAT, A_NULL);
}

void pix_movieDarwin :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename,0);
}

void pix_movieDarwin :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  //  GetMyClass(data)->changeImage((int)imgNum);
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_movieDarwin :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}

void pix_movieDarwin :: rateCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->MovRate(state);
}

void pix_movieDarwin :: ramCallback(void *data)
{
  GetMyClass(data)->LoadRam();
}

void pix_movieDarwin :: hiqualityCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_hiquality=(int)state;
}

void pix_movieDarwin :: rectangleCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_rectangle=(int)state;
}


#endif /* __APPLE__ */
