#include "variables.hpp"

#ifndef WORLD_H
#define WORLD_H

class Node
{
public:
    int top;
    int bottom;
    int right;
    int left;

    // Constructor to set up the node, by default all the walls are set
    Node()
    {
        top = WALL;
        bottom = WALL;
        right = WALL;
        left = WALL;
    }

    int path(int);
};

// Adds a pathway in the specified directionection
int Node::path(int direction)
{
    if (direction == NORTH)
        top = PATH;
    else if (direction == SOUTH)
        bottom = PATH;
    else if (direction == EAST)
        right = PATH;
    else if (direction == WEST)
        left = PATH;
    else
        return EXIT_FAIL;
    return EXIT_SUCC;
}

class Maze
{
public:
    std::vector<std::vector<Node>> maze;
    int rows;
    int columns;

    bool isLightOn;
    int tasks;

    std::vector<GLfloat> wall_vertices;
    std::vector<unsigned int> wall_indices;

    std::vector<GLfloat> end_vertices;
    std::vector<unsigned int> end_indices;

    std::vector<GLfloat> bot_kill_vertices;
    std::vector<unsigned int> bot_kill_indices;

    std::vector<GLfloat> powerup_vertices;
    std::vector<unsigned int> powerup_indices;

    std::vector<GLfloat> powerups_vertices;
    std::vector<unsigned int> powerups_indices;

    std::pair<int, int> end;

    std::pair<int, int> bot_kill;

    std::pair<int, int> powerup;

    bool powerup_activated = false;

    std::vector<std::pair<std::pair<int, int>, int>> powerup_pos;

    Maze(int r, int c)
    {
        this->rows = r;
        this->columns = c;

        this->maze = std::vector<std::vector<Node>>(rows, std::vector<Node>(columns));

        this->isLightOn = true;
        this->tasks = 2;

        this->powerup_activated = false;
    }

    // Function to add a path in the maze
    int path(int, int, int);
    // Function to generate a maze procedurally
    int init(int);

    int draw(unsigned int, GLFWwindow *);

    int canMove(std::vector<GLfloat>, glm::vec3);

    std::pair<std::pair<int, int>, std::pair<int, int>> getBounds(std::vector<float>, glm::vec3);

    void updateLights(std::vector<float>, glm::vec3);

    int lightsOn();
    int lightsOff();

    int shortest_path(std::vector<float>, glm::vec3, std::vector<float>, glm::vec3);

    void activate_powerups();
};

int Maze::path(int r, int c, int direction)
{
    if (direction == NORTH)
    {
        if (r == 0)
            return EXIT_FAIL;

        maze[r][c].path(NORTH);
        maze[r - 1][c].path(SOUTH);
    }
    else if (direction == SOUTH)
    {
        if (r == rows - 1)
            return EXIT_FAIL;

        maze[r][c].path(SOUTH);
        maze[r + 1][c].path(NORTH);
    }
    else if (direction == WEST)
    {
        if (c == 0)
            return EXIT_FAIL;

        maze[r][c].path(WEST);
        maze[r][c - 1].path(EAST);
    }
    else if (direction == EAST)
    {
        if (c == columns - 1)
            return EXIT_FAIL;

        maze[r][c].path(EAST);
        maze[r][c + 1].path(WEST);
    }
    else
        return EXIT_FAIL;

    return EXIT_SUCC;
}

