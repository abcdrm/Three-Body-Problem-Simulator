#include "Scene.h"
#include "Camera.h"
#include "MyThreeBody.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void initialize(int width, int height, string title);
void keyboardControl(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseControl(GLFWwindow* window, double xpos, double ypos);
void processKeyPress();

GLuint windowWidth = 1920;
GLuint windowHeight = 1080;

GLFWwindow* window;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool keyPress[1024];
bool isMouseInit = true;

GLfloat mouseCoordinateX = 400, mouseCoordinateY = 300;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

typedef struct
{
	glm::vec3 position;
	glm::vec3 color;
}Light;

Light lightSource;
MyThreeBody tb = MyThreeBody();

int main()
{
	initialize(windowWidth, windowHeight, "CG Project");

	tb.reset();
	Planet& p1 = tb.getPlanetA();
	Planet& p2 = tb.getPlanetB();
	Planet& p3 = tb.getPlanetC();

	// Scene ID Variables
	GLuint sun, rockPlanet, metalPlanet, background;

	// Scenes
	Scene sunScene(".\\model\\sun\\sun.obj", sun);
	sunScene.setModel(sun, true, glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 1.0));

	Scene rockScene(".\\model\\rock\\rock.obj", rockPlanet);
	rockScene.setModel(rockPlanet, true, glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0));

	Scene metalScene(".\\model\\metal\\metal.obj", metalPlanet);
	metalScene.setModel(metalPlanet, true, glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(1.0, 0.0, 0.0));

	Scene backgroundScene(".\\model\\background\\background.obj", background);
	backgroundScene.setModel(background, true, glm::vec3(0, 0, 0), glm::vec3(500, 500, 500), glm::vec3(0.0, 0.0, 1.0));

	// Shaders
	Shader shader("./shader/defaultshader.vs", "./shader/defaultshader.fs", SHADER_FROM_FILE);

	// Point of Light
	lightSource.position = glm::vec3(0.0, 0.0, 0.0);
	lightSource.color = glm::vec3(1, 1, 1);

	float radians = 0.0f;
	//------Main Loop------//
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrameTime = (GLfloat)glfwGetTime();
		deltaTime = currentFrameTime - lastFrame;
		lastFrame = currentFrameTime;

		// Check and call events
		glfwPollEvents();
		processKeyPress();

		// Clear the colorbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use shader
		shader.Use();

		// View and projection matrix setup
		glViewport(0, 0, windowWidth, windowHeight);
		glm::mat4 projection = glm::perspective(camera.zoom, (float)windowWidth / (float)windowHeight, 1.0f, 10000.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 VPMatrix = projection * view;
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "VPMatrix"), 1, GL_FALSE, glm::value_ptr(VPMatrix));

		lightSource.position.x = p1.planetCoordinate.x;
		lightSource.position.y = p1.planetCoordinate.y;
		lightSource.position.z = p1.planetCoordinate.z;

		// Setup Light
		glUniform3fv(glGetUniformLocation(shader.Program, "LightPos"), 1, glm::value_ptr(lightSource.position));
		glUniform3fv(glGetUniformLocation(shader.Program, "LightColor"), 1, glm::value_ptr(lightSource.color));

		// Draw using default shader
		tb.update(0.02);

		sunScene.setModel(sun, true, glm::vec3(p1.planetCoordinate.x, p1.planetCoordinate.y, p1.planetCoordinate.z), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 1.0));
		sunScene.Draw(shader, radians);
		rockScene.setModel(rockPlanet, true, glm::vec3(p2.planetCoordinate.x, p2.planetCoordinate.y, p2.planetCoordinate.z), glm::vec3(1.6, 1.6, 1.6), glm::vec3(0.0, 1.0, 0.0));
		rockScene.Draw(shader, radians);
		metalScene.setModel(metalPlanet, true, glm::vec3(p3.planetCoordinate.x, p3.planetCoordinate.y, p3.planetCoordinate.z), glm::vec3(0.3, 0.3, 0.3), glm::vec3(1.0, 0.0, 0.0));
		metalScene.Draw(shader, radians);

		backgroundScene.Draw(shader, 0.0f);

		radians += 0.5f;

		glfwSwapBuffers(window);
	}
	return 0;
}

void initialize(int width, int height, string title)
{
	//------GLFW Initiation------//
	if (glfwInit() == false) {
		cerr << "ERROR: GLFW initialization failed! -- (glfwinit)" << endl;
		system("PAUSE");
		glfwTerminate();
	}
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//------GLFW Window Creating------//
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		cerr << "ERROR: Main window creation failed! -- (glfwCreateWindow)" << endl;
		system("PAUSE");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	//------GLFW Options------//
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyboardControl);
	glfwSetCursorPosCallback(window, mouseControl);
	//glfwSetScrollCallback(window, scroll_callback);

	//enable glew for newer version of open GL
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cerr << "ERROR: GLEW initialization failed! -- (glewInit)" << endl;
		system("PAUSE");
	}

	//------Viewport Definition------//
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
}

// Is called whenever a key is pressed/released via GLFW
void keyboardControl(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keyPress[key] = true;
	else if (action == GLFW_RELEASE)
		keyPress[key] = false;
}

void mouseControl(GLFWwindow* window, double xpos, double ypos)
{
	if (isMouseInit)
	{
		mouseCoordinateX = (GLfloat)xpos;
		mouseCoordinateY = (GLfloat)ypos;
		isMouseInit = false;
	}

	GLfloat xOffset = (GLfloat)(xpos - mouseCoordinateX);
	GLfloat yOffset = (GLfloat)(mouseCoordinateY - ypos);

	mouseCoordinateX = (GLfloat)xpos;
	mouseCoordinateY = (GLfloat)ypos;

	camera.processMouse(xOffset, yOffset);
}

void processKeyPress()
{
	// Camera controls
	if (keyPress[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keyPress[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keyPress[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keyPress[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
	if (keyPress[GLFW_KEY_R])
		tb.reset();
}

