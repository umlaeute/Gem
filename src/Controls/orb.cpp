/*
 * orb.c -- A simple driver for the SpaceOrb 360 spaceball device 
 *          manufactured by SpaceTec IMC (Nasdaq:SIMC).   
 *
 *  Copyright 1997 John E. Stone (j.stone@acm.org)
 *
 *  $Id: orb.cpp,v 1.1 2003-04-04 14:56:01 ggeiger Exp $
 *
 */

#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orb.h"
#include "orbserial.h"

typedef struct {
  OrbCommHandle commhandle;
  unsigned char buf[256];
  char resetstring[256]; 
  int bufpos;
  int packtype;
  int packlen;
  int trans[3];
  int rot[3];
  int buttons;
  int timer;
  int usenullregion;
  int nulltrans[3];
  int nullrot[3];
} orbhandle;

static const char xorme[21]="D.SpaceWare!        ";

OrbHandle orb_open(char * orbname) {
  orbhandle * handle;

  if (orbname == NULL)
    return NULL;

  handle = (orbhandle *) malloc(sizeof(orbhandle));
  if (handle == NULL)
    return NULL;

  /* clear all values in orbhandle to 0 */
  memset(handle, 0, sizeof(orbhandle));
  handle->packlen = 1;

  if (orb_comm_open(orbname, &handle->commhandle) == -1) {
    free(handle);
    return NULL;
  }

  return handle; /* successfull open */
}


int orb_close(OrbHandle voidhandle) {
  orbhandle * handle = (orbhandle*)voidhandle;

  if (handle == NULL)
    return -1;

  orb_comm_close(&handle->commhandle);
  free(handle); 
  return 0; /* successfull close */
}


static int orb_update(OrbHandle voidhandle) {
  int i, num, packs;
  unsigned char rawbuf[1024];
  orbhandle * handle = (orbhandle*)voidhandle;

  if (handle == NULL)
    return -1;
 
  packs = 0; /* no packs received yet */

  num = orb_comm_read(handle->commhandle, (char *) rawbuf, 1023);
 
  if (num > 0) {
    for (i=0; i<num; i++) {

      if ((rawbuf[i] == 0x0d) && (handle->packtype != 0x52)) {
        handle->bufpos = 0;   /* skip bytes */
        handle->packtype = 0; /* new packet */
        handle->packlen = 1;
        continue;
      }

      if (handle->bufpos == 0) {
        switch(rawbuf[i]) {
          case 0x11:  /* line feed? do nothing.. */
            handle->bufpos = 0;   /* skip bytes */
            handle->packtype = 0; /* new packet */
            handle->packlen = 1;
            break;

          case 0x21:  /* '!' response to '?' query */
            handle->bufpos = 0;   /* skip bytes */
            handle->packtype = 0x52; /* new packet */
            handle->packlen = 83;
            break;

          case 0x44:  /* 'D' Displacement packet */
            handle->packtype = 0x44; 
            handle->packlen = 12;    /* D packs are 12 bytes long */
            break;
  
          case 0x4b:  /* 'K' Button/Key packet */
            handle->packtype = 0x4B;
            handle->packlen = 5;     /* K packs are 5 bytes long */
            break;

          case 0x4E:  /* 'N' null region info response packet */
            handle->packtype = 0x4E;
            handle->packlen = 3;
            break;

          case 0x50: /* 'P' data rate info response packet */
            handle->packtype = 0x50; 
            handle->packlen = 4;    /* reset packs are long */
            break;

          case 0x52: /* reset packet? */
            handle->packtype = 0x52; 
            handle->packlen = 51;    /* reset packs are long */
            break;

  
          default:  /* Unknown packet! */
#ifdef DEBUG
            printf("Unknown: %2x \n ", rawbuf[i]); 
            printf("   char:          %c \n", rawbuf[i] & 0x7f); 
#endif
            break;
        }
      }

      if (handle->packtype == 0x44) { 
        handle->buf[handle->bufpos] = rawbuf[i] ^ xorme[handle->bufpos];
        handle->bufpos++;
      }
      else
        handle->buf[handle->bufpos++] = rawbuf[i];

      if (handle->bufpos != handle->packlen)
        continue;

      switch (handle->packtype) {
        case 0x44:  /* ball displacement event */
          {
            unsigned int tx, ty, tz, rx, ry, rz;

            tx = ((handle->buf[2] & 0x7F) << 3) | 
                 ((handle->buf[3] & 0x70) >> 4);
 
            ty = ((handle->buf[3] & 0x0F) << 6) | 
                 ((handle->buf[4] & 0x7E) >> 1);
 
            tz = ((handle->buf[4] & 0x01) << 9) | 
                 ((handle->buf[5] & 0x7F) << 2) |
                 ((handle->buf[6] & 0x60) >> 5);
 
            rx = ((handle->buf[6] & 0x1F) << 5) | 
                 ((handle->buf[7] & 0x7C) >> 2);
 
            ry = ((handle->buf[7] & 0x03) << 8) | 
                 ((handle->buf[8] & 0x7F) << 1) |
                 ((handle->buf[9] & 0x40) >> 6);
 
            rz = ((handle->buf[9]  & 0x3F) << 4) | 
                 ((handle->buf[10] & 0x78) >> 3);
 
            handle->timer = ((handle->buf[10] & 0x07) << 7) | 
                             (handle->buf[11] & 0x7F);
 
            handle->trans[0] = (((int) tx) << 22) >> 22; 
            handle->trans[1] = (((int) ty) << 22) >> 22;
            handle->trans[2] = (((int) tz) << 22) >> 22;
            handle->rot[0] = (((int) rx) << 22) >> 22; 
            handle->rot[1] = (((int) ry) << 22) >> 22;
            handle->rot[2] = (((int) rz) << 22) >> 22;
          }
          break;
 
        case 0x4B:
          handle->buttons = handle->buf[2] & 0x7F;
          handle->timer = ((handle->buf[1] & 0x7f) << 8) | 
                           (handle->buf[4] & 0x7f);
          break;

        case 0x4E:
#ifdef DEBUG
          printf("received response to null region query\n");
          { int lv;
            for (lv=0; lv<handle->bufpos; lv++) {
              printf(" %2x ", handle->buf[lv]);
            }
            printf("\n");
          }
#endif
          break;

        case 0x50: /* response to data rate query */
#ifdef DEBUG
          printf("received response to data rate query\n");
          { int lv;
            for (lv=0; lv<handle->bufpos; lv++) {
              printf(" %2x ", handle->buf[lv]);
            }
            printf("\n");
          }
#endif
          break;

        case 0x52: /* reset pack */
          { int lv;
            memset(&handle->resetstring, 0, sizeof(handle->resetstring));
            for (lv=0; lv<handle->bufpos; lv++) 
              handle->resetstring[lv] = handle->buf[lv] & 0x7f;
#ifdef DEBUG
            printf("\n%s\n", handle->resetstring);
            printf("\n\n");
#endif
          } 


          break;


        default: 
#ifdef DEBUG
            printf("Unkpack: %2x \n ", handle->packtype ); 
            printf("   char:          %c \n",  handle->packtype & 0x7f); 
#endif
          break;
      }   
        
      /* reset */ 
      handle->bufpos = 0;   
      handle->packtype = 0; 
      handle->packlen = 1;
      packs++;
    }
  }

  return packs;
}


