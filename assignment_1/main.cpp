#include "glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>
#include <vector>
#include <string>

// DONE: Updated the titleString string to include my name instead of <Name>

// Assignment Title
std::string titleString = "Spring 2025 - Assignment 1 - Alyssa Barrientos"; // Replaced <Name> with Alyssa Barrientos


// ======================== SHADER PROGRAMS ========================


// Vertex Shader Source Code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal; // Correct transformation for normals
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
)";

// Fragment Shader Source Code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 Normal;
    in vec3 FragPos;

    uniform vec3 lightPos;
    uniform vec3 lightColor;
    uniform vec3 objectColor;

    void main() {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);

        // Ambient lighting
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Final color
        vec3 result = (ambient + diffuse) * objectColor;
        FragColor = vec4(result, 1.0);
    }
)";

// ======================== GLOBAL VARIABLES ========================





// Global rotation speeds
float rotationSpeedX = 0.0f;  // Speed of rotation around X-axis
float rotationSpeedY = 0.0f;  // Speed of rotation around Y-axis
float rotationX = 0.0f;  // Angle of rotation around X-axis
float rotationY = 0.0f;  // Angle of rotation around Y-axis
float acceleration = 1.0f;     // How much speed increases per key press
float maxSpeed = 10.0f;        // Max rotation speed to prevent excessive spinning

// Light Position and Movement Speed

// TODO: Add the ability to change the light position when using keyboard inputs. See instructions on requirements

glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 5.0f); // Light starting position
float lightSpeed = 0.5f;  // Movement speed of light

// Cube Color Variables
glm::vec3 cubeColor = glm::vec3(1.0f, 0.5f, 0.31f); // Initial color

// Preset Colors for Cube
std::vector<glm::vec3> presetColors = {

// TODO: Add your own set of preset colors in glm::vec3 format to have your code toggle through the colors when
// the associated toggle color key is pressed. You may want to add a colorIndex variable to track the current 
// color selected.

    {1.0f, 0.5f, 0.31f}, // Light Red (default)
    {0.0f, 1.0f, 0.0f},  // Green
    {0.0f, 0.0f, 1.0f},  // Blue
    {1.0f, 1.0f, 0.0f},  // Yellow
    {1.0f, 0.0f, 1.0f},  // Magenta
    {0.0f, 1.0f, 1.0f},  // Cyan
    {1.0f, 1.0f, 1.0f},  // White
    {0.5f, 0.5f, 0.5f},  // Gray


};

glm::vec4 PresetColors[] = {
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  // Red
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // Green
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  // Blue
    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  // Yellow
    glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),  // Orange
    glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),  // Purple
};

glm::vec4 currentColor = PresetColors[0];
glm::vec4 targetColor = PresetColors[1];  // Starting color
float transitionSpeed = 0.01f;  // Speed of transition (0.0f to 1.0f)
float transitionProgress = 0.0f;

void updateColorTransition() {
    if (transitionProgress < 1.0f) {
        transitionProgress += transitionSpeed;
        currentColor = glm::mix(currentColor, targetColor, transitionProgress);
    }
}


int colorIndex = 0;  // Tracks the current color index

// ======================== FUNCTION DEFINITIONS ========================

// Compiles shaders and returns shader object ID
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Creates and links vertex and fragment shaders into a shader program
unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Error Checking
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Update viewport
    glViewport(0, 0, width, height);

    // Recalculate aspect ratio and adjust projection matrix
    float aspectRatio = (float)width / (float)height;
    glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);

    // Get shader program stored in window pointer
    unsigned int shaderProgram = *(unsigned int*)glfwGetWindowUserPointer(window);

    // Update projection matrix uniform
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    (void)scancode; // Suppress unused parameter warning
    (void)mods;      // Suppress unused parameter warning
    
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);  // Close window on ESC
        }

