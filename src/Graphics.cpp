#include "Graphics.hpp"
#include "View.hpp"

int Graphics::init()
{
    lastTime = glfwGetTime();

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    printf("Before GLFW\n");
	glfwInit(); //if(!glfwInit()){printf("Failed to initialize GLFW\n");return -1;}
    printf("GLFW initialized\n");
    initWindow(); std::cout<< "Window initialized\n";
	if(glewInit() != GLEW_OK) {printf("Failed to initialize GLEW\n");return -1;} else std::cout<< "GLEW initialized\n";
    initOptions();

    healpix.init();

    std::cout<<"Initialzed: "<<(glfwGetTime() - lastTime)<<" seconds\n";

	return 0;
}

int Graphics::initWindow()
{
    // Open a GLFW window and create its OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(width, height, "ExoEarth", NULL, NULL);
	if( window == NULL ){ printf("Failed to open GLFW window.\n" ); glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    // Ensure we can capture the escape key being pressed
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, width/2, height/2);

	// Get version info
	const GLubyte* Renderer = glGetString (GL_RENDERER);
    const GLubyte* Version  = glGetString (GL_VERSION);
    printf ("Renderer: %s\n", Renderer);
    printf ("OpenGL version %s\n\n", Version);

	return 0;
}

int Graphics::initOptions()
{
    //glEnable(GL_MULTISAMPLE);

    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);   // Accept fragment if it closer to the camera than the former one

	return 0;
}

int Graphics::loop()
{
    float fps=0;
    lastTime = glfwGetTime();
	do
    {
        draw();
        glfwSwapBuffers(window); glfwPollEvents();
        fps++; if(glfwGetTime()-lastTime>1) { std::cout<<"FPS: "<<fps<<"     \xd"; lastTime = glfwGetTime(); fps=0;}
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && glfwWindowShouldClose(window)==0);

    return 0;
}

void Graphics::draw()
{
    // Clear the screen (dark blue background)
    glClearColor(0.0f, 0.0f, 0.05f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // VP from inputs
    mat4 MVP =  view.Perspective(120.0*M_PI/180.0, width/height, 0.1, 100) * view.lookAt(vec3(0,0,20), vec3(0,0,0), vec3(1,0,0));
    glUniformMatrix4fv(healpix.MVP.id, 1, GL_FALSE, &(MVP)[0][0]);

    healpix.draw();
}

int Graphics::terminate()
{
    glfwTerminate();

	return 0;
}
