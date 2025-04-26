
// Default SINNER!!!!

int script_default(void* sin, uint8_t event_flag) {
	static const void* table[] = {&&CREATE, &&STEP};
	goto *table[event_flag];

	CREATE:
		fprintf(stderr, "Event created!!!!!\n");
		return 0;
	STEP:
		fprintf(stderr, "Event stepped!!!!\n");
		return 0;
}

Sin sin_default = {0,&sin_default,&script_default,NULL};