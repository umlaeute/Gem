////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "m_pd.h"
#include "GemPixUtil.h"

#include <string.h>

GEM_EXTERN void imageStruct::copy2ImageStruct(imageStruct *to){
    if (!to || !this || !this->data) {
      error("GEM: Someone sent a bogus pointer to copy2Image");
      if (to) to->data = NULL;
      return;
    }

    // copy the information over
    to->xsize 	= xsize;
    to->ysize 	= ysize;
    to->csize 	= csize;
    to->format 	= format;
    to->type 	= type;
    to->data    = data;
    to->pdata   = pdata;
    to->datasize= datasize;
}
GEM_EXTERN void imageStruct::info() {
  post("imageStruct\t:%dx%dx%d\n\t\t%X\t(%x) %d\n\t\t%x\t%x\t%d",
       xsize, ysize, csize,
       data, pdata, datasize,
       format, type, notowned);
}

GEM_EXTERN void imageStruct::copy2Image(imageStruct *to)
{
    if (!to || !this || !this->data)
    {
        error("GEM: Someone sent a bogus pointer to copy2Image");
        if (to)
			to->data = NULL;
        return;
    }

    /* copy without new allocation if possible (speedup in convolve ..) */

    if (to->xsize*to->ysize*to->csize < xsize*ysize*csize || !to->data) {
      to->clear();
      to->allocate(xsize * ysize * csize);
    }
      
    // copy the information over

    to->xsize 	= xsize;
    to->ysize 	= ysize;
    to->csize 	= csize;
    to->format 	= format;
    to->type 	= type;
    to->datasize= to->xsize * to->ysize * to->csize;
    
    // copy the data over
    memcpy(to->data, data, to->datasize);
}

GEM_EXTERN void imageStruct::refreshImage(imageStruct *to)
{
    if (!to || !data)
    {
        error("GEM: Someone sent a bogus pointer to refreshImage");
        return;
    }

    // check if we need to reallocate memory
    if (to->xsize != xsize ||
        to->ysize != ysize ||
        to->csize != csize ||
		!to->data)
    {
        to->clear();
        copy2Image(to);
    }
    else
        // copy the data over
        memcpy(to->data, this->data, to->xsize * to->ysize * to->csize);
}

#ifndef MACOSX
int detect_mmx( void )
#if !HAVE_MMX
{
  return 0;
}
#else
{
  int mmx_bit = 1;
#if 0
  asm( "mov    %2, %%eax     \n\t"     // request feature flag
       "cpuid                \n\t"     // get CPU ID flag
       "and    %1, %%edx     \n\t"     // check MMX bit (bit 23)
       "mov    %%edx, %0     \n\t"     // move result to mmx_bit

       :       "=m" (mmx_bit)          // %0

       :       "i"  (0x00000001),      // %1
               "i"  (0x00800000)       // %2
     );
#endif
  return mmx_bit;
}

#endif


//  ConvolveKernel3x3Divide: Dij = saturation0and255( ... )
int pix_convolve3x3mmx(unsigned char *Src, unsigned char *Dest, int rows, int columns,
                      signed short *Kernel, unsigned char Divisor)
