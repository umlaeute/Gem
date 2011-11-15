////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    Copyright (c) 2011 Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_colorclassify.h"
#include "Gem/PixConvert.h"

CPPEXTERN_NEW(pix_colorclassify);

//---------------------------------------------------------------------------
#include <assert.h>


#define num_colors 11

typedef enum {
 BLACK = 0,
 WHITE = 1,
 RED   = 2,
 GREEN = 3,
 BLUE  = 4,
 YELLOW = 5,
 BLUE_GREEN = 6,
 PURPLE_PINK_LAVENDER = 7,
 PURPLE_PINK_MAGENTA = 8,
 GRAY = 9,
 NONE} color;

typedef enum {
 UNRELIABLE = 0,
 GOOD_GUESS = 1,
 CERTAIN = 2} certainty;

static void color_classify (float red, float green, float blue, color *result, color *second_guess, certainty *sure);
static void print_color (color result, color second, certainty certainty_level);




/////////////////////////////////////////////////////////
//
// pix_colorclassify
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_colorclassify :: pix_colorclassify()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_colorclassify :: ~pix_colorclassify()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_colorclassify :: processRGBAImage(imageStruct &image)
{
  // post("processing RGBA Image");
  extern unsigned char class_red[], class_green[], class_blue[];
  unsigned i = image.xsize * image.ysize;

  unsigned char *base = image.data;
	while (i--) {
      color result, second_guess;
      certainty c;

      color_classify( base[chRed]/255., base[chGreen]/255., base[chBlue]/255.,
          &result, &second_guess, &c);

      // post("color %u, %u, %u -> result %i, sec %i, cert %i", base[chRed], base[chGreen], base[chBlue],
      //    result, second_guess, c);

      if (c == CERTAIN) {
        base[chRed] = class_red[result];
        base[chGreen] = class_green[result];
        base[chBlue] = class_blue[result];
      } else {
        base[chRed] = class_red[NONE];
        base[chGreen] = class_green[NONE];
        base[chBlue] = class_blue[NONE];
      }
      base += 4;
  }
  // post("done processing RGBA Image");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_colorclassify :: obj_setupCallback(t_class *classPtr)
{
}

//---------------------------------------------------------------------------

#include <stdio.h>

static void rgb2hsv(float r, float g, float b, float *h, float *s, float *v);

char *color_name[num_colors]  = {
  "black",
  "white",
  "red",
  "green",
  "blue",
  "yellow",
  "blue-green",
  "purple-pink-lavender",
  "purple-pink-magenta",
  "gray",
  "none"
};


char *certainty_name[3] =  {
  "unreliable",
  "good-guess",
  "certain"
};

// the rgb values for the pure representative of each class, mainly for
// visualization purposes
unsigned char class_red[] = {
0,/*black,*/
255,/*white,*/
255,/*red,*/
0,/*green,*/
0,/*blue,*/
255,/*yellow,*/
0,/*blue-green,*/
170,/*purple-pink-lavender,*/
255,/*purple-pink-magenta,*/
0.5*255,/*gray,*/
154,/*none ~ brownish for visualization only ~ skin color in the future*/
};

unsigned char class_green[] = {
0,/*black,*/
255,/*white,*/
0,/*red,*/
255,/*green,*/
0,/*blue,*/
255,/*yellow,*/
235,/*blue-green,*/
0,/*purple-pink-lavender,*/
0,/*purple-pink-magenta,*/
0.5*255,/*gray,*/
115/*none*/
};

unsigned char class_blue[] = {
0,/*black,*/
255,/*white,*/
0,/*red,*/
0,/*green,*/
255,/*blue,*/
0,/*yellow,*/
217,/*blue-green,*/
255,/*purple-pink-lavender,*/
255,/*purple-pink-magenta,*/
0.5*255,/*gray,*/
86/*none*/
};


void
color_classify (
  float red,
  float green,
  float blue,
  color *result,
  color *second_guess,
  certainty *certainty_level)
{
    float hue;
    float sat;
    float val;

    *certainty_level = CERTAIN;
    *result = NONE;
    *second_guess = NONE;

    rgb2hsv(red, green, blue, &hue, &sat, &val);
    hue *= 360;

    //fprintf(stderr, "%g, %g, %g\n", hue, sat, val);

    if (val < 0.3) {
        if (sat > 0.3 && val > 0.2) {
            *second_guess = BLACK;
            if (sat > 0.6 && (hue >= 65 && hue <= 170)) {
                *result = GREEN;
                return;
            } else if (sat > 0.8) {
                *certainty_level = UNRELIABLE;
            } else {
                *certainty_level = GOOD_GUESS;
            }
            // will proceed below to guessing colors
        } else {
            *result = BLACK;
            return;
        }
    } else if ((val > 0.8 && sat < 0.2) || (val > 0.7 && sat < 0.1) || (val > 0.6 && sat < 0.05)) {
        *result = WHITE;
        if (sat > 0.1) {
            // light baby colors; could be white with offset colorbalance.
            *certainty_level = UNRELIABLE;
            // will proceed below to guessing colors
            *result = NONE;
            *second_guess = WHITE;
        }
        else {
            if (val < 0.7) {
                *second_guess = GRAY;
                *certainty_level = GOOD_GUESS;
            }
            return;
        }
    }
    else if ((val < 0.65 && sat < 0.15) ||
        (val < 0.7 && sat < 0.1) ||
        (val < 0.5 && sat < 0.3 && (hue < 40 && hue > 10))) { // heuristica ~ marrons
        if (val < 0.50) {
            *result = BLACK;
            *certainty_level = GOOD_GUESS;
            if (val > 0.40)
                *second_guess = GRAY;
            return;
        }
        else {
            if (sat < 0.08) {
                *certainty_level = GOOD_GUESS;
                *result = WHITE;
                *second_guess = GRAY;
                return;
            }
            else {
                *certainty_level = UNRELIABLE;
                *second_guess = GRAY;
            }
        }
    }

    //RED
    if (hue < 30 || hue > 330) {
        if (hue > 10 && hue <= 30 && sat < 0.4) {
            if (hue > 20) {
                *result = YELLOW;
                *certainty_level = GOOD_GUESS;
                if (val < 0.7) {
                    *second_guess = GRAY;
                }
            }
            else {
                *result = RED;
                *certainty_level = UNRELIABLE;
                if (val < 0.7) {
                    *second_guess = GRAY;
                }
            }
        }
        else {
            *result = RED;
            if ((sat < 0.3 && val < 0.6) || (sat < 0.4 && val < 0.35)) {
                if (*certainty_level == CERTAIN) {
                    *certainty_level = GOOD_GUESS;
                }
            }
        }
    }
    // GREEN
    else if (hue > 80 && hue < 170) {
        *result = GREEN;
        if (sat < 0.2) {
            if (*certainty_level == CERTAIN) {
                *certainty_level = GOOD_GUESS;
            }
        }
        if (hue > 160 && (sat < 0.6 || val > 0.6)) {
            *certainty_level = UNRELIABLE;
            *second_guess = BLUE;
        }
    }
    // BLUE
    else if (hue > 185 && hue < 270) {
        if (sat < 0.3) {
            if (*certainty_level == CERTAIN) {
                *certainty_level = GOOD_GUESS;
            }
            if (val > 0.75) {
                *result = WHITE;
                *second_guess= BLUE;
                if (*certainty_level == CERTAIN) {
                    *certainty_level = GOOD_GUESS;
                }
            }
            else {
                *result = BLUE;
                if (val < 0.6) {
                    *second_guess = GRAY;
                }
                else {
                    *second_guess = WHITE;
                }
            }
        }
        else {
            *result = BLUE;
        }
    }
    // remaining YELLOW + OTHER cases
    else {
        if (sat < 0.5) {
            *certainty_level = UNRELIABLE;
        }
        // in the real system you just discard this estimate and use the previous
        // estimate (e.g. previous frame) at this point
        if (hue >= 30 && hue <= 80) {
            if (hue >= 70 && val <= 70) {
                *result = GREEN;
            }
            else {
                *result = YELLOW;
            }
        }
        else {
            if (*certainty_level == CERTAIN) {
                *certainty_level = GOOD_GUESS;
            }
            if (hue >= 150 && hue <= 185) {
                // hard test near cyan and put a secondary label.
                if (hue >= 180) {
                    *result = BLUE;
                }
                else if (hue < 170) {
                    *result = GREEN;
                    *second_guess = BLUE;
                }
                else {
                    *result = BLUE;
                    *second_guess = GREEN;
                }
            }
            else if (hue >= 270 && hue <= 330) {
                // hard test near magenta and put a secondary label.
                if (hue < 280) {
                    *result = BLUE;
                    if (*second_guess == NONE) {
                        *second_guess = PURPLE_PINK_LAVENDER;
                    }
                }
                else {
                    *result = RED;
                    if (*second_guess == NONE) {
                        *second_guess = PURPLE_PINK_MAGENTA;
                    }
                }
            }
        }
    }
    return;
}

// Code from AnImaL animal.sf.net
// 0 <= r, g, b, h, s, v <= 1
void
rgb2hsv(float r, float g, float b, float *h, float *s, float *v)
{
   float max, min, delta;

   if (r > g) {
      max = r;
      min = g;
   } else  {
      max = g;
      min = r;
   }

   if (max < b)
      max = b;
   else if (min > b)
      min = b;

   delta = max - min;

   *v = max;
   if (max != 0.0)
     *s = delta / max;
   else
     *s = 0.0;

   if (*s == 0.0) *h = -1;
   else {
     if (r == max)
       *h = (g - b) / delta;
     else if (g == max)
       *h = 2 + (b - r) / delta;
     else // if (b == max)
       *h = 4 + (r - g) / delta;
     *h *= 60.0;
     if (*h < 0) *h += 360.0;
     *h /= 360.0;
   }
}



void
print_color (color result, color second, certainty certainty_level)
{
  assert(result < num_colors);
  printf("%s ", color_name[result]);

  assert(second < num_colors);
  printf("%s ", color_name[second]);

  assert(certainty_level < 3);
  printf("%s\n", certainty_name[certainty_level]);
}



