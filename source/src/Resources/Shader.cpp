#include <Shader.hpp>

Shader::Shader()
{
	s_m_TotalShaderNumber++;
	IResource::m_ResourceId = s_m_TotalShaderNumber;
}

// Initializing the static member variable
Shader::Shader(const char* _vertexPath, const char* _fragmentPath)
{
	ReadVertexShader(_vertexPath);
	ReadFragmentShader(_fragmentPath);
	Link();
}

Shader::~Shader() {
	ResourceUnload();
}

uint32_t Shader::GetShaderProgram() const {
	return m_ShaderProgram;
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
		vertexShaderSource = fileContent.c_str();
		success = true;
	} // Safe if file isn't open

	file.close();
	return success;
}

bool Shader::SetVertexShader()
{
	int  success = 0; // false
	const char* vertSrc = vertexShaderSource.c_str();
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShader, 1, &vertSrc, NULL);
	glCompileShader(m_VertexShader);

	char infoLog[512];
	glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_VertexShader, 512, NULL, infoLog);
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
		DEBUG_ERROR("Shader #%i file %s is BAD", m_ShaderProgram, _filename);
	}
	else if (file.fail())
	{
		DEBUG_WARNING("Shader #%i file %s opening has FAILED", m_ShaderProgram, _filename);
	}
	else if (file.is_open())
	{
		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		fragmentShaderSource = fileContent;
		success = true;
	}// Safe if file isn't open

	file.close();
	return success;
}

bool Shader::SetFragmentShader()
{
	int success = 0; // false
	const char* fragSrc = fragmentShaderSource.c_str();

	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShader, 1, &fragSrc, NULL);
	glCompileShader(m_FragmentShader);

	char infoLog[512];
	glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_FragmentShader, 512, NULL, infoLog);
		DEBUG_ERROR("SHADER #" << m_ShaderProgram << "::FRAGMENT::COMPILATION_FAILED\n" << infoLog);
	}
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
		DEBUG_ERROR("SHADER #" << m_ShaderProgram << "::LINK::COMPILATION_FAILED\n" << infoLog);
	}
	else
	{
		Log::SuccessColor();
		DEBUG_LOG("SHADER #%i::LINK::SUCCESS\n", m_ShaderProgram);
		Log::ResetColor();
	}

	isLoaded = true;
	DeleteVertFrag(); // Maybe you delete in any case
	return success;
}

// Only to name the shader
void Shader::ResourceLoad(const std::string _name, bool isMultiThread) {
	IResource::m_ResourcePath = _name;
}

void Shader::ResourceUnload()
{
	DeleteVertFrag();
	DeleteProgram();
}

void Shader::DeleteVertFrag()
{
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

void Shader::DeleteProgram() {
	glDeleteProgram(m_ShaderProgram);
}

void Shader::Use() {
	glUseProgram(m_ShaderProgram);
}

void Shader::ResetCount() {
	s_m_TotalShaderNumber = 0;
}

void Shader::SetBool(const std::string& _name, bool _value) const
{
	//Should always work, if not change this to int
	int valueLocation = static_cast<bool>(glGetUniformLocation(m_ShaderProgram, _name.c_str()));
	glUniform1i(valueLocation, _value);
}

void Shader::SetInt(const std::string& _name, int _value) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	glUniform1i(valueLocation, _value);
}

void Shader::SetUint(const std::string& _name, unsigned int _value) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	glUniform1ui(valueLocation, _value);
}

void Shader::SetFloat(const std::string& _name, float _value) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	glUniform1f(valueLocation, _value);
}

void Shader::SetVec2(const std::string& _name, Vectorf2 _value) const {
	SetVec2(_name, _value.X(), _value.Y());
}

void Shader::SetVec2(const std::string& _name, float _valueX, float _valueY) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	glUniform2f(valueLocation, _valueX, _valueY);
}

void Shader::SetVec3(const std::string& _name, Vectorf3 _value) const {
	SetVec3(_name, _value.X(), _value.Y(), _value.Z());
}

void Shader::SetVec3(const std::string& _name, float _valueX, float _valueY, float _valueZ) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	glUniform3f(valueLocation, _valueX, _valueY, _valueZ);
}

void Shader::SetVec4(const std::string& _name, Vectorf4 _value) const {
	SetVec4(_name, _value.X(), _value.Y(), _value.Z(), _value.Z());
}

void Shader::SetVec4(const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	glUniform4f(valueLocation, _valueX, _valueY, _valueZ, _valueZ);
}

void Shader::SetMat3(const std::string& _name, Matrix3x3 _value) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	float elements[9];
	for (int i = 0; i < 9; i++)
	{
		elements[i] = _value[i % 3][i / 3];
	}
	glUniformMatrix3fv(valueLocation, 1, GL_FALSE, elements);
}

void Shader::SetMat4(const std::string& _name, Matrix4x4 _value) const
{
	int valueLocation = glGetUniformLocation(m_ShaderProgram, _name.c_str());
	float elements[16];
	for (int i = 0; i < 16; i++)
	{
		elements[i] = _value[i % 4][i / 4];
	}
	glUniformMatrix4fv(valueLocation, 1, GL_FALSE, elements);
}