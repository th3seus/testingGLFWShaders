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

/* Function Definitions */

// init Shader struct
void initShader(ShaderAddrT* shader, char* vertexPath, char* fragmentPath);
char* readShaderFile(FILE* file);

// activate shader
void use(unsigned int ID);

// utility uniform functions
void setBool(char* name, int boolValue, unsigned int ID);
void setInt(char* name, int value, unsigned int ID);
void setFloat(char* name, float value, unsigned int ID);

// utitlity function for checking shader compilation/linking errors
void CheckCompileErrors(unsigned int shader, char* type);

#endif /* SHADER_H */

