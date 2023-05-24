#pragma once

#ifndef SHADER_H
#define SHADER_H
#endif

#include <vector>
#include <filesystem>
#include <vectorM.hpp>
#include <ResourcesManager.hpp>
#include <matrix.hpp>

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
	Shader(const char* _vertexPath, const char* _fragmentPath);
	~Shader();
	uint32_t GetShaderProgram() const;
	bool SetVertexShader(std::filesystem::path const& _filename);
	bool SetFragmentShader(std::filesystem::path const& _filename);
	bool Link();
	void Use();

	void SetBool(const std::string& _name, bool _value) const;
	void SetInt(const std::string& _name, int _value) const;
	void SetFloat(const std::string& _name, float _value) const;
	void SetMat4(const std::string& _name, Matrix4x4 _value) const;

	// Inherited from IResource
	virtual void LoadResource(const char* _name) override;
	virtual void UnloadResource() override;
};