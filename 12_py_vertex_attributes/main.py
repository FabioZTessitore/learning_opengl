# main.py

# vertex attributes

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
    layout (location = 1) in vec3 aColor;

    out vec3 vertexColor;

    void main()
    {
        gl_Position = vec4(position, 1.0f);
        vertexColor = aColor;
    }
    """

    fragment_shader_source = """
    #version 330 core

    in vec3 vertexColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(vertexColor, 1.0f);
    }
    """

    vertex_shader = OpenGL.GL.shaders.compileShader(vertex_shader_source, GL_VERTEX_SHADER)
    fragment_shader = OpenGL.GL.shaders.compileShader(fragment_shader_source, GL_FRAGMENT_SHADER)
    shader = OpenGL.GL.shaders.compileProgram(vertex_shader, fragment_shader)
    # END SHADERS #########################################################

    # vertices data (a triangle)
    triangle = [
        # coords             # color
        -0.5,  -0.5,  0.0,   1.0, 0.0, 0.0,
         0.5,  -0.5,  0.0,   0.0, 1.0, 0.0,
         0.0,   0.5,  0.0,   0.0, 0.0, 1.0
    ]
    triangle = numpy.array(triangle, dtype=numpy.float32)
    triangleIndices = [
        0, 1, 2
    ]
    triangleIndices = numpy.array(triangleIndices, dtype=numpy.uint32)

    triangleVAO = glGenVertexArrays(1)
    triangleVBO = glGenBuffers(1)
    triangleEBO = glGenBuffers(1)

    glBindVertexArray(triangleVAO)
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO)
    glBufferData(GL_ARRAY_BUFFER, triangle.itemsize * len(triangle), triangle, GL_STATIC_DRAW)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndices.itemsize * len(triangleIndices), triangleIndices, GL_STATIC_DRAW)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(ctypes.c_float), ctypes.c_void_p(0))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(ctypes.c_float), ctypes.c_void_p(3*sizeof(ctypes.c_float)))
    glEnableVertexAttribArray(0)
    glEnableVertexAttribArray(1)
    glBindVertexArray(0)

    # game loop
    while not glfw.window_should_close(window):
        # set the color buffer
        glClearColor(0.2, 0.3, 0.3, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        glUseProgram(shader)
        glBindVertexArray(triangleVAO)
        glDrawElements(GL_TRIANGLES, len(triangleIndices), GL_UNSIGNED_INT, None)

        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.terminate()

if __name__ == '__main__':
    main()
