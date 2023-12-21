#include "shader_program.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/ext.hpp>

ShaderProgram::ShaderProgram(const char* vert_path, const char* frag_path)
{
    /* 1. Read the shader code in from their files. */    
    std::string vert_str = readFile(vert_path);
    std::string frag_str = readFile(frag_path);
    const char* vert_code = vert_str.c_str();
    const char* frag_code = frag_str.c_str();

    /* 2. Compile the shaders. */
    int success;
    char info_log[512];
    unsigned int vertex = compileShader(vert_code, GL_VERTEX_SHADER, success, info_log, sizeof(info_log));
    unsigned int fragment = compileShader(frag_code, GL_FRAGMENT_SHADER, success, info_log, sizeof(info_log));

    /* 3. Build the program*/
    this->ProgramID = glCreateProgram();
    glAttachShader(this->ProgramID, vertex);
    glAttachShader(this->ProgramID, fragment);
    glLinkProgram(this->ProgramID);
    glGetProgramiv(this->ProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->ProgramID, sizeof(info_log), NULL, info_log);
        std::cerr << "Shader program linking failed.\n" << info_log << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void ShaderProgram::use()
{
    glUseProgram(this->ProgramID);
}

void ShaderProgram::setUniformBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(this->ProgramID, name.c_str()), int(value));
}
void ShaderProgram::setUniformInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(this->ProgramID, name.c_str()), value);
}
void ShaderProgram::setUniformFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(this->ProgramID, name.c_str()), value);
}
void ShaderProgram::setUniformMat4(const std::string& name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(this->ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::setUniformVec3(const std::string& name, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(this->ProgramID, name.c_str()), 1, glm::value_ptr(value));
}

// --------------------------------- Private Methods ------------------------------

std::string ShaderProgram::readFile(const char* path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
    }
    // Create a stringstream to store the file content
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    // Extract content from the stringstream as a string
    return buffer.str();
}

unsigned int ShaderProgram::compileShader(const char* code, unsigned int shader_type, int& success, char* info_log, int log_size)
{
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, log_size, NULL, info_log);
        std::cerr << "Shader compilation failed.\n" << info_log << std::endl;
    }

    return shader;
}
