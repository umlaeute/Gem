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

// -------------------------- vector+ ------------------------------

// instance structure
static t_class *vectorPlus_class;

typedef struct _vectorPlus
{
    t_object	x_obj;
    t_float		x_add;
	t_atom		*m_list;
	int			m_num;
	t_outlet	*t_out1;	    // the outlet
} t_vectorPlus;

static void doVectorPlus(t_vectorPlus *x, t_symbol *s, int argc, t_atom *argv)
{
	int i;
	if (argc > x->m_num)
	{
		x->m_list = (t_atom *)resizebytes(x->m_list, sizeof(t_atom) * x->m_num, sizeof(t_atom) * argc);
		x->m_num = argc;
	}
	for (i = 0; i < argc; i++)
	{
		float temp = atom_getfloat(&argv[i]);
		temp += x->x_add;
		SETFLOAT((&x->m_list[i]), temp);
	}
	outlet_list(x->t_out1, &s_list, argc, x->m_list);
}

static void *vectorPlus_new(t_floatarg n)
{
    t_vectorPlus *x = (t_vectorPlus *)pd_new(vectorPlus_class);
    x->x_add = (float)n;
    floatinlet_new(&x->x_obj, &x->x_add);
    x->t_out1 = outlet_new(&x->x_obj, 0);
	x->m_num = 3;
	x->m_list = (t_atom *)getbytes(sizeof(t_atom) * x->m_num);
    return (x);
}

static void vectorPlus_setup(void)
{
    vectorPlus_class = class_new(gensym("vector+"), (t_newmethod)vectorPlus_new, 0,
    	sizeof(t_vectorPlus), 0, A_DEFFLOAT, 0);
    class_addcreator((t_newmethod)vectorPlus_new, gensym("v+"), A_DEFFLOAT, 0);
    class_addmethod(vectorPlus_class, (t_method)doVectorPlus,
    	    &s_list, A_GIMME, A_NULL); 
}

/* -------------------------- vector- ------------------------------ */

/* instance structure */
static t_class *vectorMinus_class;

typedef struct _vectorMinus
{
    t_object	x_obj;
    t_float		x_minus;
	t_atom		*m_list;
	int			m_num;
	t_outlet    *t_out1;	    /* the outlet */
} t_vectorMinus;

static void doVectorMinus(t_vectorMinus *x, t_symbol *s, int argc, t_atom *argv)
{
	int i;
	if (argc > x->m_num)
	{
		x->m_list = (t_atom *)resizebytes(x->m_list, sizeof(t_atom) * x->m_num, sizeof(t_atom) * argc);
		x->m_num = argc;
	}
	for (i = 0; i < argc; i++)
	{
		float temp = atom_getfloat(&argv[i]);
		temp -= x->x_minus;
		SETFLOAT((&x->m_list[i]), temp);
	}
	outlet_list(x->t_out1, &s_list, argc, x->m_list);
}

static void *vectorMinus_new(t_floatarg n)
{
    t_vectorMinus *x = (t_vectorMinus *)pd_new(vectorMinus_class);
    x->x_minus = (float)n;
    floatinlet_new(&x->x_obj, &x->x_minus);
    x->t_out1 = outlet_new(&x->x_obj, 0);
	x->m_num = 3;
	x->m_list = (t_atom *)getbytes(sizeof(t_atom) * x->m_num);
    return (x);
}

static void vectorMinus_setup(void)
{
    vectorMinus_class = class_new(gensym("vector-"), (t_newmethod)vectorMinus_new, 0,
    	sizeof(t_vectorMinus), 0, A_DEFFLOAT, 0);
    class_addcreator((t_newmethod)vectorMinus_new, gensym("v-"), A_DEFFLOAT, 0);
    class_addmethod(vectorMinus_class, (t_method)doVectorMinus,
    	    &s_list, A_GIMME, A_NULL); 
}

