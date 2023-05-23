#pragma once

#ifndef SHADER_H
#define SHADER_H
#endif

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
	//Creates an empty shader, are you sure you don't want to specify paths ?
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	uint32_t GetShaderProgram() const;
	bool SetVertexShader(std::filesystem::path const& filename);
	bool SetFragmentShader(std::filesystem::path const& filename);
	bool Link();
	void Use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	// Inherited from IResource
	virtual void LoadResource(const char* _name) override;
	virtual void UnloadResource() override;
};