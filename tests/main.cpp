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


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    Shader ourShader("/home/kadufi/Desktop/ifcg/tests/shader/3.3.shader.vs", "/home/kadufi/Desktop/ifcg/tests/shader/3.3.shader1.fs");
    Shader ourShader1("/home/kadufi/Desktop/ifcg/tests/shader/3.3.shader.vs", "/home/kadufi/Desktop/ifcg/tests/shader/3.3.shader2.fs");
    Shader ourShader2("/home/kadufi/Desktop/ifcg/tests/shader/cubesky.vs", "/home/kadufi/Desktop/ifcg/tests/shader/cubesky.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(-8.0f, -2.0f, -18.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO[3], VAO[3], EBO;
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);

    // ---------------------------
    // Configuração do primeiro conjunto (VAO[0]) SEM EBO
    // ---------------------------
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo de posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de textura (s, t)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Desvincular o VAO[0]

    // ---------------------------
    // Configuração do segundo conjunto (VAO[1]) COM EBO
    // ---------------------------
    glBindVertexArray(VAO[1]); // Mudança crucial: agora estamos configurando VAO[1]!

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesF), verticesF, GL_STATIC_DRAW);

    // O EBO agora será vinculado corretamente ao VAO[1]
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesF), indicesF, GL_STATIC_DRAW);

    // Atributo de posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de textura (s, t, r)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Desvincular o VAO[1]
//skybox=============================================================================================================
    // Skybox é o terceiro
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    // Apenas posição (sem textura ou normais)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0); // Unbind

// load and create a texture=========================================================================================================================== 
    stbi_set_flip_vertically_on_load(true);
    //setBorderColor(1.0, 1.0, 1.0, 1.0);
    Texture texture0(GL_TEXTURE_2D,GL_LINEAR,GL_MIRRORED_REPEAT);
    texture0.setImg("/home/kadufi/Desktop/ifcg/tests/image/awesomeface.png");

    Texture texture1(GL_TEXTURE_2D);
    texture1.setBorderColor(1.0, 0.0, 1.0, 1.0);
    texture1.setWrap(GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER);
    texture1.setFiltering(GL_LINEAR,GL_LINEAR);  
    texture1.setImg("/home/kadufi/Desktop/ifcg/tests/image/container.jpg");
    
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
    //skybox.setCubeWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    //skybox.setCubeFiltering(GL_LINEAR, GL_LINEAR);

//=========================================================================================================================================================
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture0", 0);
    ourShader.setInt("texture1", 1);

    ourShader1.use();
    ourShader1.setInt("texture0", 1);
    ourShader1.setInt("texture1", 0);
    
    ourShader2.use();
    ourShader2.setInt("skybox", 2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

         // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        texture0.activateAndBind(0);
        texture1.activateAndBind(1);
        skybox.activateAndBind(2);
        
        // activate shader
        //ourShader1.use();
        ourShader.use();

        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();

        
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);
        
        // render boxes
        glBindVertexArray(VAO[0]);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        ourShader1.use();   
        ourShader1.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader1.setMat4("view", view);

        glm::mat4 model1 = glm::mat4(1.0f);
        angle = angle + 0.7f;
        model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader1.setMat4("model", model1);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, (sizeof(indicesF)/sizeof(indicesF[0])), GL_UNSIGNED_INT, 0);

        //skybox
        // Desabilita depth writing para a skybox não sobrescrever objetos na frente
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        // Usa o shader da skybox
        ourShader2.use();

        // Usa a mesma projection e view dos cubos
        glm::mat4 viewSkybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        ourShader2.setMat4("view", viewSkybox);
        ourShader2.setMat4("projection", projection);

        // Bind do VAO e textura
        glBindVertexArray(VAO[2]);
        // Desenha a skybox
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Restaura o depth padrão
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

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