int Maze::init(int level)
{
    std::vector<std::vector<bool>> vis(rows, std::vector<bool>(columns, false));
    std::stack<std::pair<int, int>> S;

    // Seed is different for each level
    srand(time(NULL) * level);

    // Start at random location
    S.push(std::make_pair(rand() % rows, rand() % columns));

    // Generate a perfect maze
    while (!S.empty())
    {
        std::vector<int> direction;

        std::pair<int, int> current = S.top();
        vis[current.ff][current.ss] = true;

        if(current.ff != 0 && vis[current.ff - 1][current.ss] == false) direction.push_back(NORTH);
        if(current.ff != rows - 1 && vis[current.ff + 1][current.ss] == false) direction.push_back(SOUTH);
        if(current.ss != 0 && vis[current.ff][current.ss - 1] == false) direction.push_back(WEST);
        if(current.ss != columns - 1 && vis[current.ff][current.ss + 1] == false) direction.push_back(EAST);

        if (!direction.size())
        {
            S.pop();
            continue;
        }

        int flipper = direction[rand() % direction.size()];
        path(current.ff, current.ss, flipper);

        if (flipper == NORTH) S.push(std::make_pair(current.ff - 1, current.ss));
        if (flipper == SOUTH) S.push(std::make_pair(current.ff + 1, current.ss));
        if (flipper == WEST)  S.push(std::make_pair(current.ff, current.ss - 1));
        if (flipper == EAST)  S.push(std::make_pair(current.ff, current.ss + 1));

        direction.clear();
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (i != 0)
            {
                if (rand() % 9 < 1)
                    path(i, j, NORTH);
            }
            if (i != rows - 1)
            {
                if (rand() % 9 < 1)
                    path(i, j, SOUTH);
            }
            if (j != 0)
            {
                if (rand() % 9 < 1)
                    path(i, j, WEST);
            }
            if (j != columns - 1)
            {
                if (rand() % 9 < 1)
                    path(i, j, EAST);
            }
        }
    }

    float height = (float)CELL_WIDTH / SCREEN_HEIGHT;
    float width = (float)CELL_WIDTH / SCREEN_WIDTH;

    float y = height * columns / 2;
    float x = -width * rows / 2;

    for (int i = 0; i <= rows; i++)
    {
        for (int j = 0; j <= columns; j++)
        {
            // Position
            wall_vertices.insert(wall_vertices.end(), {(float)(x + j * width), y, 0});

            // Color
            wall_vertices.insert(wall_vertices.end(), {1.0f, 0.0f, 0.4f});
        }
        y -= height;
    }

    // Adding the walls to the buffer
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < columns; j++)
        {
            if (maze[i][j].top == WALL)
                wall_indices.insert(wall_indices.end(), {((columns + 1) * i + j), ((columns + 1) * i + j + 1)});

            if (maze[i][j].left == WALL)
                wall_indices.insert(wall_indices.end(), {((columns + 1) * i + j), ((columns + 1) * i + j + columns + 1)});

            if (j == columns - 1 && maze[i][j].right == WALL)
                wall_indices.insert(wall_indices.end(), {((columns + 1) * i + j + 1), ((columns + 1) * i + j + 1 + columns + 1)});

            if (i == rows - 1 && maze[i][j].bottom == WALL)
                wall_indices.insert(wall_indices.end(), {((columns + 1) * i + j + columns + 1), ((columns + 1) * i + j + 1 + columns + 1)});
        }
    }

    end = std::make_pair(rand() % MAZE_WIDTH, rand() % MAZE_HEIGHT);

    x = width * (end.ff - MAZE_WIDTH / 2);
    y = height * (MAZE_HEIGHT / 2 - end.ss);

    for (int i = -1; i <= 1; i += 2)
    {
        for (int j = -1; j <= 1; j += 2)
        {
            end_vertices.insert(end_vertices.end(), {x + j * width / 3, y + i * height / 3, 0});
            end_vertices.insert(end_vertices.end(), {0.19, 0.90, 0.37});
        }
    }
    for (unsigned int i = 0; i < 2; i++)
        end_indices.insert(end_indices.end(), {i, i + 1, i + 2});

    return EXIT_SUCC;
}

int Maze::draw(unsigned int shaderProgram, GLFWwindow *window)
{
    unsigned int VBO, VAO, EBO;

    glUseProgram(shaderProgram);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, wall_vertices.size() * sizeof(GLfloat), &wall_vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wall_indices.size() * sizeof(unsigned int), &wall_indices[0], GL_DYNAMIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // the model and view matrices,
    glm::mat4 model = glm::mat4(1.0f);

    // make the camera look in the 'front' directionection
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // assign the uniform values for model and view matrices
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glDrawElements(GL_LINES, wall_indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, end_vertices.size() * sizeof(GLfloat), &end_vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, end_indices.size() * sizeof(unsigned int), &end_indices[0], GL_DYNAMIC_DRAW);

    return 1;
}

