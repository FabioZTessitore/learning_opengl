// main.cc

// Create a VBO object to store vertex data
// Simple Vertex and Fragment Shaders

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
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Triangle", NULL, NULL);
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

  // vertices data (a triangle)
  float vertices[] = {
     0.0f,  0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f
  };

  // create a VBO to store vertices data into GPU memory
  GLuint VBO;
  glGenBuffers(1, &VBO);
  // bind the VBO to GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // load the vertices data
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // a simple vertex shader (source)
  // forward the position of the input vertex (from location 0 of the VBO)
  // to the predefined variable gl_Position
  const char* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\n";

  // create and compile the vertex shader
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1 /* number of strings */, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // fragment shader
  // output always (for all vertex) the same color
  const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

  // create a shader program
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

  // At this point we have the shader program (Vertex + Fragment Shader)
  // and the vertices data in the GPU memory by the VBO.
  // But, how to interpret that data?
  //
  // VBO -> x y z x y z x y z ... x y z
  //          V1    V2    V3        Vn
  // x, y, z are stored as 4 bytes float
  // data start at 0
  // the next vertex is after 4 bytes * 3 coords = 12 bytes (stride)
  glVertexAttribPointer(0,                // vertex attribute to configure (loc = 0)
                        3,                // size of vertex attribute, 3 coords
                        GL_FLOAT,         // type of data
                        GL_FALSE,         // don't normalize data
                        3*sizeof(float),  // stride
                        (void*)0          // start at
  );
  glEnableVertexAttribArray(0);

  // for every object we want to draw we need:
  // 0. copy our vertices array in a buffer for OpenGL to use
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 1. then set the vertex attributes pointers
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);
  // 2. use our shader program when we want to render an object
  // glUseProgram(shaderProgram);
  // 3. now draw the object
  // someOpenGLFunctionThatDrawsOurTriangle();
  //
  // See next project to see how to draw with the help of VAO

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
