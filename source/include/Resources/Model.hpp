#pragma once
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

public:
	void LoadResources(const char* _name);
};

