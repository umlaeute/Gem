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
#include <math.h>

/* ------------------------- abs~ -------------------------- */
static t_class *abs_class;

typedef struct _abs
{
    t_object x_obj;
} t_abs;

static t_int *abs_perform(t_int *w)
{
    //t_abs *x = (t_abs *)(w[1]);
    t_float *in = (t_float *)(w[2]);
    t_float *out = (t_float *)(w[3]);
    int n = (int)(w[4]);
    while (n--)
    {
    	float f = *in++;
    	if (f < 0) f = -f;
    	*out++ = f;
    }
    return (w+5);
}

static void abs_dsp(t_abs *x, t_signal **sp)
{
    dsp_add(abs_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

static void *abs_new()
{
    t_abs *x = (t_abs *)pd_new(abs_class);
    outlet_new(&x->x_obj, &s_signal);
    return (x);
}

void abs_setup(void)
{
    abs_class = class_new(gensym("abs~"), (t_newmethod)abs_new, 0,
    	sizeof(t_abs), 0, A_NULL);
    class_addmethod(abs_class, (t_method)nullfn, &s_signal, A_NULL);
    class_addmethod(abs_class, (t_method)abs_dsp, gensym("dsp"), A_NULL);
}

