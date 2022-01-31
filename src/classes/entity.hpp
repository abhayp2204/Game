#ifndef PLAYER_H
#define PLAYER_H

#include "../utility.hpp"
using namespace std;

class Bullet
{
public:
    // To draw the bullet
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    // Variables
    glm::vec3 position;
    int direction;
    bool isFired = false;
    bool isReady = true;
    float speed;
    float fireTime;

    void init(float x, float y);
    void draw(unsigned int shaderProgram, GLFWwindow* window);
    int move(float speed);
};

class Entity
{
public:
    // Variables
    glm::vec3 position;
    int score;
    bool alive;
    bool isGhost;
    bool follow;
    float speed;
    bool collected;
    int lookingAt;
    bool invulnerable;
    
    // To draw the player
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    // Functions
    void init(float x, float y, float red, float green, float blue, float speed, bool isGhost, bool follow);
    void draw(unsigned int shaderProgram, GLFWwindow* window);
    int move(unsigned int direction, float speed, Maze& maze);
    void shoot(Entity& player, Bullet& bullet);
};

void Entity::init(float x, float y, float red, float green, float blue, float s, bool ghost, bool f)
{
    float width = ((float)CELL_WIDTH/SCREEN_WIDTH) * 0.4;
    float height = ((float)CELL_WIDTH/SCREEN_HEIGHT) * 0.4;

    alive = true;
    score = 0;
    isGhost = ghost;
    speed = s;
    follow = f;
    collected = false;
    invulnerable = false;
    
    vertices.clear();

    // Rectangle
    for(int i = -1; i<=1; i+=2)
    {
        for(int j = -1; j<=1; j+=2)
        {
            // Pixel Position
            vertices.insert(vertices.end(), {(x+j*width/2), (y+i*height/2), 0});

            // Pixel Color
            vertices.insert(vertices.end(), {red, green, blue});
        }
    }
    for(unsigned int i = 0; i<=1; i++){
        indices.insert(indices.end(), {i, i+1, i+2});
    }

    vertices.insert(vertices.end(), {x+0.0f, y+height/2, 0.0f});
    vertices.insert(vertices.end(), {red, green, blue});
}

void Entity::draw(unsigned int shaderProgram, GLFWwindow *window){
    unsigned int VBO, VAO, EBO;

    glUseProgram(shaderProgram);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Make the camera look in the front direction
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

int Entity::move(unsigned int direction, float speed, Maze& maze)
{
    if(direction == MOVE_UP)
    {
        glm::vec3 newPosition = position + speed*glm::vec3(0.0f, 1.0f, 0.0f);

        if(isGhost || maze.canMove(vertices, newPosition) == EXIT_SUCC)
        {
            position = newPosition;
            return 1;
        }
        return 0;
    }
    if(direction == MOVE_DOWN)
    {
        glm::vec3 newPosition = position - speed*glm::vec3(0.0f, 1.0f, 0.0f);

        if(isGhost || maze.canMove(vertices, newPosition) == EXIT_SUCC)
        {
            position = newPosition;
            return 1;
        }
        return 0;
    }
    if(direction == MOVE_LEFT)
    {
        glm::vec3 newPosition = position - speed*glm::vec3(1.0f, 0.0f, 0.0f);

        if(isGhost || maze.canMove(vertices, newPosition) == EXIT_SUCC)
        {
            position = newPosition;
            return 1;
        }
        return 0;
    }
    if(direction == MOVE_RIGHT)
    {
        glm::vec3 newPosition = position + speed*glm::vec3(1.0f, 0.0f, 0.0f);

        if(isGhost || maze.canMove(vertices, newPosition) == EXIT_SUCC)
        {
            position = newPosition;
            return 1;
        }
        return 0;
    }
    return -1;
}

void Entity::shoot(Entity& player, Bullet& bullet)
{
    bullet.isFired = true;
    bullet.isReady = false;
    bullet.position = player.position;
}

void Bullet::init(float x, float y)
{
    float width = ((float)CELL_WIDTH/SCREEN_WIDTH) * 0.25;
    float height = ((float)CELL_WIDTH/SCREEN_HEIGHT) * 0.25;

    isFired = false;
    isReady = true;

    vertices.clear();
    indices.clear();

    // Rectangle
    for(int i = -1; i<=1; i+=2)
    {
        for(int j = -1; j<=1; j+=2)
        {
            // Pixel Position
            vertices.insert(vertices.end(), {(x+j*width/2), (y+i*height/2), 0});

            // Pixel Color
            vertices.insert(vertices.end(), {0.8, 0.8, 0.8});
        }
    }
    for(unsigned int i = 0; i<=1; i++){
        indices.insert(indices.end(), {i, i+1, i+2});
    }

    vertices.insert(vertices.end(), {x+0.0f, y+height/2, 0.0f});
    vertices.insert(vertices.end(), {0.8, 0.8, 0.8});
}

void Bullet::draw(unsigned int shaderProgram, GLFWwindow* window)
{
    unsigned int VBO, VAO, EBO;

    glUseProgram(shaderProgram);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // make the camera look in the 'front' direction
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

int Bullet::move(float speed)
{
    if(direction == NORTH)
    {
        position += speed*glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if(direction == SOUTH)
    {
        position -= speed*glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if(direction == WEST)
    {
        position -= speed*glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if(direction == EAST)
    {
        position += speed*glm::vec3(1.0f, 0.0f, 0.0f);
    }
    return -1;
}

#endif