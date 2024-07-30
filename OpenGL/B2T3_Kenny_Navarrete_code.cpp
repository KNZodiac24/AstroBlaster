#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Instanciamos la cámara y definimos los parámetros necesarios para su funcionamiento
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // Inicializamos y configuramos glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creamos y verificamos la ventana a mostrar
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B2T3 - Kenny Navarrete - 1726986704", NULL, NULL);
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

    // Hacemos que la ventana capture el cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicializamos glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Activamos el depth testing para el 3D
    glEnable(GL_DEPTH_TEST);

    /* Realizamos todo el proceso de creación, compilación y linking de shaders mediante la Shader Class
       pasandole los archivos donde se define el código fuente para el vertex y el fragment*/
    Shader ourShader("shaders/vertexshader.vs", "shaders/fragmentshader.fs");

    /* Para cada cubo con diferente textura definimos sus vértices con
       sus respectivas texturas para luego separar los datos */
    float vertices1[] = {
            // Posiciones         // Texturas
            // Atras
            -0.5f, -0.5f, -0.5f,  0.3658f, 0.5597f,
             0.5f, -0.5f, -0.5f,  0.175f, 0.5597f,
             0.5f,  0.5f, -0.5f,  0.175f, 0.7321f,
             0.5f,  0.5f, -0.5f,  0.175f, 0.7321f,
            -0.5f,  0.5f, -0.5f,  0.3658f, 0.7321f,
            -0.5f, -0.5f, -0.5f,  0.3658f, 0.5597f,

            // Frontal
            -0.5f, -0.5f,  0.5f,  0.5596f, 0.5597f,
             0.5f, -0.5f,  0.5f,  0.7525f, 0.5597f,
             0.5f,  0.5f,  0.5f,  0.7525f, 0.7321f,
             0.5f,  0.5f,  0.5f,  0.7525f, 0.7321f,
            -0.5f,  0.5f,  0.5f,  0.5596f, 0.7321f,
            -0.5f, -0.5f,  0.5f,  0.5596f, 0.5597f,

            // Izquierda
            -0.5f,  0.5f,  0.5f,  0.5606f, 0.7321f,
            -0.5f,  0.5f, -0.5f,  0.3658f, 0.7321f,
            -0.5f, -0.5f, -0.5f,  0.3658f, 0.5597f,
            -0.5f, -0.5f, -0.5f,  0.3658f, 0.5597f,
            -0.5f, -0.5f,  0.5f,  0.5606f, 0.5597f,
            -0.5f,  0.5f,  0.5f,  0.5606f, 0.7321f,

            // Derecha
             0.5f,  0.5f,  0.5f,  0.7525f, 0.7321f,
             0.5f,  0.5f, -0.5f,  0.9463f, 0.7321f,
             0.5f, -0.5f, -0.5f,  0.9463f, 0.5597f,
             0.5f, -0.5f, -0.5f,  0.9463f, 0.5597f,
             0.5f, -0.5f,  0.5f,  0.7525f, 0.5597f,
             0.5f,  0.5f,  0.5f,  0.7525f, 0.7321f,

             // Abajo
             -0.5f, -0.5f, -0.5f,  0.5606f, 0.317f,
              0.5f, -0.5f, -0.5f,  0.7525f, 0.317f,
              0.5f, -0.5f,  0.5f,  0.7525f, 0.5597f,
              0.5f, -0.5f,  0.5f,  0.7525f, 0.5597f,
             -0.5f, -0.5f,  0.5f,  0.5606f, 0.5597f,
             -0.5f, -0.5f, -0.5f,  0.5606f, 0.317f,

             // Arriba
             -0.5f,  0.5f, -0.5f,  0.5606f, 0.9761f,
              0.5f,  0.5f, -0.5f,  0.7525f, 0.9761f,
              0.5f,  0.5f,  0.5f,  0.7525f, 0.7321f,
              0.5f,  0.5f,  0.5f,  0.7525f, 0.7321f,
             -0.5f,  0.5f,  0.5f,  0.5606f, 0.7321f,
             -0.5f,  0.5f, -0.5f,  0.5606f, 0.9761f
    };

    float vertices2[] = {
        // Posiciones         // Texturas
        // Atras
        -0.5f, -0.5f, -0.5f,  0.3564f, 0.5061f,
         0.5f, -0.5f, -0.5f,  0.1504f, 0.5061f,
         0.5f,  0.5f, -0.5f,  0.1504f, 0.685f,
         0.5f,  0.5f, -0.5f,  0.1504f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.3564f, 0.685f,
        -0.5f, -0.5f, -0.5f,  0.3564f, 0.5061f,

        // Frontal
        -0.5f, -0.5f,  0.5f,  0.5596f, 0.5061f,
         0.5f, -0.5f,  0.5f,  0.7646f, 0.5061f,
         0.5f,  0.5f,  0.5f,  0.7646f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.7646f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.5596f, 0.685f,
        -0.5f, -0.5f,  0.5f,  0.5596f, 0.5061f,

        // Izquierda
        -0.5f,  0.5f,  0.5f,  0.5606f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.3564f, 0.685f,
        -0.5f, -0.5f, -0.5f,  0.3564f, 0.5061f,
        -0.5f, -0.5f, -0.5f,  0.3564f, 0.5061f,
        -0.5f, -0.5f,  0.5f,  0.5606f, 0.5061f,
        -0.5f,  0.5f,  0.5f,  0.5606f, 0.685f,

        // Derecha
         0.5f,  0.5f,  0.5f,  0.7646f, 0.685f,
         0.5f,  0.5f, -0.5f,  0.9697f, 0.685f,
         0.5f, -0.5f, -0.5f,  0.9697f, 0.5061f,
         0.5f, -0.5f, -0.5f,  0.9697f, 0.5061f,
         0.5f, -0.5f,  0.5f,  0.7646f, 0.5061f,
         0.5f,  0.5f,  0.5f,  0.7646f, 0.685f,

         // Abajo
         -0.5f, -0.5f, -0.5f,  0.5606f, 0.2488f,
          0.5f, -0.5f, -0.5f,  0.7646f, 0.2488f,
          0.5f, -0.5f,  0.5f,  0.7646f, 0.5061f,
          0.5f, -0.5f,  0.5f,  0.7646f, 0.5061f,
         -0.5f, -0.5f,  0.5f,  0.5606f, 0.5061f,
         -0.5f, -0.5f, -0.5f,  0.5606f, 0.2488f,

         // Arriba
         -0.5f,  0.5f, -0.5f,  0.5606f, 0.94f,
          0.5f,  0.5f, -0.5f,  0.7646f, 0.94f,
          0.5f,  0.5f,  0.5f,  0.7646f, 0.685f,
          0.5f,  0.5f,  0.5f,  0.7646f, 0.685f,
         -0.5f,  0.5f,  0.5f,  0.5606f, 0.685f,
         -0.5f,  0.5f, -0.5f,  0.5606f, 0.94f
    };

    float vertices3[] = {
        // Posiciones         // Texturas
        // Atras
        -0.5f, -0.5f, -0.5f,  0.3489f, 0.5055f,
         0.5f, -0.5f, -0.5f,  0.145f, 0.5055f,
         0.5f,  0.5f, -0.5f,  0.145f, 0.6846f,
         0.5f,  0.5f, -0.5f,  0.145f, 0.6846f,
        -0.5f,  0.5f, -0.5f,  0.3489f, 0.6846f,
        -0.5f, -0.5f, -0.5f,  0.3489f, 0.5055f,

        // Frontal
        -0.5f, -0.5f,  0.5f,  0.5533f, 0.5055f,
         0.5f, -0.5f,  0.5f,  0.7584f, 0.5055f,
         0.5f,  0.5f,  0.5f,  0.7584f, 0.6846f,
         0.5f,  0.5f,  0.5f,  0.7584f, 0.6846f,
        -0.5f,  0.5f,  0.5f,  0.5533f, 0.6846f,
        -0.5f, -0.5f,  0.5f,  0.5533f, 0.5055f,

        // Izquierda
        -0.5f,  0.5f,  0.5f,  0.5533f, 0.6846f,
        -0.5f,  0.5f, -0.5f,  0.3489f, 0.6846f,
        -0.5f, -0.5f, -0.5f,  0.3489f, 0.5055f,
        -0.5f, -0.5f, -0.5f,  0.3489f, 0.5055f,
        -0.5f, -0.5f,  0.5f,  0.5533f, 0.5055f,
        -0.5f,  0.5f,  0.5f,  0.5533f, 0.6846f,

        // Derecha
         0.5f,  0.5f,  0.5f,  0.7584f, 0.6846f,
         0.5f,  0.5f, -0.5f,  0.9615f, 0.6846f,
         0.5f, -0.5f, -0.5f,  0.9615f, 0.5055f,
         0.5f, -0.5f, -0.5f,  0.9615f, 0.5055f,
         0.5f, -0.5f,  0.5f,  0.7584f, 0.5055f,
         0.5f,  0.5f,  0.5f,  0.7584f, 0.6846f,

         // Abajo
         -0.5f, -0.5f, -0.5f,  0.5606f, 0.2489f,
          0.5f, -0.5f, -0.5f,  0.7584f, 0.2489f,
          0.5f, -0.5f,  0.5f,  0.7584f, 0.5055f,
          0.5f, -0.5f,  0.5f,  0.7584f, 0.5055f,
         -0.5f, -0.5f,  0.5f,  0.5606f, 0.5055f,
         -0.5f, -0.5f, -0.5f,  0.5606f, 0.2489f,

         // Arriba
         -0.5f,  0.5f, -0.5f,  0.5606f, 0.942f,
          0.5f,  0.5f, -0.5f,  0.7584f, 0.942f,
          0.5f,  0.5f,  0.5f,  0.7584f, 0.6846f,
          0.5f,  0.5f,  0.5f,  0.7584f, 0.6846f,
         -0.5f,  0.5f,  0.5f,  0.5606f, 0.6846f,
         -0.5f,  0.5f, -0.5f,  0.5606f, 0.942f
    };

    // Definimos las coordenadas globales para cada cubo en la escena
    /* Se añade una cuarta componente que será de ayuda para indicar la textura a usar en cada cubo, 
       se la pasará mediante un uniform */
    glm::vec4 cubePositions[] = {
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(-0.5f, 0.0f, 0.0f, 2.0f),
        glm::vec4(0.5f, 0.0f, 0.0f, 3.0f),
        glm::vec4(-4.0f, 0.0f, 0.0f, 3.0f),
        glm::vec4(0.0f, 4.0f, 0.0f, 1.0f),
        glm::vec4(2.8284f, 2.8284f, 0.0f, 2.0f),
        glm::vec4(-2.0f, -1.5f, 0.0f, 1.0f),
        glm::vec4(-0.0f, -1.5f, 0.0f, 2.0f),
        glm::vec4(2.0f, -1.5f, 0.0f, 3.0f)
    };

    // Creamos los VBO y VAO para cada cubo para pasar a memoria los datos
    unsigned int VBO[3], VAO[3];
 
    for (unsigned int i = 0; i < 3; i++) {
        glGenVertexArrays(1, &VAO[i]);
        glGenBuffers(1, &VBO[i]);
    }

    // Configuramos y establecemos los buffers y separamos los datos de vértices y texturas usando glVertexAttribPointer
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Creamos las texturas
    unsigned int texture1, texture2, texture3;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Establecemos el wrapping para esta textura en los dos ejes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Establecemos el filtering para magnificación y minificación
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Cargamos la imagen para la textura y establecemos el Mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/Texture2.png", &width, &height, &nrChannels, 0);    if (data)
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Hacemos los mismo para la textura 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("textures/Texture3.jpg", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);
    
    // Hacemos los mismo para la textura 3
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("textures/Texture4.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);

    // Activamos el shader para indicarle los uniforms de las texturas
    ourShader.use(); 
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setInt("texture3", 2);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Detectamos las teclas útiles para la ventana y la cámara
        processInput(window);

        // Indicamos el color del fondo de la pantalla y limpiamos sus buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activamos y ligamos las texture units a las texturas cargadas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        // Activamos el shader
        ourShader.use();
		
		// Realizamos las transformaciones necesarias para la cámara
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Creamos la matriz de modelo que se usará para graficar y alterar cada cubo
            glm::mat4 model = glm::mat4(1.0f);
            
            // Obtenemos las posiciones en coordenadas globales de cada cubo para usarlas luego en las transformaciones
            glm::vec3 posicionesCuboActual = glm::vec3(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
            
            // A cada cubo se le dará un comportamiento diferente (transformaciones diferentes)
            switch (i) {
                case 0: /* Para cada caso, primero indicamos el VAO a utilizar para que vincule 
                           cada cubo a las coordenadas de textura correctas */
                        glBindVertexArray(VAO[0]);
                        // Cada cubo tendrá transformaciones diferentes relacionadas con rotación y traslación
                        model = glm::translate(model, posicionesCuboActual);
                        model = glm::rotate(model, glm::radians((float)glfwGetTime()*-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                        // Para cada caso, una vez aplicadas las transformaciones deseadas al modelo, lo pasamos al shader
                        ourShader.setMat4("model", model);
                        // Para cada caso, con la cuarta componente de las posiciones decimos la textura a usar en cada cubo
                        ourShader.setInt("texCubo", cubePositions[i].w);
                        break;
                case 1: glBindVertexArray(VAO[1]);
                        model = glm::translate(model, posicionesCuboActual);
                        model = glm::rotate(model, glm::radians((float)glfwGetTime()*-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        ourShader.setMat4("model", model);
                        ourShader.setInt("texCubo", cubePositions[i].w);
                        break;
                case 2: glBindVertexArray(VAO[2]);
                        model = glm::translate(model, posicionesCuboActual);
                        model = glm::rotate(model, glm::radians((float)glfwGetTime()*25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        ourShader.setMat4("model", model);
                        ourShader.setInt("texCubo", cubePositions[i].w);
                        break;
                case 3: glBindVertexArray(VAO[2]);
                        model = glm::translate(model, glm::vec3(-4.0f*cos(glfwGetTime()), 0.0f, 4.0f*sin(glfwGetTime())));
                        ourShader.setMat4("model", model);
                        ourShader.setInt("texCubo", cubePositions[i].w);
                        break;
                case 4: glBindVertexArray(VAO[0]);
                        model = glm::translate(model, glm::vec3(0.0f, 4.0f*cos(glfwGetTime()), 4.0f*sin(glfwGetTime())));
                        ourShader.setMat4("model", model);
                        ourShader.setInt("texCubo", cubePositions[i].w);
                        break;
                case 5: glBindVertexArray(VAO[1]);
                        model = glm::translate(model, glm::vec3(4.0f*cos(glfwGetTime()), 4.0f*cos(glfwGetTime()), 4.0f*sin(glfwGetTime())));
                        ourShader.setMat4("model", model);
                        ourShader.setInt("texCubo", cubePositions[i].w);
                        break;
                default: /* Para estos últimos 3 casos se aplicará únicamente escalado a cada cubo, por lo que
                            primero se indica a qué VAO corresponde cada cubo según su componente añadida de textura */
                        switch ((int)cubePositions[i].w) {
                            case 1: glBindVertexArray(VAO[0]); break;
                            case 2: glBindVertexArray(VAO[1]); break;
                            case 3: glBindVertexArray(VAO[2]); break;
                         }
                         model = glm::translate(model, posicionesCuboActual);
                         model = glm::scale(model, glm::vec3(2.0f*sin(glfwGetTime()),
                                                             2.0f*sin(glfwGetTime()),
                                                             2.0f*sin(glfwGetTime())));
                         ourShader.setMat4("model", model);
                         ourShader.setInt("texCubo", cubePositions[i].w);
                         break;
            }
            
            // Finalmente dibujamos los cubos
            glDrawArrays(GL_TRIANGLES, 0,179);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Eliminamos de memoria los recursos que hemos creado
    for (unsigned int i = 0; i < 3; i++) {
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
    }
    
    // Terminamos todos los procesos de la ventana de glfw
    glfwTerminate();
    return 0;
}

// Detecamos la tecla de Esc para salir de la ventana, y las teclas W A S D para poder mover la cámara
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

// Mantiene las proporciones de la figura respecto a la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Detectamos el movimiento y posición del mouse para la cámara
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);    
}

// Hacemos que se detecte y funcione el scroll del mouse en la cámara
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}