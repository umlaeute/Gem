/*
 * -------------------------------------------------------------------------
 * A lightweight integer and pair (x,y) list
 *
 * Code from ANIMAL - ANIMAL IMage Processing Library
 * Copyright (C) 2002-2011  Ricardo Fabbri
 * This version released in 2011 under Gem license by Ricardo Fabbri
 * Originally released under GPL v.2
 * -------------------------------------------------------------------------
 */

#include "plist.h"
#include <malloc.h>


#define Allocpoint() (plist_ptr) malloc(sizeof(pt))

void
free_plist(plist_ptr *p)
{
   plist_ptr aux;

   aux = *p;
   while (aux != NULL) {
      aux = (*p)->next;
      free(*p);
      *p = aux;
   }
}


/*
 * Returns 0 if allocation fails; 1 for success;
 */
int
PrependPList(plist_ptr *head, int x, int y)
{
   plist_ptr pnew;

   pnew = Allocpoint();
   if (pnew == NULL)
      return 0;
   pnew->x = x;
   pnew->y = y;

   pnew->next = *head;
   *head = pnew;
   return 1;
}

/*
   Returns NULL if allocation fails.
   Otherwise, returns a pointer to the newly installed element.
*/
plist_ptr
append_plist(plist_ptr *lst, int x, int y)
{
   plist_ptr p, paux, prev=NULL;

   p=*lst;

   while (p != NULL) {
      prev = p;
      p = p->next;
   }

   paux = Allocpoint();
   if (!paux)
      return NULL;

   paux->x = x;
   paux->y = y;

   if (prev != NULL)
      prev->next = paux;
   else  /* list was intitially empty */
      *lst = paux;


   paux->next = NULL;

   return paux;
}

void
print_plist(plist_ptr p)
{
   printf("\n--------------------\n");
   while (p != NULL) {
      printf("%d %d\n", p->x, p->y);
      p = p->next;
   }
   printf("---------END--------\n");
}

void
print_list(list *p)
{
   printf("\n-------LIST---------\n");
   while (isnt_empty_list(p)) {
      printf("%d\n", p->val);
      p = p->next;
   }
   printf("--------END---------\n");
}

//-----------------------------------------------------------------------------
void
free_list(list **l)
{
   list *aux;
   while (isnt_empty_list(*l)) {
      aux = *l;
      *l = next_list_node(*l);
      free(aux);
   }
   *l = NULL;
}

int
list_append(list **lst, int val)
{
   char *fname="list_append";
   list *pnode, *last=*lst;

   ANIMAL_MALLOC_OBJECT(pnode, list, fname,false);
   pnode->val = val;

   if (last) {
      while (last->next)
         last = last->next;
      pnode->next = last->next;
      last->next  = pnode;
   } else {
      pnode->next = NULL;
      *lst = pnode;
   }
   return true;
}

int
list_prepend(list **lst, int val)
{
   list *node;

   node = (list *) malloc(sizeof(list));
   if (!node) return false;
   node->val = val;
   node->next = *lst;
   *lst = node;
   return true;
}

int
list_remove_head(list **lst, int *val)
{
   int elem;
   list *aux;

   if (is_empty_list(*lst))
      return false;

   aux  = *lst;
   *lst = next_list_node(aux);
   elem = get_list_point(aux);
   free(aux);
   *val = elem;
   return true;
}
