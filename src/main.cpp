// Include neccessary files in src
// The general files are included in "varaibles.hpp"
#include "variables.hpp"
#include "setup.hpp"
#include "world.hpp"
#include "entity.hpp"
#include "zombie.hpp"
#include "game.hpp"
#include "move.hpp"
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
	Bullet bullet;

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
	bullet.init(0.0f, 0.0f);
	
	std::pair<float, float> prevPos;
	std::pair<float, float> currentPos;

	// cout << world[level].isLightOn << endl;

	// Loop
	int numZombiesAlive;
	int numHit = 0;
	while (!glfwWindowShouldClose(window))
	{
		clear();

		// Paint the screen
		glUseProgram(shaderProgram);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Movement
		processInput(window, world[level], player, bullet);
		for(int i = 0; i <= level; i++)
			moveZombie(world[level], player, zombie[i]);

		numZombiesAlive = 0;
		for(int i = 0; i <= level; i++)
		{
			numZombiesAlive += (zombie[i].alive);
		}

		// cout << "Num Zombies = " << numZombiesAlive << endl;
		// cout << "Num Hit = " << numHit << endl;
		// cout << "------------------------------" << endl;
		// cout << "Total = " << numZombiesAlive + numHit << endl;
		// cout << "Level = " << level+1 << endl;

		// Door
		if(atDoor(player, door[level], world[0]))
		{
			level++;
			numHit = 0;

			// Levels completed
			if(level == NUM_LEVELS)
			{
				totalLightsOffTime += lightsOffTime;
				player.score += (int)totalLightsOffTime;
				gameOverWin(player);
			}

			// Rescatter coins on moving to the next level
			scatterCoins(posCoin, coin);

			// Zombies may be "stuck" :-) in a wall on generation of new maze
			// adjust takes care of this
			adjust(zombie, world[level]);
			resurrectZombies(zombie);
		}

		// Bullet kills Zombie
		for(int i = 0; i <= level; i++)
		{
			if(!bullet.isFired)
				break;
			if(bulletKillsZombie(bullet, zombie[i], world[level]))
			{
				numHit++;
				zombie[i].alive = false;
				bullet.isFired = false;
			}
		}

		// Zombie Kills Entity
		for(int i = 0; i <= level; i++)
		{
			if(IMMORTAL)
				break;
			if(!zombie[i].alive)
				continue;

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

		// Draw world and player
		world[level].draw(shaderProgram, window);
		player.draw(shaderProgram, window);

		// Draw zombies
		for(int i = 0; i <= level; i++)
		{
			if(zombie[i].alive)
				zombie[i].draw(shaderProgram, window);
		}

		// Draw door
		door[level].draw(shaderProgram, window);

		// Draw bullet
		if(bullet.isFired)
		{
			bullet.draw(shaderProgram, window);
		}
		bullet.move(BULLET_SPEED);
		// HUD(player);

		// End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Free GLFW resources
	glfwTerminate();
	return 0;
}