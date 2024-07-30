#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// Definimos el código fuente de los shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColores;\n" // Definimos una entrada para los colores de las figuras
"out vec3 colores;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   colores = aColores;\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 colores;\n" // Se reciben los diferentes colores
"void main()\n"
"{\n"
"   FragColor = vec4(colores, 1.0f);\n"
"}\n\0";

int main()
{
    // Inicializamos y configuramos glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creamos y verificamos la ventana a mostrar
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B1P2 - Kenny Navarrete - 1726986704", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializamos glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Creamos los shaders con el código fuente y los compilamos
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int shaderProgram = glCreateProgram(); // Creamos el shader program
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Linkeamos los shaders en el shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
  
    // Definimos los vértices de la figura con sus respectivos colores para luego separar los datos
    float vertices[] = {
        // Vértices                             // Colores
        0.0f, 0.0f, 0.0f,   /* Centro C0 */     0.6f, 0.8f, 0.2f,
        -0.4f, 0.4f, 0.0f,  /* A */             0.6f, 0.8f, 0.2f,
        -0.4f, 0.0f, 0.0f,  /* H */             0.6f, 0.8f, 0.2f,

        -0.6f, 0.15f, 0.0f, /* F */             1.0f, 0.4f, 0.4f,
        -0.4f, 0.15f, 0.0f, /* G */             1.0f, 0.4f, 0.4f,
        -0.4f, 0.4f, 0.0f,  /* A */             1.0f, 0.4f, 0.4f,

        -0.6f, 0.15f, 0.0f, /* F */             1.0f, 0.4f, 0.4f,
        -0.4f, 0.4f, 0.0f,  /* A */             1.0f, 0.4f, 0.4f,
        -0.6f, 0.4f, 0.0f,  /* B */             1.0f, 0.4f, 0.4f,

        -0.825f, 0.15f, 0.0f,   /* E */         0.2f, 0.6f, 1.0f,
        -0.6f, 0.15f, 0.0f, /* F */             0.2f, 0.6f, 1.0f,
        -0.6f, 0.4f, 0.0f,  /* B */             0.2f, 0.6f, 1.0f,

        -0.9f, 0.4f, 0.0f,  /* C */             1.0f, 0.6f, 0.2f,
        -0.744f, 0.24f, 0.0f,   /* D */         1.0f, 0.6f, 0.2f,
        -0.6f, 0.4f, 0.0f,  /* B */             1.0f, 0.6f, 0.2f,

        -0.4f, 0.0f, 0.0f,  /* H */             1.0f, 0.8f, 0.0f,
        0.0f, -0.4f, 0.0f,  /* I */             1.0f, 0.8f, 0.0f,
        0.4f, 0.0f, 0.0f,   /* J */             1.0f, 0.8f, 0.0f,

        0.4f, 0.0f, 0.0f,   /* J */             0.4f, 0.4f, 0.8f,
        0.6f, -0.2f, 0.0f,  /* K */             0.4f, 0.4f, 0.8f,
        0.75f, 0.0f, 0.0f,  /* M */             0.4f, 0.4f, 0.8f,

        0.6f, -0.2f, 0.0f,  /* K */             0.4f, 0.4f, 0.8f,
        0.925f, -0.2f, 0.0f,    /* L */         0.4f, 0.4f, 0.8f,
        0.75f, 0.0f, 0.0f,  /* M */             0.4f, 0.4f, 0.8f,

        0.75f, 0.0f, 0.0f,  /* M */             0.2f, 0.8f, 0.4f,
        0.4f, 0.4f, 0.0f,    /* N */            0.2f, 0.8f, 0.4f,
        0.0f, 0.0f, 0.0f,   /* Centro C0 */     0.2f, 0.8f, 0.4f
    };

    // Creamos el VBO y VAO para pasar a memoria los datos
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Configuramos y establecemos los buffers y separamos los datos de vértices y colores usando glVertexAttribPointer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)){

        processInput(window);

        // Definimos el color de fondo (blanco)
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usamos el shader program con los shaders que creamos
        glUseProgram(shaderProgram);

        // Dibujamos la figura usando los datos del VAO
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 161);	

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Eliminamos de memoria los recursos que hemos creado
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminamos todos los procesos de la ventana de glfw
    glfwTerminate();
    return 0;
}

// Si se detecta que se presiona la tecla de Escape se cierra la ventana
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Mantiene las proporciones de la figura respecto a la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}