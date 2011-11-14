/*
 * -------------------------------------------------------------------------
 * A lightweight (x,y) integer point stack
 *
 * Code from ANIMAL - ANIMAL IMage Processing Library
 * Copyright (C) 2002-2011  Ricardo Fabbri
 * This version released in 2011 under Gem license by Ricardo Fabbri
 * Originally released under GPL v.2
 * -------------------------------------------------------------------------
 */
#ifndef GEM_UTILS_PSTK_H
#define GEM_UTILS_PSTK_H 1

#include "plist.h"

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif


/*
 *  Stack of (row,col) point coordinates. More suited for internal
 *  usage in pix_multiblob.
 */

typedef plist_ptr pstk_ptr;

#define new_pstk()    NULL
#define pstk_is_empty(p)   ( (p)==NULL )
#define pstk_isnt_empty(p)   ( (p)!=NULL )

typedef struct {
   int x;
   int y;
} point;


int
   ptpush(pstk_ptr *stk, point *pt);

point
   *ptpop(pstk_ptr *stk);

void
   free_pstk(pstk_ptr *p);

/*
 *   A SIMPLE INTEGER STACK
 *       - used for storing points as single indexes
 *       - number of elements is known in advance
 *       - interface is good enough for use outside Animal
 */

typedef struct stk {
   int *data;
   int top;
   int n;
} int_stk;

// Methods for a simple integer stack
#define free_stk(stk) free(stk);
#define evacuate_stk(stk) stk->top = -1
#define push(p,stk) { stk->top++; stk->data[stk->top] = (p); }
#define pop(stk) stk->data[stk->top--]
#define stk_isnt_empty(stk) (stk->top != -1)

int_stk
   *new_stk(int n);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif /* !GEM_UTILS_PSTK_H */
