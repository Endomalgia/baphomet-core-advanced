#ifndef BAPHOMET_UTIL_H
#define BAPHOMET_UTIL_H

/*
*	util.h
*	Created: 4-4-25
*	Description: Generally useful scripts, possibly will be depricated in the future.
*/

/* includes */
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* macros */

/* types */
typedef struct {
	char* name;
	long length;
	long offset;
	int type;
	int rp_index; // Index of the ring file the asset is contained within
} RIasset;

/* file and text management */
char* fileGetString(char* filepath);

#endif