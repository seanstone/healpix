GLuint compileShader(const char** source, GLenum type)
{
    GLuint shader = glCreateShader(type); // VERTEX_SHADER or FRAGMENT_SHADER
  	glShaderSource(shader, 1, source, NULL);
  	glCompileShader(shader);

    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE)
        printf("Failed to compile shader");
    	//printf("Failed to compile shader: %s", glGetShaderInfoLog(shader));

	printf("Shader compiled: %d", type);
  	return shader;
}

GLuint compileProgram(const char** vs_source, const char** fs_source)
{
	GLuint vs = compileShader(vs_source, GL_VERTEX_SHADER),
    	   fs = compileShader(fs_source, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

    int result;
	glGetProgramiv(program,GL_LINK_STATUS,&result);
	if(result != GL_TRUE)
	{
        //printf("Failed to compile shader program: %s", glGetProgramInfoLog(program));
        printf("Failed to compile shader program");
        return 0;
    }

	printf("Shader program compiled");
	return program;
}
