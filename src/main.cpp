#include <iostream>

#include "includes/gl.h"
#include "includes/gl.c"

#include "includes/sheader.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>


float width = 800, height = 600;
glm::vec3 color = glm::vec3(0.0, 0.5, 0.0);
float deltaTime = 0.0f, lastFrame = 0.0f;
float paddleY = 0.0f, paddleSpeed = 2.0f;



void pMove(Shader& myShader, GLFWwindow* window);
void paddleSet(Shader& myShader, GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void drawPaddle(std::vector <float>& vertices, std::vector<int>& indices);
void processInput(GLFWwindow* window);


std::vector<float> vertices = {
    // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f,  // top left
    -0.5f, -0.5f, 0.0f,  // bottom left
}; 

std::vector <int> indices = {
    0,1,3, //first triangle
    0,3,2,   //second triangle
};

int main ()
{

    //Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Pong", NULL, NULL);
    if(!window)
    {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1; 
    }
    glfwMakeContextCurrent(window);

    //load glad
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout <<"Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, width, height);

    //callback function for window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    Shader myShader("/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader0.vert", "/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader0.frag");
    
    
    

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        myShader.Use();
        myShader.setVec3("padColor", color);
        pMove(myShader, window);
        drawPaddle(vertices, indices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Pong Ready" << std::endl;
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
}


void processInput(GLFWwindow* window)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
}


void drawPaddle(std::vector <float>& vertices, std::vector<int>& indices)
{
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLuint iboId;
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
   

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);

    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);
}


void pMove(Shader& myShader, GLFWwindow* window)
{

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        paddleY += paddleSpeed * deltaTime;  // Move paddle up
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        paddleY -= paddleSpeed * deltaTime;  // Move paddle down   
    }

    glm::mat4 move = glm::mat4(1.0f);
    move = glm::translate(move, glm::vec3(0.0f, paddleY, 0.0f));
    move = glm::translate(move, glm::vec3(-0.8f, 0.0f, 0.0f));
    move = glm::scale(move, glm::vec3(0.02f, 0.20f, 0.0f));

    myShader.setthisMat4("model", move);
}

