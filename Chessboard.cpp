#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 deviation;\n"
	"out vec3 deviation;\n"
    	"void main()\n"
    	"{\n"
    	"   gl_Position = vec4(aPos + deviation, 1.0);\n"
    	"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 deviation;\n"
	"void main()\n"
	"{\n"
	"	if ((floor(deviation.x/8)%2 && floor(deviation.y/8)%2) || ((floor(deviation.x/8)%2)&&(floor(deviation.x/8)%2))) \n"
	"   		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"	else FragColor = vec4(0.0f,0.0f,0.0f,1.0f);\n"
    	"}\n\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int shaderProgram = glCreateProgram();
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
    
    float x=0,y=0,z=0; 

    unsigned int VBO[64], VAO[64];
    glGenVertexArrays(64, VAO);
    glGenBuffers(64, VBO);
    glBindVertexArray(VAO);
    
    for (int i=0;i<64;i++) {
	x = i%8;
	y = i/8;
	float vertices[] = {         
         0.0f,  0.0f, 0.0f,   x,y,z,
         1.0f,  0.0f, 0.0f,   x,y,z,   
         0.0f,  1.0f, 0.0f,   x,y,z,   
	 1.0f,  1.0f, 0.0f,   x,y,z
        };

    	glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    	glEnableVertexAttribArray(0);
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        for(int i=0;i<64;i++) {
	    glBindVertexArray(VAO[i]); 
            glDrawArrays(GL_QUADS, 0, 4);
	}
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(64, VAO);
    glDeleteBuffers(64, VBO);
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window) {if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height);}