#include <ring.h>

Sin** ring_get() {
	return RING;
}

error_t ring_init() {
	RING = (Sin**)malloc(sizeof(void*));
	return (RING == NULL);
}

error_t ring_resize(int num_sins) {
	RING = (Sin**)realloc(sizeof(void*) * num_sins);
	RING_LENGTH = num_sins;
	return (RING == NULL);
}

error_t ring_refactor() {
	int i = 0;
	for (;RING[RING_LENGTH - i]!=NULL;i++) {}
	return ring_resize(RING_LENGTH - i);
}

error_t ring_cleanup() {

}

error_t ring_append_sin() {

}

error_t ring_replace_sin() {

}

error_t ring_duplicate_sin() {

}

error_t ring_remove_sin() {

}

Sin* ring_get_sin_by_id(int id) {

}

Sin* ring_get_sin(int index) {

}

error_t ring_execute_event(uint8_t event, uint8_t prio_flags) {

}

error_t sin_set_acheron(Sin* sin, uint8_t prio_flags) {

}

uint8_t prio_flags sin_get_acheron(Sin* sin) {

}

error_t sin_execute_event(Sin* sin, uint8_t event) {

}