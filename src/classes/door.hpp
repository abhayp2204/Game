#ifndef DOOR_H
#define DOOR_H

#include "../utility.hpp"
using namespace std;

class Door
{
public:
    // Variables
    glm::vec3 position;
    int doorType;
    
    // To draw the door
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    // Functions
    void init(float x, float y, int doorType);
    void draw(unsigned int shaderProgram, GLFWwindow* window);
};

void Door::init(float x, float y, int type)
{
    // x = 0.24;
    // y = 0.42;

    float width = ((float)CELL_WIDTH/SCREEN_WIDTH) * 0.4;
    float height = ((float)CELL_WIDTH/SCREEN_HEIGHT) * 0.4;

    // Door color
    float r;
    float g;
    float b;
    switch(type)
    {
        case NORMAL:
            r = 170.0f/256.0f;
            g = 105.0f/256.0f;
            b = 62.0f/256.0f;
            break;
        case SPECIAL:
            r = 0.0;
            g = 1.0;
            b = 1.0;
            break;
        case FINAL:
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            break;
    }

    position = glm::vec3(x, y, 0);

    doorType = type;
    vertices.clear();

    // Rectangle
    for(int i = -1; i<=1; i+=2)
    {
        for(int j = -1; j<=1; j+=2)
        {
            // Pixel Position
            vertices.insert(vertices.end(), {(x+j*width/2), (y+i*height/2), 0});

            // Pixel Color
            vertices.insert(vertices.end(), {r, g, b});
        }
    }
    for(unsigned int i = 0; i<=1; i++){
        indices.insert(indices.end(), {i, i+1, i+2});
    }

    vertices.insert(vertices.end(), {x+0.0f, y+height/2, 0.0f});
    vertices.insert(vertices.end(), {r, g, b});
}

void Door::draw(unsigned int shaderProgram, GLFWwindow *window)
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

#endif