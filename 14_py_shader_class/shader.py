from OpenGL.GL import *
import OpenGL.GL.shaders

class Shader:
    def __init__(self, vertexShaderPath, fragmentShaderPath):
        vertexFile = open(vertexShaderPath, 'r')
        vertexShaderSource = vertexFile.read()
        vertexFile.close()

        fragmentFile = open(fragmentShaderPath, 'r')
        fragmentShaderSource = fragmentFile.read()
        fragmentFile.close()

        vertexShader = OpenGL.GL.shaders.compileShader(vertexShaderSource, GL_VERTEX_SHADER)
        fragmentShader = OpenGL.GL.shaders.compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER)
        self.ID = OpenGL.GL.shaders.compileProgram(vertexShader, fragmentShader)

    def use(self):
        glUseProgram(self.ID)

    def setBool(self, name, value):
        glUniform1i(glGetUniformLocation(self.ID, name), int(value))

    def setInt(self, name, value):
        glUniform1i(glGetUniformLocation(self.ID, name), value)

    def setFloat(self, name, value):
        glUniform1f(glGetUniformLocation(self.ID, name), value)