int Maze::canMove(std::vector<GLfloat> vertices, glm::vec3 pos)
{

    float width = (float)CELL_WIDTH / SCREEN_WIDTH;
    float height = (float)CELL_WIDTH / SCREEN_HEIGHT;

    std::pair<float, float> zero = {-width * columns / 2, height * rows / 2};
    std::pair<float, float> endsX = {INF, -INF};
    std::pair<float, float> endsY = {INF, -INF};

    for (int i = 0; i < vertices.size(); i += 6)
    {
        if (vertices[i] > endsX.ss)
            endsX.ss = vertices[i];
        if (vertices[i] < endsX.ff)
            endsX.ff = vertices[i];
        if (vertices[i + 1] > endsY.ss)
            endsY.ss = vertices[i + 1];
        if (vertices[i + 1] < endsY.ff)
            endsY.ff = vertices[i + 1];
    }

    if ((pos.x + endsX.ff - zero.ff) / width < 0)
        return EXIT_FAIL;
    if ((pos.x + endsX.ss - zero.ff) / width > columns)
        return EXIT_FAIL;
    if ((zero.ss - pos.y - endsY.ff) / height > rows)
        return EXIT_FAIL;
    if ((zero.ss - pos.y - endsY.ss) / height < 0)
        return EXIT_FAIL;

    int lowX = (pos.x + endsX.ff - zero.ff) / width;
    int highX = (pos.x + endsX.ss - zero.ff) / width;

    int lowY = (zero.ss - pos.y - endsY.ff) / height;
    int highY = (zero.ss - pos.y - endsY.ss) / height;

    if (lowX != highX && lowY != highY)
    {
        if (lowX == -1 || highX == columns || lowY == -1 || highY == rows)
            return EXIT_FAIL;
        if (maze[highY][lowX].bottom == WALL || maze[highY][lowX].right == WALL)
            return EXIT_FAIL;
        if (maze[highY][highX].bottom == WALL || maze[highY][highX].left == WALL)
            return EXIT_FAIL;
        if (maze[lowY][lowX].top == WALL || maze[lowY][lowX].right == WALL)
            return EXIT_FAIL;
        if (maze[lowY][highX].top == WALL || maze[lowY][highX].left == WALL)
            return EXIT_FAIL;
        return EXIT_SUCC;
    }

    if (lowX != highX)
    {
        if (lowX == -1 || highX == columns || lowY == -1 || highY == rows)
            return EXIT_FAIL;
        if (maze[lowY][lowX].right == WALL)
            return EXIT_FAIL;
        if (maze[highY][lowX].right == WALL)
            return EXIT_FAIL;
        return EXIT_SUCC;
    }
    if (lowY != highY)
    {
        if (lowY == -1 || highY == rows || lowX == -1 || highX == columns)
            return EXIT_FAIL;
        if (maze[lowY][lowX].top == WALL)
            return EXIT_FAIL;
        if (maze[lowY][highX].top == WALL)
            return EXIT_FAIL;
        return EXIT_SUCC;
    }
    return EXIT_SUCC;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Maze::getBounds(std::vector<float> vertices, glm::vec3 pos)
{
    float width = (float)CELL_WIDTH / SCREEN_WIDTH;
    float height = (float)CELL_WIDTH / SCREEN_HEIGHT;

    std::pair<float, float> zero = {-width * columns / 2, height * rows / 2};
    std::pair<float, float> endsX = {INF, -INF};
    std::pair<float, float> endsY = {INF, -INF};

    for (int i = 0; i < vertices.size(); i += 6)
    {
        if (vertices[i] > endsX.ss)
            endsX.ss = vertices[i];
        if (vertices[i] < endsX.ff)
            endsX.ff = vertices[i];
        if (vertices[i + 1] > endsY.ss)
            endsY.ss = vertices[i + 1];
        if (vertices[i + 1] < endsY.ff)
            endsY.ff = vertices[i + 1];
    }

    std::pair<int, int> x, y;

    x.ff = (pos.x + endsX.ff - zero.ff) / width;
    x.ss = (pos.x + endsX.ss - zero.ff) / width;

    y.ff = (zero.ss - pos.y - endsY.ff) / height;
    y.ss = (zero.ss - pos.y - endsY.ss) / height;

    std::pair<std::pair<int, int>, std::pair<int, int>> bounds = std::make_pair(x, y);

    return bounds;
}

int Maze::lightsOff()
{
    isLightOn = false;
    return EXIT_SUCC;
}

int Maze::lightsOn()
{
    isLightOn = true;
    return EXIT_SUCC;
}

int Maze::shortest_path(std::vector<float> src_vertices, glm::vec3 src_pos, std::vector<float> dest_vertices, glm::vec3 dest_pos)
{
    std::pair<std::pair<int, int>, std::pair<int, int>> bounds = getBounds(dest_vertices, dest_pos);

    std::vector<std::vector<int>> dist(rows, std::vector<int>(columns, -1));

    std::queue<std::pair<int, int>> q;

    // std::cout << bounds.ff.ff << " " << bounds.ff.ss << " " << bounds.ss.ff << " " << bounds.ss.ss << "\n";

    if (bounds.ff.ff != bounds.ff.ss && bounds.ss.ff != bounds.ss.ss)
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ss));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ss));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ss][bounds.ff.ff] = 0;
        dist[bounds.ss.ff][bounds.ff.ss] = 0;
        dist[bounds.ss.ss][bounds.ff.ss] = 0;
    }
    else if (bounds.ff.ff != bounds.ff.ss)
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ff));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ff][bounds.ff.ss] = 0;
    }
    else if (bounds.ss.ff != bounds.ss.ss)
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ss));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ss][bounds.ff.ff] = 0;
    }
    else
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
    }

    // BFS
    while (!q.empty())
    {
        std::pair<int, int> current = q.front();
        q.pop();
        if (maze[current.ss][current.ff].top == PATH && dist[current.ss - 1][current.ff] == -1)
        {
            dist[current.ss - 1][current.ff] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff, current.ss - 1));
        }

        if (maze[current.ss][current.ff].bottom == PATH && dist[current.ss + 1][current.ff] == -1)
        {
            dist[current.ss + 1][current.ff] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff, current.ss + 1));
        }

        if (maze[current.ss][current.ff].right == PATH && dist[current.ss][current.ff + 1] == -1)
        {
            dist[current.ss][current.ff + 1] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff + 1, current.ss));
        }

        if (maze[current.ss][current.ff].left == PATH && dist[current.ss][current.ff - 1] == -1)
        {
            dist[current.ss][current.ff - 1] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff - 1, current.ss));
        }
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> src_bounds = getBounds(src_vertices, src_pos);

    int direction = 0;
    int m = INF;

    if (src_bounds.ff.ff != src_bounds.ff.ss && src_bounds.ss.ff != src_bounds.ss.ss)
    {
        if (dist[src_bounds.ss.ss][src_bounds.ff.ff] < dist[src_bounds.ss.ff][src_bounds.ff.ff])
        {
            direction = NORTH;
        }
        if (dist[src_bounds.ss.ss][src_bounds.ff.ss] < dist[src_bounds.ss.ff][src_bounds.ff.ff])
        {
            direction = NORTH;
        }
        else
        {
            direction = SOUTH;
        }
    }
    else if (src_bounds.ff.ff != src_bounds.ff.ss)
    {
        if (dist[src_bounds.ss.ff][src_bounds.ff.ff] < dist[src_bounds.ss.ff][src_bounds.ff.ss])
        {
            direction = WEST;
        }
        else
        {
            direction = EAST;
        }
    }
    else if (src_bounds.ss.ff != src_bounds.ss.ss)
    {
        if (dist[src_bounds.ss.ff][src_bounds.ff.ff] < dist[src_bounds.ss.ss][src_bounds.ff.ff])
        {
            direction = SOUTH;
        }
        else
        {
            direction = NORTH;
        }
    }
    else
    {
        if (maze[src_bounds.ss.ff][src_bounds.ff.ff].top == PATH)
        {
            if (dist[src_bounds.ss.ff - 1][src_bounds.ff.ff] < m)
            {
                direction = NORTH;
                m = dist[src_bounds.ss.ff - 1][src_bounds.ff.ff];
            }
        }
        if (maze[src_bounds.ss.ff][src_bounds.ff.ff].bottom == PATH)
        {
            if (dist[src_bounds.ss.ff + 1][src_bounds.ff.ff] < m)
            {
                direction = SOUTH;
                m = dist[src_bounds.ss.ff + 1][src_bounds.ff.ff];
            }
        }
        if (maze[src_bounds.ss.ff][src_bounds.ff.ff].right == PATH)
        {
            if (dist[src_bounds.ss.ff][src_bounds.ff.ff + 1] < m)
            {
                direction = EAST;
                m = dist[src_bounds.ss.ff][src_bounds.ff.ff + 1];
            }
        }
        if (maze[src_bounds.ss.ff][src_bounds.ff.ff].left == PATH)
        {
            if (dist[src_bounds.ss.ff][src_bounds.ff.ff - 1] < m)
            {
                direction = WEST;
                m = dist[src_bounds.ss.ff][src_bounds.ff.ff - 1];
            }
        }
    }

    return direction;
}

