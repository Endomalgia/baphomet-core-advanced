#ifndef BAPHOMET_RING_H
#define BAPHOMET_RING_H

/*
*	ring.h
*	Created: a really long time ago
*	Description: "We don't want to reinvent the wheel" Hehe :3c
*/

/* includes */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* macros */
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
Sin** ringGetDebug(); // Probably shouldnt be used

/* ring management */
void ringInit();    // Only exists for convenience CHANGE
void ringBufferElement(int id, int ptr_widths);  // For hardcoding bs
void ringCleanup();

/* sin manipulation */
void ringAppendSins(Sin** sins, int num);
void ringReplaceSin();
void ringDuplicateSins();
void ringPurgeSin(int id);
Sin* ringGetSinByID(int id);

/* event execution */
void ringExecuteEvent(uint8_t event);

/* sin (object) commands */
//error_t RingSetAcheron(Sin* sin, int prio_flags);
//uint8_t prio_flags SinGetAcheron(Sin* sin);
void ringExecuteEventSin(Sin* sin, uint8_t event);      // Warning DO NOT PREMATURELY EXECUTE THE DELETE EVENT

#endif
