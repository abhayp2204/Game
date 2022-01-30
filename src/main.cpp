// Include fils in src
#include "variables.hpp"
#include "setup.hpp"
#include "world.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "utility.hpp"

// Set std as the default namespace
// std::cout --> cout
using namespace std;

// Main function
int main()
{
	// Use time elapsed since epoch as the seed for srand
	// Epoch: Jan 1st 1970
	srand(time(NULL));

	// Graphics setup: window and shader
	GLFWwindow *window = setupWindow(window);
	if (window == NULL) return 1;
	setupShader(shaderProgram);

	// Array of worlds
	std::vector<Maze> world(NUM_LEVELS, Maze(MAZE_WIDTH, MAZE_HEIGHT));

	// Entities
	Entity player;					// You are the player
	Entity zombie[NUM_LEVELS];		// Zombie chases the player
	Entity door[NUM_LEVELS];		// Player must reach the door at each level
	Entity coin[NUM_COINS];			// Collect coins to increase score

	std::pair<float, float> posZombie = randomSpawn();
	std::pair<float, float> posDoor = randomSpawn();
	std::pair<float, float> posCoin[NUM_COINS];

	scatterCoins(posCoin, coin);

	// Initialize Entities
	//          x    y    R     G     B     speed   ghost  follow
	player.init(0.0, 0.0, 0.0f, 1.0f, 0.2f, 0.005f, false, false);
	spawnEntity(zombie, 1.0, 0.0, 0.0);
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		std::pair<float, float> pos = randomSpawn();
		float x = pos.ff;
		float y = pos.ss;

		while (mod(x) > 0.5 || mod(y) > 0.85)
		{
			pos = randomSpawn();
			x = pos.ff;
			y = pos.ss;
		}

		if(i == NUM_LEVELS-1)
		{
			door[i].init(pos.ff, pos.ss, 1.0f, 1.0f, 1.0f, 0.000f, false, false);
			break;
		}
		door[i].init(pos.ff, pos.ss, 0.0f, 0.6f, 1.0f, 0.000f, false, false);
	}
	for(int i = 0; i < NUM_LEVELS; i++)
		world[i].init(i);
	
	std::pair<float, float> prevPos;
	std::pair<float, float> currentPos;

	// cout << world[level].isLightOn << endl;

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		// Paint the screen
		glUseProgram(shaderProgram);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Movement
		processInput(window, world[level], player);
		for(int i = 0; i <= level; i++)
			moveZombie(world[level], player, zombie[i]);

		// Door
		if(atDoor(player, door[level], world[0]))
		{
			// Rescatter coins on moving to the next level
			scatterCoins(posCoin, coin);

			// Zombies may be "stuck" :-) in a wall on generation of new maze
			// adjust takes care of this
			adjust(zombie, world[level]);
		}

		// Zombie Kills Entity
		for(int i = 0; i <= level; i++)
		{
			if (zombieKilledPlayer(player, zombie[i], world[0]))
			{
				totalLightsOffTime += lightsOffTime;
				player.score += (int)totalLightsOffTime;
				gameOverLose(player);
			}
		}

		// Collect Coins
		for (int i = 0; i < NUM_COINS; i++)
		{
			if (!coin[i].collected)
			{
				coinCollected(player, coin[i], world[0]);
				coin[i].draw(shaderProgram, window);
			}
		}
		// showStats(player);

		// Draw
		world[level].updateLights(player.vertices, player.position);
		for(int i = 0; i <= level; i++)
			updateZombieVisibility(player, zombie[i], world[level]);
		world[level].draw(shaderProgram, window);
		player.draw(shaderProgram, window);
		for(int i = 0; i <= level; i++)
			zombie[i].draw(shaderProgram, window);
		door[level].draw(shaderProgram, window);
		// HUD(player);

		// End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Free GLFW resources
	glfwTerminate();
	return 0;
}