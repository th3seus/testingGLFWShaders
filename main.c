/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: root
 *
 * Created on May 6, 2018, 1:49 PM
 */


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <KHR/khrplatform.h>
#include <cglm/call.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shader.h"
#include "camera.h"

/* Function Prototypes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

/* Settings */
#define SCR_WIDTH  800
#define SCR_HEIGHT  600

// Camera
CameraPtrT camera = NULL;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
int firstMouse = 1;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char** argv) {
    ShaderPtrT shader = NULL;
    mat4 projection;
    mat4 view;
    mat4 model;
    vec3 positionAndFront;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    }; 
    vec3 cubePositions[] = {
      {0.0f, 0.0f, 0.0f},
      {2.0f, 5.0f, -15.0f},
      {-1.5f, -2.2f, -2.5f},
      {-3.8f, -2.0f, -12.3f},
      {2.4f, -0.4f, -3.5f},
      {-1.7f, 3.0f, -7.5f},
      {1.3f, -2.0f, -2.5f},
      {1.5f, 2.0f, -2.5f},
      {1.5f, 0.f, -1.5f},
      {-1.3f, 1.0f, -1.5f}
              
    };
    
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    
    float currentFrame;
    float angle;
    int i;
    
    camera = (CameraPtrT)malloc(sizeof(CameraT));
    initCameraVec((vec3){0.0f, 0.0f, 3.0f}, &camera);
    
    /* glfw: initialize and configure
     ------------------------------------------*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    /* glfw window creation
     ------------------------------------------*/
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Doin' Shit", NULL, NULL);
    if (window == NULL) {
        printf("\nFailed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // Tell GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    /* glad: load all OpenGL function pointers 
     -----------------------------------------*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("\nFailed to initialize GLAD");
        return -1;
    }
    
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    
    /* tell OpenGL what size the window will be 
     --------------------------------------- */
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    
    /* build and compile shader program */
    shader = (ShaderPtrT)malloc(sizeof(ShaderT));
    
    initShader(&shader, "4.5.shader.vs", "4.5.shader.fs");
    
    /* set up vertex data and buffers. configure vertex attributes 
     ------------------------------------------*/
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffers, then configure vertex attributes
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);*/
    
    // position attribue
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5  * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    /* able to unbind VAO afterwards so other VAO calls won't accidentally modify
     this VAO, but this rarely happens. Modifying other VAOs requires a call to 
     glBindVertexArray(0) */
    
    /* render loop 
     ------------------------------------------- */
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        /* input 
         ----------------------------------- */
        processInput(window);
        //updateCameraVectors(&camera);
        
        /* render 
         ----------------------------------- */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // activate shader 
        use(shader->ID);
        
        // pass projection matrix to shader
        glm_make_rad(&(camera)->Zoom);
        glm_perspective((camera)->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);
        setMat4("projection", &projection, shader->ID);
        //updateCameraVectors(&camera);
        
        //camera/view transformation
        for (i = 0; i < 3; i++)
            positionAndFront[i] = (camera)->Position[i] + (camera)->Front[i];
        GetViewMatrix((camera)->Position, positionAndFront, (camera)->Up, &view);
        setMat4("view", &view, shader->ID);
        //updateCameraVectors(&camera);
        
        // render boxes
        glBindVertexArray(VAO);
        for (i = 0; i < 10; i++) {
            // calculate model matrix for each object and pass to shader before drawing
            glm_translate(model, cubePositions[i]);
            angle = 20.0f * i;
            glm_make_rad(&angle);
            glm_rotate(model, angle, (vec3){1.0f, 0.3f, 0.5f});
            setMat4("model", &model, shader->ID);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        /* glfw: swap buffers and poll IO events 
         ------------------------------------ */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    /* de-allocate all resources once they've outlived their purpose 
     ----------------------------------------------------- */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    /* de-allocate shader struct 
     ------------------------------------------------------ */
    free(shader);
    shader = NULL;
    
    /* glfw terminate - clear all previously allocated GLFW resources 
     -------------------------------------------------------- */
    glfwTerminate();
    return (0);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(FORWARD, deltaTime, &camera);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(BACKWARD, deltaTime, &camera);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(LEFT, deltaTime, &camera);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(RIGHT, deltaTime, &camera);
        
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = 0;
    }
    
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    ProcessMouseMovement(xOffset, yOffset, &camera);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    ProcessMouseScroll(yOffset, &camera);
}