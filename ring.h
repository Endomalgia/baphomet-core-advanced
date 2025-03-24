#ifndef BAPHOMET_RING_H
#define BAPHOMET_RING_H

/*
*	ring.h
*	Created: xxxx-xx-xx
*	Description:
*		ring.h is a loose implimentation of a somewhat hodgepodge system for
*   executing 'sin' scripts in order. The idea is that every available object
*   will be created and listed in a large include file which "room sins"
*   directly running out of the mainloop can pull from to make interesting
*   things happen :3. I think it may be a good idea to have a python
*   preprocessor make this happen? I could write it all manually by defining
*   name_event_xxx functions with an object definition at the end of the file
*   all included in said larger include; However, that is like atleast 5 seconds
*   of work and like... come on.. thats unacceptable. NOTE: Global variables
*   are shared among object instances. Only the 'malloc'ed data is local
*/

#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* macros */

// user events
#define EVENT_CREATE      0
#define EVENT_STEP        1
#define EVENT_DRAW        2
#define EVENT_ERASE       3
#define EVENT_DESTROY     4

/* types */
typedef struct {
  int           id;     // Assigned in pre-defined order
  void*         oid;    // Pointer to self
  int (*event_script)(void* sin, uint8_t event_flag);
	void*         data;
} Sin;

/* debug */
Sin** DebugRingGet(); // Probably shouldnt be used

/* ring management */
int RingInit();    // Only exists for convenience CHANGE
int RingBufferElement(int id, int ptr_widths);  // For hardcoding bs
int RingCleanup();

/* sin manipulation */
int RingAppendSins(Sin** sins, int num);
int RingReplaceSin();
int RingDuplicateSins();
int RingPurgeSin();
Sin* RingGetSinByID(int id);

/* event execution */
int RingExecuteEvent(uint8_t event);

/* sin (object) commands */
//error_t RingSetAcheron(Sin* sin, int prio_flags);
//uint8_t prio_flags SinGetAcheron(Sin* sin);
int SinExecuteEvent(Sin* sin, uint8_t event);      // Warning DO NOT PREMATURELY EXECUTE THE DELETE EVENT

#endif
