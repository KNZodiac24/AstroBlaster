#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cambiarColores(unsigned int VAO, unsigned int VBO);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// Definimos los vértices de la figura con sus respectivos colores para luego separar los datos
float vertices[] = {
    // Vértices                             // Colores
    -0.9f, 0.4f, 0.0f,  /* C */             1.0f, 0.6f, 0.2f,
    -0.744f, 0.24f, 0.0f,   /* D */         1.0f, 0.6f, 0.2f,
    -0.6f, 0.4f, 0.0f,  /* B */             1.0f, 0.6f, 0.2f,

    -0.6f, 0.4f, 0.0f,  /* B */             0.2f, 0.6f, 1.0f,
    -0.825f, 0.15f, 0.0f,   /* E */         0.2f, 0.6f, 1.0f,
    -0.6f, 0.15f, 0.0f, /* F */             0.2f, 0.6f, 1.0f,

    -0.6f, 0.15f, 0.0f, /* F */             1.0f, 0.4f, 0.4f,
    -0.6f, 0.4f, 0.0f,  /* B */             1.0f, 0.4f, 0.4f,
    -0.4f, 0.4f, 0.0f,  /* A */             1.0f, 0.4f, 0.4f,
    
    -0.4f, 0.4f, 0.0f,  /* A */             1.0f, 0.4f, 0.4f,
    -0.6f, 0.15f, 0.0f, /* F */             1.0f, 0.4f, 0.4f,
    -0.4f, 0.15f, 0.0f, /* G */             1.0f, 0.4f, 0.4f,
    
    -0.4f, 0.4f, 0.0f,  /* A */             0.6f, 0.8f, 0.2f,
    0.0f, 0.0f, 0.0f,   /* Centro C0 */     0.6f, 0.8f, 0.2f,
    -0.4f, 0.0f, 0.0f,  /* H */             0.6f, 0.8f, 0.2f,

    -0.4f, 0.0f, 0.0f,  /* H */             1.0f, 0.8f, 0.0f,
    0.0f, -0.4f, 0.0f,  /* I */             1.0f, 0.8f, 0.0f,
    0.4f, 0.0f, 0.0f,   /* J */             1.0f, 0.8f, 0.0f,

    0.4f, 0.4f, 0.0f,    /* N */            0.2f, 0.8f, 0.4f,
    0.0f, 0.0f, 0.0f,   /* Centro C0 */     0.2f, 0.8f, 0.4f,
    0.75f, 0.0f, 0.0f,  /* M */             0.2f, 0.8f, 0.4f,

    0.75f, 0.0f, 0.0f,  /* M */             0.4f, 0.4f, 0.8f,
    0.4f, 0.0f, 0.0f,   /* J */             0.4f, 0.4f, 0.8f,
    0.6f, -0.2f, 0.0f,  /* K */             0.4f, 0.4f, 0.8f,

    0.6f, -0.2f, 0.0f,  /* K */             0.4f, 0.4f, 0.8f,
    0.925f, -0.2f, 0.0f,    /* L */         0.4f, 0.4f, 0.8f,
    0.75f, 0.0f, 0.0f,  /* M */             0.4f, 0.4f, 0.8f 
};

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B1P3 - Kenny Navarrete - 1726986704", NULL, NULL);
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

    /* Realizamos todo el proceso de creación, compilación y linking de shaders mediante la Shader Class
       pasandole los archivos donde se define el código fuente para el vertex y el fragment*/ 
    Shader ourShader("shaders/shader_vs.vs", "shaders/shader_fs.fs"); 

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

    // Tomamos el tiempo inicial al abrir la ventana, este se usará más abajo
    double tiempoAnterior = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // Definimos el color de fondo (blanco)
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usamos el shader program creado con la Shader Class
        ourShader.use();

        // Obtenemos el tiempo actual en tiempo de ejecución
        double tiempoActual = glfwGetTime();
        
        /* Hacemos que cada vez que pase 1 segundo se ejecute la función cambiarColores, que
           itera los colores de los triángulos. Con esto, se logra que cada segundo el color
           de un triángulo se "mueva" hacia el triángulo que se encuentra a su derecha. */ 
        if (tiempoActual - tiempoAnterior >= 1 ) {
            cambiarColores(VAO, VBO);
            tiempoAnterior = tiempoActual;
        }

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

// Mantiene las proporciones de la figura respecto a la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* Esta función cambia el color de cada triángulo de la figura. Reemplaza el color del
   triángulo actual por el color del triángulo que está a su izquierda. En el caso del
   primer triángulo a la izquierda, reemplaza su color por el color del triángulo más
   a la derecha. */
void cambiarColores(unsigned int VAO, unsigned int VBO) {

    float temporalUltimoColor[3] = {vertices[147],vertices[148],vertices[149]};
    
    for (short i = 129; i >= 3; i -= 18) {
        float temporalColorActual[3] = {vertices[i], vertices[i+1], vertices[i+2]};
        
        for (short i2 = i+18; i2 <= i+30; i2+=6) {
            vertices[i2] = temporalColorActual[0];
            vertices[i2+1] = temporalColorActual[1];
            vertices[i2+2] = temporalColorActual[2];
        }
    }

    for (short i3 = 3; i3 <= 15; i3 += 6) {
        vertices[i3] = temporalUltimoColor[0];
        vertices[i3+1] = temporalUltimoColor[1];
        vertices[i3+2] = temporalUltimoColor[2];
    }

    // Volvemos a especificar el proceso de los buffers para que se realicen los cambios de los colores
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

