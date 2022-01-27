#include "defs.hpp"
#include "setup.hpp"
#include "player.hpp"

using namespace std;

// Main function
int main()
{
	GLFWwindow* window = setupWindow(window);
	if(window == NULL){return 1;}
	setupShader(shaderProgram);

	Player player;
	player.init(0.0, 0.0);

	// Loop
	while(!glfwWindowShouldClose(window))
	{
		glUseProgram(shaderProgram);
		processInput(window, player);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.draw(shaderProgram, window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}