#ifndef MOVE_H
#define MOVE_H

// Functions
void processInput(GLFWwindow* window, Maze& world, Entity &player, Bullet& bullet);
void moveZombie(Maze& world, Entity& player, Entity& zombie);

void move(GLFWwindow* window, Maze& world, Entity &player, Bullet& bullet, Entity zombie[])
{
    processInput(window, world, player, bullet);
    for(int i = 0; i <= level; i++)
		moveZombie(world, player, zombie[i]);
}

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

    // Shoot = Space
    bool shootPress = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || leftClick;
    bool inCooldown = glfwGetTime() - bullet.fireTime <= COOLDOWN;
    if(shootPress && !inCooldown)
    {
        
        bullet.direction = player.lookingAt;
        bullet.fireTime = glfwGetTime();
        player.shoot(player, bullet);
        leftClick = false;
    }
    /*
    If the player clicks during cooldown, leftClick is set to true and the player will shoot automatically when the cooldown is over.
    Set leftClick to false when the player shoots during cooldown
    */
    if(inCooldown)
    {
        leftClick = false;
    }

    // Lights
    toggleLights(window, world, player, GLFW_KEY_L);
}

void moveZombie(Maze& world, Entity& player, Entity& zombie)
{
    int zombieDirection = world.shortest_path(zombie.vertices, zombie.position, player.vertices, player.position);

    if(zombie.isGhost)
    {
        float u;
        float v;
        float sign;

        glm::vec3 offset = zombie.spawnPosition;
        glm::vec3 tempX = zombie.position + zombie.speed*glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 tempY = zombie.position + zombie.speed*glm::vec3(0.0f, 1.0f, 0.0f);

        // Initial separation
        u = vectorDistance(player.position, zombie.position, offset);

        // Horizontally movement
        v = vectorDistance(player.position, tempX, offset);
        sign = ((v<u)-0.5f)*2;
        zombie.position = zombie.position + sign*zombie.speed*glm::vec3(1.0f, 0.0f, 0.0f);

        // Vertical movement
        v = vectorDistance(player.position, tempY, offset);
        sign = ((v<u)-0.5f)*2;
        zombie.position = zombie.position + sign*zombie.speed*glm::vec3(0.0f, 1.0f, 0.0f);

        return;
    }

    if(zombieDirection == MOVE_UP || zombieDirection == MOVE_DOWN)
    {
        zombie.move(zombieDirection, zombie.speed, world);
    }
    else if(zombieDirection == MOVE_LEFT || zombieDirection == MOVE_RIGHT)
    {
        zombie.move(zombieDirection, zombie.speed, world);
    }
}

#endif