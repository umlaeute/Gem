/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    nop.h
       - contains nop functions/macros
       - part of GEM

    Copyright (c) 2016 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_UTILS_NOP_H_
#define _INCLUDE__GEM_UTILS_NOP_H_

#if __cplusplus > 199711L || __STDC_VERSION__ >= 199901L
# define nop(...) do {} while (0)
#else
static inline void gem__nop(...) { ; }
# define nop gem__nop
#endif

static inline void gem__nop_post(void) { ; }
static inline void gem__nop_post(const char*fmt, ...) { ; }
#define nop_post gem__nop_post

#endif  // for header file
