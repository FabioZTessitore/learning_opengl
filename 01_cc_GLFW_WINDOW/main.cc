// main.cc

// Open a GLFW Window, manage window color buffer, resizing and ESC press

// Testing the development environment
// $ mkdir build
// $ cd build
// $ cmake ..
// $ make
// $ ./main


// glad.h includes gl.h (must be the first one)
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// handle window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// handle key press
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

  // create the window
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW WINDOW", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  // set the callback for window resizing
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // init GLAD before calling any OpenGL function
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // set the OpenGL viewport to the whole window
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // game loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);

    // set the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
