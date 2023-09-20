#include <Shader.hpp>

Shader::Shader()
{
	s_m_totalShaderNumber++;
	IResource::m_resourceId = s_m_totalShaderNumber;
}

Shader::~Shader() {
	ResourceUnload();
}

uint32_t Shader::GetShaderProgram() const {
	return m_shaderProgram;
}

bool Shader::ReadVertexShader(std::filesystem::path const& _filename)
{
	std::ifstream file;
	file.open(_filename);
	bool success = false;

	if (file.bad())
	{
		DEBUG_ERROR("Shader file %s is BAD", _filename);
	}
	else if (file.fail())
	{
		DEBUG_WARNING("Shader file %s opening has FAILED", _filename);
	}
	else if (file.is_open())
	{
		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		m_vertexShaderSource = fileContent.c_str();
		success = true;
	} // Safe if file isn't open

	m_isRead = true;
	file.close();
	return success;
}

bool Shader::SetVertexShader()
{
	int  success = 0; // false
	const char* vertSrc = m_vertexShaderSource.c_str();
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &vertSrc, NULL);
	glCompileShader(m_vertexShader);

	char infoLog[512];
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
		DEBUG_ERROR("SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog);
	}
	return static_cast<bool>(success);
}

bool Shader::ReadFragmentShader(std::filesystem::path const& _filename)
{
	std::ifstream file;
	file.open(_filename);
	bool success = false;

	if (file.bad())
	{
		DEBUG_ERROR("Shader #%i file %s is BAD", m_shaderProgram, _filename);
	}
	else if (file.fail())
	{
		DEBUG_WARNING("Shader #%i file %s opening has FAILED", m_shaderProgram, _filename);
	}
	else if (file.is_open())
	{
		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		m_fragmentShaderSource = fileContent;
		success = true;
	}// Safe if file isn't open

	m_isRead = true;
	file.close();
	return success;
}

bool Shader::SetFragmentShader()
{
	int success = 0; // false
	const char* fragSrc = m_fragmentShaderSource.c_str();

	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(m_fragmentShader);

	char infoLog[512];
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
		DEBUG_ERROR("SHADER #" << m_shaderProgram << "::FRAGMENT::COMPILATION_FAILED\n" << infoLog);
	}
	return static_cast<bool>(success);
}

bool Shader::Link()
{
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);
	glLinkProgram(m_shaderProgram);

	int success;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	char infoLog[512];
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		DEBUG_ERROR("SHADER #" << m_shaderProgram << "::LINK::COMPILATION_FAILED\n" << infoLog);
	}
	else
	{
		Log::SuccessColor();
		DEBUG_LOG("SHADER #%i::LINK::SUCCESS\n", m_shaderProgram);
		Log::ResetColor();
	}

	m_isLoaded = true;
	DeleteVertFrag(); // Maybe you delete in any case
	return success;
}

// Only to name the shader
void Shader::SetName(const std::string _name) {
	m_resourcePath = _name;
}

void Shader::ResourceUnload()
{
	DeleteVertFrag();
	DeleteProgram();
}

void Shader::DeleteVertFrag()
{
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
}

void Shader::DeleteProgram() {
	glDeleteProgram(m_shaderProgram);
}

void Shader::Use() {
	glUseProgram(m_shaderProgram);
}

void Shader::ResetCount() {
	s_m_totalShaderNumber = 0;
}

void Shader::SetBool(const std::string& _name, bool _value) const
{
	//Should always work, if not change this to int
	int valueLocation = static_cast<bool>(glGetUniformLocation(m_shaderProgram, _name.c_str()));
	glUniform1i(valueLocation, _value);
}

void Shader::SetInt(const std::string& _name, int _value) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	glUniform1i(valueLocation, _value);
}

void Shader::SetUint(const std::string& _name, unsigned int _value) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	glUniform1ui(valueLocation, _value);
}

void Shader::SetFloat(const std::string& _name, float _value) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	glUniform1f(valueLocation, _value);
}

void Shader::SetVec2(const std::string& _name, Vectorf2 _value) const {
	SetVec2(_name, _value.X(), _value.Y());
}

void Shader::SetVec2(const std::string& _name, float _valueX, float _valueY) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	glUniform2f(valueLocation, _valueX, _valueY);
}

void Shader::SetVec3(const std::string& _name, Vectorf3 _value) const {
	SetVec3(_name, _value.X(), _value.Y(), _value.Z());
}

void Shader::SetVec3(const std::string& _name, float _valueX, float _valueY, float _valueZ) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	glUniform3f(valueLocation, _valueX, _valueY, _valueZ);
}

void Shader::SetVec4(const std::string& _name, Vectorf4 _value) const {
	SetVec4(_name, _value.X(), _value.Y(), _value.Z(), _value.Z());
}

void Shader::SetVec4(const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	glUniform4f(valueLocation, _valueX, _valueY, _valueZ, _valueZ);
}

void Shader::SetMat3(const std::string& _name, Matrix3x3 _value) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	float elements[9];
	for (int i = 0; i < 9; i++)
	{
		elements[i] = _value[i % 3][i / 3];
	}
	glUniformMatrix3fv(valueLocation, 1, GL_FALSE, elements);
}

void Shader::SetMat4(const std::string& _name, Matrix4x4 _value) const
{
	int valueLocation = glGetUniformLocation(m_shaderProgram, _name.c_str());
	float elements[16];
	for (int i = 0; i < 16; i++)
	{
		elements[i] = _value[i % 4][i / 4];
	}
	glUniformMatrix4fv(valueLocation, 1, GL_FALSE, elements);
}