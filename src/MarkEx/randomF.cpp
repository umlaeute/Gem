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
#include <stdlib.h>
#ifdef NT
int random(void)
{
    static int foo = 1489853723;
    foo = foo * (int)435898247 + (int)9382842987;
    return (foo & 0x7fffffff);
}
#endif

/* -------------------------- randomF / randF ------------------------------ */

/* instance structure */

static t_class *randomF_class;

typedef struct _randomF
{
    t_object	x_obj;
    t_float		x_range;
	t_outlet    *t_out1;	    /* the outlet */
} t_randomF;

static void *randomF_new(t_floatarg n)
{
    t_randomF *x = (t_randomF *)pd_new(randomF_class);
    x->x_range = (float)n;
    floatinlet_new(&x->x_obj, &x->x_range);
    x->t_out1 = outlet_new(&x->x_obj, 0);
    return (x);
}

static void randomF_bang(t_randomF *x)
{
    double range = (x->x_range < 0 ? 0 : x->x_range);
    double n = (double)range * (double)random() * (1. / 2147483648.);
    if (n >= range) n = range - 1;
    outlet_float(x->t_out1, (float)n);
}

void randomF_setup(void)
{
    randomF_class = class_new(gensym("randomF"), (t_newmethod)randomF_new, 0,
    	sizeof(t_randomF), 0, A_DEFFLOAT, 0);
    class_addcreator((t_newmethod)randomF_new, gensym("randF"), A_DEFFLOAT, 0);
    class_addbang(randomF_class, (t_method)randomF_bang);
}

/* -------------------------- tripleRand ------------------------------ */

/* instance structure */

static t_class *tripleRand_class;

typedef struct _tripleRand
{
    t_object	x_obj;
    t_float		x_range[3];
	t_outlet    *t_out1;	    /* the outlet */
} t_tripleRand;

void setTripleRandVals(t_tripleRand *x, t_symbol *s, int argc, t_atom *argv)
{
	if (argc < 1 || argc > 3)
	{
		error("tripleRand: Requires 1 - 3 values");
		return;
	}
	if (argc >= 1)
	{
		x->x_range[0] = atom_getfloat(&argv[0]);
		if (x->x_range[0] < 0) x->x_range[0] = 0;
		x->x_range[2] = x->x_range[1] = x->x_range[0];
	}
	if (argc >= 2)
	{
		x->x_range[1] = atom_getfloat(&argv[1]);
		if (x->x_range[1] < 0) x->x_range[1] = 0;
		x->x_range[2] = x->x_range[1];
	}
	if (argc >= 3)
	{
		x->x_range[2] = atom_getfloat(&argv[2]);
		if (x->x_range[2] < 0) x->x_range[2] = 0;
	}
}

static void tripleRand_bang(t_tripleRand *x)
{
	int i;
	t_atom argv[3];
	
	for (i = 0; i < 3; i++)
	{
	    float n = (float)((double)x->x_range[i] * (double)random() * (1. / 2147483648.));
	    if (n > x->x_range[i]) n = x->x_range[i] - 1;
		SETFLOAT((&argv[i]), (float)n);
	}

	outlet_list(x->t_out1, &s_list, 3, argv);
}

static void *tripleRand_new(t_floatarg arg1, t_floatarg arg2, t_floatarg arg3)
{
    t_tripleRand *x = (t_tripleRand *)pd_new(tripleRand_class);
	if (arg1 < 0.f)
		x->x_range[0] = 0.f;
	else
		x->x_range[0] = (float)arg1;
    if (arg2 <= 0.f)
		x->x_range[1] = x->x_range[0];
	else
		x->x_range[1] = (float)arg2;
    if (arg3 <= 0.f)
		x->x_range[2] = x->x_range[1];
	else
		x->x_range[2] = (float)arg3;
    x->t_out1 = outlet_new(&x->x_obj, 0);

    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("newVals"));
    return (x);
}

void tripleRand_setup(void)
{
    tripleRand_class = class_new(gensym("tripleRand"), (t_newmethod)tripleRand_new, 0,
    	sizeof(t_tripleRand), 0, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(tripleRand_class, (t_method)tripleRand_bang);
    class_addmethod(tripleRand_class, (t_method)setTripleRandVals,
    	    gensym("newVals"), A_GIMME, A_NULL); 
}


