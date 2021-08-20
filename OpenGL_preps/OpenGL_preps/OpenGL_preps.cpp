#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;


//vertex shader
static const char* vShader = "                      \n\
#version 330                                        \n\
                                                    \n\
layout (location = 0) in vec3 pos;                  \n\
                                                    \n\
uniform mat4 model;                                 \n\
                                                    \n\
void main()                                         \n\
{gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);}";

//fragment shader

static const char* fShader = "                      \n\
#version 330                                        \n\
                                                    \n\
out vec4 colour;                                    \n\
                                                    \n\
void main()                                         \n\
{colour = vec4(1.0, 1.0, 0.0, 1.0);}";


void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: %s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();

    if (!shader) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: %s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: %s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
    //initialize GLFW
    if (!glfwInit()) {
        printf("GLWF initialization failed!");
        glfwTerminate();
        return 1;
    }

    //Setup GLFW window properties
    //OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //core profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    //Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Set the context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    //setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    //loop until windows closed
    while (!glfwWindowShouldClose(mainWindow)) {
        //get and handle user input events
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }

        curAngle += 0.03f;
        if (curAngle >= 360) {
            curAngle -= 360;
        }

        if (sizeDirection)
        {
            curSize += 0.0001f;
        }
        else {
            curSize -= 0.0001f;
        }

        if (curSize >= maxSize || curSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }

        //clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //red
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
        model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }
    

    return 0;
}

