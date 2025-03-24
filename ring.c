#include "ring.h"

/* variables */
Sin** RING = NULL;
long RING_LENGTH = 0;

Sin** DebugRingGet() {
	return RING;
}

int RingInit() {
	RING = (Sin**)malloc(sizeof(void*));
	return (RING == NULL);
}

int RingBufferElement(int id, int ptr_widths) {
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH + ptr_widths));
	for (int y=RING_LENGTH;y>id;y--) {
		RING[y]->id = y + ptr_widths;
		RING[y + ptr_widths] = RING[y]; // Consider memcpy :3
	} // Give a particular object extra space just cuz
	RING_LENGTH+=ptr_widths;
	RING[RING_LENGTH+1] = NULL;
	return (RING == NULL);
}

int RingCleanup() {
	free(RING);
	return 0;
}

int RingAppendSins(Sin** sins, int num) {
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

int RingReplaceSin() {
	// TODO (don't really need this right now. Same with other ID priority based stuff)
}

int RingDuplicateSins() {
	// TODO (This function keeps the object data, thats why its cool :3)
}

int RingPurgeSin(int id) { // Inefficient, should allow multiple deletions at once like append.
	SinExecuteEvent(RING[id], EVENT_DESTROY);
	for (int g=id;g<RING_LENGTH;g++) {
		RING[g] = RING[g+1]; // Im sure this could also be memcpy-d
		RING[g]->id = g;
	}
	RING_LENGTH--;
	RING = (Sin**)realloc(RING, sizeof(void*) * (RING_LENGTH));
	return (RING == NULL);
}

Sin* RingGetSinByID(int id) {
	return RING[id];
}

int RingExecuteEvent(uint8_t event) {
	for (int j=0;j<=RING_LENGTH;j++) {
		SinExecuteEvent(RING[j],event); // Very suprising I know
	}
}

int SinExecuteEvent(Sin* sin, uint8_t event) {
	sin->event_script(sin, event);
}
