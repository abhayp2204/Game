#ifndef MOVE_H
#define MOVE_H

void processInput(GLFWwindow* window, Maze& world, Entity &player, Bullet& bullet)
{
    // Escape = Quit Game
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
	    glfwSetWindowShouldClose(window, true);
        gameOverLose(player);
        exit(0);
    }

    // WASD Controls
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if(player.move(MOVE_UP, PLAYER_SPEED, world) && player.follow)
            cameraPos += player.speed * cameraUp;
        player.lookingAt = NORTH;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if(player.move(MOVE_DOWN, PLAYER_SPEED, world) && player.follow)
            cameraPos -= player.speed * cameraUp;
            player.lookingAt = SOUTH;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if(player.move(MOVE_LEFT, PLAYER_SPEED, world) && player.follow)
            cameraPos -= player.speed * cameraRight;
        player.lookingAt = WEST;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if(player.move(MOVE_RIGHT, PLAYER_SPEED, world) && player.follow)
            cameraPos += player.speed * cameraRight;
            player.lookingAt = EAST;
    }

    // Space = Shoot
    // cout << "Dir = " << player.lookingAt << endl;
    // cout << bullet.isFired << endl;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS &&
       glfwGetTime() - bullet.fireTime >= COOLDOWN)
    {
        bullet.direction = player.lookingAt;
        bullet.fireTime = glfwGetTime();
        player.shoot(player, bullet);
    }

    // Lights
    toggleLights(window, world, player, GLFW_KEY_L);
}

void moveZombie(Maze& world, Entity& player, Entity& zombie)
{
    int zombieDirection = world.shortest_path(zombie.vertices, zombie.position, player.vertices, player.position);

    if(zombieDirection == MOVE_UP || zombieDirection == MOVE_DOWN)
    {
        zombie.move(zombieDirection, ZOMBIE_SPEED, world);
    }
    else if(zombieDirection == MOVE_LEFT || zombieDirection == MOVE_RIGHT)
    {
        zombie.move(zombieDirection, ZOMBIE_SPEED, world);
    }
}

#endif