#if !HAVE_MMX
{
  return 0;
}
#else
{
  if ((columns < 3) || (rows < 3) || (Divisor == 0)) return 0;

  asm volatile
      (
       "push %%eax \n\t"
       "push %%ebx \n\t"
       "push %%ecx \n\t"
       "push %%edx \n\t"
       "push %%esi \n\t"

        "pxor      %%mm0, %%mm0 \n\t"    // zero MM0
        "xor       %%ebx, %%ebx \n\t"    // zero EBX
        "mov           %5, %%bl \n\t"    // load Divisor into BL

        "mov          %4, %%edx \n\t"    // load Kernel address into EDX
        "movq    (%%edx), %%mm5 \n\t"    // MM5 = {0,K2,K1,K0}
        "add          $8, %%edx \n\t"    // second row              |K0 K1 K2 0|
        "movq    (%%edx), %%mm6 \n\t"    // MM6 = {0,K5,K4,K3}  K = |K3 K4 K5 0|
        "add          $8, %%edx \n\t"    // third row               |K6 K7 K8 0|
        "movq    (%%edx), %%mm7 \n\t"    // MM7 = {0,K8,K7,K6}
// ---
        "mov          %3, %%eax \n\t"    // load columns into EAX
        "mov          %1, %%esi \n\t"    // ESI = Src row 0 address
        "mov          %0, %%edi \n\t"    // load Dest address to EDI
        "add       %%eax, %%edi \n\t"    // EDI = EDI + columns
        "inc              %%edi \n\t"    // 1 byte offset from the left edge

        "mov          %2, %%edx \n\t"    // initialize ROWS counter
        "sub          $2, %%edx \n\t"    // do not use first and last row
// ---
        ".L10320:               \n\t"

        "mov       %%eax, %%ecx \n\t"    // initialize COLUMS counter
        "sub          $2, %%ecx \n\t"    // do not use first and last column

        ".align 16              \n\t"    // 16 byte allignment of the loop entry
        ".L10322:               \n\t"
// ---
        "movq    (%%esi), %%mm1 \n\t"    // load 8 bytes of the image first row
        "add       %%eax, %%esi \n\t"    // move one row below
        "movq    (%%esi), %%mm2 \n\t"    // load 8 bytes of the image second row
        "add       %%eax, %%esi \n\t"    // move one row below
        "movq    (%%esi), %%mm3 \n\t"    // load 8 bytes of the image third row

        "punpcklbw %%mm0, %%mm1 \n\t"    // unpack first 4 bytes into words
        "punpcklbw %%mm0, %%mm2 \n\t"    // unpack first 4 bytes into words
        "punpcklbw %%mm0, %%mm3 \n\t"    // unpack first 4 bytes into words

        "pmullw    %%mm5, %%mm1 \n\t"    // multiply words first row  image*Kernel
        "pmullw    %%mm6, %%mm2 \n\t"    // multiply words second row image*Kernel
        "pmullw    %%mm7, %%mm3 \n\t"    // multiply words third row  image*Kernel

        "paddsw    %%mm2, %%mm1 \n\t"    // add 4 words of the first and second rows
        "paddsw    %%mm3, %%mm1 \n\t"    // add 4 words of the third row and result
        "movq      %%mm1, %%mm2 \n\t"    // copy MM1 into MM2
        "psrlq       $32, %%mm1 \n\t"    // shift 2 left words to the right
        "paddsw    %%mm2, %%mm1 \n\t"    // add 2 left and 2 right result words
        "movq      %%mm1, %%mm3 \n\t"    // copy MM1 into MM3
        "psrlq       $16, %%mm1 \n\t"    // shift 1 left word to the right
        "paddsw    %%mm3, %%mm1 \n\t"    // add 1 left and 1 right result words
// --
        "movd      %%eax, %%mm2 \n\t"    // save EAX in MM2
        "movd      %%edx, %%mm3 \n\t"    // save EDX in MM3
        "movd      %%mm1, %%eax \n\t"    // copy MM1 into EAX
        "psraw       $15, %%mm1 \n\t"    // spread sign bit of the result
        "movd      %%mm1, %%edx \n\t"    // fill EDX with a sign bit
        "idivw             %%bx \n\t"    // IDIV - VERY EXPENSIVE
        "movd      %%eax, %%mm1 \n\t"    // move result of division into MM1
        "packuswb  %%mm0, %%mm1 \n\t"    // pack division result with saturation
        "movd      %%mm1, %%eax \n\t"    // copy saturated result into EAX
        "mov      %%al, (%%edi) \n\t"    // copy a byte result into Dest
        "movd      %%mm3, %%edx \n\t"    // restore saved EDX
        "movd      %%mm2, %%eax \n\t"    // restore saved EAX
// --
        "sub       %%eax, %%esi \n\t"    // move two rows up
        "sub       %%eax, %%esi \n\t"    //
        "inc              %%esi \n\t"    // move Src  pointer to the next pixel
        "inc              %%edi \n\t"    // move Dest pointer to the next pixel
// ---
        "dec              %%ecx \n\t"    // decrease loop counter COLUMNS
        "jnz            .L10322 \n\t"    // check loop termination, proceed if required

        "add          $2, %%esi \n\t"    // move to the next row in Src
        "add          $2, %%edi \n\t"    // move to the next row in Dest

        "dec              %%edx \n\t"    // decrease loop counter ROWS
        "jnz            .L10320 \n\t"    // check loop termination, proceed if required
// ---
        "emms                   \n\t"    // exit MMX state

       "push %%esi \n\t"
       "push %%edx \n\t"
       "push %%ecx \n\t"
       "push %%ebx \n\t"
       "push %%eax \n\t"
       :
       : "m"  (Dest),                    // %0
           "m"  (Src),                   // %1
           "m"  (rows),                  // %2
           "m"  (columns),               // %3
           "m"  (Kernel),                // %4
           "m"  (Divisor)                // %5
      );
  return 1;
}
#endif




