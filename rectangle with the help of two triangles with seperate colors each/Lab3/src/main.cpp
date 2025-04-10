#include "glad.h"
#include "glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

// First fragment shader (Magenta)
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n" // Magenta color (1, 0, 1)
    "}\n\0";

// Second fragment shader (Cyan)
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n" // Cyan color (0, 1, 1)
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tonny Talukder Anne", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile shader programs
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramMagenta = glCreateProgram();
    unsigned int shaderProgramCyan = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader1);
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader2);

    // link the first shader program (for Magenta)
    glAttachShader(shaderProgramMagenta, vertexShader);
    glAttachShader(shaderProgramMagenta, fragmentShader1);
    glLinkProgram(shaderProgramMagenta);

    // link the second shader program (for Cyan)
    glAttachShader(shaderProgramCyan, vertexShader);
    glAttachShader(shaderProgramCyan, fragmentShader2);
    glLinkProgram(shaderProgramCyan);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float firstTriangle[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         -0.5f,  0.5f, 0.0f  // top 
    };
    float secondTriangle[] = {
        0.5f,  0.5f, 0.0f, // top 
        0.5f, -0.5f, 0.0f, // right 
        -0.5f,  0.5f, 0.0f  // top 
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);

    // first triangle setup (Magenta)
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	
    glEnableVertexAttribArray(0);

    // second triangle setup (Cyan)
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // tightly packed data
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background color yellow
        glClear(GL_COLOR_BUFFER_BIT);

        // now when we draw the first triangle we use the Magenta fragment shader
        glUseProgram(shaderProgramMagenta);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // this call should output a Magenta triangle

        // now when we draw the second triangle we use the Cyan fragment shader
        glUseProgram(shaderProgramCyan);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // this call should output a Cyan triangle

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramMagenta);
    glDeleteProgram(shaderProgramCyan);

    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // pressing 'T' will close the window
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
