/*
 * orbserial.h -- Machine/OS dependent serial I/O code for LibOrb
 *
 *  Copyright 1997 John E. Stone (j.stone@acm.org)
 *
 *  $Id: orbserial.h,v 1.1 2003-04-04 14:56:01 ggeiger Exp $
 *
 */

/*
 * Machine/OS dependent serial port I/O routines.
 *
 * orb_comm_open() - open the serial port device for communication with
 *                   the orb.  Settings are 9600,N,8,1, non-blocking,
 *                   no controlling tty.
 * orb_comm_read() - nonblocking read of up to size bytes
 * orb_comm_write() - blocking write of up to size bytes
 * orb_comm_close() - close the serial port device
 */

typedef void * OrbCommHandle;

int orb_comm_open(char * commname, OrbCommHandle * commhandleptr); 
int orb_comm_write(OrbCommHandle commhandle, char * buf); 
int orb_comm_read(OrbCommHandle commhandle, char * buf, int sz); 
int orb_comm_close(OrbCommHandle * commhandleptr); 

