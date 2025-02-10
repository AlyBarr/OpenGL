// Include headers
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector> // Include vector for rotations and colorOffsets


// Default string to display
std::string displayString = "Alyssa Barrientos";


// Default string for window title
std::string titleString = "Assignment 0 - Alyssa Barrientos";


/* *************************************************** */


// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
void main() {
    gl_Position = transform * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
void main() {
    FragColor = vec4(color, 1.0);
}
)";

// Screen dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Rotation angles and color values
std::vector<float> rotations;
std::vector<float> colorOffsets;

// Letter data for alphabet
std::unordered_map<char, std::vector<float>> letters = {
    {'A', {
         0.6f,  0.6f,  0.1f,   0.6f, -0.6f,  0.1f,   0.3f,  0.6f,  0.1f,
         0.3f,  0.6f,  0.1f,   0.6f, -0.6f,  0.1f,   0.3f, -0.6f,  0.1f,
        -0.3f,  0.6f,  0.1f,  -0.3f, -0.6f,  0.1f,  -0.6f,  0.6f,  0.1f,
        -0.6f,  0.6f,  0.1f,  -0.3f, -0.6f,  0.1f,  -0.6f, -0.6f,  0.1f,
         0.3f,  0.6f,  0.1f,   0.3f,  0.4f,  0.1f,  -0.3f,  0.6f,  0.1f,
        -0.3f,  0.6f,  0.1f,  -0.3f,  0.4f,  0.1f,   0.3f,  0.6f,  0.1f,
         0.3f,  0.6f,  0.1f,  -0.3f,  0.4f,  0.1f,   0.3f,  0.4f,  0.1f,
        -0.3f,  0.1f,  0.1f,  -0.3f, -0.1f,  0.1f,   0.3f,  0.1f,  0.1f,
         0.3f,  0.1f,  0.1f,  -0.3f, -0.1f,  0.1f,   0.3f, -0.1f,  0.1f
    }},
    {'B', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.4f,  0.6f,  0.1f,
         0.4f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.4f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.4f,  0.1f,
         0.4f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.6f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.4f,  0.1f,  0.1f,
         0.4f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.4f, -0.1f,  0.1f,

        -0.6f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
        -0.6f,  -0.1f,  0.1f,   -0.6f,  -0.4f,  0.1f,  -0.3f,  -0.1f,  0.1f,
        -0.3f,  -0.1f,  0.1f,   -0.6f,  -0.4f,  0.1f,  -0.3f,  -0.4f,  0.1f,
         0.3f,  0.4f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.4f,  0.1f,
         0.6f,  0.4f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.3f, -0.1f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f, -0.1f,  0.1f,
         0.6f, -0.1f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f, -0.4f,  0.1f
    }},
    {'C', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.4f,  0.1f,
         0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f,
        -0.6f,  0.4f,  0.1f,   -0.6f, -0.4f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f, -0.4f,  0.1f,  -0.3f, -0.4f,  0.1f
    }},
    {'D', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.4f,  0.6f,  0.1f,
         0.4f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.4f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.4f,  0.1f,
         0.4f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.6f,  0.1f,

        -0.6f,  0.4f,  0.1f,   -0.6f, -0.4f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f, -0.4f,  0.1f,  -0.3f, -0.4f,  0.1f,
        
         0.3f,  0.4f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
         0.6f,  0.4f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f, -0.4f,  0.1f
    }},
    {'E', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.4f,  0.1f,
         0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f, -0.1f,  0.1f,

        -0.6f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
        -0.6f,  -0.1f,  0.1f,   -0.6f,  -0.4f,  0.1f,  -0.3f,  -0.1f,  0.1f,
        -0.3f,  -0.1f,  0.1f,   -0.6f,  -0.4f,  0.1f,  -0.3f,  -0.4f,  0.1f
    }},
    {'F', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f, -0.1f,  0.1f,

        -0.6f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
        -0.6f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.1f,  0.1f,
        -0.3f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.6f,  0.1f
    }},
    {'G', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.4f,  0.1f,
         0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f,
        -0.6f,  0.4f,  0.1f,   -0.6f, -0.4f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f, -0.4f,  0.1f,  -0.3f, -0.4f,  0.1f,

         0.3f,  0.1f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.6f,  0.1f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f, -0.4f,  0.1f
    }},
    {'H', {
        -0.6f,  0.6f,  0.1f,  -0.3f,  0.6f,  0.1f,  -0.3f, -0.6f, 0.1f,
        -0.6f,  0.6f,  0.1f,  -0.6f, -0.6f,  0.1f,  -0.3f, -0.6f, 0.1f,
         0.6f,  0.6f,  0.1f,   0.3f,  0.6f,  0.1f,   0.3f, -0.6f, 0.1f,
         0.6f,  0.6f,  0.1f,   0.6f, -0.6f,  0.1f,   0.3f, -0.6f, 0.1f,
        -0.3f,  0.1f,  0.1f,   -0.3f, -0.1f,  0.1f,   0.3f,  0.1f,  0.1f,
         0.3f,  0.1f,  0.1f,   -0.3f, -0.1f,  0.1f,   0.3f, -0.1f,  0.1f
    }},
    {'I', {
        -0.15f,  0.6f,  0.1f,   0.15f,  0.6f,  0.1f,   0.15f, -0.6f, 0.1f,
        -0.15f,  0.6f,  0.1f,  -0.15f, -0.6f,  0.1f,   0.15f, -0.6f, 0.1f
    }},
    {'J', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f, -0.6f, 0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.4f,  0.1f,
         0.4f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.6f,  0.1f,
        -0.3f,  0.1f,  0.1f,   -0.3f, -0.4f,  0.1f,  -0.6f,  0.1f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.3f, -0.4f,  0.1f,  -0.6f, -0.4f,  0.1f
    }},
    {'K', {
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.4f,  0.1f,  0.1f,
         0.4f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.4f, -0.1f,  0.1f,

        -0.6f,  0.6f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.6f,  0.1f,
        -0.3f,  0.6f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
        -0.6f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.1f,  0.1f,
        -0.3f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.6f,  0.1f,
         0.3f,  0.6f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.3f, -0.1f,  0.1f,    0.3f, -0.6f,  0.1f,   0.6f, -0.1f,  0.1f,
         0.6f, -0.1f,  0.1f,    0.3f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f
    }},
    {'L', {
        -0.6f,  0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,  -0.3f,  0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.4f,  0.1f,
         0.4f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.4f, -0.6f,  0.1f
    }},
    {'M', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.4f, 0.1f,    
        -0.1f,  0.4f,  0.1f,   0.1f,  0.4f,  0.1f,   0.1f, -0.6f,  0.1f,
        -0.1f,  0.4f,  0.1f,  -0.1f, -0.6f,  0.1f,   0.1f, -0.6f,  0.1f
    }},
    {'N', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.4f, 0.1f    
    }},
    {'O', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,

        -0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.4f, 0.1f,    

        -0.3f, -0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,   0.3f, -0.6f, 0.1f,
         0.3f, -0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,   0.3f, -0.4f, 0.1f    
    }},
    {'P', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f, -0.1f,  0.1f,

        -0.6f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
        -0.6f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.1f,  0.1f,
        -0.3f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.6f,  0.1f,
         0.3f,  0.4f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.4f,  0.1f,
         0.6f,  0.4f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.1f,  0.1f
    }},
    {'Q', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.4f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.4f, 0.1f,   0.3f, -0.4f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,  -0.6f, -0.4f, 0.1f,

        -0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,  -0.3f,  0.4f, 0.1f,   0.3f,  0.4f, 0.1f,    

        -0.3f, -0.4f, 0.1f,  -0.3f, -0.2f, 0.1f,   0.3f, -0.4f, 0.1f,
         0.3f, -0.4f, 0.1f,  -0.3f, -0.2f, 0.1f,   0.3f, -0.2f, 0.1f,    

         0.3f, -0.4f, 0.1f,   0.3f, -0.6f, 0.1f,   0.1f, -0.4f, 0.1f,
         0.1f, -0.4f, 0.1f,   0.3f, -0.6f, 0.1f,   0.1f, -0.6f, 0.1f

    }},
    {'R', {
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.4f,  0.1f,  0.1f,
         0.4f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.4f, -0.1f,  0.1f,

        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.4f,  0.6f,  0.1f,
         0.4f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.4f,  0.4f,  0.1f,

        -0.6f,  0.6f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.6f,  0.1f,
        -0.3f,  0.6f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
        -0.6f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.1f,  0.1f,
        -0.3f,  -0.1f,  0.1f,   -0.6f,  -0.6f,  0.1f,  -0.3f,  -0.6f,  0.1f,

         0.3f,  0.4f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.4f,  0.1f,
         0.6f,  0.4f,  0.1f,    0.3f,  0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.3f, -0.1f,  0.1f,    0.3f, -0.6f,  0.1f,   0.6f, -0.1f,  0.1f,
         0.6f, -0.1f,  0.1f,    0.3f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f
    }},
    {'S', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.4f,  0.1f,
         0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f, -0.1f,  0.1f,

        -0.6f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.4f,  0.1f,
        -0.3f,  0.4f,  0.1f,   -0.6f,  0.1f,  0.1f,  -0.3f,  0.1f,  0.1f,
         0.3f, -0.1f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f, -0.1f,  0.1f,
         0.6f, -0.1f,  0.1f,    0.3f, -0.4f,  0.1f,   0.6f, -0.4f,  0.1f
    }},
    {'T', {
        -0.6f,  0.6f,  0.1f,   0.6f,  0.6f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f,  0.6f,  0.1f,  -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.15f,  0.4f,  0.1f,   0.15f,  0.4f,  0.1f,   0.15f, -0.6f,  0.1f,
        -0.15f,  0.4f,  0.1f,  -0.15f, -0.6f,  0.1f,   0.15f, -0.6f,  0.1f
    }},
    {'U', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,

        -0.3f, -0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,   0.3f, -0.6f, 0.1f,
         0.3f, -0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,   0.3f, -0.4f, 0.1f    
    }},
    {'V', {
         0.6f,  0.6f, 0.1f,   0.6f,  0.1f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f,  0.1f, 0.1f,   0.3f,  0.1f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f,  0.1f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f,  0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,

         0.4f,  0.1f, 0.1f,   0.4f, -0.4f, 0.1f,   0.1f,  0.1f, 0.1f,
         0.1f,  0.1f, 0.1f,   0.4f, -0.4f, 0.1f,   0.1f, -0.4f, 0.1f,
        -0.1f,  0.1f, 0.1f,  -0.1f, -0.4f, 0.1f,  -0.4f,  0.1f, 0.1f,
        -0.4f,  0.1f, 0.1f,  -0.1f, -0.4f, 0.1f,  -0.4f, -0.4f, 0.1f,

        -0.2f, -0.6f, 0.1f,  -0.2f, -0.4f, 0.1f,   0.2f, -0.6f, 0.1f,
         0.2f, -0.6f, 0.1f,  -0.2f, -0.4f, 0.1f,   0.2f, -0.4f, 0.1f    
    }},
    {'W', {
         0.6f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f, -0.6f, 0.1f,   0.3f, -0.6f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f, -0.6f, 0.1f,  -0.6f, -0.6f, 0.1f,
        -0.3f, -0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,   0.3f, -0.6f, 0.1f,
         0.3f, -0.6f, 0.1f,  -0.3f, -0.4f, 0.1f,   0.3f, -0.4f, 0.1f,    
        -0.1f,  0.4f,  0.1f,   0.1f,  0.4f,  0.1f,   0.1f, -0.4f,  0.1f,
        -0.1f,  0.4f,  0.1f,  -0.1f, -0.4f,  0.1f,   0.1f, -0.4f,  0.1f
    }},
    {'X', {
         0.6f,  0.6f, 0.1f,   0.6f,  0.1f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f,  0.1f, 0.1f,   0.3f,  0.1f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f,  0.1f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f,  0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,

         0.6f, -0.6f, 0.1f,   0.6f, -0.1f, 0.1f,   0.3f, -0.6f, 0.1f,
         0.3f, -0.6f, 0.1f,   0.6f, -0.1f, 0.1f,   0.3f, -0.1f, 0.1f,
        -0.3f, -0.6f, 0.1f,  -0.3f, -0.1f, 0.1f,  -0.6f, -0.6f, 0.1f,
        -0.6f, -0.6f, 0.1f,  -0.3f, -0.1f, 0.1f,  -0.6f, -0.1f, 0.1f,

        -0.3f, -0.1f, 0.1f,  -0.3f,  0.1f, 0.1f,   0.3f, -0.1f, 0.1f,
         0.3f, -0.1f, 0.1f,  -0.3f,  0.1f, 0.1f,   0.3f,  0.1f, 0.1f    
    }},
    {'Y', {
         0.6f,  0.6f, 0.1f,   0.6f,  0.1f, 0.1f,   0.3f,  0.6f, 0.1f,
         0.3f,  0.6f, 0.1f,   0.6f,  0.1f, 0.1f,   0.3f,  0.1f, 0.1f,
        -0.3f,  0.6f, 0.1f,  -0.3f,  0.1f, 0.1f,  -0.6f,  0.6f, 0.1f,
        -0.6f,  0.6f, 0.1f,  -0.3f,  0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,

        -0.6f, -0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,   0.6f, -0.1f, 0.1f,
         0.6f, -0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,   0.6f,  0.1f, 0.1f,

        -0.15f, -0.1f,  0.1f,   0.15f, -0.1f,  0.1f,   0.15f, -0.6f,  0.1f,
        -0.15f, -0.1f,  0.1f,  -0.15f, -0.6f,  0.1f,   0.15f, -0.6f,  0.1f
    }},
    {'Z', {
        -0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.6f,  0.1f,
         0.6f,  0.6f,  0.1f,   -0.6f,  0.4f,  0.1f,   0.6f,  0.4f,  0.1f,
        -0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.4f,  0.1f,
         0.6f, -0.4f,  0.1f,   -0.6f, -0.6f,  0.1f,   0.6f, -0.6f,  0.1f,
        -0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f,  0.1f,  0.1f,
         0.6f,  0.1f,  0.1f,   -0.6f, -0.1f,  0.1f,   0.6f, -0.1f,  0.1f,

         0.6f,  0.4f,  0.1f,    0.6f,  0.1f,  0.1f,   0.3f,  0.4f,  0.1f,
         0.3f,  0.4f,  0.1f,    0.6f,  0.1f,  0.1f,   0.3f,  0.1f,  0.1f,
        -0.3f, -0.1f,  0.1f,   -0.3f, -0.4f,  0.1f,  -0.6f, -0.1f,  0.1f,
        -0.6f, -0.1f,  0.1f,   -0.3f, -0.4f,  0.1f,  -0.6f, -0.4f,  0.1f
    }},
    {'-', {
        -0.6f, -0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,   0.6f, -0.1f, 0.1f,
         0.6f, -0.1f, 0.1f,  -0.6f,  0.1f, 0.1f,   0.6f,  0.1f, 0.1f
    }},
    {' ', {
        // Empty space is represented by an empty vertex array
    }}
};


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, titleString.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Convert displayString to uppercase
    for (char& c : displayString) {
        c = std::toupper(c);
    }

    // Calculate spacing and scaling based on string length
    float spacing = 2.0f / displayString.length();
    float offset = -1.0f + (spacing / 2.0f); // Offset to center letters correctly
    float scale = std::min(0.5f, 1.0f / displayString.length()); // Adjust scale to fit all letters
    rotations.resize(displayString.length(), 0.0f);
    colorOffsets.resize(displayString.length());
    for (size_t i = 0; i < colorOffsets.size(); ++i) {
        colorOffsets[i] = static_cast<float>(i);
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        for (size_t i = 0; i < displayString.length(); ++i) {
            char letter = displayString[i];
            if (letters.find(letter) == letters.end()) {
                std::cerr << "Letter " << letter << " not defined!" << std::endl;
                continue;
            }

            std::vector<float>& letterVertices = letters[letter];

            // Generate VAO, VBO for the current letter
            unsigned int VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, letterVertices.size() * sizeof(float), letterVertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Transformation matrix
            float angle = rotations[i] * (float)M_PI / 180.0f;
            float cosA = cos(angle);
            float sinA = sin(angle);

            float transform[16] = {
                scale * cosA, 0.0f, scale * sinA, offset + (i * spacing),
                0.0f, scale, 0.0f, 0.0f,
                -scale * sinA, 0.0f, scale * cosA, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_TRUE, transform);

            // Set color
            float red = (sin(glfwGetTime() + colorOffsets[i]) + 1.0f) / 2.0f;
            float green = (sin(glfwGetTime() + colorOffsets[i] + 2.0f) + 1.0f) / 2.0f;
            float blue = (sin(glfwGetTime() + colorOffsets[i] + 4.0f) + 1.0f) / 2.0f;
            int colorLoc = glGetUniformLocation(shaderProgram, "color");
            glUniform3f(colorLoc, red, green, blue);

            // Draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, letterVertices.size() / 3);

            // Update rotation
            rotations[i] += 0.1f;

            // Clean up
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// Process input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

