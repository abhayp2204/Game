#include "apoc/variables.hpp"
#include "classes/door.hpp"
#include "utility.hpp"

#ifndef INTERACTIONS_H
#define INTERACTIONS_H

using namespace std;

// Functions
bool objectsCollided(Entity& obj1, Entity& obj2, Maze& world);
bool objectsCollided2(std::vector<GLfloat> vertices1, glm::vec3 position1,
                      std::vector<GLfloat> vertices2, glm::vec3 position2,
                      Maze& world);
void resurrectZombies(Entity Zombie[]);
float distanceFromEntity(Entity& player, floatPair pixel);

// Door
void playerAtDoor(Entity& player, Entity zombie[], Entity coin[], Door door[], Maze& world)
{
    // if(objectsCollided(player, door[level], world))
    if(objectsCollided2(player.vertices, player.position,
                       door[level].vertices, door[level].position,
                       world))
    {
        level++;
        
        invulnerableStartTime = glfwGetTime();
        player.invulnerable = true;

        numHit = 0;

        // Levels completed
        if(level == NUM_LEVELS)
        {
            totalLightsOffTime += lightsOffTime;
            player.score += (int)totalLightsOffTime;
            gameOverWin(player);
        }

        // Rescatter coins on moving to the next level
        scatterCoins(coin);
        // scatterCoins(posCoin, coin);

        // Zombies may be "stuck" :-) in a wall on generation of new maze
        // adjust takes care of this
        respawn(zombie, world);	// Was causing freeze
        resurrectZombies(zombie);
    }
}

bool objectsCollided(Entity& obj1, Entity& obj2, Maze& world)
{
    // Bounds for object 1 and 2
    std::pair<intPair, intPair> B1 = world.getBounds(obj1.vertices, obj1.position);
    std::pair<intPair, intPair> B2 = world.getBounds(obj2.vertices, obj2.position);

    return (B1 == B2);
}

bool objectsCollided2(std::vector<GLfloat> vertices1, glm::vec3 position1,
                      std::vector<GLfloat> vertices2, glm::vec3 position2,
                      Maze& world)
{
    std::pair<intPair, intPair> B1 = world.getBounds(vertices1, position1);
    std::pair<intPair, intPair> B2 = world.getBounds(vertices2, position2);

    return (B1 == B2);
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

void collectCoins(Entity &player, Entity coin[], Maze &world)
{
    for(int i = 0; i < NUM_COINS; i++)
    {
        if(coin[i].collected)
            continue;

        if(!objectsCollided(player, coin[i], world))
            continue;
        
        player.score += COIN_VALUE;
        coin[i].collected = true;
    }
}

#endif