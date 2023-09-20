#pragma once

#include <glad/glad.h>

#include <matrix.hpp>
#include <vector>

struct Vertex
{
	Vectorf3 Position;
	Vectorf2 Uv;
	Vectorf3 Normal;
};

class Mesh
{
private:
	unsigned int VAO = -1, VBO = -1, EBO = -1;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	Matrix4x4 local = Matrix4x4(true);

public:
	Mesh() = default;
	Mesh(const std::vector<Vertex>& _tmpVertices, const std::vector<uint32_t>& _tmpIdxPositions, const std::vector<uint32_t>& _tmpIdxUvs, const std::vector<uint32_t>& _tmpIdxNormals);
	~Mesh();

	void Unload();

	void SetVertices(const std::vector<Vertex>& _vertices);
	void SetIndices(const std::vector<unsigned int>& _indices);

	void SetupMesh();
	void Draw();
};