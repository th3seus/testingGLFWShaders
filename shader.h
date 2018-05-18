/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   shader.h
 * Author: root
 *
 * Created on May 6, 2018, 2:17 PM
 */

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <setjmp.h>


struct Shader {
    unsigned int ID;
    unsigned int vertex;
    unsigned int fragment;
};

typedef struct Shader ShaderT;
typedef ShaderT* ShaderPtrT;
typedef ShaderT* ShaderAddrT;

/* Function Prototypes */

// init Shader struct
void initShader(ShaderAddrT* shader, char* vertexPath, char* fragmentPath);
char* readShaderFile(FILE* file);

// activate shader
void use(unsigned int ID);

// utility uniform functions
void setBool(char* name, int boolValue, unsigned int ID);
void setInt(char* name, int value, unsigned int ID);
void setFloat(char* name, float value, unsigned int ID);
void setVec2(char* name, vec2* value, unsigned int ID);
void setVec3v(char* name, vec3* value, unsigned int ID);
void setVec3f(char* name, float x, float y, float z, unsigned int ID);
void setVec4v(char* name, vec4* value, unsigned int ID);
void setVec4f(char* name, float x, float y, float z, float w, unsigned int ID);
// void setMat2(char* name, mat2& mat, unsigned int ID);
void setMat3(char* name, mat3* mat, unsigned int ID);
void setMat4(char* name, mat4* mat, unsigned int ID);

// utitlity function for checking shader compilation/linking errors
void checkCompileErrors(unsigned int shader, char* type);

#endif /* SHADER_H */

