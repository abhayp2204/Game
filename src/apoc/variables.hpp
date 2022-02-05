#ifndef VARIABLES_H
#define VARIABLES_H

// Dimensions
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define CELL_WIDTH 180
#define CELL_HEIGHT 180
#define MAZE_WIDTH 11
#define MAZE_HEIGHT 11

// Game Parameters
#define NUM_LEVELS 5
#define NUM_COINS 20
#define COIN_VALUE 10
#define LUMINOSITY 0.25
#define COOLDOWN 1
#define IMMORTAL 1
#define INVULNERABLE_TIME 2
#define FREQUENCY 5

// Door
#define NORMAL  101
#define SPECIAL 102
#define FINAL   103

int level = 0;
float invulnerableStartTime;
int numZombiesAlive;
int numHit = 0;

// Speed
#define PLAYER_SPEED 0.0040
#define ZOMBIE_SPEED 0.0016
#define GHOST_SPEED 0.0007
// float zombieSpeed =  0.0005;
float ghostSpeed = 0.0009;
#define BULLET_SPEED 0.0070

// Maze
#define ff first
#define ss second
#define min(a, b)   (a<b?a:b)
#define max(a, b)   (a>b?a:b)

// World
#define PATH 0
#define WALL 1
float width = (float)CELL_WIDTH/SCREEN_WIDTH;
float height = (float)CELL_WIDTH/SCREEN_HEIGHT;

// Exit status
#define EXIT_FAIL -1
#define EXIT_SUCC 1
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

// Toggle Lights
bool key = 0;
bool prevKey = 0;
int toggleKey = 1;

unsigned int shaderProgram;

// Camera
glm::vec3 cameraPos   = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 cameraFront = glm::vec3(0, 0.0, -1.0);
glm::vec3 cameraUp    = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 cameraRight = glm::vec3(1.0, 0.0, 0.0);

// Time
float start;
float stop;
float lightsOffTime;
float totalLightsOffTime = 0;

// Mouse
bool leftClick;

#endif