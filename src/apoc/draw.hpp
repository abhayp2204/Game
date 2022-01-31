#include "variables.hpp"
#include "../classes/entity.hpp"
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
          Entity door[])
{
    // World
    world[level].draw(shaderProgram, window);

    // Player
    player.draw(shaderProgram, window);

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

    // Door
    door[level].draw(shaderProgram, window);
}

#endif