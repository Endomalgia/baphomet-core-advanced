
// Default camera object

struct data_camera {
	vec3* from;
	GFXcamera cam;
};

int script_camera(void* oid, uint8_t event_flag) {
	static const void* table[] = {&&CREATE, &&STEP, NULL, NULL, &&DESTROY};
	goto *table[event_flag];

	static struct data_camera* dat = NULL;
	static vec3 to = {0.0, 0.0, 0.0};		// Memory leak here with lazier code, all vec/mats need to be preallocated (augh....)

	CREATE:
		dat = ((Sin*)oid)->data = malloc(sizeof(struct data_camera));

		dat->from = (vec3*)malloc(sizeof(float)*3);
		(*(dat->from))[0] = 0.05f;
		(*(dat->from))[1] = 0.00f;
		(*(dat->from))[2] = -0.8f;	// Wrap into vec3 memory allocation function in future

		dat->cam = gfxCreateCamera();
		gfxSetCamera(&(dat->cam));

		gfxCameraSetViewLookat(&(dat->cam), *(dat->from), to);
		gfxCameraSetProjectionPersp(&(dat->cam), 210.0 * 3.14 / 360.0, 1.0, 0.0001, 10000.0);
		
		return 0;
	STEP:
		(*(dat->from))[0] = sin(glfwGetTime()) * 1.5;	// x coord
		(*(dat->from))[2] = cos(glfwGetTime()) * 1.5; // y coord
		gfxCameraSetViewLookat(&(dat->cam), *(dat->from), to);

		return 0;
	DESTROY:
		free(dat);
		return 0;
}

Sin sin_camera = {0,&sin_camera,&script_camera,NULL};