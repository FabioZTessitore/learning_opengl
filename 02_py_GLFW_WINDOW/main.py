# main.py

# Open a GLFW Window, manage window color buffer, resizing and ESC press

import glfw
from OpenGL.GL import *

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

    # game loop
    while not glfw.window_should_close(window):
        # set the color buffer
        glClearColor(0.2, 0.3, 0.3, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.terminate()

if __name__ == '__main__':
    main()
