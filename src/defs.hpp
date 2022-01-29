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
#include <bits/stdc++.h>
#include <cmath>
#include "camera.h"
#include "shader.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define CELL_WIDTH 180
#define CELL_HEIGHT 180

#define NUM_LEVELS 10

// Maze
#define ff first
#define ss second
#define min(a, b)   (a<b?a:b)
#define max(a, b)   (a>b?a:b)

#define MAZE_WIDTH 11
#define MAZE_HEIGHT 11
#define NUM_COINS 10
#define GRADIENT 0.2

// World
#define PATH 0
#define WALL 1
float width = (float)CELL_WIDTH/SCREEN_WIDTH;
float height = (float)CELL_WIDTH/SCREEN_HEIGHT;

// Exit status
#define EXT_FAIL -1
#define EXT_SUCC 1
#define INF 1e9

// Directions
#define MOVE_UP 2
#define MOVE_DOWN 3
#define MOVE_LEFT 4
#define MOVE_RIGHT 5

#define NORTH 2
#define SOUTH 3
#define WEST 4
#define EAST 5

// Player
#define PLAYER_SPEED 0.0030
#define ZOMBIE_SPEED 0.0004

unsigned int shaderProgram;
int level;
bool key = 0;
bool prevKey = 0;
int toggleKey = 1;

glm::vec3 cameraPos   = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 cameraFront = glm::vec3(0, 0.0, -1.0);
glm::vec3 cameraUp    = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 cameraRight = glm::vec3(1.0, 0.0, 0.0);

#endif