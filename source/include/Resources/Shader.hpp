#pragma once
#include <vector>
#include <filesystem>
#include <vectorM.hpp>
#include <ResourcesManager.hpp>

class Shader : public IResource
{
private:
	inline static unsigned int s_m_TotalShaderNumber;
	unsigned int m_ShaderProgram;
	unsigned int m_VertexShader;
	unsigned int m_FragmentShader;
public:
	Shader();
	~Shader();
	bool SetVertexShader(std::filesystem::path const& filename);
	bool SetFragmentShader(std::filesystem::path const& filename);
	bool Link();
	void Use();
	// Hérité via IResource
	virtual void LoadResource(const char* _name) override;
	virtual void UnloadResource() override;
};