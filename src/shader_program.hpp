#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    /*
        Construct a ShaderProgram object.
        Takes parameters pointing to the location of the shader code for each shader in the program.
        The code is read in from those files, then compiled.
    */
    ShaderProgram(const char* vert_path, const char* frag_path);
    /*
        Tell OpenGL that we want this shader program to be used.
        This must be called before any of the uniform setting functions are called.
    */
    void use();
    /* Set a Boolean Uniform in the shader program */
    void setUniformBool(const std::string& name, bool value);
    /* Set an Integer Uniform in the shader program */
    void setUniformInt(const std::string& name, int value);
    /* Set a Float Uniform in the shader program */
    void setUniformFloat(const std::string& name, float value);
    /* Set a 4x4 Matrix Uniform in the shader program */
    void setUniformMat4(const std::string& name, glm::mat4 value);
    /* Set a 3D Vector Uniform in the shader program */
    void setUniformVec3(const std::string& name, glm::vec3 value);

private:
    /* Helper function to read a file into a string */
    std::string readFile(const char* path);
    /* Helper function to compile GLSL shaders */
    unsigned int compileShader(const char* code, unsigned int shader_type, int& success, char* info_log, int log_size);

private:
    /* Hold the ID of the shader program used by OpenGL */
    unsigned int ProgramID;
};


#endif  // SHADER_PROGRAM_HPP