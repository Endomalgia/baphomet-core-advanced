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

/* macros */

// user events
#define EVENT_CREATE      0
#define EVENT_STEP        1
#define EVENT_DRAW        2
#define EVENT_ERASE       3
#define EVENT_DESTROY     4

/* types */

typedef struct {
  int id;       // Assigned in pre-defined order
  void* oid;    // Pointer to self
  SDL_AppResult (*event_create)(void* object);
  SDL_AppResult (*event_step)(void* object, double delta_time);
  SDL_AppResult (*event_draw)(void* object, double delta_time);
  SDL_AppResult (*event_erase)(void* object);
  SDL_AppResult (*event_destroy)(void* object);
	void*  data;  
} Sin;

/* variables */

Sin** RING = NULL;
double int RING_LENGTH = 0;

/* debug */
Sin** ring_get();

/* ring management */
error_t ring_init();
error_t ring_resize(int num_sins);
error_t ring_refactor();
error_t ring_allocate() 


error_t ring_append_sin(Sin* sin);
error_t ring_replace_sin();
error_t ring_duplicate_sin();
error_t ring_remove_sin();
Sin* ring_get_sin_by_id(int id);
Sin* ring_get_sin(int index);

/* event execution */
error_t ring_execute_event(uint8_t event, uint8_t prio_flags);

/* sin (object) commands */
error_t ring_set_acheron(Sin* sin, uint8_t prio_flags);
uint8_t prio_flags sin_get_acheron(Sin* sin);
error_t sin_execute_event(Sin* sin, uint8_t event);      // Warning DO NOT PREMATURELY EXECUTE THE DELETE EVENT

#endif