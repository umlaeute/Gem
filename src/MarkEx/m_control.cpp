/*
 * Copyright (c) 1997-1999 Mark Danks.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 */

#ifdef NT
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4091 )
#endif
#include "m_pd.h"

/* -------------------------- invert ------------------------------ */

/* instance structure */

static t_class *invert_class;

typedef struct _invert
{
	t_object    x_obj;			/* obligatory object header */
	t_outlet    *t_out1;	    /* the outlet */
} t_invert;

static void invert_float(t_invert *x, t_floatarg n)
{
    if (n) outlet_float(x->t_out1, 0.f);
    else outlet_float(x->t_out1, 1.f);
}

static void *invert_new() /* init vals in struc */
{
    t_invert *x = (t_invert *)pd_new(invert_class);
    x->t_out1 = outlet_new(&x->x_obj, 0);
    return(x);
}

void invert_setup(void)
{
    invert_class = class_new(gensym("invert"), (t_newmethod)invert_new, 0,
    	    	    	sizeof(t_invert), 0, A_NULL);
    class_addfloat(invert_class, (t_method)invert_float);
}

/* -------------------------- change ------------------------------ */

/* instance structure */

static t_class *change_class;

typedef struct _change
{
	t_object    x_obj;			/* obligatory object header */
	float		x_cur;
	t_outlet    *t_out1;	    /* the outlet */
} t_change;

static void change_float(t_change *x, t_floatarg n)
{
    if (n != x->x_cur)
	{
		outlet_float(x->t_out1, n);
		x->x_cur = n;
    }
}

static void *change_new() /* init vals in struc */
{
    t_change *x = (t_change *)pd_new(change_class);
    x->x_cur = -1.f;
	x->t_out1 = outlet_new(&x->x_obj, 0);
    return(x);
}

void change_setup(void)
{
    change_class = class_new(gensym("change"), (t_newmethod)change_new, 0,
    	    	    	sizeof(t_change), 0, A_NULL);
    class_addfloat(change_class, change_float);
}

/* -------------------------- alternate ------------------------------ */

/* instance structure */

static t_class *alternate_class;

typedef struct _alternate
{
	t_object    x_obj;	        /* obligatory object header */
	int	        a_which;	    /* which outlet to go out 0 = left, 1 = right    */
	t_outlet    *t_out1;	    /* the outlet */
	t_outlet    *t_out2;	    /* the other outlet */
} t_alternate;

static void alternate_list(t_alternate *x, t_symbol *s, int argc, t_atom *argv)
{
	static t_symbol *listSym = NULL;

	if (!listSym)
		listSym = gensym("list");

    if (!x->a_which) outlet_list(x->t_out1, listSym, argc, argv);
    else outlet_list(x->t_out2, listSym, argc, argv);
    x->a_which++;
    x->a_which = (x->a_which > 1) ? 0 : x->a_which;
}

static void alternate_float(t_alternate *x, t_floatarg n)
{
    if (!x->a_which) outlet_float(x->t_out1, n);
    else outlet_float(x->t_out2, n);
    x->a_which++;
    x->a_which = (x->a_which > 1) ? 0 : x->a_which;
}

static void alternate_bang(t_alternate *x)
{
    if (!x->a_which) outlet_bang(x->t_out1);
    else outlet_bang(x->t_out2);
    x->a_which++;
    x->a_which = (x->a_which > 1) ? 0 : x->a_which;
}

static void alternate_reset(t_alternate *x)
{
    x->a_which = 0;
}

static void *alternate_new() /* init vals in struc */
{
    t_alternate *x = (t_alternate *)pd_new(alternate_class);
    x->t_out1 = outlet_new(&x->x_obj, 0);
    x->t_out2 = outlet_new(&x->x_obj, 0);
    x->a_which = 0;
    return (x);
}

void alternate_setup(void)
{
    alternate_class = class_new(gensym("alternate"), (t_newmethod)alternate_new, 0,
    	    	    	sizeof(t_alternate), 0, A_NULL);
    class_addfloat(alternate_class, (t_method)alternate_float);
    class_addbang(alternate_class, (t_method)alternate_bang);
    class_addmethod(alternate_class, (t_method)alternate_reset, gensym("reset"), A_NULL);
    class_addmethod(alternate_class, (t_method)alternate_list,
    	    gensym("list"), A_GIMME, A_NULL); 
}

/* -------------------------- oneshot ------------------------------ */

/* instance structure */

static t_class *oneshot_class;

typedef struct _oneshot
{
	t_object    x_obj;	        /* obligatory object header */
	int	        t_armed;	    /* if willing to pass data */
	t_outlet    *t_out1;	    /* the outlet */
} t_oneshot;

static void oneshot_float(t_oneshot *x, t_floatarg n)
{
    if (x->t_armed)
    {
	    outlet_float(x->t_out1, n);
	    x->t_armed = 0;
    }
}

static void oneshot_bang(t_oneshot *x)
{
    if (x->t_armed)
    {
	    outlet_bang(x->t_out1);
	    x->t_armed = 0;
    }
}

static void oneshot_clear(t_oneshot *x)
{
    x->t_armed = 1;
}

static void *oneshot_new() /* init vals in struc */
{
    t_oneshot *x = (t_oneshot *)pd_new(oneshot_class);
    x->t_out1 = outlet_new(&x->x_obj, 0);
    x->t_armed = 1;
    return (x);
}

void oneshot_setup(void)
{
    oneshot_class = class_new(gensym("oneshot"), (t_newmethod)oneshot_new, 0,
    	    	    	sizeof(t_oneshot), 0, A_NULL);
    class_addfloat(oneshot_class, (t_method)oneshot_float);
    class_addbang(oneshot_class, (t_method)oneshot_bang);
    class_addmethod(oneshot_class, (t_method)oneshot_clear, gensym("clear"), A_NULL);
}

