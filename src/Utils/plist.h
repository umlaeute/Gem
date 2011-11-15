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

#ifndef GEM_UTILS_PLIST_H
#define GEM_UTILS_PLIST_H 1

#include <m_pd.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif

#define animal_malloc_object(obj_type) ((obj_type *) malloc(sizeof(obj_type)))

#define ANIMAL_MALLOC_OBJECT(ptr, obj_type, fname, retval)  do { \
   ptr = animal_malloc_object(obj_type);                         \
   if (!ptr) {                                                   \
      error("pstk: could not allocate memory");                  \
      return retval;                                             \
   }                                                             \
   } while(0)

#define animal_malloc_array(obj_type,size) \
           ((obj_type *)malloc((size)*sizeof(obj_type)))

#define ANIMAL_MALLOC_ARRAY(ptr, obj_type, size, fname, retval) do {   \
   ptr = animal_malloc_array(obj_type,size);                           \
   if (!ptr) {                                                         \
      error("pstk: could not allocate memory for array");              \
      return retval;                                                   \
   }                                                                   \
   } while(0)


/*
 *  List of (row,col) point coordinates.
 */

/* --- Types --- */
typedef struct pt *plist_ptr;
typedef struct pt {
   int x;
   int y;
   struct pt *next;
} pt;

typedef struct list *list_ptr;
typedef struct list { // list of single integer indexes
   int val;
   struct list *next;
} list;


/* --- API --- */
#define NewPList()    NULL
#define is_empty(p)   ( (p)==NULL )  /* "p" is plist_ptr */
#define isnt_empty(p)   ( (p)!=NULL )  /* "p" is plist_ptr */
#define Nextpoint(p) ( (p)->next )

plist_ptr
   append_plist(plist_ptr *p, int x, int y);

int
   PrependPList(plist_ptr *head, int x, int y);

void
   free_plist(plist_ptr *p),
   print_plist(plist_ptr p);


/*
   @@@ the interface to list is more abstract than the plist interface
*/
#define new_list()    NULL
#define is_empty_list(l)   ( (l)==NULL )
#define isnt_empty_list(l) ( (l)!=NULL )
#define next_list_node(listptr)  ( (listptr)->next )
#define get_list_point(listptr) ( (listptr)->val )
#define not_nil(listptr) ((listptr)!=NULL)
#define get_list_head(l)   (l)  // returns  a listptr

int
   list_remove_head(list **l, int *val),
   list_append(list **l, int val),
   list_prepend(list **l, int val);

void
   print_list(list *l),
   free_list(list **l);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif /* !GEM_UTILS_PLIST_H */
