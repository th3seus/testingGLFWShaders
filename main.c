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
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"

/* Function Prototypes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/* Settings */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv) {
    ShaderPtrT shader = NULL;
    float vertices[] = {
        // positions        // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // top
    };
    unsigned int VBO;
    unsigned int VAO;
    
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
    
    /* glad: load all OpenGL function pointers 
     -----------------------------------------*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("\nFailed to initialize GLAD");
        return -1;
    }
    
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
    // bind the Vertex Array Object first, then bind and set vertex buffers, then configure vertex attributes
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // position attribue
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6  * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    /* able to unbind VAO afterwards so other VAO calls won't accidentally modify
     this VAO, but this rarely happens. Modifying other VAOs requires a call to 
     glBindVertexArray(0) */
    
    /* render loop 
     ------------------------------------------- */
    while (!glfwWindowShouldClose(window)) {
        /* input 
         ----------------------------------- */
        processInput(window);
        
        /* render 
         ----------------------------------- */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* render triangles 
         ----------------------------------- */
        use(shader->ID);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
