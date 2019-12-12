#include "Engine.h"


int main() {
	// init GLFW
	// create window
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	
	Engine engine("Test", WINDOW_WIDTH, WINDOW_HEIGHT, 4, 6, false);
	/* window options
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// main loop*/
	while (!engine.getWindowShouldClose()) {
		// update input
		engine.update();
		engine.render();
		
	}

	return 0;
}