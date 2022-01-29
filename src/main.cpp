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
	srand(0);

	GLFWwindow *window = setupWindow(window);
	if (window == NULL)
	{
		return 1;
	}
	setupShader(shaderProgram);

	level = 0;
	Maze world1(MAZE_WIDTH, MAZE_HEIGHT);
	Maze world2(MAZE_WIDTH, MAZE_HEIGHT);
	Maze world3(MAZE_WIDTH, MAZE_HEIGHT);

	Player player;
	Player zombie[3];
	Player door[3];
	Player coin[NUM_COINS];

	std::pair<float, float> posZombie = randomSpawn();
	std::pair<float, float> posDoor = randomSpawn();
	std::pair<float, float> posCoin[NUM_COINS];

	scatterCoins(posCoin, coin);

	//            x       y      R    G    B   speed  ghost follow
	player.init(0.0, 0.0, 0.0, 1.0f, 0.2f, 0.005f, 0, 0);

	spawnEntity(zombie, 1.0, 1.0, 1.0);
	// for (int i = 0; i < 3; i++)
	// {
	// 	std::pair<float, float> pos = randomSpawn();
	// 	int x = pos.ff;
	// 	int y = pos.ss;

	// 	while (mod(x) > 0.5 || mod(y) > 0.85)
	// 	{
	// 		pos = randomSpawn();
	// 		x = pos.ff;
	// 		y = pos.ss;
	// 	}

	// 	float color = 0.3f*i + 0.4f;
	// 	zombie[i].init(pos.ff, pos.ss, color, color, color, 0.000f, 0, 0);
	// }

	for (int i = 0; i < 3; i++)
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

		door[i].init(pos.ff, pos.ss, 0.0f, 0.6f, 1.0f, 0.000f, 0, 0);
	}
	world1.init(1);
	world2.init(2);
	world3.init(3);
	
	std::pair<float, float> prevPos;
	std::pair<float, float> currentPos;

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(shaderProgram);

		// Switch between lavels
		switch(level)
		{
			case 0: processInput(window, world1, player, zombie[0]); break;
			case 1: processInput(window, world2, player, zombie[1]); break;
			case 2: processInput(window, world3, player, zombie[2]); break;
		}
		switch(level)
		{
			case 0:	moveZombie(world1, player, zombie[0]); break; 

			case 1:	moveZombie(world2, player, zombie[0]); 
					moveZombie(world2, player, zombie[1]); break;

			case 2:	moveZombie(world3, player, zombie[0]); 
					moveZombie(world3, player, zombie[1]); 
					moveZombie(world3, player, zombie[2]); break;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch(level)
		{
			case 0: world1.draw(shaderProgram, window); break;
			case 1: world2.draw(shaderProgram, window); break;
			case 2: world3.draw(shaderProgram, window); break;
		}

		// Door
		if(atDoor(player, door[level], world1))
		{
			scatterCoins(posCoin, coin);

			switch(level)
			{
				case 1: adjust(zombie, world2); break;
				case 2: adjust(zombie, world3); break;
			}
		}

		// Zombie Kills Player
		for(int i = 0; i <= level; i++)
		{
			if (zombieKilledPlayer(player, zombie[i], world1))
			{
				gameOver("LOSE", player);
			}
		}

		// Collecte Coins
		for (int i = 0; i < NUM_COINS; i++)
		{
			if (!coin[i].collected)
			{
				coinCollected(player, coin[i], world1);
				coin[i].draw(shaderProgram, window);
			}
			// showScore(player);
		}

		player.draw(shaderProgram, window);
		for(int i = 0; i <= level; i++)
			zombie[i].draw(shaderProgram, window);
		door[level].draw(shaderProgram, window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all previously allocated GLFW resources
	glfwTerminate();
	cout << "level: " << level << endl;

	return 0;
}