int orb_rezero(OrbHandle voidhandle) {
  orbhandle * handle = (orbhandle*)voidhandle;
  char outbuf[200];

  if (handle == NULL)
    return -1;

  sprintf(outbuf, "\rZ\r");
  orb_comm_write(handle->commhandle, outbuf);

  return 0;
}

int orb_init(OrbHandle voidhandle) {
  orbhandle * handle = (orbhandle*)voidhandle;
  char outbuf[200];
 
  if (handle == NULL)
    return -1;
 
  orb_update(handle);

  sprintf(outbuf, "\r");
  orb_comm_write(handle->commhandle, outbuf);
  orb_update(handle);

  orb_rezero(handle);

  return 0;
}


void orb_set_nullregion(OrbHandle voidhandle, 
	int nulltx, int nullty, int nulltz, 
	int nullrx, int nullry, int nullrz) {
  orbhandle * handle = (orbhandle*)voidhandle;

  handle->nulltrans[0] = abs(nulltx);
  handle->nulltrans[1] = abs(nullty);
  handle->nulltrans[2] = abs(nulltz);

  handle->nullrot[0] = abs(nullrx);
  handle->nullrot[1] = abs(nullry);
  handle->nullrot[2] = abs(nullrz);
 
  handle->usenullregion = 1;
}


static int nullregion(int null, int val) {
  if (abs(val) > null) {
    return ((val > 0) ? (val - null) : (val + null));
  }
  return 0;
}

static void orb_do_nullregion(OrbHandle voidhandle) {
  orbhandle * handle = (orbhandle*)voidhandle;
 
  handle->trans[0] = nullregion(handle->nulltrans[0], handle->trans[0]);  
  handle->trans[1] = nullregion(handle->nulltrans[1], handle->trans[1]);  
  handle->trans[2] = nullregion(handle->nulltrans[2], handle->trans[2]);  
  handle->rot[0] = nullregion(handle->nullrot[0], handle->rot[0]);  
  handle->rot[1] = nullregion(handle->nullrot[1], handle->rot[1]);  
  handle->rot[2] = nullregion(handle->nullrot[2], handle->rot[2]);  
}

int orb_getstatus(OrbHandle voidhandle, int * tx, int * ty, int * tz,
                 int * rx, int * ry, int * rz, int * buttons) {
  orbhandle * handle = (orbhandle*)voidhandle;
  int events;

  if (handle == NULL)
    return -1;
   
  events = orb_update(handle); /* check for new data */

  /* perform null region processing */
  if (handle->usenullregion) 
    orb_do_nullregion(voidhandle);
  
  if (tx != NULL)
    *tx = handle->trans[0]; 
  if (ty != NULL)
    *ty = handle->trans[1]; 
  if (tz != NULL)
    *tz = handle->trans[2]; 
 
  if (rx != NULL)
    *rx = handle->rot[0]; 
  if (ry != NULL)
    *ry = handle->rot[1]; 
  if (rz != NULL)
    *rz = handle->rot[2]; 

  if (buttons != NULL)
    *buttons = handle->buttons;

  /* no timer code yet */
 
  return events; 
}





