// #ifndef ENEMY_H
// #define ENEMY_H

// class Enemy
// {
// public:
//     // To draw the enemy
//     std::vector<GLfloat> vertices;
//     std::vector<unsigned int> incides;

//     // Variables
//     glm::vec3 position;
//     bool alive;
//     unsigned int health;

//     // Functions
//     void init(float x, float y);
//     void draw(unsigned int shaderProgram, GLFWwindow* window);
//     void move(unsigned int direction, float speed);
//     void attack();
// }

// void Enemy::init(float x, float y)
// {
//     float width = ((float)CELL_WIDTH/SCREEN_WIDTH) * 0.3;
//     float height = ((float)CELL_WIDTH/SCREEN_HEIGHT) * 0.3;

//     // Rectangle
//     for(int i = -1; i<=1; i+=2)
//     {
//         for(int j = -1; j<=1; j+=2)
//         {
//             // Pixel Position
//             vertices.insert(vertices.end(), {(x+j*width/2), (y+i*height/2), 0});

//             // Pixel Color
//             vertices.insert(vertices.end(), {0.4f, 0.4f, 0.4f});
//         }
//     }
//     for(unsigned int i = 0; i<=1; i++){
//         indices.insert(indices.end(), {i, i+1, i+2});
//     }
// }

// void Enemy::draw(unsigned int shaderProgram, GLFWwindow* window)
// {
//     unsigned int VBO, VAO, EBO;

//     glUseProgram(shaderProgram);

//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);
    
//     // first parameter is for number of buffer objects to create
//     // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

//     // Position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     // Color attribute
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);

//     // make the camera look in the 'front' direction
//     glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//     unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

//     glm::mat4 model = glm::mat4(1.0f);
//     model = glm::translate(model, position);
    
//     unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

//     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
// }

// void Enemy::move(unsigned int direction, float speed)
// #endif