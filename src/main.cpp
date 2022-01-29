#include "defs.hpp"
#include "setup.hpp"
#include "world.hpp"
#include "player.hpp"
#include "game.hpp"
#include "utility.hpp"

using namespace std;

std::pair<float, float> randomSpawn();
std::pair<float, float> spawn();

// Main function
int main()
{
	srand(time(NULL));

	GLFWwindow *window = setupWindow(window);
	if (window == NULL)
	{
		return 1;
	}
	setupShader(shaderProgram);

	level = 0;
	std::vector<Maze> world(NUM_LEVELS, Maze(MAZE_WIDTH, MAZE_HEIGHT));

	Player player;
	Player zombie[NUM_LEVELS];
	Player door[NUM_LEVELS];
	Player coin[NUM_COINS];

	std::pair<float, float> posZombie = randomSpawn();
	std::pair<float, float> posDoor = randomSpawn();
	std::pair<float, float> posCoin[NUM_COINS];

	scatterCoins(posCoin, coin);

	//          x    y    R     G     B     speed   ghost  follow  semi
	player.init(0.0, 0.0, 0.0f, 1.0f, 0.2f, 0.005f, false, false , false);

	spawnEntity(zombie, 1.0, 1.0, 1.0);

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

		door[i].init(pos.ff, pos.ss, 0.0f, 0.6f, 1.0f, 0.000f, 0, 0, 0);
	}

	for(int i = 0; i < NUM_LEVELS; i++)
		world[i].init(i);
	
	std::pair<float, float> prevPos;
	std::pair<float, float> currentPos;

	cout << world[level].isLightOn << endl;

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(shaderProgram);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Movement
		processInput(window, world[level], player, zombie[level]);
		for(int i = 0; i <= level; i++)
			moveZombie(world[level], player, zombie[i]);


		// Door
		if(atDoor(player, door[level], world[0]))
		{
			scatterCoins(posCoin, coin);
			adjust(zombie, world[level]);
		}

		// Zombie Kills Player
		for(int i = 0; i <= level; i++)
		{
			if (zombieKilledPlayer(player, zombie[i], world[0]))
			{
				totalLightsOffTime += lightsOffTime;
				gameOver("LOSE", player);
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
		showStats(player);

		// Draw
		world[level].updateLights(player.vertices, player.position);
		for(int i = 0; i <= level; i++)
			updateZombieVisibility(player, zombie[i], world[level]);
		world[level].draw(shaderProgram, window);
		player.draw(shaderProgram, window);
		for(int i = 0; i <= level; i++)
			zombie[i].draw(shaderProgram, window);
		door[level].draw(shaderProgram, window);

		// End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all previously allocated GLFW resources
	glfwTerminate();

	return 0;
}