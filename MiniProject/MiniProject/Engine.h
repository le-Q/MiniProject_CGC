#pragma once

#include "libs.h"
#include "Camera.h"


// ENUMERATIONS
enum shader_enum { SHADER_CORE_PROGRAM = 0, SKYBOX_PROGRAM };
enum texture_enum { TEX_CONTAINER = 0, TEX_CONTAINER_SPECULAR, TEX_WOOD, TEX_AWESOME, SKYBOX };
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

class Engine
{
private:
	// Variables
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;
	bool first = true;

	// OpenGL Context
	const int GL_VERSION_MAJOR = 4;
	const int GL_VERSION_MINOR = 6;

	// Delta time
	float deltaTime;
	float curTime;
	float lastTime;

	// Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	// Camera
	Camera camera;

	// Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 cameraUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	// Shaders
	std::vector<Shader*> shaders;

	// Textures
	std::vector<Texture*> textures;

	// Materials
	std::vector<Material*> materials;

	// Models
	std::vector<Model*> models;
	Skybox * skybox;

	// Lights
	std::vector<glm::vec3*> lights;

	// Private functions
	void initializeGLFW();
	void initializeWindow(const char* title);
	void initializeGLEW();
	void initializeOpenGLOptions();
	void initializeMatrices();
	void initializeShaders();
	void initializeTextures();
	void initializeMaterials();
	void initializeModels();
	void initializeLights();
	void initializeUniforms();

	void updateUniforms();

	// Static variables

public:
	// Constructors / Destructors
	Engine(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR
	);
	virtual ~Engine();

	// Accessors
	int getWindowShouldClose();

	// Modifiers
	void setWindowShouldClose();

	// Functions
	void updatedeltaTime();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();
	void update();
	void render();
};