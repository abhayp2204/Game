#include "variables.hpp"
#include "../classes/entity.hpp"
#include "../classes/door.hpp"
#include "../utility.hpp"

#ifndef DRAW_H
#define DRAW_H

using namespace std;

void draw(int shaderProgram,
          GLFWwindow* window,
          std::vector<Maze> world,
          Entity& player,
          Entity zombie[],
          Bullet& bullet,
          Entity coin[],
          Entity door[],
          Door door2[])
{
    // World
    world[level].draw(shaderProgram, window);

    // Player
    int t = (int)(glfwGetTime() * FREQUENCY);
    int digit = t%10;
    if(!player.invulnerable || digit%2)
    {
        player.draw(shaderProgram, window);
    }

    // Zombies
    for(int i = 0; i <= level; i++)
    {
        if(zombie[i].alive)
            zombie[i].draw(shaderProgram, window);
    }

    // Bullet
    if(bullet.isFired)
    {
        bullet.draw(shaderProgram, window);
    }

    // Coins
    for(int i = 0; i < NUM_COINS; i++)
    {
        if(!coin[i].collected)
            coin[i].draw(shaderProgram, window);
    }

    // Door
    // door[level].draw(shaderProgram, window);
    door2[level].draw(shaderProgram, window);
}

#endif