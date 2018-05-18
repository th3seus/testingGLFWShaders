/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   camera.h
 * Author: root
 *
 * Created on May 15, 2018, 4:02 PM
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// Define seberal possible options for camera movement
// Used as abstraction to stay away from window-specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

typedef enum CameraMovement Camera_Movement;



struct Camera {
    // Camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    
    // Euler Angles
    float Yaw;
    float Pitch;
    
    // Camera Options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
};

typedef struct Camera CameraT;
typedef CameraT* CameraPtrT;
typedef CameraT* CameraAddrT;

// initialize camera struct using vectors
void initCameraVec(vec3 position, CameraAddrT* camera);

// initialize camera struct using scalars
void initCameraScal(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, CameraAddrT* camera);

// calculates view matrix using Euler angles and the LookAt Matrix
void GetViewMatrix(vec3 position, vec3 positionAndFront, vec3 up, mat4* m4);

// process movement input from the keyboard - wasd
void ProcessKeyboard(Camera_Movement direction, float deltaTime, CameraAddrT* camera);

// process movement input from the mouse CameraAddrT* camera);
void ProcessMouseMovement(float xOffset, float yOffset, CameraAddrT* camera);

// Process input received from scroll-wheel on mouse -- used for zoom
void ProcessMouseScroll(float yOffset, CameraAddrT* camera);

// Calculate the front vector from the Camera's (updated) Euler angles
void updateCameraVectors(CameraAddrT* camera);
#endif /* CAMERA_H */

