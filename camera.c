/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>
#include "camera.h"

// Default Camera Values
float Yaw = -90.0f;
float Pitch = 0.0f;
float Speed = 2.5f;
float Sensitivity = 0.1f;
float Zoom = 45.0f;

// initialize camera struct using vectors
void initCameraVec(vec3 position, CameraAddrT* camera) {
    (*camera)->Position[0] = position[0];
    (*camera)->Position[1] = position[1];
    (*camera)->Position[2] = position[2];
    
    (*camera)->Front[0] = 0.0f;
    (*camera)->Front[1] = 0.0f;
    (*camera)->Front[2] = -1.0f;
    
    (*camera)->WorldUp[0] = 0.0f;
    (*camera)->WorldUp[1] = 1.0f;
    (*camera)->WorldUp[2] = 0.0f;
            
    (*camera)->Yaw = Yaw;
    (*camera)->Pitch = Pitch;
    
    (*camera)->MovementSpeed = Speed;
    (*camera)->MouseSensitivity = Sensitivity;
    (*camera)->Zoom = Zoom;
    
    updateCameraVectors(camera);
}

// initialize camera struct using scalars
void initCameraScal(float posX, float posY, float posZ, 
                    float upX, float upY, float upZ, 
                    float yaw, float pitch, CameraAddrT* camera) {
    (*camera)->Position[0] = posX;
    (*camera)->Position[1] = posY;
    (*camera)->Position[2] = posZ;
    
    (*camera)->WorldUp[0] = upX;
    (*camera)->WorldUp[1] = upY;
    (*camera)->WorldUp[2] = upZ;
    
    (*camera)->Yaw = yaw;
    (*camera)->Pitch = pitch;
    
    (*camera)->MovementSpeed = Speed;
    (*camera)->MouseSensitivity = Sensitivity;
    (*camera)->Zoom = Zoom;
    
    updateCameraVectors(camera);
}

// calculates view matrix using Euler angles and the LookAt Matrix
void GetViewMatrix(vec3 position, vec3 positionAndFront, vec3 up, mat4* m4) {
    glm_lookat(position, positionAndFront, up, *m4);
}

// process movement input from the keyboard - wasd
void ProcessKeyboard(Camera_Movement direction, float deltaTime, CameraAddrT* camera) {
    float velocity = (*camera)->MovementSpeed * deltaTime;
    int i;
    
    if (direction == FORWARD){
        for (i = 0; i < 3; i++)
            (*camera)->Position[i] += (*camera)->Front[i] * velocity;
        printf("position = %f", (*camera)->Position);
    }
    if (direction == BACKWARD) {
        for (i = 0; i < 3; i++)
            (*camera)->Position[i] -= (*camera)->Front[i] * velocity;
        printf("position = %f", (*camera)->Position);
    }
    if (direction == LEFT) {
        for (i = 0; i < 3; i++)
            (*camera)->Position[i] -= (*camera)->Right[i] * velocity;
        printf("position = %f", (*camera)->Position);
    }
    if (direction == RIGHT) {
        for (i = 0; i < 3; i ++)
            (*camera)->Position[i] += (*camera)->Right[i] * velocity;
        printf("position = %f", (*camera)->Position);
    }
}

// process movement input from the mouse
void ProcessMouseMovement(float xOffset, float yOffset, CameraAddrT* camera) {
    xOffset *= (*camera)->MouseSensitivity;
    yOffset *= (*camera)->MouseSensitivity;
    int constrainPitch = 1;
    
    (*camera)->Yaw += xOffset;
    (*camera)->Pitch += yOffset;
    
    if (constrainPitch) {
        if ((*camera)->Pitch > 89.0f)
            (*camera)->Pitch = 89.0f;
        if ((*camera)->Pitch < -89.0f)
            (*camera)->Pitch = -89.0f;
    }
    
    updateCameraVectors(camera);
}

// Process input received from scroll-wheel on mouse -- used for zoom
void ProcessMouseScroll(float yOffset, CameraAddrT* camera) {
    if ((*camera)->Zoom >= 1.0f && (*camera)->Zoom <= 45.0f)
        (*camera)->Zoom -= yOffset;
    if ((*camera)->Zoom <= 1.0f)
        (*camera)->Zoom = 1.0f;
    if ((*camera)->Zoom >= 45.0f)
        (*camera)->Zoom = 45.0f;
}

// Calculate the front vector from the Camera's (updated) Euler angles
void updateCameraVectors(CameraAddrT* camera) {
    // Calculate new Front vector
    vec3 front;
    
    //convert floats to radians
    glm_make_rad(&((*camera)->Yaw));
    glm_make_rad(&((*camera)->Pitch));
    
    front[0] = cos((*camera)->Yaw) * cos((*camera)->Pitch);
    front[1] = sin((*camera)->Pitch);
    front[2] = sin((*camera)->Yaw) * cos((*camera)->Pitch);
    
    glm_vec_normalize_to(front, (*camera)->Front);
    
    // cross vector products
    glm_vec_cross((*camera)->Front, (*camera)->WorldUp, (*camera)->Right);
    glm_vec_cross((*camera)->Right, (*camera)->Front, (*camera)->Up);
    
    // Calculate Right and Up vectors
    glm_vec_normalize((*camera)->Right);
    glm_vec_normalize((*camera)->Up);
    
    printf("Position = %f", (*camera)->Position);
}