// TODO: Add keyboard input handlers to manage the keyboard inputs for the requirements of the assignment. Refer to 
// GLFW documentation to make sure where to add the GLFW keycallback command, and how to create your callback function.
    if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, true); // Close window on ESC
      }
    }

    // Toggle through preset colors with the 'C' key
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        colorIndex = (colorIndex + 1) % presetColors.size(); // Loop through colors
        cubeColor = presetColors[colorIndex];
        std::cout << "Cube color changed to: " << cubeColor.r << ", " << cubeColor.g << ", " << cubeColor.b << std::endl;
    }

    // Rotate the model with arrow keys
    if (key == GLFW_KEY_UP) {
        rotationX -= 5.0f;  // Rotate upwards
    }
    if (key == GLFW_KEY_DOWN) {
        rotationX += 5.0f;  // Rotate downwards
    }
    if (key == GLFW_KEY_LEFT) {
       rotationY -= 5.0f;  // Rotate left
    }
    if (key == GLFW_KEY_RIGHT) {
        rotationY += 5.0f;  // Rotate right
    }


    // Light position movement controls
    float lightMoveSpeed = 0.5f; // Movement step size

    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        lightPos.x -= lightMoveSpeed; // Move light left
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        lightPos.x += lightMoveSpeed; // Move light right
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        lightPos.y += lightMoveSpeed; // Move light up
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        lightPos.y -= lightMoveSpeed; // Move light down
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        lightPos.z -= lightMoveSpeed; // Move light forward
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        lightPos.z += lightMoveSpeed; // Move light backward
    }

    // Print updated light position
    std::cout << "Light position: (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;

    bool vKeyPressed = false;
    int currentColorIndex = 0;

   
        if (key == GLFW_KEY_V && action == GLFW_PRESS) {
            vKeyPressed = true;
            currentColorIndex = (currentColorIndex + 1) % 6;  // Cycle through colors
            targetColor = PresetColors[currentColorIndex];
            transitionProgress = 0.0f;  // Reset progress for the new color transition
        }
    if (vKeyPressed) {
        updateColorTransition();
        // Use currentColor for drawing the object
    }


}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, titleString.c_str(), NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Viewport
    glViewport(0, 0, 800, 600);

    // Define cube vertices with normals
    float vertices[] = {
    // TODO: This is where you define each of the vertices of the cube, including the normal for each vertex. The format for the 
    // array will be 3 values for the x, y, and x values of the vertex, followed by the x, y and z values for the normal of 
    // the vertex as a unit vector value. This can be done other ways, but it is ideal for the configured shaders provided
    // in the assignment.
            // Front face (normal = (0.0f, 0.0f, 1.0f))
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            // Back face (normal = (0.0f, 0.0f, -1.0f))
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            // Left face (normal = (-1.0f, 0.0f, 0.0f))
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

            // Right face (normal = (1.0f, 0.0f, 0.0f))
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            // Top face (normal = (0.0f, 1.0f, 0.0f))
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

            // Bottom face (normal = (0.0f, -1.0f, 0.0f))
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f


    };


    unsigned int indices[] = {
    // TODO: This is where you define your triangles for your faces. Make sure each value of the array is set
    // so each 3 values points to the index of the defined vertices of the triangle. Remember, order matters.
    
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    
    };


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int shaderProgram = createShaderProgram();
    glUseProgram(shaderProgram);

    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 1.0f, 1.0f, 5.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.5f, 0.31f);

    float rotationX = 0.0f;
    float rotationY = 0.0f;

    float aspectRatio = 800.0f / 600.0f;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);;

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowUserPointer(window, &shaderProgram);
    glfwSetKeyCallback(window, key_callback);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);



        // Create model transformation matrix (apply rotation)
        glm::mat4 model = glm::mat4(1.0f);
        
        // TODO: Apply the rotation to the model matrix here. Use rotationX and rotationY to track the angle of rotation

        model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
        model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis

      
        // Create view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Send uniforms to shader
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), cubeColor.x, cubeColor.y, cubeColor.z);

        // Render the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}


