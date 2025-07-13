#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Engine {
    class GLShader
    {
    public:
        // state
        unsigned int id;
        // constructor
        GLShader(const char* vertexShader, const char* fragmentShader);
        // Sets shader as the active shader
        GLShader& Use();
        // compiles the shader from given source code
        void Compile(const char* vertexShader, const char* fragmentShader);

        void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
        void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
    };
}