#if 0
int pix_addmean(unsigned char Dest, unsigned char *Src, int length)
{
  static unsigned char Mask[8] = { 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F };

  if (length < 8) return 0;              // image size must be at least 8 bytes 
  asm volatile
      (
        "movq    (%3), %%mm0 \n\t"    // load Mask into mm0
        "shr          $3, %2 \n\t"    // counter/8 (MMX loads 8 bytes at a time)

        ".align 16              \n\t"    // 16 byte allignment of the loop entry
        ".L1011:                \n\t"

        "movq    (%0), %%mm1 \n\t"    // load 8 bytes from Src1 into mm1
        "movq    (%1), %%mm2 \n\t"    // load 8 bytes from Src2 into mm2

                                         // --- Byte shift via Word shift ---

        "psrlw        $1, %%mm1 \n\t"    // shift 4 WORDS of mm1 1 bit to the right
        "psrlw        $1, %%mm2 \n\t"    // shift 4 WORDS of mm2 1 bit to the right

//      "pand      %%mm0, %%mm1 \n\t"    // apply Mask to 8 BYTES of mm1
        ".byte     0x0f, 0xdb, 0xc8 \n\t"

//      "pand      %%mm0, %%mm2 \n\t"    // apply Mask to 8 BYTES of mm2
        ".byte     0x0f, 0xdb, 0xd0 \n\t"

        "paddusb   %%mm2, %%mm1 \n\t"    // mm1=mm1+mm2 (add 8 bytes with saturation)
        "movq    %%mm1, (%0) \n\t"    // store result in Dest

        "add          $8, %0 \n\t"    // increase Src1, Src2 and Dest 
        "add          $8, %1 \n\t"    // register pointers by 8

        "dec              %2 \n\t"    // decrease loop counter
        "jnz             .L1011 \n\t"    // check loop termination, proceed if required

        "emms                   \n\t"    // exit MMX state

        : :"p"  (Dest),                   // %0
           "p"  (Src),                  // %1
           "r"  (length),                // %2
           "p"  (Mask)                   // %3
      );

  return 1;
}
#endif
#endif // MACOSX


int pix_addsat(unsigned char *leftPix, unsigned char *rightPix, int datasize)
{
  while(datasize--)
    {           
      *leftPix = CLAMP_HIGH((int)*leftPix + (int)*rightPix);
      leftPix++;
      rightPix++;
    }
}


int pix_sub(unsigned char *leftPix, unsigned char *rightPix, int datasize)
{
  while(datasize--)
    {           
      *leftPix = CLAMP_LOW((int)*leftPix++ - (int)*rightPix++);
    }
}
