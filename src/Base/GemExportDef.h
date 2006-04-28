/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Export crap

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMEXPORTDEF_H_
#define INCLUDE_GEMEXPORTDEF_H_

#ifdef _MSC_VER

/* turn of some warnings on vc-compilers */
# pragma warning( disable : 4244 )
# pragma warning( disable : 4305 )
# pragma warning( disable : 4091 )
// "switch" without "case" (just "default")
# pragma warning( disable : 4065 )

// Windows requires explicit import and exporting of functions and classes.
// While this is a pain to do sometimes, in large software development
//      projects, it is very usefull.
#ifdef GEM_INTERNAL     // GEM exporting things
#define GEM_EXTERN __declspec(dllexport)
#else                   // other's importing
#define GEM_EXTERN __declspec(dllimport)
#endif                  // for GEM_INTERNAL

#else                   // other OS's
#define GEM_EXTERN
#endif

#endif	// for header file
