#include <glad/glad.h>         // Sempre inclua o GLAD antes do GLFW
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.hpp"
#include "texture.hpp"
#include "vertices.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float angle = 0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//grid
float blockSize = 1.0f; // lado do cubo
int gridX = 3;
int gridY = 3;
int gridZ = 3;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader shaderPlano("/home/kadufi/Desktop/ifcg/tests/shader/plano.vs", "/home/kadufi/Desktop/ifcg/tests/shader/plano.fs");
    Shader shaderSkyBox("/home/kadufi/Desktop/ifcg/tests/shader/cubesky.vs", "/home/kadufi/Desktop/ifcg/tests/shader/cubesky.fs");
    Shader shaderTerra("/home/kadufi/Desktop/ifcg/tests/shader/terra.vs", "/home/kadufi/Desktop/ifcg/tests/shader/terra.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
   


    unsigned int VBO[3], VAO[3], EBO[3];
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(3, EBO);
//skybox=============================================================================================================
    // Skybox é o terceiro
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    // Apenas posição (sem textura ou normais)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0); // Unbind

//plano====================================================================================================================

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlano), verticesPlano, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlano), indicesPlano, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//cubo=====================================================================================================================
    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCubo), indicesCubo, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


// load and create a texture=========================================================================================================================== 
    //plano
    stbi_set_flip_vertically_on_load(true);
    //setBorderColor(1.0, 1.0, 1.0, 1.0);
    Texture plano0(GL_TEXTURE_2D,GL_LINEAR,GL_REPEAT);
    plano0.setImg("/home/kadufi/Desktop/ifcg/tests/image/bedrock.jpeg");
    
    stbi_set_flip_vertically_on_load(false);
    //skybox
    std::vector<std::string> faces = {
        "/home/kadufi/Desktop/ifcg/tests/image/skybox/right.jpg",
        "/home/kadufi/Desktop/ifcg/tests/image/skybox/left.jpg",
        "/home/kadufi/Desktop/ifcg/tests/image/skybox/top.jpg",
        "/home/kadufi/Desktop/ifcg/tests/image/skybox/bottom.jpg",
        "/home/kadufi/Desktop/ifcg/tests/image/skybox/front.jpg",
        "/home/kadufi/Desktop/ifcg/tests/image/skybox/back.jpg"
    };
    
    Texture skybox(GL_TEXTURE_CUBE_MAP);
    skybox.setCubeImg(faces);
    skybox.setCubeDefaults();
    
    //terra
    stbi_set_flip_vertically_on_load(true);

    Texture terra(GL_TEXTURE_2D, GL_LINEAR, GL_REPEAT);
    terra.setImg("/home/kadufi/Desktop/ifcg/tests/image/dirt.jpg");

    //=========================================================================================================================================================
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shaderPlano.use();
    shaderPlano.setInt("plano0", 0);
    
    shaderSkyBox.use();
    shaderSkyBox.setInt("skybox", 1);

    shaderTerra.use();
    shaderTerra.setInt("terra", 2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // --- Tempo ---
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // --- Entrada ---
        processInput(window);

        // --- Limpeza ---
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Matriz de projeção (calcular só se a janela mudou tamanho) ---
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 
                                                0.1f, 100.0f);

        // --- Matriz de view (câmera) ---
        glm::mat4 view = camera.GetViewMatrix();

        // --- RENDER SKYBOX ---
        glDepthMask(GL_FALSE);               // desativa escrita no depth buffer
        glDepthFunc(GL_LEQUAL);              // skybox deve passar no depth <=

        shaderSkyBox.use();
        glm::mat4 viewSkybox = glm::mat4(glm::mat3(view)); // remove translação
        shaderSkyBox.setMat4("view", viewSkybox);
        shaderSkyBox.setMat4("projection", projection);

        // Ativa e vincula textura da skybox
        skybox.activateAndBind(1);  // geralmente skybox usa a textura na unidade 0

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthMask(GL_TRUE);               // reativa escrita no depth buffer
        glDepthFunc(GL_LESS);

        // --- RENDER PLANO ---
        shaderPlano.use();

        // Construir model do plano — se não muda, pode declarar fora do loop
        glm::mat4 modelPlano = glm::mat4(1.0f);
        
        shaderPlano.setMat4("model", modelPlano);
        shaderPlano.setMat4("view", view);
        shaderPlano.setMat4("projection", projection);

        // Ativa e vincula textura do plano
        plano0.activateAndBind(0);

        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
        // --- Terra ---
        shaderTerra.use();

        
        shaderTerra.setMat4("view", view);
        shaderTerra.setMat4("projection", projection);

         // desenhar grade de cubos
        for (int x = 0; x < gridX; x++) {
            for (int y = 0; y < gridY; y++) {
                for (int z = 0; z < gridZ; z++) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x * blockSize, y * blockSize, z * blockSize));
                    shaderTerra.setMat4("model", model);

                    glBindVertexArray(VAO[2]);
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                }
            }
        }
        
        terra.activateAndBind(2);
        // --- Finaliza frame ---
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
