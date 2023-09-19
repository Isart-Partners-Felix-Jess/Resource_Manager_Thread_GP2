#include <Mesh.hpp>

Mesh::Mesh(const std::vector<Vertex>& _tmpVertices, const std::vector<uint32_t>& _tmpIdxPos, const std::vector<uint32_t>& _tmpIdxUvs, const std::vector<uint32_t>& _tmpIdxNormals)
{
	// Build final VAO (Mesh)
	size_t total_size = std::max({ _tmpIdxPos.size(), _tmpIdxUvs.size(), _tmpIdxNormals.size() });
	std::vector<Vertex> vertices;
	vertices.resize(total_size);
	for (size_t i = 0; i < total_size; i++)
	{
		vertices[i].Position = _tmpVertices[_tmpIdxPos[i] - 1].Position;

		if (!_tmpIdxUvs.empty())
			vertices[i].Uv = _tmpVertices[_tmpIdxUvs[i] - 1].Uv;
		else
			vertices[i].Uv = { 0 };

		if (!_tmpIdxNormals.empty())
			vertices[i].Normal = _tmpVertices[_tmpIdxNormals[i] - 1].Normal;
		else
			vertices[i].Normal = { 0 };
	}
	this->SetVertices(vertices);
}

Mesh::~Mesh()
{
	m_Indices.clear();
	m_Vertices.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::SetVertices(const std::vector<Vertex>& _vertices) {
	m_Vertices = _vertices;
}

void Mesh::SetIndices(const std::vector<unsigned int>& _indices) {
	m_Indices = _indices;
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0].Position[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Uv)));
	// Vertex normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}