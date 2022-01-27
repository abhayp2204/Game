#include "defs.hpp"
#include "setup.hpp"
#include "player.hpp"

using namespace std;

// Functions
// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Main function
int main()
{
	GLFWwindow* window = setupWindow(window);
	if(window == NULL){return 1;}
	setupShader(shaderProgram);

	Player player;

	// Loop
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.attack();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}