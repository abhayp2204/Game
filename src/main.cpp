// Include neccessary files in src
// The general files are included in "varaibles.hpp"

#include "apoc/variables.hpp"
#include "apoc/setup.hpp"
#include "apoc/move.hpp"
#include "apoc/draw.hpp"

#include "classes/world.hpp"
#include "classes/entity.hpp"
#include "classes/zombie.hpp"

#include "game.hpp"
#include "utility.hpp"

// Set std as the default namespace
// std::cout --> cout
using namespace std;

// Main function
int main()
{
	// Use time (in seconds) elapsed since epoch as the seed for srand
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

	floatPair posDoor = randomSpawn();
	floatPair posCoin[NUM_COINS];

	scatterCoins(coin);

	// Initialize Entities
	//          x    y    R     G     B     speed   ghost  follow
	player.init(0.0, 0.0, 0.0f, 1.0f, 0.2f, 0.005f, false, false);
	spawnEntity(zombie, 1.0, 0.0, 0.0);
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		floatPair pos = randomSpawn();
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

	while (!glfwWindowShouldClose(window))
	{
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
			numZombiesAlive += zombie[i].alive;

		// stats1(numZombiesAlive, numHit);

		// Door
		playerAtDoor(player, zombie, coin, door, world[level]);

		if(glfwGetTime() - invulnerableStartTime >= INVULNERABLE_TIME)
		{
			player.invulnerable = false;
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
		bullet.move(BULLET_SPEED);

		// Zombie Kills Player
		for(int i = 0; i <= level; i++)
		{
			if(IMMORTAL)
				break;
			if(player.invulnerable)
				break;
			if(!zombie[i].alive)
				continue;

			if (objectsCollided(player, zombie[i], world[0]))
			{
				totalLightsOffTime += lightsOffTime;
				player.score += (int)totalLightsOffTime;
				gameOverLose(player);
			}
		}

		// Collect coins
		collectCoins(player, coin, world[0]);

		// showStats(player);

		// Draw
		world[level].updateLights(player.vertices, player.position);
		// for(int i = 0; i <= level; i++)
			// updateZombieVisibility(player, zombie[i], world[level]);

		// Draw
		draw(shaderProgram, window, world, player, zombie, bullet, coin, door);
		// HUD(player);

		// End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Free GLFW resources
	glfwTerminate();
	return 0;
}