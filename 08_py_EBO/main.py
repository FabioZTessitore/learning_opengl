# main.py

# create two VAO

import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy

(SCREEN_WIDTH, SCREEN_HEIGHT) = (800, 600)


# handle window resizing
def framebuffer_size_callback(window, width, height):
    glViewport(0, 0, width, height)

# handle key press
def key_callback(window, key, scancode, action, mode):
    if key == glfw.KEY_ESCAPE and action == glfw.PRESS:
        glfw.set_window_should_close(window, GL_TRUE)

def main():
    if not glfw.init():
        return -1

    #configure glfw (using OpenGL 3.3 Core)
    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

    # create the window
    window = glfw.create_window(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", None, None)
    if not window:
        glfw.terminate()
        return -1
    glfw.make_context_current(window)
    # set callbacks
    glfw.set_window_size_callback(window, framebuffer_size_callback)
    glfw.set_key_callback(window, key_callback)

    # set the OpenGL viewport to the whole window
    (width, height) = glfw.get_framebuffer_size(window)
    glViewport(0, 0, width, height)

    # SHADERS #########################################################
    vertex_shader_source = """
    #version 330 core

    layout (location = 0) in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0f);
    }
    """

    orange_fragment_shader_source = """
    #version 330 core

    out vec4 color;

    void main()
    {
        color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
    """

    green_fragment_shader_source = """
    #version 330 core

    out vec4 color;

    void main()
    {
        color = vec4(0.5f, 1.0f, 0.2f, 1.0f);
    }
    """

    vertex_shader = OpenGL.GL.shaders.compileShader(vertex_shader_source, GL_VERTEX_SHADER)
    orange_fragment_shader = OpenGL.GL.shaders.compileShader(orange_fragment_shader_source, GL_FRAGMENT_SHADER)
    green_fragment_shader = OpenGL.GL.shaders.compileShader(green_fragment_shader_source, GL_FRAGMENT_SHADER)
    orange_shader = OpenGL.GL.shaders.compileProgram(vertex_shader, orange_fragment_shader)
    green_shader = OpenGL.GL.shaders.compileProgram(vertex_shader, green_fragment_shader)
    # END SHADERS #########################################################

    # vertices data (a triangle)
    triangle = [
        -0.5,  0.75, 0.0,
        -0.9, -0.75, 0.0,
        -0.1, -0.75, 0.0
    ]
    triangle = numpy.array(triangle, dtype=numpy.float32)

    triangleVAO = glGenVertexArrays(1)
    triangleVBO = glGenBuffers(1)

    glBindVertexArray(triangleVAO)
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO)
    glBufferData(GL_ARRAY_BUFFER, triangle.itemsize * len(triangle), triangle, GL_STATIC_DRAW)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, None)
    glEnableVertexAttribArray(0)
    glBindVertexArray(0)

    # vertices data (a square)
    square = [
    0.1,  -0.4, 0.0,
    0.1,   0.4, 0.0,
    0.9,  -0.4, 0.0,
    0.9,   0.4, 0.0
    ]
    square = numpy.array(square, dtype=numpy.float32)
    squareIndices = [
        0, 1, 2,    # first triangle
        1, 2, 3     # second triangle
    ]
    squareIndices = numpy.array(squareIndices, dtype=numpy.uint32)

    squareVAO = glGenVertexArrays(1)
    squareVBO = glGenBuffers(1)
    squareEBO = glGenBuffers(1)

    glBindVertexArray(squareVAO)
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO)
    glBufferData(GL_ARRAY_BUFFER, square.itemsize * len(square), square, GL_STATIC_DRAW)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, squareIndices.itemsize * len(squareIndices), squareIndices, GL_STATIC_DRAW)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, None)
    glEnableVertexAttribArray(0)
    glBindVertexArray(0)

    # game loop
    while not glfw.window_should_close(window):
        # set the color buffer
        glClearColor(0.2, 0.3, 0.3, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        glUseProgram(orange_shader)
        glBindVertexArray(triangleVAO)
        glDrawArrays(GL_TRIANGLES, 0, 3)

        glUseProgram(green_shader)
        glBindVertexArray(squareVAO)
        glDrawElements(GL_TRIANGLES, len(squareIndices), GL_UNSIGNED_INT, None)

        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.terminate()

if __name__ == '__main__':
    main()
