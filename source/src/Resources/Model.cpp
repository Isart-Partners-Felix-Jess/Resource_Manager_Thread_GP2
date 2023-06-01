#include <Model.hpp>
#include <Log.hpp>
#include <glad/glad.h>
#include <Shader.hpp>
#include <Material.hpp>

Model::~Model()
{
	m_Indices.clear();
	m_Textures.clear();
	m_Vertices.clear();
	UnloadResource();
}

void Model::LoadResource(const char* _name)
{
	std::ifstream file;
	std::filesystem::path path = "assets/meshes/";
	path += _name;
	file.open(path);
	//If we want to have the full path
	//m_ResourcePath = path.generic_string();
	if (file.bad())
	{
		DEBUG_ERROR("Model File %s is BAD", _name);
		file.close();
		return;
	}
	if (file.fail())
	{
		DEBUG_WARNING("Model File %s opening has FAILED", _name);
		file.close();
		return;
	}
	//m_Vertices.push_back(Vertex{ {} });
		//temp vertex
	std::vector<Vertex> temp_Vertices;

	//temp index
	std::vector<uint32_t> temp_idx_Positions;
	std::vector<uint32_t> temp_idx_Uvs;
	std::vector<uint32_t> temp_idx_Normals;

	if (file.is_open())
	{
		Log::SuccessColor();
		DEBUG_LOG("Model File %s has been opened", _name);
		Log::ResetColor();

		//clear in case of double load
		temp_Vertices.clear();
		m_Vertices.clear();
		m_Indices.clear();
		//load .obj
		std::string line;
		unsigned int vIdx = 0;
		unsigned int vtIdx = 0;
		unsigned int vnIdx = 0;
		//only informative
		unsigned int faceIdx = 0;

		while (std::getline(file, line))
		{
			int hashpos = line.find('#');
			if (hashpos != -1)
				line = line.substr(0, hashpos);
			if (line.empty())
				continue;
			float x, y, z;
			// Process each line
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "v") // Vertex position
			{
				iss >> x >> y >> z;
				if(vIdx < temp_Vertices.size())
				temp_Vertices[vIdx].Position = Vectorf3{x, y, z}; //Assumes that vertex position is the first of the 3 coordinates
				else
					temp_Vertices.push_back({ Vectorf3(x, y,z)});
				vIdx++;
			}
			else if (type == "vt") // Texture position
			{
				iss >> x >> y;
				//temp_Vertices.Uv.push_back(Vectorf2{ x, y });
				if (vtIdx < temp_Vertices.size())
					temp_Vertices[vtIdx].Uv = Vectorf2(x, y);
				else
					temp_Vertices.push_back({ {}, Vectorf2(x, y) });
				vtIdx++;
			}
			else if (type == "vn")// Normal position
			{
				iss >> x >> y >> z;
				//temp_Normals.push_back(Vectorf3{ x, y, z });
				if (vnIdx < temp_Vertices.size())
					temp_Vertices[vnIdx].Normal = Vectorf3(x, y, z);
				else
					temp_Vertices.push_back({ {},{}, Vectorf3(x, y, z) });
				vnIdx++;
			}
			else if (type == "f")// Face indices (assumes that model is an assembly of triangles only)
			{
				unsigned int vertexIdx = 0;
				do {
					while (iss.peek() == ' ')
						iss.ignore();
					for (int elementsToAdd = 3; elementsToAdd > 0; --elementsToAdd)
					{
						unsigned int i = 0;
						// Extract the value into i
						iss >> i;
						if (!i)
							break;
						if (elementsToAdd == 3)
						{
							temp_idx_Positions.push_back(i);
							if(vertexIdx/2) // NewTriangle
							{
								m_Indices.push_back(faceIdx);
								m_Indices.push_back(faceIdx + vertexIdx - 1);
								m_Indices.push_back(faceIdx + vertexIdx);
							}
							vertexIdx++;
						}
						if (elementsToAdd == 2)
							temp_idx_Uvs.push_back(i);
						if (elementsToAdd == 1)
							temp_idx_Normals.push_back(i);

						if (iss.peek() == ' ')
						{
							break;
						}
						if (iss.peek() == '/')
						{
							iss.ignore();
							if (iss.peek() == '/')
							{
								elementsToAdd--;
							}
							continue;
						}
					}
				} while (iss);
				faceIdx += vertexIdx; //triangle count
			}


		}
		file.close();

	}
	//Build final VAO (Mesh)
	size_t total_size = std::max(temp_idx_Positions.size(), temp_idx_Uvs.size());
	total_size = std::max(total_size, temp_idx_Normals.size());
	m_Vertices.resize(total_size);
	for (size_t i = 0; i < total_size; ++i)
	{
		m_Vertices[i].Position = temp_Vertices[temp_idx_Positions[i] - 1].Position;
		m_Vertices[i].Uv = temp_Vertices[temp_idx_Uvs[i] - 1].Uv;
		m_Vertices[i].Normal = temp_Vertices[temp_idx_Normals[i] - 1].Normal;
	}
}

void Model::UnloadResource()
{
	m_Vertices.clear();
	m_Indices.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Model::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0].Position[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
		&m_Indices[0], GL_STATIC_DRAW);
	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Uv)));
	// vertex normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);



	glBindVertexArray(0);
}

void Model::Draw(Shader& _shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		//std::string name = m_Textures[i].type;
		//if (name == "texture_diffuse")
		//	number = std::to_string(diffuseNr++);
		//else if (name == "texture_specular")
		//	number = std::to_string(specularNr++);
		//_shader.SetInt(("material." + name + number).c_str(), i);
		material::none.InitShader(_shader);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].GetID());
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArrays(GL_TRIANGLES,0, m_Vertices.size());
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}