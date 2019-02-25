#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLuint &mode)
{
	GLchar* vertexShader;
	GLchar* fragmentShader;
	if (mode == SHADER_FROM_FILE)
	{
		// 1.1 Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderPath;
		std::ifstream fragmentShaderPath;
		// ensures ifstream objects can throw exceptions:
		vertexShaderPath.exceptions(std::ifstream::badbit);
		fragmentShaderPath.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vertexShaderPath.open(vertexPath);
			fragmentShaderPath.open(fragmentPath);
			std::stringstream vertexShaderStream, fragmentShaderStream;
			// Read file's buffer contents into streams
			vertexShaderStream << vertexShaderPath.rdbuf();
			fragmentShaderStream << fragmentShaderPath.rdbuf();
			// close file handlers
			vertexShaderPath.close();
			fragmentShaderPath.close();
			// Convert stream into string
			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		vertexShader = _strdup(vertexCode.c_str());
		fragmentShader = _strdup(fragmentCode.c_str());
	}
	// 1.2 From string
	else if (mode == SHADER_FROM_STRING)
	{
		vertexShader = _strdup(vertexPath);
		fragmentShader = _strdup(fragmentPath);
	}


	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShader, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShader, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// clean up shader code strings because of strdup
	delete vertexShader;
	delete fragmentShader;
}


Shader::~Shader()
{
}
