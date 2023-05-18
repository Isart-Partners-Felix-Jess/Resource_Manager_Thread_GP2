#include <Model.hpp>
#include <Log.hpp>

Model::~Model() 
{
	UnloadResource();
}

void Model::LoadResource(const char* _name)
{
	std::fstream file;
	std::filesystem::path path = "assets/meshes/";
	path += _name;
	file.open(path);
	//If we want to have the full path
	//m_ResourcePath = path.generic_string();
	if (file.bad())
	{
		DEBUG_WARNING("Model File %s is BAD", _name);
		return;
	}
	if (file.fail())
	{
		DEBUG_ERROR("Model File %s opening has FAILED", _name);
		return;
	}
	if(file.is_open())
	{
		Log::SuccessColor();
		DEBUG_LOG("Model File %s has been opened",_name);
		Log::ResetColor();
	}
}

void Model::UnloadResource()
{
	m_VertexBuffer.clear();
	m_IndexBuffer.clear();
}