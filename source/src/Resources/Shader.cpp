#include <Shader.hpp>
#include <fstream>
#include <glad/glad.h>


Shader::Shader()
{
	s_m_TotalShaderNumber++;
	m_ShaderProgram = s_m_TotalShaderNumber;
}

// Initializing the static member variable
//unsigned int Shader::s_m_TotalShaderNumber = 0;
Shader::Shader(const char* vertexPath, const char* fragmentPath) : Shader()
{
	SetVertexShader(vertexPath);
	SetFragmentShader(fragmentPath);
	Link();
}
Shader::~Shader()
{
	UnloadResource();
}
uint32_t Shader::GetShaderProgram() const
{
	return m_ShaderProgram;
}
bool Shader::SetVertexShader(std::filesystem::path const& filename)
{
	std::ifstream file;
	file.open(filename);
	int  success = 0; //false
	if (file.bad())
	{
		DEBUG_ERROR("Shader file %s is BAD", filename);
	}
	else if (file.fail())
	{
		DEBUG_WARNING("Shader file %s opening has FAILED", filename);
	}
	else if (file.is_open())
	{
		std::string fileContent((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		const char* vertexShaderSource = fileContent.c_str();
		m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_VertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(m_VertexShader);
		char infoLog[512];
		glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_VertexShader, 512, NULL, infoLog);
			DEBUG_ERROR("SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog);
		}
	}//safe if file isn't open
	file.close();
	return static_cast<bool>(success);
}

bool Shader::SetFragmentShader(std::filesystem::path const& filename)
{
	std::ifstream file;
	file.open(filename);
	int  success = 0; //false
	if (file.bad())
	{
		DEBUG_ERROR("Shader file %s is BAD", filename);
	}
	else if (file.fail())
	{
		DEBUG_WARNING("Shader file %s opening has FAILED", filename);
	}
	else if (file.is_open())
	{
		std::string fileContent((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		const char* fragmentShaderSource = fileContent.c_str();
		m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_FragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(m_FragmentShader);
		char infoLog[512];
		glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_FragmentShader, 512, NULL, infoLog);
			DEBUG_ERROR("SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog);
		}
	}
	//safe if file isn't open
	file.close();
	return static_cast<bool>(success);
}

bool Shader::Link()
{
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);
	glLinkProgram(m_ShaderProgram);
	int success;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	char infoLog[512];
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		DEBUG_ERROR("SHADER::LINK::COMPILATION_FAILED\n" << infoLog);
	}
	UnloadResource(); //Maybe you delete in any case
	return success;
}

void Shader::LoadResource(const char* _name)
{
	std::string namestr(_name);
	SetVertexShader(namestr + ".vert"); //TODO: Check the extensions of the shader files
	SetFragmentShader(namestr + ".frag");
	Link();
}

void Shader::UnloadResource()
{
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

void Shader::Use()
{
	glUseProgram(m_ShaderProgram);
}

void Shader::setBool(const std::string& name, bool value) const
{
	//Should always work, if not change this to int
	bool valueLocation = static_cast<bool>(glGetUniformLocation(m_ShaderProgram, name.c_str()));
	glUniform1i(valueLocation, value);
}

void Shader::setInt(const std::string& name, int value) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, name.c_str());
	glUniform1i(valueLocation, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	float valueLocation = glGetUniformLocation(m_ShaderProgram, name.c_str());
	glUniform1f(valueLocation, value);
}

