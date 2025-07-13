#include "GLShader.h"
#include "engine/core/core.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {
    GLShader::GLShader(const char* vertexSource, const char* fragmentSource) {
        unsigned int vs, fs;

        // vertex Shader
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexSource, NULL);
        glCompileShader(vs);

        // Ensure shader compilation succeeded
        int vtSuccess;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &vtSuccess);
        if (!vtSuccess) {
            char infoLog[512];
            glGetShaderInfoLog(vs, 512, NULL, infoLog);
            EG_CORE_FATAL("Vertex shader compilation failed! {0}", infoLog);
            EG_CORE_ASSERT(false, "OpenGL Error");
        }

        // fragment Shader
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentSource, NULL);
        glCompileShader(fs);

        // Ensure shader compilation succeeded
        int fragSuccess;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &fragSuccess);
        if (!fragSuccess) {
            char infoLog[512];
            glGetShaderInfoLog(fs, 512, NULL, infoLog);
            EG_CORE_FATAL("Fragment shader compilation failed! {0}", infoLog);
            EG_CORE_ASSERT(false, "OpenGL Error");
        }

        // shader program
        this->id = glCreateProgram();
        glAttachShader(this->id, vs);
        glAttachShader(this->id, fs);
        glLinkProgram(this->id);

        // Ensure shader program linking succeeded
        int linkSuccess;
        glGetProgramiv(this->id, GL_LINK_STATUS, &linkSuccess);
        if (!linkSuccess) {
            char infoLog[512];
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            EG_CORE_FATAL("Shader program linking failed! {0}", infoLog);
            EG_CORE_ASSERT(false, "OpenGL Error");
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    GLShader& GLShader::Use() {
        glUseProgram(this->id);
        return *this;
    }

    void GLShader::Compile(const char* vertexSource, const char* fragmentSource) {
        unsigned int vs, fs;

        // vertex Shader
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexSource, NULL);
        glCompileShader(vs);

        // Ensure shader compilation succeeded
        int vtSuccess;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &vtSuccess);
        if (!vtSuccess) {
            char infoLog[512];
            glGetShaderInfoLog(vs, 512, NULL, infoLog);
            EG_CORE_FATAL("Vertex shader compilation failed! {0}", infoLog);
            EG_CORE_ASSERT(false, "OpenGL Error");
        }

        // fragment Shader
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentSource, NULL);
        glCompileShader(fs);

        // Ensure shader compilation succeeded
        int fragSuccess;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &fragSuccess);
        if (!fragSuccess) {
            char infoLog[512];
            glGetShaderInfoLog(fs, 512, NULL, infoLog);
            EG_CORE_FATAL("Fragment shader compilation failed! {0}", infoLog);
            EG_CORE_ASSERT(false, "OpenGL Error");
        }

        // shader program
        this->id = glCreateProgram();
        glAttachShader(this->id, vs);
        glAttachShader(this->id, fs);
        glLinkProgram(this->id);

        // Ensure shader program linking succeeded
        int linkSuccess;
        glGetProgramiv(this->id, GL_LINK_STATUS, &linkSuccess);
        if (!linkSuccess) {
            char infoLog[512];
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            EG_CORE_FATAL("Shader program linking failed! {0}", infoLog);
            EG_CORE_ASSERT(false, "OpenGL Error");
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void GLShader::SetVector3f(const char* name, const glm::vec3& value, bool useShader) {
        if (useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
    }

    void GLShader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader) {
        if (useShader)
            this->Use();
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
    }
}