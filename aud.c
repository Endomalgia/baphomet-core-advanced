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
		fprintf(stderr, "An error occured when opening: %s\n", filepath);

	sfx->user_data = (void*)soundfile;
	sfx->rate = sfx->soundfile_info.samplerate;
	Param_OUT->channelCount = sfx->soundfile_info.channels;

	// Default frame length (4096) is here
	PaError err = Pa_OpenStream(&(sfx->stream), NULL, Param_OUT, sfx->rate, 4096, paClipOff, _DEFAULT_CALLBACK_LIBSNDFILE, sfx->user_data);
	if (err != paNoError)
		fprintf(stderr, "audio_create_callback(): Unable to open stream -> %s\n", Pa_GetErrorText(err));
	return sfx;
}


AUDsfx* audSpecialLoadSfx(char* filepath) {
	SNDFILE* 			sndfile;
	SF_INFO*			sfinfo;
	SF_EMBED_FILE_INFO*  embed_info;
	sf_count_t			flen;
	int 				fd, offset = 0;

	sfinfo = malloc(sizeof(SF_INFO));
	embed_info = malloc(sizeof(SF_EMBED_FILE_INFO));

	memset(&sfinfo, 0, sizeof(SF_INFO));

	int file = open("./assets/sfx/try.ri", O_RDONLY, S_IRUSR);
	lseek(file, 60, SEEK_SET);

	printf("hello\n");

	sndfile = sf_open_fd(file, SFM_READ, sfinfo, SF_FALSE);

	// USE sf_open_virtual. Just make custom callbacks for reading from within a larger file!!!! HOLY SHIT THANK YOU libsndfiledevs ;w; ;w; ;w; ;w;

	printf("world\n");

	sf_command(sndfile, SFC_GET_EMBED_FILE_INFO, embed_info, sizeof(embed_info));

	fprintf(stderr, "Soundfile located at [%lx] with length of %lu b\n\tFORMAT : %u\n\tRATE : %u\n", 
								embed_info->offset, embed_info->length, sfinfo->format, sfinfo->samplerate);

	close(file);

	/*
	fd = open(filepath, O_RDONLY, S_IRUSR);

	struct stat st;
	fstat(fd, &st); 
	flen = st.st_size;

	int sum = 0;
	for (int fc=0;sum<flen;fc++) {
		lseek(fd, sum, SEEK_SET);
		

		
		fprintf(stderr, "VARIABLES: \n\tSUM : %d\n\tOFFSET : %d\n\ttell(fd) : %d\n", sum, offset, lseek(fd, 0, SEEK_CUR));


		memset(&sfinfo, 0, sizeof (sfinfo)) ;
		sndfile = sf_open_fd(fd, SFM_READ, sfinfo, SF_FALSE);

		if (sndfile == NULL) {
			sum++;
			continue;
		}

		sf_command(sndfile, SFC_GET_EMBED_FILE_INFO, &embed_info, sizeof(embed_info));
		fprintf(stderr, "Soundfile located at [%lx] with length of %lu b\n\tFORMAT : %u\n\tRATE : %u\n", 
								embed_info.offset, embed_info.length, sfinfo->format, sfinfo->samplerate);
		
		sf_close(sndfile);
		offset=embed_info.length;
		sum += offset;
	}
	close(fd);
	*/

	free(embed_info);
	free(sfinfo);
	return (AUDsfx*)NULL;
}

/*
AUDsfx* audSpecialLoadSfx(char* filepath) {
	SNDFILE*			sndfile;
	SF_INFO*			sfinfo;
	SF_EMBED_FILE_INFO	embed_info;
	sf_count_t			filelen;
	int 				fd, k = 0;

	//unlink(filepath); // What does this do?

	fprintf(stderr,  "ERROR CALLED : %s\n", strerror(errno));
	fprintf(stderr, "\terrno = %x\n", errno); // this is not zero :/

	// Open the file
	fd = open(filepath, O_RDONLY, S_IRUSR);
	fprintf(stderr, "Opening file for reading. fd = [%u]\n", (unsigned int)fd);

	fprintf(stderr,  "ERROR CALLED : %s\n", strerror(errno));

	// Get size of the file from its fd (seeking might be better here)
	struct stat st;
	fstat(fd, &st); 
	filelen = st.st_size;

	// Set some arbitrary variables
	embed_info.offset = 4;
	embed_info.length = 0;

	fprintf(stderr,  "ERROR CALLED : %s\n", strerror(errno));

	fprintf(stderr, "hello? FILELEN = %lu", filelen);

	int fc = 1;
	for (fc = 1; embed_info.offset + embed_info.length < filelen; fc++) {
		int err;

		fprintf(stderr, "hello?");

		err = lseek(fd, embed_info.offset + embed_info.length, SEEK_CUR); // Set the file offset
		if (err < 0) {
			fprintf(stderr,  "ERROR CALLED : %s", strerror(errno));
			fprintf(stderr, "Seek FAILED\n\toffset = %u\n",(unsigned int)(embed_info.offset + embed_info.length));
		}

		fprintf(stderr, "hello?");

		memset(&sfinfo, 0, sizeof(sfinfo)); // Zero out sfinfo for some reason

		fprintf(stderr, "post memset");

		sndfile = sf_open_fd(fd, SFM_READ, &sfinfo, SF_FALSE);
		if (sndfile == NULL) {
			printf ("\n\nLine %d: sf_open_fd failed\n", __LINE__) ;
			printf ("Embedded file number : %d   offset : %lx\n", fc, embed_info.offset) ;
			//puts (sf_strerror (sndfile)) ;
			// dump_log_buffer (sndfile) ; tf does this do?
			embed_info.length++;
		} else {
			sf_command(sndfile, SFC_GET_EMBED_FILE_INFO, &embed_info, sizeof(embed_info));

			fprintf(stderr, "Soundfile located at [%lx] with length of %lu b\n\tFORMAT : %u\n\tRATE : %u\n", 
			embed_info.offset, embed_info.length, sfinfo->format, sfinfo->samplerate);
	
		}

		sf_close(sndfile);
	}
	
	close(fd);
	//unlink(filepath);	
	return (AUDsfx*)NULL;
}
*/

void audUnloadSfx(AUDsfx* sfx) {
	free(sfx); // Should do more than this obvi
}

void audSoundPlay(AUDsfx* sfx, int loop) {
	PaError err;
	err = Pa_StartStream(sfx->stream);
	(err != paNoError) ? fprintf(stderr, "audio_sound_play(): %s\n", Pa_GetErrorText(err)) : 1;
}

void audSoundPause(AUDsfx* sfx) {
	PaError err;
	err = Pa_StopStream(sfx->stream);
	(err != paNoError) ? fprintf(stderr, "audio_sound_pause(): %s\n", Pa_GetErrorText(err)) : 1;
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