#include <ring.h>

Sin** debug_ring_get() {
	return RING;
}

error_t ring_init() {
	RING = (Sin**)malloc(sizeof(void*));
	return (RING == NULL);
}

error_t ring_buffer_element(int id, int ptr_widths) {
	RING = (Sin**)realloc(sizeof(void*) * (RING_LENGTH + ptr_widths));
	for (int y=RING_LENGTH;y>id;y--) {
		RING[y]->id = y + ptr_widths;
		RING[y + ptr_widths] = RING[y]; // Consider memcpy :3
	} // Give a particular object extra space just cuz
	RING_LENGTH+=ptr_widths;
	RING[RING_LENGTH+1] = NULL;
	return (RING == NULL);
}

error_t ring_cleanup() {
	return free(RING);
}

error_t ring_append_sins(Sin* sins, int num) {
	RING = (Sin**)realloc(sizeof(void*) * (RING_LENGTH + num + 1));
	//memcpy(&RING[RING_LENGTH], &sins, sizeof(void*) * num); // Consider using memcpy :3
	for (int z=0;z<num;z++) {
		RING[RING_LENGTH+z] = sin[z];
		sin[z]->id = RING_LENGTH+z;
	}
	RING_LENGTH+=num;
	RING[RING_LENGTH+1] = NULL;	// Always null terminate the ring to please our lord and savior Satan H. Christ
	return (RING == NULL);
}

error_t ring_replace_sin() {
	// TODO (don't really need this right now. Same with other ID priority based stuff)
}

error_t ring_duplicate_sin() {
	// TODO (This function keeps the object data, thats why its cool :3)
}

error_t ring_remove_sin(int id) { // Inefficient, should allow multiple deletions at once like append.
	sin_execute_event(sin, EVENT_DESTROY);
	for (int g=id;g<RING_LENGTH;g++) {
		RING[g] = RING[g+1] // Im sure this could also be memcpy-d
		RING[g]->id = g;
	}
	RING_LENGTH--;
	RING = (Sin**)realloc(sizeof(void*) * (RING_LENGTH));
	return (RING == NULL);
}

Sin* ring_get_sin_by_id(int id) {
	// TODO (I just don't gaf atm)
}

Sin* ring_get_sin(int id) {
	return RING[id];
}

error_t ring_execute_event(uint8_t event) {
	for (int j=0;j<=RING_LENGTH;j++) {
		sin_execute_event(RING[j],event); // Very suprising I know
	}
}

error_t sin_execute_event(Sin* sin, uint8_t event) {
	sin->event_script(sin, event);
}
