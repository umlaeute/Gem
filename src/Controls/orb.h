/*
 * orb.h -- The user accessible API for liborb.  
 *
 *  Copyright 1997 John E. Stone (j.stone@acm.org)
 *
 *  $Id: orb.h,v 1.1 2003-04-04 14:56:01 ggeiger Exp $
 */

#ifdef  __cplusplus
extern "C" {
#endif

typedef void * OrbHandle; /* Orb Handle type, used by all orb API functions */

/* SpaceOrb Button bit-masks */
#define ORB_BUTTON_A      1
#define ORB_BUTTON_B      2
#define ORB_BUTTON_C      4
#define ORB_BUTTON_D      8
#define ORB_BUTTON_E      16
#define ORB_BUTTON_F      32
#define ORB_BUTTON_RESET  64

/* 
 * orb_open() 
 *   Open a named serial port which a SpaceOrb 360 is attached to.
 * Returns a handle which is used by all other orb API functions.
 * If the serial port open fails, or the orb does not pass initialization
 * tests, then a NULL is returned as the handle.   
 */
OrbHandle orb_open(char * orbname); 

/*
 * orb_close()
 *   Closes down the SpaceOrb serial port, frees allocated resources and
 * discards any unprocessed orb messages.
 */ 
int orb_close(OrbHandle voidhandle);

/*
 * orb_getstatus()
 *   Polls the SpaceOrb serial port for new packets, performs any optional
 * postprocessing of SpaceOrb data such as null-region, scaling, and
 * value clamping.  The most recent values for translation, rotation and
 * buttons are stored in the memory locations supplied by the caller.
 * Returns the number of events processed.  If the number of events returned
 * is less than 1, either an error occured or there were no SpaceOrb
 * events to process.
 */
int orb_getstatus(OrbHandle voidhandle, 
                 int * tx, int * ty, int * tz, 
                 int * rx, int * ry, int * rz, int * buttons);

/*
 * orb_rezero()
 *   Forces the Orb to re-zero itself at the present twist/position.
 * All future event data is relative to this zero point.
 */
int orb_rezero(OrbHandle voidhandle);

/* 
 * orb_init()
 *   Performs a software re-initialization of the SpaceOrb, clearing
 * all unprocessed events.  Initialization also forces the Orb to re-zero 
 * itself.
 */
int orb_init(OrbHandle voidhandle);

/* 
 * orb_set_nullregion()
 *  Enables null-region processing on SpaceOrb output.
 * The null-region is the area (centered at 0) around which
 * each coordinate will report zero even when the SpaceOrb itself
 * reports a number whose absolute value is less than the null region
 * value for that coordinate.  For example, if the null region on the
 * X translation coordinate is set to 50, all orb_getstatus() would report
 * 0 if X is less than 50 and greater than -50.  If X is 51, orb_getstatus
 * would report 1.  If X is -51, orb_getstatus() would report -1.  
 * Null-regions help novice users gradually become accustomed to the 
 * incredible sensitivity of the SpaceOrb, and make some applications 
 * significantly easier to control.  A resonable default nullregion for all
 * six axes is 65.  Null regions should be tunable by the user, since its
 * likely that not all SpaceOrbs are quite identical, and it is guaranteed
 * that users have varying levels of manual dexterity.
 * Note that setting the null-region too high significantly reduces the
 * dynamic range of the output values from the SpaceOrb.
 */
void orb_set_nullregion(OrbHandle voidhandle,
        int nulltx, int nullty, int nulltz,
        int nullrx, int nullry, int nullrz);


#ifdef  __cplusplus
}
#endif


