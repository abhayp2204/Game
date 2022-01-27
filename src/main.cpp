// Header files
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unistd.h>

#include "camera.h"
#include "shader.h"

#define PI 3.14159265
#define PROPS 6

// Features
void translateObject(GLFWwindow* window);
void flyingCamera(GLFWwindow* window);
void prismFashionShow(GLFWwindow* window);

// Reset
void resetCamera();
void resetObject();
void resetSimulation();

// Misc
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;
using namespace std;

// Object
float x = 0.0f;
float y = 0.0f;
float z = 0.0f;
float objSpeed = 0.0012f;
bool objectTranslating = 0;
bool cameraTranslating = 0;
bool resetOnFly;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float camX = 0.0f;
float camY = 0.0f;
float camZ = 3.0f;
float radius = 3.0f;
float camSpeed = 0.0025f;
float alpha = 0.0f;
float revolutionAngle = 0.0f;
float revolutionSpeed = 0.003f;
glm::vec3 position1 = glm::vec3(4.0f, -2.0f, 4.0f);
glm::vec3 position2 = glm::vec3(-1.0f, 3.0f, -2.0f);

// Toggle T
bool T_pressed = 0;
bool T_pressedCurrently = 0;
bool T_toggle = 0;

// Toggle R
bool R_pressed = 0;
bool R_pressedCurrently = 0;
bool R_toggle = 0;

// Handle resets
bool translated;
bool fashion;

// Main function
int main()
{
	// Initialize
	glfwInit();

	// Window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shape", NULL, NULL);
	if(window == NULL)
	{
		cout << "Failed to create OpenGL Window" << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load OpenGL pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to intialize glad" << endl;
		return -1;
	}

	// Shaders
	Shader S("../src/vertex.shader", "../src/fragment.shader");

	// Input number of sides
	int n;
	cout << "Enter the number of sides" << endl;
	cin >> n;

	// Create the vertices
	float prism_vertices[12*n*PROPS];
										// N is the coefficient of phi ie Rcos(N*phi), Rsin(N*phi)
	float R = 0.3f;		   				// Radius of the prism
	float phi = 2*PI/n;    				// Internal angle of a polygon
	int k;								// k = index % 3
	int s;								// s = index / 3
	int N;								// N = k + s - 1

	// Front and Back faces
	for(int i = 0; i < n*3*2*PROPS; i += PROPS)
	{
		// Sign to change from front face to back face
		int sign = (i < n*3*PROPS? 1 : -1);
	
		int index = i/PROPS;

		if(index % 3 == 0)
		{
			prism_vertices[i] = 0;
			prism_vertices[i+1] = 0;
			prism_vertices[i+2] = 0.5*sign;
			prism_vertices[i+3] = 1.0;
			prism_vertices[i+4] = 0.0;
			prism_vertices[i+5] = 0.0;
			continue;
		}
		
		// Form the coefficient of phi
		k = index % 3;
		s = index/3;
		N = k + s - 1;

		prism_vertices[i+0] = R*glm::cos(N*phi);
		prism_vertices[i+1] = R*glm::sin(N*phi);
		prism_vertices[i+2] = 0.5 * sign;
		prism_vertices[i+3] = 0.0;

		if(k == 1)
		{
			prism_vertices[i+4] = 1.0;
			prism_vertices[i+5] = 0.0;
		}
		if(k == 2)
		{
			prism_vertices[i+4] = 0.0;
			prism_vertices[i+5] = 1.0;
		}
	}

	// Rectangles that join the front and back faces
	int base = 6*n*PROPS;
	for(int a = 0; a < n; a++)
	{
		// Triangle 1
		for(int i = 0; i < PROPS; i++)
		{
			if(i < PROPS/2) 
				prism_vertices[base++] = prism_vertices[(3*a + 1)*PROPS + i];
			else
				prism_vertices[base++] = 1 - prism_vertices[(3*a + 1)*PROPS + i];
		}
		for(int i = 0; i < PROPS; i++)
		{
			if(i < PROPS/2)
				prism_vertices[base++] = prism_vertices[(3*a + 2)*PROPS + i];
			else
				prism_vertices[base++] = 1 - prism_vertices[(3*a + 2)*PROPS + i];
		}
		for(int i = 0; i < PROPS; i++)
		{
			if(i < PROPS/2)
				prism_vertices[base++] = prism_vertices[(3*a + 3*n + 1)*PROPS + i];
			else
				prism_vertices[base++] = 1 - prism_vertices[(3*a + 3*n + 1)*PROPS + i];
		}

		// Triangle 2
		for(int i = 0; i < PROPS; i++)
		{
			if(i < PROPS/2)
				prism_vertices[base++] = prism_vertices[(3*a + 2)*PROPS + i];
			else
				prism_vertices[base++] = 1 - prism_vertices[(3*a + 2)*PROPS + i];
		}
		for(int i = 0; i < PROPS; i++)
		{
			if(i < PROPS/2)
				prism_vertices[base++] = prism_vertices[(3*a + 3*n + 1)*PROPS + i];
			else
				prism_vertices[base++] = 1 - prism_vertices[(3*a + 3*n + 1)*PROPS + i];
		}
		for(int i = 0; i < PROPS; i++)
		{
			if(i < PROPS/2)
				prism_vertices[base++] = prism_vertices[(3*a + 3*n + 2)*PROPS + i];
			else
				prism_vertices[base++] = 1 - prism_vertices[(3*a + 3*n + 2)*PROPS + i];
		}
	}

	unsigned int NUM_VERTICES = sizeof(prism_vertices)/(sizeof(prism_vertices[0])*6);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(prism_vertices), prism_vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Depth
	glEnable(GL_DEPTH_TEST);

	// Loop
	while(!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Render
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Identity matrix
		glm::mat4 transform = glm::mat4(1.0f);

		// Translation
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS 	||
		   glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS  ||
		   glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS 	||
		   glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS	||
		   glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS 	||
		   glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			objectTranslating = 1;
			translateObject(window);
		}
		else
			objectTranslating = 0;

		glm::vec3 OBJ_TRANSLATE = glm::vec3(x, y, z);
		transform = glm::translate(transform, OBJ_TRANSLATE);

		// Rotation: Toggle rotate when R key is released
		glm::vec3 OBJ_ROTATE = glm::vec3(0.0f, 1.0f, 0.0f);
		R_pressedCurrently = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

		// If R is released
		if(R_pressed && !R_pressedCurrently)
			R_toggle = !R_toggle;

		// Increment angle
		if(R_toggle)
			alpha += camSpeed;

		// Update
		R_pressed = R_pressedCurrently;

		transform = glm::rotate(transform, alpha, OBJ_ROTATE);

		// Shader
		S.use();
		S.setMat4("transform", transform);

		// View matrix
		glm::mat4 view = camera.GetViewMatrix(glm::vec3(0.0f));

		// Turntables
		T_pressedCurrently = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

		if((translated || fashion) && !T_pressed && T_pressedCurrently)
		{
			resetSimulation();
			translated = 0;
			fashion = 0;
		}

		// If T is released
		if(T_pressed && !T_pressedCurrently)
			T_toggle = !T_toggle;

		// Increment angle
		if(T_toggle)
		{
			cameraTranslating = 1;

			glm::vec3 prismVector = glm::vec3(x, y, z);
			glm::vec3 cameraVector = glm::vec3(camX, camY, camZ);
			float distance = glm::distance(prismVector, cameraVector);

			revolutionAngle += revolutionSpeed;
			camX = sin(revolutionAngle) * distance;
			camZ = cos(revolutionAngle) * distance;
		}
		T_pressed = T_pressedCurrently;

		// Fashion Show
		prismFashionShow(window);

		// Flying Camera
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS 	||
		   glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS     ||
		   glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS 	||
		   glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS		||
		   glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS 	||
		   glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			flyingCamera(window);
			cameraTranslating = 1;
		}
		else
			cameraTranslating = 0;

		// Update camera view
		view = glm::lookAt(glm::vec3(camX, camY, camZ),			// Camera position
						   glm::vec3(0.0f, 0.0f, 0.0f),			// Target position
						   glm::vec3(0.0f, 1.0f, 0.0f));		// Up
		S.setMat4("view", view);

		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
												(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
												0.1f, 100.0f);
		S.setMat4("projection", projection);

		glBindVertexArray(VAO);

		// Draw calls are expensive
		glDrawArrays(GL_TRIANGLES, 0, 12*n);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Clear all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