void Maze::updateLights(std::vector<float> vertices, glm::vec3 pos)
{
    if (isLightOn)
    {
        for (int i = 0; i < wall_vertices.size(); i += 6)
        {
            wall_vertices[i + 3] = 0.0f;
            wall_vertices[i + 4] = 1.0f;
            wall_vertices[i + 5] = 1.0f;
        }
        for (int i = 0; i < end_vertices.size(); i += 6)
        {
            end_vertices[i + 3] = 0.19;
            end_vertices[i + 4] = 0.90;
            end_vertices[i + 5] = 0.37;
        }
        return;
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> bounds = getBounds(vertices, pos);
    std::vector<std::vector<int>> dist(rows, std::vector<int>(columns, -1));
    std::queue<std::pair<int, int>> q;

    // std::cout << bounds.ff.ff << " " << bounds.ff.ss << " " << bounds.ss.ff << " " << bounds.ss.ss << "\n";

    if (bounds.ff.ff != bounds.ff.ss && bounds.ss.ff != bounds.ss.ss)
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ss));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ss));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ss][bounds.ff.ff] = 0;
        dist[bounds.ss.ff][bounds.ff.ss] = 0;
        dist[bounds.ss.ss][bounds.ff.ss] = 0;
    }
    else if (bounds.ff.ff != bounds.ff.ss)
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ff));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ff][bounds.ff.ss] = 0;
    }
    else if (bounds.ss.ff != bounds.ss.ss)
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ss));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ss][bounds.ff.ff] = 0;
    }
    else
    {
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
    }

    // BFS
    while (!q.empty())
    {
        std::pair<int, int> current = q.front();
        q.pop();
        if (maze[current.ss][current.ff].top == PATH && dist[current.ss - 1][current.ff] == -1)
        {
            dist[current.ss - 1][current.ff] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff, current.ss - 1));
        }

        if (maze[current.ss][current.ff].bottom == PATH && dist[current.ss + 1][current.ff] == -1)
        {
            dist[current.ss + 1][current.ff] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff, current.ss + 1));
        }

        if (maze[current.ss][current.ff].right == PATH && dist[current.ss][current.ff + 1] == -1)
        {
            dist[current.ss][current.ff + 1] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff + 1, current.ss));
        }

        if (maze[current.ss][current.ff].left == PATH && dist[current.ss][current.ff - 1] == -1)
        {
            dist[current.ss][current.ff - 1] = dist[current.ss][current.ff] + 1;
            q.push(std::make_pair(current.ff - 1, current.ss));
        }
    }

    int vert;
    float scale;

    for (int i = 0; i < wall_vertices.size(); i += 6)
    {
        vert = i / 6;

        scale = 1000;

        if (vert % (MAZE_WIDTH + 1) - 1 >= 0 && vert / (MAZE_WIDTH + 1) - 1 >= 0)
            scale = min(scale, dist[vert / (MAZE_WIDTH + 1) - 1][vert % (MAZE_WIDTH + 1) - 1]);

        if (vert % (MAZE_WIDTH + 1) - 1 >= 0 && vert / (MAZE_WIDTH + 1) < MAZE_HEIGHT)
            scale = min(scale, dist[vert / (MAZE_WIDTH + 1)][vert % (MAZE_WIDTH + 1) - 1]);

        if (vert % (MAZE_WIDTH + 1) < MAZE_WIDTH && vert / (MAZE_WIDTH + 1) - 1 >= 0)
            scale = min(scale, dist[vert / (MAZE_WIDTH + 1) - 1][vert % (MAZE_WIDTH + 1)]);

        if (vert % (MAZE_WIDTH + 1) < MAZE_WIDTH && vert / (MAZE_WIDTH + 1) < MAZE_HEIGHT)
            scale = min(scale, dist[vert / (MAZE_WIDTH + 1)][vert % (MAZE_WIDTH + 1)]);

        wall_vertices[i + 4] = max(0.0, 1.0 - LUMINOSITY * scale);
        wall_vertices[i + 5] = max(0.0, 1.0 - LUMINOSITY * scale);
    }

    for (int i = 0; i < end_vertices.size(); i += 6)
    {
        end_vertices[i + 3] = max(0.0, 0.19 * (1 - dist[end.ss][end.ff] * LUMINOSITY));
        end_vertices[i + 4] = max(0.0, 0.90 * (1 - dist[end.ss][end.ff] * LUMINOSITY));
        end_vertices[i + 5] = max(0.0, 0.37 * (1 - dist[end.ss][end.ff] * LUMINOSITY));
    }
}

#endif