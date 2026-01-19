
// Solar system object

struct data_solar {
	GFXtexture t_sun;
	GFXtexture t_moon;
};

int script_solar(void* oid, uint8_t event_flag) {
	static const void* table[] = {&&CREATE, &&STEP, NULL, NULL, &&DESTROY};
	goto *table[event_flag];

	static struct data_solar* dat = NULL;

	CREATE:
		dat = ((Sin*)oid)->data = malloc(sizeof(struct data_solar));

		dat->t_sun = gfxLoadTexture("assets/sprites/sun.png", GL_RGBA);
		dat->t_moon = gfxLoadTexture("assets/sprites/moon.png", GL_RGBA);

		return 0;
	STEP:
		float x = sin(glfwGetTime()*1.2312f)*1.0f;
		float y = cos(glfwGetTime()*1.2312f)*1.0f;
		if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
			x = sin(glfwGetTime()*15.0f)*1.0f;
			y = cos(glfwGetTime()*15.0f)*1.0f;
		}

		gfxDrawTexture2D(&(dat->t_moon), x - 0.1f, y + 0.1f, 0.2f);
		gfxDrawTexture2D(&(dat->t_sun), -0.75f, 0.75f, 1.5f);

		return 0;
	DESTROY:
		gfxUnloadTexture(&(dat->t_sun));
		gfxUnloadTexture(&(dat->t_moon));

		free(dat);
		return 0;
}

Sin sin_solar = {0,&sin_solar,&script_solar,NULL};

/*

compressed form ->

DATA:
	GFXtexture t_sun;
	GFXtexture t_moon;

CREATE:
	t_sun = gfxLoadTexture("assets/sprites/sun.png", GL_RGBA);
	t_moon = gfxLoadTexture("assets/sprites/moon.png", GL_RGBA);

STEP:
	float x = sin(glfwGetTime()*1.2312f)*1.0f;
	float y = cos(glfwGetTime()*1.2312f)*1.0f;
	if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
		x = sin(glfwGetTime()*15.0f)*1.0f;
		y = cos(glfwGetTime()*15.0f)*1.0f;
	}

	gfxDrawTexture2D(&t_moon, x, y, 0.0005f);
	gfxDrawTexture2D(&t_sun, 0.0f, 0.0f, 0.001f);

DESTROY:
	gfxUnloadTexture(&t_sun);
	gfxUnloadTexture(&t_moon);

*/
