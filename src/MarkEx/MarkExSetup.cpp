/*
 * Copyright (c) 1997-1999 Mark Danks.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 */

// -------------------------- setup functions ------------------------------

// control objects
void alternate_setup(); //build as an abstraction!
void average_setup();
void counter_setup();//build as an abstraction!
// void multipack_setup();
void multiselect_setup();
void oneshot_setup();//build as an abstraction!
void invert_setup();//build as an abstraction!
void strcat_setup();

void vector_setup();

// GEM utilities
void tripleLine_setup();//build as an abstraction!
void randomF_setup();
void tripleRand_setup();//build as an abstraction!
void hsv2rgb_setup();//build as an abstraction!
void rgb2hsv_setup();//build as an abstraction!

// DSP objects
void reson_setup();
void abs_setup();

void MarkEx_setup()
{
    alternate_setup();
    average_setup();
    counter_setup();
//    multipack_setup();
    multiselect_setup();
    oneshot_setup();
    invert_setup();
    strcat_setup();

	tripleLine_setup();
	randomF_setup();
	tripleRand_setup();
	vector_setup();
	hsv2rgb_setup();
	rgb2hsv_setup();

    reson_setup();
    abs_setup();
}

class markex_cppclass {
public:
	markex_cppclass(){MarkEx_setup();}
};
static markex_cppclass markex_instance;
