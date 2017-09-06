// main.cc

// shader class

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "shader.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
  glfwInit();

  // configure glfw (using OpenGL 3.3 Core)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shader Class", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // init GLAD before calling any OpenGL function
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  Shader ourShader("../shader/shader.vs", "../shader/shader.fs");

  // vertices data (a triangle)
  float triangleVertices[] = {
    // coords               // color
    -0.5f,  -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,
     0.0f,   0.5f,  0.0f,   0.0f, 0.0f, 1.0f
  };
  GLuint triangleIndices[] = {
    0, 1, 2
  };

  GLuint triangleVAO, triangleVBO, triangleEBO;
  glGenVertexArrays(1, &triangleVAO);
  glGenBuffers(1, &triangleVBO);
  glGenBuffers(1, &triangleEBO);

  // bind triangle data
  glBindVertexArray(triangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
  glVertexAttribPointer(0,                  // vertex attribute to configure (loc = 0)
                          3,                // size of vertex attribute, 3 coords
                          GL_FLOAT,         // type of data
                          GL_FALSE,         // don't normalize data
                          6*sizeof(float),  // stride
                          (void*)0          // start at
  );
  glVertexAttribPointer(1,                  // vertex attribute to configure (loc = 1)
                          3,                // size of vertex attribute, rgb
                          GL_FLOAT,         // type of data
                          GL_FALSE,         // don't normalize data
                          6*sizeof(float),  // stride
                          (void*)(3*sizeof(GL_FLOAT))  // start at
  );
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // game loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);

    // set the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // load the shader program
    ourShader.use();
    //
    float t = glfwGetTime();
    float delta = sin(t) * 0.5f;
    ourShader.setFloat("delta", delta);
    // draw
    glBindVertexArray(triangleVAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

// handler for window resizing (called ad startup)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// handle key press
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
