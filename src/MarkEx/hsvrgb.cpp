////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// mark@danks.org
//
//    Copyright (c) 1997-1999 Mark Danks.
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "m_pd.h"

#include <math.h>
#include "Base/GemFuncUtil.h"

static inline float TRI_MAX(float v1, float v2, float v3)
    { if (v1 > v2 && v1 > v3) return(v1);
	  if (v2 > v3) return(v2);
	  return(v3);
	}

static inline float TRI_MIN(float v1, float v2, float v3)
    { if (v1 < v2 && v1 < v3) return(v1);
	  if (v2 < v3) return(v2);
	  return(v3);
	}

/////////////////////////////////////////////////////////
//
// rgb2hsv
//
/////////////////////////////////////////////////////////
// instance structure
static t_class *rgb2hsv_class;

typedef struct _rgb2hsv
{
	t_object    x_obj;	        /* obligatory object header */
	t_outlet    *t_out1;	    /* the outlet */
}t_rgb2hsv;

static void rgb2hsv_float(t_rgb2hsv *x, t_floatarg r, t_floatarg g, t_floatarg b)
{
	t_atom argv[3];

	float h=0, s, v;

	r = FLOAT_CLAMP(r);
	g = FLOAT_CLAMP(g);
	b = FLOAT_CLAMP(b);
	float max = TRI_MAX(r, g, b);
	float min = TRI_MIN(r, g, b);
	v = max;		// the value

	// calculate saturation
	if (max != 0.0f)
		s = (max - min) / max;
	else
		s = 0.0f;

	if (s == 0.0f)
	{
		h = 0.0f;		// hue is undefined if no saturation	
	}
	// chromatic case - calculate hue
	else
	{
		float delta = max - min;
		if (r == max)						// between magenta and cyan
			h = (g - b) / delta;
		else if (g == max)					// between yellow and magenta
			h = 2.0f + (b - r) / delta;
		else if (b == max)					// between cyan and yellow
			h = 4.0f + (r - g) / delta;

		// convert hue to degrees
		h *= 60.0f;
		// make sure hue is nonnegative
		if (h < 0.0)
			h += 360.f;
		// normalize hue
		h /= 360.f;
	}

	SETFLOAT(&argv[0], h);
	SETFLOAT(&argv[1], s);
	SETFLOAT(&argv[2], v);

    outlet_list(x->t_out1, &s_list, 3, argv);
}

static void rgb2hsv_list(t_rgb2hsv *x, t_symbol *s, int argc, t_atom *argv)
{
	if (argc >= 3)
	{
		float r = atom_getfloat(&argv[0]);
		float g = atom_getfloat(&argv[1]);
		float b = atom_getfloat(&argv[2]);
		rgb2hsv_float(x, r, g, b);
	}
}

static void *rgb2hsv_new()	// init vals in struct
{
    t_rgb2hsv *x = (t_rgb2hsv *)pd_new(rgb2hsv_class);
    x->t_out1 = outlet_new(&x->x_obj, 0);
    return (x);
}

void rgb2hsv_setup()
{
	rgb2hsv_class = class_new(gensym("rgb2hsv"), (t_newmethod)rgb2hsv_new, 0,
    	    	    	sizeof(t_rgb2hsv), CLASS_DEFAULT, A_NULL);
//    class_addmethod(rgb2hsv_class, (t_method)rgb2hsv_float, &s_list, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addlist(rgb2hsv_class, (t_method)rgb2hsv_list);
}

/////////////////////////////////////////////////////////
//
// hsv2rgb
//
/////////////////////////////////////////////////////////
// instance structure
static t_class *hsv2rgb_class;

typedef struct _hsv2rgb
{
	t_object    x_obj;	        // obligatory object header
	t_outlet    *t_out1;	    // the outlet
}t_hsv2rgb;

static void hsv2rgb_float(t_hsv2rgb *x, t_floatarg h, t_floatarg s, t_floatarg v)
{
	t_atom argv[3];

	h = FLOAT_CLAMP(h);
	s = FLOAT_CLAMP(s);
	v = FLOAT_CLAMP(v);
	float r=0, g=0, b=0;
	// convert hue to degrees
	h *= 360.f;
	
	if (s == 0.0)		// black and white
	{
		r = g = b = v;
	}
	else
	{
		if (h == 360.0)			// 360 == 0 degrees
			h = 0.0f;
		h /= 60.0f;				// hue is now [0, 6]
		int i = (int)floor(h);
		float f = h - i;		// f is the fractional part of h
		float p = v * (1 - s);
		float q = v * (1 - s * f);
		float t = v * (1 - s * (1 - f));
		
		switch (i)
		{
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			case 5:
				r = v;
				g = p;
				b = q;
				break;
		}
	}
	SETFLOAT(&argv[0], r);
	SETFLOAT(&argv[1], g);
	SETFLOAT(&argv[2], b);
    outlet_list(x->t_out1, &s_list, 3, argv);
}

static void hsv2rgb_list(t_hsv2rgb *x, t_symbol *s, int argc, t_atom *argv)
{
	if (argc >= 3)
	{
		float h = atom_getfloat(&argv[0]);
		float s = atom_getfloat(&argv[1]);
		float v = atom_getfloat(&argv[2]);
		hsv2rgb_float(x, h, s, v);
	}
}

static void *hsv2rgb_new()	// init vals in struct
{
    t_hsv2rgb *x = (t_hsv2rgb *)pd_new(hsv2rgb_class);
    x->t_out1 = outlet_new(&x->x_obj, 0);
    return (x);
}

void hsv2rgb_setup()
{
	hsv2rgb_class = class_new(gensym("hsv2rgb"), (t_newmethod)hsv2rgb_new, 0,
    	    	    	sizeof(t_hsv2rgb), CLASS_DEFAULT, A_NULL);
//    class_addmethod(hsv2rgb_class, (t_method)hsv2rgb_float, &s_list, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addlist(hsv2rgb_class, (t_method)hsv2rgb_list);
}

