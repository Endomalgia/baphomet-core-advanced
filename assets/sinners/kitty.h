
// Default SINNER!!!!

int script_kitty(void* sin, uint8_t event_flag) {
	static const void* table[] = {&&CREATE, &&STEP};
	goto *table[event_flag];

	CREATE:
		fprintf(stderr, "Event created but kitty!!!!!\n");
		return 0;
	STEP:
		fprintf(stderr, "Event stepped but kitty!!!!\n");
		return 0;
}

Sin sin_kitty = {0,&sin_kitty,&script_kitty,NULL};