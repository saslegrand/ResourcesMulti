#include "shader.hpp"

#include <fstream>

#include "resources_manager.hpp"
#include "debug.hpp"

#include "utils.hpp"

namespace Resources
{
#pragma region SHADER
	Shader::Shader(const std::string& shaderPath)
	{
		Core::Debug::Log::info("Loading " + shaderPath);

        std::string codeString = loadFromFile(shaderPath);
        const char* code = codeString.c_str();

        glShaderSource(shaderID, 1, &code, NULL);
        glCompileShader(shaderID);

        // Check if the compilation is a success
        GLint success;
        const GLsizei infoSize = 256;
        GLchar infoLog[infoSize];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shaderID, infoSize, NULL, infoLog);
            Core::Debug::Log::error(shaderPath + " shader compilation failed" + infoLog);
        }
	}

    Shader::~Shader()
    {
        if (shaderID)
            glDeleteShader(shaderID);
    }

	std::string Shader::loadFromFile(const std::string& filePath)
	{
        std::ifstream ifs(filePath);

        // Check if the file can be read
        if (!ifs.fail())
        {
            // Create the shader by checking its extension
            if (Utils::hasSuffix(filePath, ".vert"))
                shaderID = glCreateShader(GL_VERTEX_SHADER);

            else if (Utils::hasSuffix(filePath, ".frag"))
                shaderID = glCreateShader(GL_FRAGMENT_SHADER);

            else if (Utils::hasSuffix(filePath, ".geom"))
                shaderID = glCreateShader(GL_GEOMETRY_SHADER);

            else
                Core::Debug::Log::error("File extension is not compatible");
        }
        else
            Core::Debug::Log::error("Cannot open the file " + filePath);

        // Send the code to OpenGL as a char*
        std::string string_code;
        string_code.assign((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

        return string_code;
	}
#pragma endregion


#pragma region SHADER_PROGRAM
	ShaderProgram::ShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
        : programID(glCreateProgram()), name(programName)
	{
        Core::Debug::Log::info("Loading program " + programName);

        Core::Debug::Log::info("Linking " + vertPath + " and " + fragPath + " to " + programName);

        std::shared_ptr<Shader> vert = Resources::ResourcesManager::loadShader(vertPath);
        std::shared_ptr<Shader> frag = Resources::ResourcesManager::loadShader(fragPath);
        
        // Attach the two shaders and link them
        glAttachShader(programID, vert->shaderID);
        glAttachShader(programID, frag->shaderID);

        if (geomPath != "")
        {
            std::shared_ptr<Shader> geom = Resources::ResourcesManager::loadShader(geomPath);
            glAttachShader(programID, geom->shaderID);
        }

        glLinkProgram(programID);

        // Check if there is an error
        GLint success;
        const GLsizei infoSize = 256;
        GLchar infoLog[infoSize];
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programID, infoSize, NULL, infoLog);
            Core::Debug::Log::error(programName + " shader program linking failed: " + infoLog);
        }
        // If there is no error, load the uniform locations of the program
        else
            loadLocations();
	}

    ShaderProgram::~ShaderProgram()
    {
        if (programID)
            glDeleteProgram(programID);
    }

    void ShaderProgram::loadLocations()
    {
        uniforms.clear();

        // Get the active uniforms count
        GLint uniformCount;
        glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &uniformCount);

        // Loop over the active uniforms
        for (GLint i = 0; i < uniformCount; i++)
        {
            GLsizei nameLength;
            GLint size;             // Size of the variable
            GLenum type;            // Type of the variable
            const GLsizei bufSize = 256;
            GLchar uniName[bufSize];

            // Get the current uniform informations
            glGetActiveUniform(programID, i, bufSize, &nameLength, &size, &type, uniName);

            GLint location = glGetUniformLocation(programID, uniName);

            if (location < 0)
            {
                Core::Debug::Log::error("Cannot find the uniform named: " + std::string(uniName) + " - Location not valid");
                continue;
            }

            // Create a new uniform with the location and the type
            // And add it to a map
            uniforms[uniName] = { location, type };
        }

        int test = 0;
    }

    void ShaderProgram::setUniform(const std::string& target, const void* value,
                                   int count, bool transpose) const
    {
        // Get the iterator with the target
        const auto& currentIt = uniforms.find(target);

        // Check if this iterator is valid
        if (currentIt == uniforms.end())
        {
            Core::Debug::Log::error("Cannot find the uniform named: " + target + " - There is no uniform named like that");
            return;
        }

        const Uniform& uniform = currentIt->second;

        // Check if the location is valid
        if (uniform.location < 0)
        {
            Core::Debug::Log::error("Cannot find the uniform named: " + target + " - Location not valid");
            return;
        }

        // Call the correct function in function of the uniform's type
        #pragma region Uniform seter
        switch (uniform.type)
        {
            case GL_INT:
            case GL_BOOL:
            case GL_SAMPLER_2D_ARB:
            case GL_SAMPLER_CUBE_ARB:
                glUniform1iv(uniform.location, count, (GLint*)value);
                break;

            case GL_INT_VEC2:
            case GL_BOOL_VEC2:
                glUniform2iv(uniform.location, count, (GLint*)value);
                break;

            case GL_INT_VEC3:
            case GL_BOOL_VEC3:
                glUniform3iv(uniform.location, count, (GLint*)value);
                break;

            case GL_INT_VEC4:
            case GL_BOOL_VEC4:
                glUniform4iv(uniform.location, count, (GLint*)value);
                break;

            case GL_FLOAT:
                glUniform1fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_VEC2:
                glUniform2fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_VEC3:
                glUniform3fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_VEC4:
                glUniform4fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(uniform.location, count, transpose, (GLfloat*)value);
                break;

            case GL_FLOAT_MAT3:
                glUniformMatrix3fv(uniform.location, count, transpose, (GLfloat*)value);
                break;

            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(uniform.location, count, transpose, (GLfloat*)value);
                break;

            default:
                // If the type is not supported, log this message
                Core::Debug::Log::error("Type not supported");
                break;
        }
        #pragma endregion
    }

    void ShaderProgram::bind() const
    {
        glUseProgram(programID);
    }

    void ShaderProgram::unbind() const
    {
        glUseProgram(0);
    }

    std::string ShaderProgram::getName()
    {
        return name;
    }
}