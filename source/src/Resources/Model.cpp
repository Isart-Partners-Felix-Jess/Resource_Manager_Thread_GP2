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
		unsigned int vtIdx = 0;
		unsigned int vnIdx = 0;
		//only informative
		unsigned int fIdx = 0;
		//m_Vertices.push_back(Vertex{ {} });
		while (std::getline(file, line))
		{
			int hashpos = line.find('#');
			if (hashpos != -1)
				line = line.substr(0, hashpos);
			if (line.empty())
				continue;
			std::vector<Vectorf3> temp_Positions;
			std::vector<Vectorf3> temp_Uvs;
			std::vector<Vectorf3> temp_Normals;

			float x, y, z;
			// Process each line
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "v") // Vertex position
			{
				iss >> x >> y >> z;
				temp_Positions.push_back( Vectorf3{ x, y, z }); //Assumes that vertex position is the first of the 3 coordinates
			}
			else if (type == "vt") // Texture position
			{
				iss >> x >> y;
				temp_Uvs.push_back(Vectorf2{x, y});
				//if (vtIdx < m_Vertices.size())
				//	m_Vertices[vtIdx].Uv = Vectorf2(x, y);
				//else
				//	m_Vertices.push_back({ {}, Vectorf2(x, y),{} });
				//vtIdx++;
			}
			else if (type == "vn")// Normal position
			{
				iss >> x >> y >> z;
				temp_Normals.push_back(Vectorf3{ x, y, z });
				//if (vtIdx < m_Vertices.size())
				//	m_Vertices[vnIdx].Normal = Vectorf3(x, y, z);
				//else
				//	m_Vertices.push_back({ {},{}, Vectorf3(x, y, z) });
				//vnIdx++;
			}
			else if (type == "f")// Face indices (assumes that model is an assembly of triangles only)
			{
				//only informative
				fIdx++;
				char separator = '/'; // Variable to separator the slash characters
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
						std::cout << i << std::endl;
						m_Indices.push_back(i-1);

						if (iss.peek() == ' ')
						{
							while (--elementsToAdd)
							{
								m_Indices.push_back(0);
							}
							continue;
						}
						if (iss.peek() == '/')
						{
							iss.ignore();
							if (iss.peek() == '/')
							{
								m_Indices.push_back(0);
								elementsToAdd--;
							}
							continue;
						}
					}
				} while (iss);

			}
			//Ask Erik : Records starting with the letter "l" (lowercase L) specify the order of the vertices which build a polyline.
			//mtl ?
			//o for obj name?
			//g for groupe name?
			//s shading
		}
		file.close();
	}
	for (int i = 0; i < m_Indices.size(); i++)
	{
		std::cout << m_Indices[i];
		if (i % 9 == 8)
			std::cout << std::endl;
		else if (i % 3 == 2)
			std::cout << ' ';
		else
			std::cout << '/';
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