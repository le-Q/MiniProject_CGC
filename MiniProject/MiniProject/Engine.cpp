#include "Engine.h"

// Private functions
void Engine::initGLFW()
{
	// INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Engine::initWindow(const char* title)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwMakeContextCurrent(this->window);
}

void Engine::initGLEW()
{
	// INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	// Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Engine::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition + this->camFront, this->cameraUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Engine::initShaders()
{
	this->shaders.push_back(new Shader("vertex.glsl", "fragment.glsl"));
	this->shaders.push_back(new Shader("vertex_sky.glsl", "fragment_sky.glsl"));
}

void Engine::initTextures()
{
	std::vector<std::string> faces
	{
		"Skybox/right.jpg",
		"Skybox/left.jpg",
		"Skybox/top.jpg",
		"Skybox/bottom.jpg",
		"Skybox/front.jpg",
		"Skybox/back.jpg"
	};

	// TEXTURE 0
	this->textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D));

	// TEXTURE 1
	this->textures.push_back(new Texture("Images/wood.png", GL_TEXTURE_2D));

	// SkyBox
	this->textures.push_back(new Texture(faces, GL_TEXTURE_CUBE_MAP));
}

void Engine::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
}

void Engine::initModels()
{
	std::vector<Mesh*>meshes;
	std::vector<Mesh*>floor;
	std::vector<Mesh*>cubes;
	std::vector<Mesh*>skyboxTest;

	skybox = new Skybox();

	skyboxTest.push_back(
		new Mesh(
			&SkyboxShape(),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(20.f)
		)
	);

	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[0],
		this->textures[SKYBOX],
		this->textures[SKYBOX],
		skyboxTest
	)
	);

	floor.push_back(
		new Mesh(
			&Square(),
			glm::vec3(0.f, 0.f, -.5f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(270.f, 0.f, 0.f),
			glm::vec3(20.f)
		)
	);

	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[0],
		this->textures[TEX_WOOD],
		this->textures[TEX_WOOD],
		floor
	)
	);

	meshes.push_back(
		new Mesh(
			&Cube(),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);

	cubes.push_back(
		new Mesh(
			&Cube(),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f, 45.f, 0.f),
			glm::vec3(1.f)
		)
	);

	cubes.push_back(
		new Mesh(
			&Cube(),
			glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);

	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);

	this->models.push_back(new Model(
		glm::vec3(2.f, 0.f, 2.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		cubes
	)
	);

	this->models.push_back(new Model(
		glm::vec3(4.f, 0.f, 0.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);

	this->models.push_back(new Model(
		glm::vec3(-2.f, 0.f, 0.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		cubes
	)
	);
	

	for (auto*& i : meshes)
		delete i;

	for (auto*& i : cubes)
		delete i;
}

void Engine::initLights()
{
	this->lights.push_back(new glm::vec3(2.5f, 2.f, 2.5f));
}

void Engine::initUniforms()
{
	// INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SKYBOX_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SKYBOX_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");


	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
}

void Engine::updateUniforms()
{
	// Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	this->shaders[SKYBOX_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

	lights[0] = new glm::vec3(2.5f * sin(glfwGetTime()), 1.f, 2.5f * cos(glfwGetTime()));

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");

	// Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SKYBOX_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

// Constructors / Destructors
Engine::Engine(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	// Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->cameraPosition = glm::vec3(0.f, 0.f, 1.f);
	this->cameraUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->deltaTime = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Engine::~Engine()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];
}

// Accessor
int Engine::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

// Modifier
void Engine::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// Functions
void Engine::updatedeltaTime()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->deltaTime = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Engine::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	// Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	// Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Engine::updateKeyboardInput()
{
	// Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	// Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.move(this->deltaTime, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.move(this->deltaTime, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.move(this->deltaTime, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.move(this->deltaTime, RIGHT);
	}
}

void Engine::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(deltaTime, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Engine::update()
{
	// UPDATE INPUT ---
	this->updatedeltaTime();
	this->updateInput();

	this->models[3]->rotate(glm::vec3(0.f, 1.f, 0.f));


	//this->models[3]->move(glm::vec3(0.f, 0.f, -0.002f), 8);
	// this->models[3]->rotate(glm::vec3(0.f, 1.f, 0.f));

}

void Engine::render()
{
	
	// DRAW ---
	// Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Update the uniforms
	this->updateUniforms();

	
	//skybox->render(this->shaders[SKYBOX_PROGRAM]);

	// Render models
	for (auto&i : this->models) {
		//if (first) {
			//i->render(this->shaders[SKYBOX_PROGRAM]);
			//glDepthFunc(GL_LESS);
			//first = false;
		//}
		//else
			i->render(this->shaders[SHADER_CORE_PROGRAM]);
	}
	first = true;
	
	// End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