// Keybinds: Up, Down, Left, Right, O(+z), P(-z)
void translateObject(GLFWwindow* window)
{
	translated = 1;

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		y += objSpeed;
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		y -= objSpeed;
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		x -= objSpeed;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		x += objSpeed;
	if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		z += objSpeed;
	if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		z -= objSpeed;

	// Prepare for reset after translation
	resetOnFly = 1;
}

// Keybinds: W A S D E Q
void flyingCamera(GLFWwindow* window)
{
	/*
	We want to reset to the initial situation when we move/rotate
	the camera after having translated the prism.

	objectTranslating:
	cameraTranslating:
		Do not reset when either the object or the camera is in motion
		as this will cause a frozen display.

	resetOnFly:
		ensures that we do not reset when the user lets go
		of the WASDEQ keys i.e. avoid resetting if the user is
		only flying the camera without translating the prism
	*/

	if(!objectTranslating && !cameraTranslating && resetOnFly)
		resetSimulation();

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camY += camSpeed;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camY -= camSpeed;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camX -= camSpeed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camX += camSpeed;
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camZ += camSpeed;
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camZ -= camSpeed;

	resetOnFly = 0;
}

// Keybinds: N M
void prismFashionShow(GLFWwindow* window)
{
	fashion = 1;

	if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		resetSimulation();
						
		camX = position1.x;
		camY = position1.y;
		camZ = position1.z;
	}
	if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		resetSimulation();

		camX = position2.x;
		camY = position2.y;
		camZ = position2.z;
	}
}

void resetCamera()
{
	camX = 0.0f;
	camY = 0.0f;
	camZ = 3.0f;
}

void resetObject()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

void resetSimulation()
{
	resetCamera();
	resetObject();
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}