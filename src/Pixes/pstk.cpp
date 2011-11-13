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
  

#include "pstk.h"
#include <malloc.h>


int 
ptpush(pstk_ptr *pstk, point *pt)
{
   return PrependPList(pstk, pt->x, pt->y);
}

point *
ptpop(pstk_ptr *pstk)
{
   pstk_ptr aux, stk;
   point *p;

   stk = *pstk;
   assert(pstk_isnt_empty(stk));

   ANIMAL_MALLOC_OBJECT(p,point,"ptpop",NULL);

   p->x = stk->x; p->y = stk->y;
   
   /* remove top */
   aux=stk;
   stk=stk->next;
   free(aux);
   *pstk=stk;
   return p;
}

void 
free_pstk(pstk_ptr *p)
{
   free_plist(p);
}


// SIMPLE INTEGER STACK METHODS

int_stk *
new_stk(int n)
{
   int_stk *stk;
   char *func="new_stk";

   assert(n);
   ANIMAL_MALLOC_OBJECT(stk,int_stk,func,NULL);
   ANIMAL_MALLOC_ARRAY(stk->data, int, n, func,NULL);

   stk->n   =  n;
   stk->top = -1; 
   return stk;
}
