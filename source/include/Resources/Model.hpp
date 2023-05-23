#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <vectorM.hpp>
#include <vector>
#include <ResourcesManager.hpp>

struct Vertex {
	Vectorf3 Position;
	Vectorf3 Normal;
	Vectorf2 Uv;
};

class Model : public IResource
{
private:
	std::vector<Vertex> m_VertexBuffer;
	std::vector<uint32_t> m_IndexBuffer;
	~Model() override;
public:
	void LoadResource(const char* _name) override;
	void UnloadResource() override;
};
#endif // MODEL_H