#ifndef BAPHOMET_AUD_H
#define BAPHOMET_AUD_H

/*
*	aud.h
*	Created: 30-4-25
*	Description: Generally useful scripts, possibly will be depricated in the future.
*/

/* includes */
#include <portaudio.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

/* macros */
#define SND_CALLBACK_SNDFILE &_DEFAULT_CALLBACK_LIBSNDFILE

/* types */
typedef struct {
	int					rate;
	void*				user_data;
	SF_INFO			soundfile_info;
	PaStreamCallback*	callback;
	PaStream*			stream;
} AUDsfx;

/* portaudio stuff */
PaStreamParameters* audGetInputParams();
PaStreamParameters* audGetOutputParams();
void audSetInputParams(PaStreamParameters* in);
void audSetOutputParams(PaStreamParameters* out);
void audSetInputDevice(int dev_index);
void audSetOutputDevice(int dev_index);
char** audGetDeviceList();	// Free this after using, theres a malloc here

AUDsfx* audSpecialLoadSfx(char* filepath);

/* audio loading */
AUDsfx* audLoadSfx(char* filepath);
void audUnloadSfx(AUDsfx* sfx);

/* playback management */
void audSoundPlay(AUDsfx* sfx, int loop);
void audSoundPause(AUDsfx* sfx);

static int _DEFAULT_CALLBACK_LIBSNDFILE(const void *inputBuffer, 
										void *outputBuffer, 
										unsigned long framesPerBuffer, 
										const PaStreamCallbackTimeInfo* timeInfo, 
										PaStreamCallbackFlags statusFlags, 
										void *userData);

#endif