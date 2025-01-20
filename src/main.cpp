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
glm::vec3 lcolor = glm::vec3(0.0, 0.5, 0.0);
glm::vec3 rcolor = glm::vec3(0.0, 0.2, 0.5);
float deltaTime = 0.0f, lastFrame = 0.0f;
float paddleY = 0.0f, paddleX= -0.8f, paddleSpeed = 2.5f;
float deltaTime1 = 0.0f, lastFrame1 = 0.0f;
float paddleY1 = 0.0f, paddleX1 = 0.8f;
float movex = 0.0f, lastFrameB = 0.0f, ballSpeed = 0.5f;
float ballX = 0.0f, ballY = 0.0f;

void ballMove(Shader& myShader);
void drawBall(std::vector <float>& vertices, std::vector<int>& indices);
std::vector<float> genCircle(float radius, int sectorCount);
std::vector<int> genIndices(int sectorCount);
void p2Move(Shader& myPaddleShader, GLFWwindow* window);
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

std::vector <float> vertices1 = {
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


    //Ball stuff
    std::vector<float> ballVertices = genCircle(0.0125, 100);
    std::vector<int> ballIndices = genIndices(100);
    
    Shader myShader("/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader0.vert", "/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader0.frag");
    Shader mypaddleShader("/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader1.vert", "/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader1.frag");
    Shader myballShader("/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader2.vert", "/home/ennom/Desktop/OpenGL/Pong/src/shaders/shader2.frag");
    
    

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        myShader.Use();
        myShader.setVec3("padColor", lcolor);
        pMove(myShader,window);
        drawPaddle(vertices, indices);

        mypaddleShader.Use();
        mypaddleShader.setVec3("padColor", rcolor);
        p2Move(mypaddleShader, window);
        drawPaddle(vertices1, indices);

        myballShader.Use();
        myballShader.setVec3("padColor", lcolor);
        ballMove(myballShader);
        drawBall(ballVertices, ballIndices);
        
        

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
    move = glm::translate(move, glm::vec3(paddleX, paddleY, 0.0f));
    move = glm::scale(move, glm::vec3(0.02f, 0.20f, 0.0f));

    myShader.setthisMat4("model", move);
}


void p2Move(Shader& myPaddleShader, GLFWwindow* window)
{

    float currentTime = glfwGetTime();
    float deltaTime1 = currentTime - lastFrame1;
    lastFrame1 = currentTime;

    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        paddleY1 += paddleSpeed * deltaTime1;  // Move paddle up
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        paddleY1 -= paddleSpeed * deltaTime1;  // Move paddle down   
    }

    glm::mat4 move = glm::mat4(1.0f);
    move = glm::translate(move, glm::vec3(paddleX1, paddleY1, 0.0f));
    move = glm::scale(move, glm::vec3(0.02f, 0.20f, 0.0f));

    myPaddleShader.setthisMat4("modelM", move);

}

std::vector<float> genCircle(float radius, int sectorCount)
{

    std::vector<float> vertices;

    float sectorStep = 2 * M_PI/sectorCount;
    float sectorAngle;

    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);

    for(int i= 0; i<= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        float x = radius * cos(sectorAngle);
        float y = radius * sin(sectorAngle);
        float z = 0;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

    }

    return vertices;
}

std::vector<int> genIndices(int sectorCount)
{
    std::vector<int>indices;

    for(int i = 0; i< sectorCount; ++i)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    return indices;
}

void  drawBall(std::vector <float>& vertices, std::vector<int>& indices)
{


    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    GLuint vboId;
    glGenBuffers( 1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    GLuint iboId;
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    //glDeleteBuffers(1, &txVBO);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &iboId);
    glDeleteVertexArrays(1, &vaoId); 


}

void ballMove(Shader& myShader)
{
    glm::vec3 ballpos = glm::vec3(ballX, ballY, 0.0f);
    
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastFrameB;
    lastFrameB = currentTime;
    ballX += deltaTime * ballSpeed;
    
    glm::mat4 move = glm::mat4(1.0f);
    
    if ( ballSpeed > 0 && paddleX1 - ballX <= 0.01)
    {
        ballSpeed = ballSpeed * -1;
    }

    else if (ballSpeed < 0 && abs(paddleX - ballX) <= 0.01)
    {
        ballSpeed = ballSpeed * -1; 
    }

    move= glm::translate(move, ballpos);
    
    myShader.setthisMat4("ballMove", move);
}

