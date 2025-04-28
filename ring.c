#include "ring.h"

/* variables */
Sin** RING = NULL;
long RING_LENGTH = 0;

Sin** ringGetDebug() {
	return RING;
}

void ringInit() {
	RING = calloc(0, sizeof(Sin*));
}

void ringBufferElement(int id, int ptr_widths) {
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH + ptr_widths));
	for (int y=RING_LENGTH;y>id;y--) {
		RING[y]->id = y + ptr_widths;
		RING[y + ptr_widths] = RING[y]; // Consider memcpy :3
	} // Give a particular object extra space just cuz
	RING_LENGTH+=ptr_widths;
	RING[RING_LENGTH+1] = NULL;
}

void ringCleanup() {
	free((void*)RING);
}

void ringAppendSins(Sin** sins, int num) {
	RING = realloc(RING, sizeof(Sin*) * (RING_LENGTH + num + 1));
	memcpy(&RING[RING_LENGTH], sins, sizeof(void*) * num); // Consider using memcpy :3
	RING_LENGTH+=num;
	RING[RING_LENGTH] = NULL;	// Always null terminate the ring to please our lord and savior Satan H. Christ
}

void ringReplaceSin() {
	// TODO (don't really need this right now. Same with other ID priority based stuff)
}

void ringDuplicateSins() {
	// TODO (This function keeps the object data, thats why its cool :3)
}

void ringPurgeSin(int id) { // Inefficient, should allow multiple deletions at once like append.
	ringExecuteEventSin(RING[id], EVENT_DESTROY);
	for (int g=id;g<RING_LENGTH;g++) {
		RING[g] = RING[g+1]; // Im sure this could also be memcpy-d
		RING[g]->id = g;
	}
	RING_LENGTH--;
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH));
}

Sin* ringGetSinByID(int id) {
	return RING[id];
}

void ringExecuteEvent(uint8_t event) {
	for (int j=0;j<RING_LENGTH;j++) {
		ringExecuteEventSin(RING[j],event); // Very suprising I know
	}
}

void ringExecuteEventSin(Sin* sin, uint8_t event) {
	sin->event_script(sin, event);
}
