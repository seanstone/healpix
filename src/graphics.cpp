#include "graphics.h"
#include "view.h"

int graphics::init()
{
    lastTime = glfwGetTime();

	if(!glfwInit()){fprintf(stderr, "Failed to initialize GLFW\n");return -1;} // Initialise GLFW
  std::cout<< "GLFW initialized\n";
    initWindow();
  std::cout<< "Window initialized\n";
	if(glewInit() != GLEW_OK) {fprintf(stderr, "Failed to initialize GLEW\n");return -1;} // Initialize GLEW
  std::cout<< "GLEW initialized\n";
    initOptions();
    initProgram();
  std::cout<< "Program initialized\n";
    initTexture();
  std::cout<< "Textures initialized\n";
    Healpix.initVBO();
  std::cout<< "VBO initialized\n";

    std::cout<<"Initialzed: "<<(glfwGetTime() - lastTime)<<" seconds\n";

	return 0;
}

int graphics::initWindow()
{
    // Open a GLFW window and create its OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	Window = glfwCreateWindow(Width, Height, "HEALPix", NULL, NULL);
	if( Window == NULL ){fprintf(stderr, "Failed to open GLFW window.\n" );glfwTerminate();return -1;}
    glfwMakeContextCurrent(Window);

    // Ensure we can capture the escape key being pressed
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(Window, Width/2, Height/2);

	// Get version info
	const GLubyte* Renderer = glGetString (GL_RENDERER);
    const GLubyte* Version  = glGetString (GL_VERSION);
    printf ("Renderer: %s\n", Renderer);
    printf ("OpenGL version %s\n\n", Version);

	return 0;
}

int graphics::initOptions()
{
    //glEnable(GL_MULTISAMPLE);

    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);   // Accept fragment if it closer to the camera than the former one

	return 0;
}

int graphics::initProgram()
{
    // Create and compile our GLSL program from the shaders
	Program = loadShaders("glsl/vertex.glsl", "glsl/fragment.glsl");

	// Get a handle for attributes
    Healpix.Uniform_Rotor   = glGetUniformLocation(Program, "Rotor");
    Healpix.Uniform_Sampler = glGetUniformLocation(Program, "Sampler");
    Healpix.Uniform_MVP     = glGetUniformLocation(Program, "MVP");
    Healpix.In_VertexXYZ    = glGetAttribLocation(Program, "VertexXYZ");
    Healpix.In_VertexUV     = glGetAttribLocation(Program, "VertexUV");

    // Use shader
    glUseProgram(Program);

    return 0;
}

int graphics::initTexture()
{
    Texture = loadBMP("textures/earth.bmp");
    glActiveTexture(GL_TEXTURE0); // Bind our texture in Texture Unit 0
    glBindTexture(GL_TEXTURE_2D, Texture); // Set our sampler to use Texture Unit 0
    glUniform1i(Healpix.Uniform_Sampler, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_CLAMP_TO_EDGE

    return 0;
}

int graphics::loop()
{
    float fps=0;
    lastTime = glfwGetTime();
	do
    {
        View.inputs(Window);
        //Healpix.update();
        draw();
        glfwSwapBuffers(Window); glfwPollEvents();
        fps++; if(glfwGetTime()-lastTime>1) { std::cout<<"FPS: "<<fps<<"     \xd"; lastTime = glfwGetTime(); fps=0;}
    }
    while(glfwGetKey(Window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && glfwWindowShouldClose(Window)==0);

    return 0;
}

void graphics::draw()
{
    // Clear the screen (dark blue background)
    glClearColor(0.0f, 0.0f, 0.05f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // VP from inputs
    //mat4 MVP =  View.Perspective(120.0*M_PI/180.0, Width/Height, 0.1, 100) * View.lookAt(vec3(0,0,20), vec3(0,0,0), vec3(1,0,0));
    //mat4 MVP =  View.Simple() * View.lookAt(vec3(0,0,-10), vec3(0,0,0), vec3(1,0,0));
    //std::cout << MVP;
    //Send MVP to OpenGL
    glUniform4f(Healpix.Uniform_Rotor, View.Rotor.x, View.Rotor.y, View.Rotor.z, View.Rotor.w);
    //glUniformMatrix4fv(Healpix.Uniform_MVP, 1, GL_FALSE, &(MVP)[0][0]);

    Healpix.draw();
}

int graphics::cleanup()
{
    // Delete textures
    glDeleteTextures(1, &Texture);

    // Delete program
	glDeleteProgram(Program);

    // Terminate GLFW
	glfwTerminate();

	return 0;
}
