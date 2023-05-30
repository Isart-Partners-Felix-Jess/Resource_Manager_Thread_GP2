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
	if (file.is_open())
	{
		Log::SuccessColor();
		DEBUG_LOG("Model File %s has been opened", _name);
		Log::ResetColor();

		//clear in case of double load
		m_Vertices.clear();
		m_Indices.clear();
		//load .obj
		std::string line;
		uint32_t vtIdx = 0;
		uint32_t vnIdx = 0;
		while (std::getline(file, line))
		{
			float x, y, z;
			// Process each line
			std::istringstream iss(line);
			std::string type;
			iss >> type;
			if (type == "#") //Note
			{
				continue;
			}
			else if (type == "v") // Vertex position
			{
				iss >> x >> y >> z;
				m_Vertices.push_back(Vertex{ Vectorf3{ x, y, z }, Vectorf3(), Vectorf2() }); //Assumes that vertex position is the first of the 3 coordinates
			}
			else if (type == "vt") // Texture position
			{
				iss >> x >> y;
				m_Vertices[vtIdx].Uv = Vectorf2(x, y);
				vtIdx++;
			}
			else if (type == "vn")// Normal position
			{
				iss >> x >> y >> z;
				m_Vertices[vnIdx].Normal = Vectorf3(x, y, z);
				vnIdx++;
			}
			else if (type == "f")// Face indices (assumes that model is an assembly of triangles only)
			{
				uint32_t i;
				char separator ='/'; // Variable to separator the slash characters
				while(!iss.eof())
				{
					// Ignore the separator '/' or ' ' if present
					if (iss.peek() == separator)
					{
						iss.ignore();
						if (iss.peek() == separator || iss.peek() == ' ')
						{
							m_Indices.push_back(0);
							iss.ignore();
						}
					}
				// Extract the value into i
				iss >> i;
				// Extract the three values directly into vi, ti, and ni
				m_Indices.push_back(i); // Store the vertex index // Subtract 1 to convert to 0-based indexing

				}
			}
			//Ask Erik : Records starting with the letter "l" (lowercase L) specify the order of the vertices which build a polyline.
			//mtl ?
			//o for obj name?
			//g for groupe name?
			//s shading
		}
		file.close();
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

	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0].Normal[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t),
		&m_Indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Uv)));

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
	glDrawArrays(GL_TRIANGLES,0, m_Vertices.size());
	//glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}