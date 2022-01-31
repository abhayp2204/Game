#include "apoc/variables.hpp"
#include "utility.hpp"

#ifndef GAME_H
#define GAME_H

using namespace std;

void gameOverWin(Entity& player);
void gameOverLose(Entity& player);
float distanceFromEntity(Entity& player, floatPair pixel);

floatPair randomSpawn()
{
	int x = rand() % MAZE_WIDTH;
	int y = rand() % MAZE_HEIGHT;
	while (x == MAZE_WIDTH / 2)
	{
		x = rand() % MAZE_WIDTH;
	}
	x = (float)x;
	y = (float)y;

	floatPair pos = {-width * (x - (float)MAZE_WIDTH / 2 - 0.5),
								   height * (y - (float)MAZE_HEIGHT / 2 - 0.5)};

	return pos;
}

void spawnEntity(Entity zombie[], float R, float G, float B)
{
    for(int i = 0; i < NUM_LEVELS; i++)
    {
        floatPair pos = randomSpawn();
		int x = pos.ff;
		int y = pos.ss;

		while (mod(pos.ff) > 0.5 || mod(pos.ss) > 0.85)
			pos = randomSpawn();

		float color = 0.3f*i + 0.4f;
		zombie[i].init(pos.ff, pos.ss, R, G, B, 0.000f, false, false);
    }
}

void adjust(Entity player[], Maze& world)
{
    for(int i = 0; i < NUM_LEVELS; i++)
    {
        while(world.canMove(player[i].vertices, player[i].position) == EXIT_FAIL)
        {
            // player[i].position += ZOMBIE_SPEED*glm::vec3(1.0f, 0.0f, 0.0f);
            player[i].position[0] += 0.0001;
        }
    }
}

// Scatter coins across the maze
void scatterCoins(floatPair posCoin[], Entity coin[])
{
    floatPair pos;
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
		coin[i].init(posCoin[i].ff, posCoin[i].ss, 1.0f, 0.7f, 0.0f, 0.000f, false, false);
	}
}

bool objectsCollided(Entity& obj1, Entity& obj2, Maze& world)
{
    // Bounds for object 1 and 2
    std::pair<intPair, intPair> B1 = world.getBounds(obj1.vertices, obj1.position);
    std::pair<intPair, intPair> B2 = world.getBounds(obj2.vertices, obj2.position);

    return (B1 == B2);
}

void resurrectZombies(Entity zombie[])
{
    for(int i = 0; i < NUM_LEVELS; i++)
    {
        zombie[i].alive = true;
    }
}

bool bulletKillsZombie(Bullet& bullet, Entity& zombie, Maze& world)
{
    // Can't kill if it's already dead
    if(!zombie.alive)
        return false;

    // Bounds for object 1
    std::pair<std::pair<int, int>, std::pair<int, int>> B1 = world.getBounds(bullet.vertices, bullet.position);

    // Bounds for object 2
    std::pair<std::pair<int, int>, std::pair<int, int>> B2 = world.getBounds(zombie.vertices, zombie.position);

    // Bullet kills zombie
    if(B1 == B2)
    {
        return true;
    }
    return false;
}

void coinCollected(Entity &player, Entity &coin, Maze &world)
{
    // Bounds for object 1
    std::pair<std::pair<int, int>, std::pair<int, int>> B1 = world.getBounds(player.vertices, player.position);

    // Bounds for object 2
    std::pair<std::pair<int, int>, std::pair<int, int>> B2 = world.getBounds(coin.vertices, coin.position);

    // Collect coin
    if(B1 == B2)
    {
        if(!coin.collected)
        {
            player.score += COIN_VALUE;
        }
        coin.collected = true;
    }
}

void showStats(Entity& player)
{
    clear();

    cout << "Level: " << level+1 << endl;
    cout << "Score: " << player.score << endl;
    cout << "Lights Off: " << lightsOffTime << "s" <<endl;
    cout << "Total Off: " << totalLightsOffTime << "s" <<endl;
}

void stats1(int numZombiesAlive, int numHit)
{
    clear();

    cout << "Num Zombies = " << numZombiesAlive << endl;
    cout << "Num Hit = " << numHit << endl;
    cout << "------------------------------" << endl;
    cout << "Total = " << numZombiesAlive + numHit << endl;
    cout << "Level = " << level+1 << endl;
}

float distanceFromEntity(Entity& player, floatPair pixel)
{
    float x1 = player.position[0];
    float y1 = player.position[1];

    float x2 = pixel.ff;
    float y2 = pixel.ss;

    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void toggleLights(GLFWwindow* window, Maze& world, Entity& player, int K)
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

// void HUD(Entity& player){
//     GLTtext *text1 = gltCreateText();
// 	gltSetText(text1, "Hello World!");
//     char str[50];

//     gltBeginDraw();

//     gltColor(1.0f, 1.0f, 1.0f, 1.0f);
//     sprintf(str, "Score: %d", player.score);
//     gltSetText(text1, str);
//     gltDrawText2D(text1, 10.0f, 10.0f, 2.0f);

//     gltColor(1.0f, 1.0f, 1.0f, 1.0f);

//     gltSetText(text1, str);
//     gltDrawText2D(text1, 10.0f, 50.0f, 2.0f);

//     gltColor(1.0f, 1.0f, 1.0f, 1.0f);
//     gltSetText(text1, str);
//     gltDrawText2D(text1, 10.0f, 90.0f, 2.0f);

//     gltColor(1.0f, 1.0f, 1.0f, 1.0f);
//     gltSetText(text1, str);
//     gltDrawText2D(text1, 10.0f, 130.0f, 2.0f);

//     gltEndDraw();

// }

void gameOverWin(Entity &player)
{
    // clear();
    cout << "------------------------------------------------" << endl;
    cout << "-------------------- YOU WIN -------------------" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Level: " << level << endl;
    cout << "Score: " << player.score << endl;
    // cout << "Lights: " << totalLightsOffTime << endl;
    exit(0);
}

void gameOverLose(Entity &player)
{
    // clear();
    cout << "------------------------------------------------" << endl;
    cout << "-------------------- YOU LOSE ------------------" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Level: " << level << endl;
    cout << "Score: " << player.score << endl;
    // cout << "Lights: " << totalLightsOffTime << endl;
    exit(0);
}

#endif