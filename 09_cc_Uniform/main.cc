// main.cc

// using uniform variable in fragment shader

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

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

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Uniform", NULL, NULL);
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

  // SHADERS ///////////////////////////////////////////////////
  const char* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\n";

  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1 /* number of strings */, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "\n"
  "uniform vec4 color;"
  "\n"
  "void main()\n"
  "{\n"
  "  FragColor = color;\n"
  "}\n";
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // END SHADERS ///////////////////////////////////////////////////

  // vertices data (a triangle)
  float triangleVertices[] = {
    -0.5f,  -0.5f,  0.0f,
     0.5f,  -0.5f,  0.0f,
     0.0f,   0.5f,  0.0f
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
                          3*sizeof(float),  // stride
                          (void*)0          // start at
  );
  glEnableVertexAttribArray(0);
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
    glUseProgram(shaderProgram);
    // set the uniform color
    float t = glfwGetTime();
    float green = (sin(t) + 1.0f) * 0.5f;
    int vertexColor = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(vertexColor, 0.0f, green, 0.0f, 1.0f);
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
