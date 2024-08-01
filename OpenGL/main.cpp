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
glm::vec3 getShipForwardDirection(float rotacionNave);

struct Bullet {
    glm::vec3 position;
    glm::vec3 direction;
    float speed;  // Add speed to the bullet struct
};

vector<Bullet> bullets;

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
    glm::vec3 posicion;
    float escala;
    float velocidad;
};
// Lista de asteroides
std::vector<Asteroide> asteroides;
// Función para generar asteroides aleatoriamente
void generarAsteroides(int cantidad) {

    for (int i = 0; i < cantidad; ++i) {
        for (int i = 0; i < cantidad; ++i) {
            Asteroide asteroide;
            float offsetX = (rand() % 20 - 10) * 0.5f; // Ajusta el rango según tu escena
            float offsetY = (rand() % 20 - 10) * 0.5f;
            float distance = (rand() % 50 + 10); // Distancia inicial desde la cámara hacia adelante
            asteroide.posicion = camera.Position + camera.Front * distance + glm::vec3(offsetX, offsetY, 0.0f);
            asteroide.escala = ((rand() % 100) / 100000.0f) + 0.0005f; // Escala aleatoria entre 0.0005 y 0.0010
            asteroide.velocidad = (rand() % 10 + 1) / 10.0f; // Velocidad aleatoria
            asteroides.push_back(asteroide);
        }
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
    Shader ourShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");
	Shader skyboxShader("shaders/6.1.skybox.vs", "shaders/6.1.skybox.fs");

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

    // load models
    Model nave("model/spaceship/spaceship.obj");
    // Carga el modelo del asteroide
    Model modeloAsteroide("model/asteroide/asteroide.obj");
   
    // Genera asteroides al inicio
    generarAsteroides(5);

    float bulletVertices[] = {
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

    unsigned int bulletVAO, bulletVBO;
    glGenVertexArrays(1, &bulletVAO);
    glGenBuffers(1, &bulletVBO);
    glBindVertexArray(bulletVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


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

        // don't forget to enable shader before setting uniforms
        ourShader.use();
     
        // Cambiar la dirección de la cámara para que mire hacia atrás
       // camera.Front = glm::vec3(-1.3f, -1.0f, 7.5f);
       // Configurar la dirección de la cámara para que mire hacia la posición dada
        camera.Front = ubicacionNave;
        camera.Position= glm::vec3(0.0f, 0.0f, 0.0f);
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        // Actualiza y renderiza los asteroides
        for (auto& asteroide : asteroides) {
            // Mueve los asteroides en la dirección opuesta a camera.Front
            asteroide.posicion -= camera.Front * asteroide.velocidad * deltaTime;
            if (glm::dot(asteroide.posicion - camera.Position, -camera.Front) > 1.0f) {
                // Resetear posición del asteroide cuando pasa más allá de la cámara
                float offsetX = (rand() % 20 - 10) * 0.5f;
                float offsetY = (rand() % 20 - 10) * 0.5f;
                float distance = (rand() % 50 + 10);
                asteroide.posicion = camera.Position + camera.Front * distance + glm::vec3(offsetX, offsetY, 0.0f);
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, asteroide.posicion);
            model = glm::scale(model, glm::vec3(asteroide.escala));
            ourShader.setMat4("model", model);
            modeloAsteroide.Draw(ourShader);
        }
        // render the loaded modela
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ubicacionNave);
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::rotate(model, glm::radians(164.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotacionNave, glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);
        nave.Draw(ourShader);
    
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

        // Renderizar las balas

        ourShader.use();
        ourShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));

        for (auto& bullet : bullets) {
            bullet.position += bullet.direction * bullet.speed * deltaTime;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, bullet.position);
            model = glm::scale(model, glm::vec3(0.7f));
            ourShader.setMat4("model", model);
            glBindVertexArray(bulletVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //Eliminar Bala lejanas
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return glm::length(b.position - ubicacionNave) > 20.0f; }),
            bullets.end());


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
        ubicacionNave += glm::vec3(0.005f, 0.0f, 0.0012f);
        rotacionNave = glm::radians(15.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        if (teclaAPresionada) {
            teclaAPresionada = false;
            rotacionNave = glm::radians(0.0f);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        if (!teclaDPresionada) {
            teclaDPresionada = true;
        }
        ubicacionNave += glm::vec3(-0.005f, 0.0f, -0.0012f);
        rotacionNave = glm::radians(-15.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        if (teclaDPresionada) {
            teclaDPresionada = false;
            rotacionNave = glm::radians(0.0f);
        }
    }
        
    static float lastShot = 0.0f;
    float currentTime = glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && currentTime - lastShot > 0.5f) {
        Bullet newBullet;
        // Calculamos la posición inicial de la bala basada en la posición y rotación de la nave
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotacionNave, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 offset = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -0.5f, 0.0f));
        newBullet.position = ubicacionNave + offset;

        // La dirección de la bala siempre será hacia adelante en el eje Z
        newBullet.direction = getShipForwardDirection(rotacionNave);
        newBullet.speed = 3.0f;
        bullets.push_back(newBullet);
        lastShot = currentTime;
        std::cout << "Bullet created at position: " << newBullet.position.x << ", " << newBullet.position.y << ", " << newBullet.position.z << std::endl;
    }
}

glm::vec3 getShipForwardDirection(float rotacionNave) {
    // Calculate the forward direction based on the ship's rotation
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotacionNave, glm::vec3(0.0f, 0.0f, 1.0f));
    return glm::normalize(glm::vec3(rotationMatrix * glm::vec4(-0.12f, 0.15f, 0.5f, 0.0f)));
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

    float xoffset =0.0;
    float yoffset =0.0; // reversed since y-coordinates go from bottom to top

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