/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2016, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  nop functions/macros

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
