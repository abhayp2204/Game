#ifndef DEFS_H
#define DEFS_H

// Header files
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unistd.h>

#include "camera.h"
#include "shader.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define CELL_WIDTH 200
#define CELL_HEIGHT 200


// Directions
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4

// Player
#define PLAYER_SPEED 1

unsigned int shaderProgram;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 cameraFront = glm::vec3(0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 cameraRight = glm::vec3(1.0, 0.0, 0.0);

#endif