
// Solar system object

struct data_solar {
	GFXtexture t_sun;
	GFXtexture t_moon;
	GFXmesh	   m_sun;
	GFXmesh	   m_moon;
};

int script_solar(void* oid, uint8_t event_flag) {
	static const void* table[] = {&&CREATE, &&STEP, NULL, &&DESTROY};
	goto *table[event_flag];

	static struct data_solar* dat = NULL;

	CREATE:
		dat = ((Sin*)oid)->data = malloc(sizeof(struct data_solar)); 

		dat->t_sun = gfxLoadTexture("assets/sprites/sun.png", GL_RGBA);
		dat->t_moon = gfxLoadTexture("assets/sprites/moon.png", GL_RGBA);

		dat->m_sun = gfxGenerateRect(1.0f, 1.0f);
		dat->m_moon = gfxGenerateRect(0.5f, 0.5f);
		
		return 0;
	STEP:
	
		gfxShaderSetUniformVec2(gfxGetShader(), "position", sin(glfwGetTime()*1.2312f)*1.0f, cos(glfwGetTime()*1.2312f)*1.0f);
		if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
			gfxShaderSetUniformVec2(gfxGetShader(), "position", sin(glfwGetTime()*15.0f)*1.0f, cos(glfwGetTime()*15.0f)*1.0f);
		}

		gfxSetTexture(&(dat->t_moon));
		gfxDrawMesh(&(dat->m_moon));

		gfxShaderSetUniformVec2(gfxGetShader(), "position", 0.0f, 0.0f);

		gfxSetTexture(&(dat->t_sun));
		gfxDrawMesh(&(dat->m_sun));

		return 0;
	DESTROY:
		free(dat);
		return 0;
}

Sin sin_solar = {0,&sin_solar,&script_solar,NULL};