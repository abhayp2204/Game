#include "variables.hpp"
#include "../classes/world.hpp"
#include "../classes/entity.hpp"
#include "../utility.hpp"
#include "../game.hpp"

#ifndef SETUP_H
#define SETUP_H

using namespace std;
bool mouseInScreen;

// Functions
void clear();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursorPositionCallback(GLFWwindow* window, double x, double y);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Vertex Shader
const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = view * model * vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";


// Fragment Shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

GLFWwindow* setupWindow(GLFWwindow* window)
{
	// Initialize
	glfwInit();

	// Window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Apple
	#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

	// Create window
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", NULL, NULL);
	if(window == NULL)
	{
		cout << "Window: Failed to create window" << endl;
	}

	glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Mouse
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

	return window;
}

void setupShader(unsigned int &shaderProgram)
{
	// Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(1);
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        exit(1);
    }

    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		exit(1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(projectionLoc, SCREEN_WIDTH/SCREEN_HEIGHT, GL_FALSE, glm::value_ptr(projection));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void cursorPositionCallback(GLFWwindow* window, double x, double y)
{
    return;
    cout << "x position: " << x << endl;
    cout << "y position: " << y << endl;
    cout << endl;
}

void cursorEnterCallback(GLFWwindow* window, int entered)
{
    mouseInScreen = entered;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        leftClick = true;
    }

}

int updateZombieVisibility(Entity &player, Entity &bot, Maze& world){
    if(world.isLightOn){
        for(int i = 0; i<24; i+=6){
            bot.vertices[i+3] = 0.86f;
            bot.vertices[i+4] = 0.08f;
            bot.vertices[i+5] = 0.24f;
        }
        for(int i = 24; i<48; i+=6){
            bot.vertices[i+3] = 0.9000f;
            bot.vertices[i+4] =  0.9100f;
            bot.vertices[i+5] =  0.9800f;
        }
        for(int i = 48; i<bot.vertices.size(); i+=6){
            bot.vertices[i+3] = 0.86f;
            bot.vertices[i+4] = 0.08f;
            bot.vertices[i+5] = 0.24f;
        }
        return EXIT_SUCC;
    }


    std::pair<std::pair<int, int>, std::pair<int, int>> bounds = world.getBounds(player.vertices, player.position);

    std::vector<std::vector<int>> dist(world.rows, std::vector<int>(world.columns, -1));

    std::queue<std::pair<int, int>> q;

    // std::cout << bounds.ff.ff << " " << bounds.ff.ss << " " << bounds.ss.ff << " " << bounds.ss.ss << "\n";

    if(bounds.ff.ff != bounds.ff.ss && bounds.ss.ff != bounds.ss.ss){
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ss));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ss));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ss][bounds.ff.ff] = 0;
        dist[bounds.ss.ff][bounds.ff.ss] = 0;
        dist[bounds.ss.ss][bounds.ff.ss] = 0;
    }
    else if(bounds.ff.ff != bounds.ff.ss){
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ss, bounds.ss.ff));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ff][bounds.ff.ss] = 0;
    }
    else if(bounds.ss.ff != bounds.ss.ss){
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ss));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
        dist[bounds.ss.ss][bounds.ff.ff] = 0;
    }
    else{
        q.push(std::make_pair(bounds.ff.ff, bounds.ss.ff));
        dist[bounds.ss.ff][bounds.ff.ff] = 0;
    }

    // BFS
    while(!q.empty()){
        std::pair<int, int> cur = q.front();
        q.pop();
        if(world.maze[cur.ss][cur.ff].top == PATH && dist[cur.ss-1][cur.ff] == -1){
            dist[cur.ss-1][cur.ff] = dist[cur.ss][cur.ff] + 1;
            q.push(std::make_pair(cur.ff, cur.ss-1));
        }

        if(world.maze[cur.ss][cur.ff].bottom == PATH && dist[cur.ss+1][cur.ff] == -1){
            dist[cur.ss+1][cur.ff] = dist[cur.ss][cur.ff] + 1;
            q.push(std::make_pair(cur.ff, cur.ss+1));
        }

        if(world.maze[cur.ss][cur.ff].right == PATH && dist[cur.ss][cur.ff+1] == -1){
            dist[cur.ss][cur.ff+1] = dist[cur.ss][cur.ff] + 1;
            q.push(std::make_pair(cur.ff+1, cur.ss));
        }

        if(world.maze[cur.ss][cur.ff].left == PATH && dist[cur.ss][cur.ff-1] == -1){
            dist[cur.ss][cur.ff-1] = dist[cur.ss][cur.ff] + 1;
            q.push(std::make_pair(cur.ff-1, cur.ss));
        }
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> bot_bounds = world.getBounds(bot.vertices, bot.position);

    int scale = 1000;
    scale = min(scale, dist[bot_bounds.ss.ff][bot_bounds.ff.ff]);
    scale = min(scale, dist[bot_bounds.ss.ff][bot_bounds.ff.ss]);
    scale = min(scale, dist[bot_bounds.ss.ss][bot_bounds.ff.ff]);
    scale = min(scale, dist[bot_bounds.ss.ss][bot_bounds.ff.ss]);
    
    for(int i = 0; i<24; i+=6){
            bot.vertices[i+3] = max(0.0, 0.86*(1.0 - LUMINOSITY*scale));
            bot.vertices[i+4] = max(0.0, 0.08*(1.0 - LUMINOSITY*scale));
            bot.vertices[i+5] = max(0.0, 0.24*(1.0 - LUMINOSITY*scale));
        }
    for(int i = 24; i<48; i+=6){
            bot.vertices[i+3] = max(0.0, 0.90*(1.0 - LUMINOSITY*scale));
            bot.vertices[i+4] = max(0.0, 0.91*(1.0 - LUMINOSITY*scale));
            bot.vertices[i+5] = max(0.0, 0.98*(1.0 - LUMINOSITY*scale));
        }
    for(int i = 48; i<bot.vertices.size(); i+=6){
            bot.vertices[i+3] = max(0.0, 0.86*(1.0 - LUMINOSITY*scale));
            bot.vertices[i+4] = max(0.0, 0.08*(1.0 - LUMINOSITY*scale));
            bot.vertices[i+5] = max(0.0, 0.24*(1.0 - LUMINOSITY*scale));
        }
    return 0;
}

#endif