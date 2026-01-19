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
#ifndef THROW
#define THROW(...) do { \
	fprintf(stderr, "[E]\t In %s:%d : ", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "\n"); \
	exit(0); \
} while (0);
#endif

#ifndef WARN
#define WARN(...) do { \
	fprintf(stderr, "[W]\t In %s:%d : ", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "\n"); \
} while (0);
#endif

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