#include "defs.hpp"
#include "utility.hpp"

#ifndef GAME_H
#define GAME_H

using namespace std;

void gameOver(char* message, Player& player);
float distanceFromPlayer(Player& player, std::pair<float, float> pixel);

std::pair<float, float> randomSpawn()
{
	int x = rand() % MAZE_WIDTH;
	int y = rand() % MAZE_HEIGHT;
	while (x == MAZE_WIDTH / 2)
	{
		x = rand() % MAZE_WIDTH;
	}
	x = (float)x;
	y = (float)y;

	std::pair<float, float> pos = {-width * (x - (float)MAZE_WIDTH / 2 - 0.5),
								   height * (y - (float)MAZE_HEIGHT / 2 - 0.5)};

	return pos;
}

void spawnEntity(Player player[], float R, float G, float B)
{
    for(int i = 0; i < NUM_LEVELS; i++)
    {
        std::pair<float, float> pos = randomSpawn();
		int x = pos.ff;
		int y = pos.ss;

		while (mod(pos.ff) > 0.5 || mod(pos.ss) > 0.85)
			pos = randomSpawn();

		float color = 0.3f*i + 0.4f;
		player[i].init(pos.ff, pos.ss, R, G, B, 0.000f, false, false, false);
    }
}

void adjust(Player player[], Maze& world)
{
    for(int i = 0; i < NUM_LEVELS; i++)
    {
        while(world.can_move(player[i].vertices, player[i].position) == EXT_FAIL)
        {
            // player[i].position += ZOMBIE_SPEED*glm::vec3(1.0f, 0.0f, 0.0f);
            player[i].position[0] += 0.0001;
        }
    }
}

// Scatter coins across the maze
void scatterCoins(std::pair<float, float> posCoin[], Player coin[])
{
    std::pair<float, float> pos;
    float x;
    float y;

	for (int i = 0; i < NUM_COINS; i++)
	{
		pos = randomSpawn();
		x = pos.ff;
		y = pos.ss;

		while (mod(x) > 0.5 || abs(y) > 0.85)
		{
			pos = randomSpawn();
			x = pos.ff;
			y = pos.ss;
		}

		posCoin[i] = pos;
		coin[i].init(posCoin[i].ff, posCoin[i].ss, 1.0f, 0.7f, 0.0f, 0.000f, false, false, false);
	}
}

bool atDoor(Player &player, Player &zombie, Maze &world)
{
    std::pair<std::pair<int, int>, std::pair<int, int>> pbounds = world.get_bounds(player.vertices, player.position);

    std::pair<std::pair<int, int>, std::pair<int, int>> bbounds = world.get_bounds(zombie.vertices, zombie.position);

    if(pbounds == bbounds)
    {
        level++;

        if(level == NUM_LEVELS)
            gameOver("WIN", player);

        return true;
    }
    return false;
}

bool zombieKilledPlayer(Player &player, Player &zombie, Maze &world)
{
    if(!zombie.alive)
        return false;

    std::pair<std::pair<int, int>, std::pair<int, int>> pbounds = world.get_bounds(player.vertices, player.position);

    std::pair<std::pair<int, int>, std::pair<int, int>> bbounds = world.get_bounds(zombie.vertices, zombie.position);

    if(pbounds == bbounds)
        return true;
    return false;
}

void coinCollected(Player &player, Player &coin, Maze &world)
{
    std::pair<std::pair<int, int>, std::pair<int, int>> pbounds = world.get_bounds(player.vertices, player.position);

    std::pair<std::pair<int, int>, std::pair<int, int>> bbounds = world.get_bounds(coin.vertices, coin.position);

    if(pbounds == bbounds)
    {
        if(!coin.collected)
        {
            player.score += 10;
        }
        coin.collected = true;
    }
}

void showStats(Player& player)
{
    clear();

    cout << "Level: " << level+1 << endl;
    cout << "Score: " << player.score << endl;
    cout << "Lights Off: " << lightsOffTime << "s" <<endl;
    cout << "Total Off: " << totalLightsOffTime << "s" <<endl;
}

float distanceFromPlayer(Player& player, std::pair<float, float> pixel)
{
    float x1 = player.position[0];
    float y1 = player.position[1];

    float x2 = pixel.ff;
    float y2 = pixel.ss;

    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void toggleLights(GLFWwindow* window, Maze& world, Player& player, int K)
{
    key = (glfwGetKey(window, K) == GLFW_PRESS);
    if(leaveKey(prevKey, key))
    {
        toggleKey = !toggleKey;

        if(toggleKey)
        {
            totalLightsOffTime += lightsOffTime;
            player.score += (int)totalLightsOffTime;
        }
    }
    if(toggleKey)
    {
        world.lightsOn();
        world.isLightOn = true;
        
        start = glfwGetTime();
    }
    else
    {
        world.lightsOff();
        world.isLightOn = false;
        
        stop = glfwGetTime();
        lightsOffTime = stop - start;
    }
    prevKey = (glfwGetKey(window, K) == GLFW_PRESS);
}

void gameOver(char* message, Player &player)
{
    clear();
    cout << "------------------------------------------------" << endl;
    cout << "-------------------- YOU " << message << " -------------------" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Level: " << level << endl;
    cout << "Score: " << player.score << endl;
    cout << "Lights: " << totalLightsOffTime << endl;
    exit(0);
}

#endif