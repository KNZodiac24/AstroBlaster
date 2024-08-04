#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadCubemap(vector<std::string> faces);
glm::vec3 getShipForwardDirection();
unsigned int loadTexture(char const* path);

struct Bala {
    glm::vec3 posicion;
    glm::vec3 direccion;
    float velocidad;  // Add speed to the bullet struct
};

vector<Bala> balas;

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Parámetros nave
glm::vec3 ubicacionNave = glm::vec3(-1.3f, -1.0f, 7.5f);
float rotacionNave = glm::radians(0.0f);
bool teclaAPresionada = false;
bool teclaDPresionada = false;

struct Asteroide {
    glm::vec3 posicionInicial;
    glm::vec3 posicionActual;
    float escala;
    float velocidad;
};
// Lista de asteroides
std::vector<Asteroide> asteroides;

// Función para generar asteroides aleatoriamente
static void generarAsteroides() {

    glm::vec3 pos[] = {
        glm::vec3(-4.5f, 6.8f, 25.62f),
        glm::vec3(-5.4f, 6.8f, 25.4f),
        glm::vec3(-6.4f, 6.8f, 25.15f),
        glm::vec3(-7.4f, 6.8f, 24.92f),
        glm::vec3(-8.4f, 6.8f, 24.7f)
    };

    for (int i = 0; i < 15; i++) {
        Asteroide asteroide;
        float offset = (rand() % 5) * 0.5f;
        int ran = (rand() % 5);
        asteroide.posicionInicial = pos[ran] ;
        asteroide.posicionActual = asteroide.posicionInicial + glm::vec3(0.0f, offset, offset);
        asteroide.escala = 0.0025f;
        asteroide.velocidad = 22.0f;
        asteroides.push_back(asteroide);
    }
}

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Naves", NULL, NULL);
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
    const unsigned int ANCHO_MONITOR = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    const unsigned int ALTO_MONITOR = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    glfwSetWindowPos(window, (ANCHO_MONITOR / 2) - (SCR_WIDTH / 2), (ALTO_MONITOR / 2) - (SCR_HEIGHT / 2));

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

    // build and compile shaders
    // -------------------------
    Shader modeloShader("shaders/modelo.vs", "shaders/modelo.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    Shader solShader("shaders/sol.vs", "shaders/sol.fs");
    Shader balaShader("shaders/bala.vs", "shaders/bala.fs");

    float skyboxVertices[] = {
        // positions
        // atras          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        // izquierda
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // derecha
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        // frontal
       -1.0f, -1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,

       // arriba
       -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,

       // abajo
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    unsigned int solVAO, solVBO;
    glGenVertexArrays(1, &solVAO);
    glGenBuffers(1, &solVBO);
    glBindVertexArray(solVAO);
    glBindBuffer(GL_ARRAY_BUFFER, solVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<std::string> faces
    {
        "textures/spacerigth3.png",//derecha
        "textures/spaceleft3.png",//izquierda
        "textures/spaceup3.png",//abajo
        "textures/spacedown3.png",//arriba
        "textures/spacefront3.png",//adelante
        "textures/spaceback3.png"//atras
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Cargamos los modelos
    Model nave("model/spaceship/spaceship.obj");
    Model modeloAsteroide("model/asteroide/asteroide.obj");

    // Genera asteroides al inicio
    generarAsteroides();

    float verticesBala[] = {
        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,
        -0.05f, -0.05f, -0.05f,

        -0.05f, -0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,
        -0.05f, -0.05f,  0.05f,

        -0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f, -0.05f,
        -0.05f, -0.05f, -0.05f,
        -0.05f, -0.05f, -0.05f,
        -0.05f, -0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,

         0.05f,  0.05f,  0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,

        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
        -0.05f, -0.05f,  0.05f,
        -0.05f, -0.05f, -0.05f,

        -0.05f,  0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f, -0.05f
    };

    unsigned int balaVAO, balaVBO;
    glGenVertexArrays(1, &balaVAO);
    glGenBuffers(1, &balaVBO);
    glBindVertexArray(balaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, balaVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBala), verticesBala, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    unsigned int emissionMap = loadTexture("model/spaceship/material_0_emissive.jpeg");
    // shader configuration
    // --------------------
    modeloShader.use();
    modeloShader.setInt("emission1", 0);


    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        solShader.use();

        glm::mat4 sol = glm::mat4(1.0f);
        sol = glm::translate(sol, glm::vec3(1.5f, 2.0f, 10.0f));
        sol = glm::scale(sol, glm::vec3(0.25f, 0.25f, 0.25f));

        solShader.setMat4("model", sol);
        solShader.setMat4("projection", projection);
        solShader.setMat4("view", view);

        glBindVertexArray(solVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // don't forget to enable shader before setting uniforms
        modeloShader.use();
     
        modeloShader.setVec3("light.position", 1.5f, 2.0f, 10.0f);
        modeloShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        modeloShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        modeloShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
        modeloShader.setMat4("projection", projection);
        modeloShader.setMat4("view", view);

        balaShader.use();

        balaShader.setMat4("projection", projection);
        balaShader.setMat4("view", view);

        modeloShader.use();

        // Actualiza y renderiza los asteroides
        for (auto& asteroide : asteroides) {
            
            // Mueve los asteroides en dirección hacia la nave
            asteroide.posicionActual -= glm::vec3(-0.057f, 0.087f, 0.2f) * asteroide.velocidad * deltaTime;

            if (glm::dot(asteroide.posicionActual - camera.Position, -camera.Front) > 1.0f) {
                // Resetear posición del asteroide cuando pasa más allá de la cámara
                float offset = (rand() % 5) * 0.5f;
                asteroide.posicionActual = asteroide.posicionInicial + glm::vec3(0.0f, offset, offset);
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, asteroide.posicionActual);
            model = glm::scale(model, glm::vec3(asteroide.escala));
            modeloShader.setMat4("model", model);
            modeloAsteroide.Draw(modeloShader);
        }
           
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ubicacionNave);
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::rotate(model, glm::radians(164.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotacionNave, glm::vec3(0.0f, 0.0f, 1.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, emissionMap);

        modeloShader.setMat4("model", model);
        nave.Draw(modeloShader);
    
        // Renderizar las balas

        balaShader.use();
        balaShader.setFloat("light.constant", 1.0f);
        balaShader.setFloat("light.linear", 0.09f);
        balaShader.setFloat("light.quadratic", 0.032f);
        balaShader.setVec3("light.ambient", 0.0f, 1.0f, 0.0f);
        balaShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        balaShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


        for (auto& bala : balas) {
            bala.posicion += bala.direccion * bala.velocidad * deltaTime;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, bala.posicion);
            model = glm::scale(model, glm::vec3(0.7f));
            balaShader.setVec3("light.position", bala.posicion);

            balaShader.setMat4("model", model);
            glBindVertexArray(balaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //std::cout << bala.posicion.x << ", " << bala.posicion.y << ", " << bala.posicion.z << std::endl;
        }

        //Eliminar balas lejanas
        balas.erase(std::remove_if(balas.begin(), balas.end(),
            [](const Bala& b) { return glm::length(b.posicion - ubicacionNave) > 20.0f; }),
            balas.end());

        processInput(window);
        
		
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        if (!teclaAPresionada) {
            teclaAPresionada = true;
        }
        if (ubicacionNave.x <= 0.99f && ubicacionNave.z <= 8.37f) {
            ubicacionNave += glm::vec3(0.005f, 0.0f, 0.0012f);
            rotacionNave = glm::radians(15.0f);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        if (teclaAPresionada) {
            teclaAPresionada = false;
            rotacionNave = glm::radians(0.0f);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (!teclaDPresionada) {
            teclaDPresionada = true;
        }
        if (ubicacionNave.x >= -3.74f && ubicacionNave.z >= 6.6f) {
            ubicacionNave += glm::vec3(-0.005f, 0.0f, -0.0012f);
            rotacionNave = glm::radians(-15.0f);
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        if (teclaDPresionada) {
            teclaDPresionada = false;
            rotacionNave = glm::radians(0.0f);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (ubicacionNave.y <= 1.2f) {
            ubicacionNave += glm::vec3(0.0f, 0.003f, 0.0f);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if(ubicacionNave.y >= -2.0f )
        ubicacionNave += glm::vec3(0.0f, -0.003f, 0.0f);
    }
        
    static float lastShot = 0.0f;
    float currentTime = glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && currentTime - lastShot > 0.35f) {
        Bala nuevaBala;
        // Calculamos la posición inicial de la bala basada en la posición y rotación de la nave
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotacionNave, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 offset = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        nuevaBala.posicion = ubicacionNave /*+ offset*/;

        // La dirección de la bala siempre será hacia adelante en el eje Z
        nuevaBala.direccion = getShipForwardDirection();
        nuevaBala.velocidad = 3.0f;
        balas.push_back(nuevaBala);
        lastShot = currentTime;
        //std::cout << "Bullet created at position: " << nuevaBala.posicion.x << ", " << nuevaBala.posicion.y << ", " << nuevaBala.posicion.z << std::endl;
    }
}

glm::vec3 getShipForwardDirection() {
    // Calculate the forward direction based on the ship's rotation
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), /*rotacionNave*/0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    return glm::normalize(glm::vec3(rotationMatrix * glm::vec4(-0.13f, 0.2f, 0.45f, 0.0f)));
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = 0.0;
    float yoffset = 0.0; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //stbi_set_flip_vertically_on_load(true);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}