/* -------------------------- vector* ------------------------------ */

/* instance structure */
static t_class *vectorMult_class;

typedef struct _vectorMult
{
    t_object	x_obj;
    t_float		x_mult;
	t_atom		*m_list;
	int			m_num;
	t_outlet    *t_out1;	    /* the outlet */
} t_vectorMult;

static void doVectorMult(t_vectorMult *x, t_symbol *s, int argc, t_atom *argv)
{
	int i;
	if (argc > x->m_num)
	{
		x->m_list = (t_atom *)resizebytes(x->m_list, sizeof(t_atom) * x->m_num, sizeof(t_atom) * argc);
		x->m_num = argc;
	}
	for (i = 0; i < argc; i++)
	{
		float temp = atom_getfloat(&argv[i]);
		temp *= x->x_mult;
		SETFLOAT((&x->m_list[i]), temp);
	}
	outlet_list(x->t_out1, &s_list, argc, x->m_list);
}

static void *vectorMult_new(t_floatarg n)
{
    t_vectorMult *x = (t_vectorMult *)pd_new(vectorMult_class);
    x->x_mult = (float)n;
    floatinlet_new(&x->x_obj, &x->x_mult);
    x->t_out1 = outlet_new(&x->x_obj, 0);
	x->m_num = 3;
	x->m_list = (t_atom *)getbytes(sizeof(t_atom) * x->m_num);
    return (x);
}

static void vectorMult_setup(void)
{
    vectorMult_class = class_new(gensym("vector*"), (t_newmethod)vectorMult_new, 0,
    	sizeof(t_vectorMult), 0, A_DEFFLOAT, 0);
    class_addcreator((t_newmethod)vectorMult_new, gensym("v*"), A_DEFFLOAT, 0);
    class_addmethod(vectorMult_class, (t_method)doVectorMult,
		&s_list, A_GIMME, A_NULL); 
}

/* -------------------------- vector/ ------------------------------ */

/* instance structure */
static t_class *vectorDiv_class;

typedef struct _vectorDiv
{
    t_object	x_obj;
    t_float		x_div;
	t_atom		*m_list;
	int			m_num;
	t_outlet    *t_out1;	    /* the outlet */
} t_vectorDiv;

static void doVectorDiv(t_vectorDiv *x, t_symbol *s, int argc, t_atom *argv)
{
	int i;
	if (argc > x->m_num)
	{
		x->m_list = (t_atom *)resizebytes(x->m_list, sizeof(t_atom) * x->m_num, sizeof(t_atom) * argc);
		x->m_num = argc;
	}
	for (i = 0; i < argc; i++)
	{
		float temp = atom_getfloat(&argv[i]);
		temp /= x->x_div;
		SETFLOAT((&x->m_list[i]), temp);
	}
	outlet_list(x->t_out1, &s_list, argc, x->m_list);
}

static void *vectorDiv_new(t_floatarg n)
{
    t_vectorDiv *x = (t_vectorDiv *)pd_new(vectorDiv_class);
    x->x_div = (float)n;
    floatinlet_new(&x->x_obj, &x->x_div);
    x->t_out1 = outlet_new(&x->x_obj, 0);
	x->m_num = 3;
	x->m_list = (t_atom *)getbytes(sizeof(t_atom) * x->m_num);
    return (x);
}

static void vectorDiv_setup(void)
{
    vectorDiv_class = class_new(gensym("vector/"), (t_newmethod)vectorDiv_new, 0,
    	sizeof(t_vectorDiv), 0, A_DEFFLOAT, 0);
    class_addcreator((t_newmethod)vectorDiv_new, gensym("v/"), A_DEFFLOAT, 0);
    class_addmethod(vectorDiv_class, (t_method)doVectorDiv,
    	    &s_list, A_GIMME, A_NULL); 
}

/* -------------------------- vectorabs ------------------------------ */

/* instance structure */
static t_class *vectorAbs_class;

