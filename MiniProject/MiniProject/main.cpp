#include "Engine.h"

// Screen
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const unsigned int MAJOR_VERSION = 4;
const unsigned int MINOR_VERSION = 6;

int main() {
	Engine engine("Quang - 173006", WINDOW_WIDTH, WINDOW_HEIGHT, MAJOR_VERSION, MINOR_VERSION);

	while (!engine.getWindowShouldClose()) {
		engine.update();
		engine.render();
	}

	return 0;
}