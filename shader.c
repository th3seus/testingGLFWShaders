#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void initShader(ShaderAddrT* shader, char* vertexPath, char* fragmentPath) {
    const char* vertexCode = NULL;
    const char* fragmentCode = NULL;
    FILE* vShaderFile = NULL;
    FILE* fShaderFile = NULL;
    long length = 0;
    
    vShaderFile = fopen(vertexPath, "r");
    fShaderFile = fopen(fragmentPath, "r");
    
    /* Check File Streams for Errors before proceeding */
    if (!vShaderFile){
        printf("\nERROR:SHADER::FILE NOT SUCCESSFULLY READ\n%d", errno);
        fclose(vShaderFile);
        free(vShaderFile);
        vShaderFile = NULL;
    } else {
        vertexCode = readShaderFile(vShaderFile);
        fclose(vShaderFile);
    }
    
    if (!fShaderFile) {
        printf("\nERROR:SHADER::FILE NOT SUCCESSFULLY READ\n%d", errno);
        fclose(fShaderFile);
        free(fShaderFile);
        fShaderFile = NULL;
    } else {
        fragmentCode = readShaderFile(fShaderFile);
        fclose(fShaderFile);
    }
    
    if (vertexCode && fragmentCode) {
        // vertex Shader
        (*shader)->vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource((*shader)->vertex, 1, vertexCode, NULL);
        glCompileShader((*shader)->vertex);
        // checkCompileErrors((*shader)->vertex, "VERTEX");
        
        // fragment Shader
        (*shader)->fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource((*shader)->fragment, 1, fragmentCode, NULL);
        glCompileShader((*shader)->fragment);
        // checkCompileErrors((*shader)->fragment, "FRAGMENT");
        
        // shader Program
        (*shader)->ID = glCreateProgram();
        glAttachShader((*shader)->ID, (*shader)->vertex);
        glAttachShader((*shader)->ID, (*shader)->fragment);
        glLinkProgram((*shader)->ID);
        // checkCompileErrors((*shader)->ID, "PROGRAM");
        
        // delete shaders now that they're attached to the program
        glDeleteShader((*shader)->vertex);
        glDeleteShader((*shader)->fragment);
    }
}

char* readShaderFile(FILE* file) {
    char* buffer = NULL;
    long length = 0;
    
    fseek(file, 0, SEEK_END); // seeks the end of the file stream
    length = ftell(file); // sets length to current value of the file stream
    fseek(file, 0, SEEK_SET);
    buffer = malloc(length);
        
    if (buffer) {
        /* reads a number of items determined by length variable,
        each 1 byte long, from the file stream, and stores
        them in the buffer */
        fread(buffer, 1, length, file);
    }
    
    return buffer;
}

void use(unsigned int ID) {
    glUseProgram(ID);
}

void setBool(char* name, int boolValue, unsigned int ID) {
    glUniform1i(glGetUniformLocation(ID, name), boolValue);
}

void setInt(char* name, int value, unsigned int ID) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void setFloat(char* name, float value, unsigned int ID) {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void CheckCompileErrors(unsigned int shader, char* type) {
    int success;
    char infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("\nERROR:SHADER_COMPILATION_ERROR of type: %s\n%s\n"
                    "-- -------------------------------------------- --\n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("\nERROR:PROGRAM_COMPILATION_ERROR of type: %s\n%s\n"
                    "-- -------------------------------------------- --\n", type, infoLog);                  
        }
    }
}