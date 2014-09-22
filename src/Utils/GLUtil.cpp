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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GLUtil.h"
#include <ctype.h>
#include <string.h>

#include "Gem/RTE.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WIN32
/* disable warnings about unknown pragmas */
# pragma warning( disable : 4068 )
#endif


#define _GL_UNDEFINED -1

// if error dump gl errors to debugger string, return error
GLenum gem::utils::gl::glReportError (bool verbose)
{
  GLenum err = glGetError();
  if (verbose && GL_NO_ERROR != err) {
    post("GL: %s",(char*)gluErrorString(err));
  }
  // ensure we are returning an OSStatus noErr if no error condition
  if (err == GL_NO_ERROR)
    return 0;
  else
    return err;
}

    }
  }

}
  }
}
