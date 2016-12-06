#include <iostream>
#include <string>
using namespace std;

#include <Pulsar.hpp>
#include <PulsarWindow.hpp>
#include <PulsarShader.hpp>
#include <PulsarCanvas.hpp>
#include "CanvasShader.hpp"
#include "Intersect.hpp"
using namespace Pulsar;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <html5.h>
#endif

class MainWindow : public Window
{
public:
	MainWindow();
	~MainWindow();
	void render();
	void update();
	CanvasShader* shader;

protected:
	Renderer renderer;
	Canvas* canvas;
};

MainWindow window;
int width = 1200, height = 800, isFullscreen;

Camera camera;
Sphere sphere;

#ifdef __EMSCRIPTEN__
EM_BOOL ResizeHandler(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
	emscripten_get_canvas_size(&width, &height, &isFullscreen);
	glViewport(0, 0, width, height);
	window.shader->setParameter("iResolution", vec3(width, height, float(width)/height));
	camera.resolution = vec2(width, height);
	camera.aspectRatio = (double) width / (double) height;
	return true;
}

EM_BOOL MouseHandler(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
	int x, y;
	switch (eventType)
	{
		case EMSCRIPTEN_EVENT_MOUSEMOVE:
		{
			x = mouseEvent->clientX; y = mouseEvent->clientY;
			EM_ASM_( {document.getElementById("mousepos").innerHTML = $0 + ", " + $1; }, x, y);
			Ray ray = createCameraRay(camera, vec2((float) x / (float) width, (float) y / (float) height));
			Intersection intersection = findSphereIntersection(sphere, ray);
			vec3 ip = intersection.point;
			float r = sphere.radius;
			vec3 p = ip/r * .5f + 1.f;
			if (intersection.intersect)
				EM_ASM( { document.getElementById("mousepos").innerHTML += "</br>IN "; } );
		}
			break;
		case EMSCRIPTEN_EVENT_MOUSEDOWN:
			break;
		case EMSCRIPTEN_EVENT_MOUSEUP:
			break;
		default:
			break;
	}
	return true;
}
#endif

void init_vars()
{
	camera.position = vec3(0,0,-4);
	camera.resolution = vec2(width, height);
	camera.aspectRatio = (float) width / height;
	camera.fov = radians(90.);
	camera.right = vec3(1,0,0);
	camera.up = vec3(0,1,0);
	camera.dir = cross(camera.right, camera.up);

	sphere.origin = vec3(0,0,0);
	sphere.radius = 3.;
}

MainWindow::MainWindow()
{
	createWindow(width, height, "Earth");

	renderer.init();
	renderer.setClearColor(vec3(0, 0, 0.5));

	canvas = new Canvas;
	canvas->setVertices();

	bool result = true;
	shader = new CanvasShader;
	result &= shader->addVertexShader(File::readAllText("data/shader/simple.vs"));
	result &= shader->addFragmentShader(File::readAllText("data/shader/simple.fs"));
	result &= shader->compile();
	if(!result)
	{
		terminate();
		exit(0);
	}
	shader->bind();

	#ifdef __EMSCRIPTEN__
	emscripten_get_canvas_size(&width, &height, &isFullscreen);
	shader->setParameter("iResolution", vec3(width, height, float(width)/height));
	emscripten_set_resize_callback(0, 0, true, ResizeHandler);
	emscripten_set_mousedown_callback(0, 0, true, MouseHandler);
	emscripten_set_mouseup_callback(0, 0, true, MouseHandler);
	emscripten_set_mousemove_callback(0, 0, true, MouseHandler);
	#endif

}

MainWindow::~MainWindow()
{
	delete shader;
}

void MainWindow::render()
{
	renderer.clearScreen();
	renderer.initFrame();
	shader->bind();
	canvas->render();
}

void MainWindow::update()
{
}

int main()
{
	init_vars();
	window.startLoop();
	window.terminate();
	return 0;
}