typedef struct _vectorAbs
{
    t_object	x_obj;
	t_atom		*m_list;
	int			m_num;
	t_outlet    *t_out1;	    /* the outlet */
} t_vectorAbs;

static void doVectorAbs(t_vectorAbs *x, t_symbol *s, int argc, t_atom *argv)
{
  int i;
  if (argc > x->m_num)
    {
      x->m_list = (t_atom *)resizebytes(x->m_list, sizeof(t_atom) * x->m_num, sizeof(t_atom) * argc);
      x->m_num = argc;
    }
  for (i = 0; i < argc; i++)
    {
      float temp = atom_getfloat(&argv[i]);
      if (temp < 0.f)
	temp = temp * -1.f;
      SETFLOAT((&x->m_list[i]), temp);
    }
  outlet_list(x->t_out1, &s_list, argc, x->m_list);
}

static void *vectorAbs_new()
{
    t_vectorAbs *x = (t_vectorAbs *)pd_new(vectorAbs_class);
    x->t_out1 = outlet_new(&x->x_obj, 0);
	x->m_num = 3;
	x->m_list = (t_atom *)getbytes(sizeof(t_atom) * x->m_num);
    return (x);
}

static void vectorAbs_setup(void)
{
    vectorAbs_class = class_new(gensym("vectorabs"), (t_newmethod)vectorAbs_new, 0,
    	sizeof(t_vectorAbs), 0, A_NULL);
    class_addcreator((t_newmethod)vectorAbs_new, gensym("vabs"), A_NULL);
    class_addmethod(vectorAbs_class, (t_method)doVectorAbs,
		&s_list, A_GIMME, A_NULL); 
}

/* -------------------------- vectorpack ------------------------------ */

static t_class *vectorPack_class;

typedef struct _vectorPack
{
    t_object	x_obj;
    t_float		x_val;
	t_atom		*m_list;
	int			m_num;
	t_outlet    *t_out1;	    /* the outlet */
} t_vectorPack;

static void doVectorPack(t_vectorPack *x, t_symbol *s, int argc, t_atom *argv)
{
	int i;
	if ((argc + 1) > x->m_num)
	{
		x->m_list = (t_atom *)resizebytes(x->m_list, sizeof(t_atom) * x->m_num, sizeof(t_atom) * (argc + 1));
		x->m_num = argc;
	}
	for (i = 0; i < argc; i++)
	{
		float temp = atom_getfloat(&argv[i]);
		SETFLOAT((&x->m_list[i]), temp);
	}
	SETFLOAT((&x->m_list[argc]), x->x_val);
	outlet_list(x->t_out1, &s_list, (argc + 1), x->m_list);
}

static void *vectorPack_new(t_floatarg n)
{
    t_vectorPack *x = (t_vectorPack *)pd_new(vectorPack_class);
    x->x_val = (float)n;
    floatinlet_new(&x->x_obj, &x->x_val);
    x->t_out1 = outlet_new(&x->x_obj, 0);
	x->m_num = 4;
	x->m_list = (t_atom *)getbytes(sizeof(t_atom) * x->m_num);
    return (x);
}

static void vectorPack_setup(void)
{
    vectorPack_class = class_new(gensym("vectorpack"), (t_newmethod)vectorPack_new, 0,
    	sizeof(t_vectorPack), 0, A_DEFFLOAT, 0);
    class_addcreator((t_newmethod)vectorPack_new, gensym("vpack"), A_DEFFLOAT, 0);
    class_addmethod(vectorPack_class, (t_method)doVectorPack,
    	    &s_list, A_GIMME, A_NULL); 
}

/* -------------------------- setup ------------------------------ */


void vector_setup(void)
{
	vectorPlus_setup();
	vectorMinus_setup();
	vectorMult_setup();
	vectorDiv_setup();
	vectorAbs_setup();
	vectorPack_setup();
}
