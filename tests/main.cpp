#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

constexpr float colors[][3] = {
    {1.0f, 0.0f, 0.0f}, // Vermelho
    {0.0f, 1.0f, 0.0f}, // Verde
    {0.0f, 0.0f, 1.0f}, // Azul
    {1.0f, 1.0f, 1.0f}, // Branco
    {1.0f, 1.0f, 0.0f}  // Amarelo
};

class Shader
{
private:
    unsigned int vertexShader, fragmentShader, shaderProgram;
    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = ourColor;\n"
        "}\0";

public:
    Shader();
    void shaderDelet();
    void useColorMovement(int t, int c);
    ~Shader();
};

Shader::Shader()
{
    // Criar e compilar Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Verificação de erro
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Criar e compilar Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Verificação de erro
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Criar programa de shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verificar se a linkagem do programa foi bem-sucedida
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Remover shaders individuais após a linkagem
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::shaderDelet()
{
    glDeleteProgram(shaderProgram);
}

void Shader::useColorMovement(int t, int c)//1,0 movimento, 1 ... 0 cores
{
    glUseProgram(shaderProgram);

    if(t == 1)
    {    
        float color1Value = colors[c][0];
        float color2Value = colors[c][1];
        float color3Value = colors[c][2];

       
        float timeValue = glfwGetTime();

        if(color1Value == 1.0f){
            color1Value = sin(timeValue) / 2.0f + 0.5f ;
        }
        if(color2Value == 1.0f){
            color2Value = sin(timeValue) / 2.0f + 0.5f ;
        }
        if(color3Value == 1.0f){
            color3Value = sin(timeValue) / 2.0f + 0.5f ;
        }
        

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, color1Value, color2Value, color3Value, 1.0f);

    }else{
    
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, colors[c][0], colors[c][1], colors[c][2], 1.0f);
    
    }
}

Shader::~Shader(){}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Configurações da janela
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criar janela GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Criar o shader **após** inicializar OpenGL
    Shader shader;
    
    // Configuração dos triângulos
    float firstTriangle[] = {
        -0.5f,  0.5f, 0.0f,  
        -0.75f, 0.0f, 0.0f,  
        -0.25f, 0.0f, 0.0f,  
    };

    float secondTriangle[] = {
         0.5f,  0.5f, 0.0f,  
         0.75f, 0.0f, 0.0f,  
         0.25f, 0.0f, 0.0f,  
    };
    
    unsigned int VAO[2], VBO[2];
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Loop de renderização
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.useColorMovement( 1, 2);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        shader.useColorMovement( 1, 4);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteBuffers(1, &VBO[0]);

    glfwTerminate();
    return 0;
}

// Processamento de entrada
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
