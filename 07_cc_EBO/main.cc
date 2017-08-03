// main.cc

// using EBO for the square

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Two Object, with EBO", NULL, NULL);
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

  // orange shader
  const char* orangeFragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n";
  GLuint orangeFragmentShader;
  orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
  glCompileShader(orangeFragmentShader);
  glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // green shader
  const char* greenFragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);\n"
  "}\n";
  GLuint greenFragmentShader;
  greenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(greenFragmentShader, 1, &greenFragmentShaderSource, NULL);
  glCompileShader(greenFragmentShader);
  glGetShaderiv(greenFragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(greenFragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // create two shader program
  GLuint orangeShaderProgram;
  orangeShaderProgram = glCreateProgram();
  glAttachShader(orangeShaderProgram, vertexShader);
  glAttachShader(orangeShaderProgram, orangeFragmentShader);
  glLinkProgram(orangeShaderProgram);
  glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(orangeShaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  GLuint greenShaderProgram;
  greenShaderProgram = glCreateProgram();
  glAttachShader(greenShaderProgram, vertexShader);
  glAttachShader(greenShaderProgram, greenFragmentShader);
  glLinkProgram(greenShaderProgram);
  glGetProgramiv(greenShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(greenShaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(orangeFragmentShader);
  glDeleteShader(greenFragmentShader);
  // END SHADERS ///////////////////////////////////////////////////

  // vertices data (a triangle)
  float triangleVertices[] = {
    -0.5f,   0.75f,  0.0f,
    -0.9f,  -0.75f,  0.0f,
    -0.1f,  -0.75f,  0.0f
  };

  GLuint triangleVAO, triangleVBO;
  glGenVertexArrays(1, &triangleVAO);
  glGenBuffers(1, &triangleVBO);

  // bind triangle data
  glBindVertexArray(triangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
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

  // vertices data (a square)
  // Now using EBO (element buffer object)
  // to handle duplicated vertices
  float squareVertices[] = {
    0.1f,  -0.4f, 0.0f,
    0.1f,   0.4f, 0.0f,
    0.9f,  -0.4f, 0.0f,
    0.9f,   0.4f, 0.0f
  };
  GLuint squareIndices[] = {
    0, 1, 2,  // first triangle
    1, 2, 3   // second triangle
  };

  GLuint squareVAO, squareVBO, squareEBO;
  glGenVertexArrays(1, &squareVAO);
  glGenBuffers(1, &squareVBO);
  glGenBuffers(1, &squareEBO);

  // bind square data
  glBindVertexArray(squareVAO);
  glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
  // store indices data in the VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
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

    // draw the trinagle
    // 1. select the shader program
    // 2. bind the VAO (it carry the VBO)
    // 3. draw!
    glUseProgram(orangeShaderProgram);
    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // draw the square (using glDrawElements)
    glUseProgram(greenShaderProgram);
    glBindVertexArray(squareVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 /* offset */);

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
