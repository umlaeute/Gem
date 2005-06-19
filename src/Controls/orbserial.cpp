/*
 * orbserial.c -- Machine/OS dependent serial I/O code for LibOrb
 *
 *  Copyright 1997 (unix code)  John E. Stone (j.stone@acm.org)
 *                 (Win32 code) Mike Connell  (M.Connell@swansea.ac.uk)  
 *
 *  $Id: orbserial.cpp,v 1.3 2005-06-19 18:45:39 zmoelnig Exp $
 *
 */


#if !defined(__WIN32__) && !defined(_MSC_VER)

/*
 * Unix serial I/O code by John Stone
 */

#define _POSIX_SOURCE 1

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "orbserial.h" /* protos and types for this file */

typedef struct {
  int fd;        /* serial port device file descriptor */
} commstruct;

int orb_comm_open(char * commname, OrbCommHandle * commhandleptr) {
  struct termios orbtermio;
  commstruct * comm;

  *commhandleptr = NULL;

  comm = (commstruct*) malloc(sizeof(commstruct));
  if (comm == NULL)
    return -1;

  comm->fd = open(commname, O_RDWR | O_NONBLOCK | O_NOCTTY);

  if (comm->fd == -1) {
    free(comm);
    return -1;  /* failed open of comm port */
  }
  tcgetattr(comm->fd, &orbtermio);

  orbtermio.c_lflag = 0;
  orbtermio.c_lflag = 0;
  orbtermio.c_iflag = IGNBRK | IGNPAR;
  orbtermio.c_oflag = 0;
  orbtermio.c_cflag = CREAD | CS8 | CLOCAL | HUPCL;
  orbtermio.c_cc[VEOL] = '\r';
  orbtermio.c_cc[VERASE] = 0;
  orbtermio.c_cc[VKILL] = 0;
  orbtermio.c_cc[VMIN] = 0;
  orbtermio.c_cc[VTIME] = 0;

  /* use of baud rate in cflag is deprecated according to the */
  /* single unix spec, also doesn't work in IRIX > 6.2        */
  cfsetispeed(&orbtermio, B9600);
  cfsetospeed(&orbtermio, B9600);

  tcsetattr(comm->fd, TCSAFLUSH, &orbtermio);

  *commhandleptr = (OrbCommHandle) comm;
 
  return 0;
}

int orb_comm_write(OrbCommHandle commhandle, char * buf) {
  commstruct * comm = (commstruct *) commhandle;

  if (comm == NULL) 
    return -1;

  return write(comm->fd, buf, strlen(buf));
}

int orb_comm_read(OrbCommHandle commhandle, char * buf, int sz) {
  commstruct * comm = (commstruct *) commhandle;

  if (comm == NULL) 
    return -1;

  return read(comm->fd, buf, sz);
}

int orb_comm_close(OrbCommHandle * commhandleptr) {
  commstruct * comm = (commstruct *) (*commhandleptr);

  if (comm == NULL)
    return -1;

  close(comm->fd);

  free(*commhandleptr);
  *commhandleptr = NULL;

  return 0;
}

/* end of unix code */

#else

/* 
 * Win32 serial code by Mike Connell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "orbserial.h" /* protos and types for this file */
#include "serial.h"    /* Win32 specific serial stuff */

typedef T_serial commstruct; /* all the serial port data */

int orb_comm_open(char * commname, OrbCommHandle * commhandleptr) {
  commstruct * serial;
  int port;

  *commhandleptr = NULL;

  serial = (commstruct *)malloc(sizeof(commstruct));
  if (serial == NULL)
    return -1;

  /* translate com port name to appropriate integer */
  if (sscanf(commname,"%d",&port)!=1) {
    free(serial);
    return -1; /* print some error message here if desired */
  }

  if (F_spaceorb_serial_start(serial, port, 9600, 1, FALSE, "SpaceOrb")) {
    *commhandleptr = serial;
    return 0;
  }
  else {
    free(serial);
    return -1;
  }
}

int orb_comm_write(OrbCommHandle commhandle, char * buf) {
  commstruct * serial = (commstruct *) commhandle;
  int len;

  if (serial == NULL) 
    return -1;

  len=strlen(buf);

  if (F_spaceorb_serial_write(serial,(unsigned char*)buf,len))
    return len;
  else
    return 0;
}

int orb_comm_read(OrbCommHandle commhandle, char * buf, int sz) {
  commstruct * serial = (commstruct *) commhandle;

  if (serial == NULL) 
    return -1;

  return F_spaceorb_serial_read(serial,(unsigned char*)buf,sz);
}

int orb_comm_close(OrbCommHandle * commhandleptr) {
  commstruct * serial = (commstruct *) *commhandleptr;

  if (serial == NULL)
    return -1;

  F_spaceorb_serial_close_connection(serial);

  free(*commhandleptr);
  *commhandleptr = NULL;
 
  return 0;
}

/* end of Win32 code */

#endif 


