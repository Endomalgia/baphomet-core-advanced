#include "ring.h"

/* variables */
Sin** RING = NULL;
long RING_LENGTH = 0;

Sin** ringGetDebug() {
	return RING;
}

int ringInit() {
	RING = (Sin**)malloc(sizeof(void*));
	return (RING == NULL);
}

int ringBufferElement(int id, int ptr_widths) {
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH + ptr_widths));
	for (int y=RING_LENGTH;y>id;y--) {
		RING[y]->id = y + ptr_widths;
		RING[y + ptr_widths] = RING[y]; // Consider memcpy :3
	} // Give a particular object extra space just cuz
	RING_LENGTH+=ptr_widths;
	RING[RING_LENGTH+1] = NULL;
	return (RING == NULL);
}

int ringCleanup() {
	free(RING);
	return 0;
}

int ringAppendSins(Sin** sins, int num) {
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH + num + 1));
	//memcpy(&RING[RING_LENGTH], &sins, sizeof(void*) * num); // Consider using memcpy :3
	for (int z=0;z<num;z++) {
		RING[RING_LENGTH+z] = sins[z];
		sins[z]->id = RING_LENGTH+z;
	}
	RING_LENGTH+=num;
	RING[RING_LENGTH+1] = NULL;	// Always null terminate the ring to please our lord and savior Satan H. Christ
	return (RING == NULL);
}

int ringReplaceSin() {
	// TODO (don't really need this right now. Same with other ID priority based stuff)
}

int ringDuplicateSins() {
	// TODO (This function keeps the object data, thats why its cool :3)
}

int ringPurgeSin(int id) { // Inefficient, should allow multiple deletions at once like append.
	sinExecuteEvent(RING[id], EVENT_DESTROY);
	for (int g=id;g<RING_LENGTH;g++) {
		RING[g] = RING[g+1]; // Im sure this could also be memcpy-d
		RING[g]->id = g;
	}
	RING_LENGTH--;
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH));
	return (RING == NULL);
}

Sin* ringGetSinByID(int id) {
	return RING[id];
}

int ringExecuteEvent(uint8_t event) {
	for (int j=0;j<RING_LENGTH;j++) {
		sinExecuteEvent(RING[j],event); // Very suprising I know
	}
	return 0;
}

int sinExecuteEvent(Sin* sin, uint8_t event) {
	sin->event_script(sin, event);
	return 0;
}
