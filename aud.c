#include "aud.h"

PaStreamParameters* Param_IN	= NULL;
PaStreamParameters* Param_OUT	= NULL;

PaStreamParameters* audGetInputParams() {
	return Param_IN;
}

PaStreamParameters* audGetOutputParams() {
	return Param_OUT;
}

void audSetInputParams(PaStreamParameters* in) {
	Param_IN = in;
}

void audSetOutputParams(PaStreamParameters* out) {
	Param_OUT = out;
}

void audSetInputDevice(int dev_index) {
	const PaDeviceInfo* dev_info = Pa_GetDeviceInfo(dev_index);
	Param_IN->suggestedLatency = dev_info->defaultHighInputLatency;
	Param_IN->channelCount = dev_info->maxInputChannels;
	Param_IN->device = dev_index;
}

/* Should error if no devices exist lmao */
void audSetOutputDevice(int dev_index) {
	const PaDeviceInfo* dev_info = Pa_GetDeviceInfo(dev_index);
	Param_OUT->suggestedLatency = dev_info->defaultHighOutputLatency;
	Param_OUT->channelCount = dev_info->maxOutputChannels;
	Param_OUT->device = dev_index;
}

char** audGetDeviceList() {
	int dev_count = Pa_GetDeviceCount();
	char** dev_list = malloc(sizeof(char*) * (dev_count + 1));
	for (int i=0; i<dev_count; i++) {
		const PaDeviceInfo* dev_info = Pa_GetDeviceInfo(i);
		dev_list[i] = (char*)dev_info->name;
	}
	dev_list[dev_count] = NULL;
	return dev_list;
}

AUDsfx* audLoadSfx(char* filepath) {
	AUDsfx* sfx;
	SNDFILE* soundfile;
	PaStreamParameters out_param;

	sfx = malloc(sizeof(AUDsfx));

	// This works with no issues....
	int fd = open(filepath, O_RDONLY, S_IRUSR);
	soundfile = sf_open_fd(fd, SFM_READ, &(sfx->soundfile_info), SF_TRUE);
	//soundfile = sf_open(filepath, SFM_READ, &(sfx->soundfile_info));
	if (soundfile == NULL)
		THROW("An error occured when opening file %s", filepath);

	sfx->user_data = (void*)soundfile;
	sfx->rate = sfx->soundfile_info.samplerate;
	if (Param_OUT == NULL) {
		WARN("Opening audiostream without setting output parameters. Setting to device index 0");
		audSetOutputDevice(0);
	}
	Param_OUT->channelCount = sfx->soundfile_info.channels;

	// Default frame length (4096) is here
	PaError err = Pa_OpenStream(&(sfx->stream), NULL, Param_OUT, sfx->rate, 4096, paClipOff, _DEFAULT_CALLBACK_LIBSNDFILE, sfx->user_data);
	if (err != paNoError)
		THROW("Unable to open audio stream from file %s\n\t%s",filepath,Pa_GetErrorText(err))
	return sfx;
}

void audUnloadSfx(AUDsfx* sfx) {
	free(sfx); // Should do more than this obvi
}

void audSoundPlay(AUDsfx* sfx, int loop) {
	PaError err;
	err = Pa_StartStream(sfx->stream);
	if (err != paNoError) 
		THROW("Unable to play audio\n\t%s", Pa_GetErrorText(err));
}

void audSoundPause(AUDsfx* sfx) {
	PaError err;
	err = Pa_StopStream(sfx->stream);
		THROW("Unable to pause audio\n\t%s", Pa_GetErrorText(err));
}

static int _DEFAULT_CALLBACK_LIBSNDFILE(const void *inputBuffer, 
										void *outputBuffer, 
										unsigned long framesPerBuffer, 
										const PaStreamCallbackTimeInfo* timeInfo, 
										PaStreamCallbackFlags statusFlags, 
										void *userData) {
	float *out = (float*)outputBuffer;
	sf_readf_float((SNDFILE*)userData, out, framesPerBuffer);
	return paContinue;
}

/* Good for debugging in future
// Print all audio devices
	char** log = audGetDeviceList();
	fprintf(stderr, "Devices : \n");
	for(int i=0;log[i]!=NULL;i++) {
		fprintf(stderr, "\t (%u) %s\n", (unsigned int)i, log[i]);
	}
	free(log);
*/