#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <vectorM.hpp>
#include <vector>
#include <ResourcesManager.hpp>
#include <Texture.hpp>

class Shader;

struct Vertex {
	Vectorf3 Position;
	Vectorf3 Normal;
	Vectorf2 Uv;
};

class Model : public IResource
{
private:
	unsigned int VAO, VBO, EBO;

	~Model() override;
//public: ?
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	std::vector<Texture> m_Textures;

public:
	void LoadResource(const char* _name) override;
	void UnloadResource() override;

	void SetupMesh();
	void Draw(Shader& _shader);

};
#endif // MODEL_H