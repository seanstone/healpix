#include <iostream>
#include <string>
using namespace std;

#include <Pulsar.hpp>
#include <PulsarWindow.hpp>
#include <PulsarShader.hpp>
#include <PulsarScene.hpp>
#include <PulsarCanvas.hpp>
#include "CanvasShader.hpp"
using namespace Pulsar;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

class MainWindow : public Window
{
public:
	MainWindow();
	~MainWindow();
	void render();
	void update();

protected:
	Renderer renderer;
	CanvasShader* shader;
	Canvas* canvas;
};

MainWindow::MainWindow()
{
	createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Earth");

	renderer.init();
	renderer.setClearColor(vec3(0, 0, 0.5));

	bool result = true;
	shader = new CanvasShader;
	result &= shader->addVertexShader(File::readAllText("data/shader/simple.vs"));
	result &= shader->addFragmentShader(File::readAllText("data/shader/simple.fs"));
	result &= shader->compile();
	if(result == false)
	{
		terminate();
		exit(0);
	}

	canvas = new Canvas;
	canvas->setVertices();
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
	MainWindow window;
	window.startLoop();
	window.terminate();
	return 0;
}
