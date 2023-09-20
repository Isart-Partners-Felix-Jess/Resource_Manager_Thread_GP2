#include <Model.hpp>

#include <Scene.hpp>
#include <Graph.hpp>

static unsigned int s_ModelNumber = 0;

void Model::ResourceFileRead(const std::string _name)
{
	m_resourceId = s_ModelNumber++;
	std::ifstream file;
	std::filesystem::path path = "assets/meshes/";
	path += _name + std::string(".obj");
	file.open(path);
	// If we want to have the full path
	//m_resourcePath = path.generic_string();
	if (file.bad())
	{
		DEBUG_ERROR("Model File %s is BAD", _name.c_str());
		file.close();
		return;
	}
	if (file.fail())
	{
		DEBUG_WARNING("Model File %s opening has FAILED", _name.c_str());
		file.close();
		return;
	}
	if (file.is_open())
	{
		Log::SuccessColor();
		DEBUG_LOG("Model File %s has been opened", _name.c_str());
		Log::ResetColor();

		// Clear in case of double load
		m_tmpVertices.clear();
		// Load .obj
		std::string line;
		unsigned int vIdx = 0;
		unsigned int vtIdx = 0;
		unsigned int vnIdx = 0;
		unsigned int faceIdx = 0;

		while (std::getline(file, line))
		{
			if (line.empty())
				continue;
			float x, y, z;
			// Process each line
			std::istringstream iss(line);
			std::string type;
			iss >> type;
			if (type == "#")
				continue;
			if (type[0] == 'v')
			{
				iss >> x >> y >> z;
				if (type == "v") // Vertex position
				{
					if (vIdx < m_tmpVertices.size())
						m_tmpVertices[vIdx].Position = Vectorf3{ x, y, z };
					else
						m_tmpVertices.push_back({ Vectorf3(x, y,z) });
					vIdx++;
				}
				else if (type[1] == 't') // Texture position
				{
					if (vtIdx < m_tmpVertices.size())
						m_tmpVertices[vtIdx].Uv = Vectorf2(x, y);
					else
						m_tmpVertices.push_back({ {}, Vectorf2(x, y) });
					vtIdx++;
				}
				else if (type[1] == 'n') // Normal position
				{
					if (vnIdx < m_tmpVertices.size())
						m_tmpVertices[vnIdx].Normal = Vectorf3(x, y, z);
					else
						m_tmpVertices.push_back({ {},{}, Vectorf3(x, y, z) });
					vnIdx++;
				}
			}
			else if (type == "g"
				&& line.find("default") != -1
				&& !m_tmpVertices.empty()) // Group
			{
				m_meshMtx.lock();
				Mesh* next_mesh = new Mesh(m_tmpVertices, m_tmpIdxPositions, m_tmpIdxUvs, m_tmpIdxNormals);
				next_mesh->SetIndices(m_indices);
				meshes.push_back(next_mesh);
				m_meshMtx.unlock();
			}
			else if (type == "f") // Face indices (assumes that model is an assembly of triangles only)
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

						if (elementsToAdd == 1)
							m_tmpIdxNormals.push_back(i);

						if (elementsToAdd == 2)
							m_tmpIdxUvs.push_back(i);

						if (elementsToAdd == 3)
						{
							m_tmpIdxPositions.push_back(i);
							if (vertexIdx / 2) // New triangle
							{
								m_indices.push_back(faceIdx);
								m_indices.push_back(faceIdx + vertexIdx - 1);
								m_indices.push_back(faceIdx + vertexIdx);
							}
							vertexIdx++;
						}

						if (iss.peek() == '/')
						{
							iss.ignore();
							if (iss.peek() == '/')
							{
								iss.ignore();
								elementsToAdd--;
							}
						}

						if (iss.peek() == ' ')
							break;
					}
				} while (iss);
				faceIdx += vertexIdx; // Triangle count
			}
			//Success
		}
		file.close();
	}
	m_isRead = true;
}

void Model::ResourceLoadOpenGL(const std::string _name)
{
	if (meshes.empty())
	{
		Mesh* next_mesh = new Mesh(m_tmpVertices, m_tmpIdxPositions, m_tmpIdxUvs, m_tmpIdxNormals);
		next_mesh->SetIndices(m_indices);
		meshes.push_back(next_mesh);
	}

	for (Mesh* mesh : meshes)
		mesh->SetupMesh();

	m_tmpVertices.clear();
	m_tmpIdxPositions.clear();
	m_tmpIdxUvs.clear();
	m_tmpIdxNormals.clear();
	m_indices.clear();
	m_isLoaded = true;
}

void Model::Draw(Shader& _shader)
{
	for (Mesh* mesh : meshes)
		mesh->Draw();
}

void Model::Draw() {
	Draw(*shader);
}

void Model::ResourceUnload()
{
	for (Mesh* mesh : meshes)
	{
		mesh->Unload();
		delete mesh;
	}
}

void Model::ProcessNode(SceneNode* _node, const Scene* _scene) {
	ProcessNode(_node, _scene, _node->shader);
}

void Model::ProcessNode(SceneNode* _node, const Scene* _scene, Shader* _shader)
{
	Assert(_shader, std::string("No Shader for Model nb" + m_resourceId).c_str());
	_shader->Use();
	Matrix4x4 model = _node->GetTransform().ModelMatrix();
	Matrix4x4 MVP = _scene->camera.viewProjection * model;

	_shader->SetMat4("model", model);
	_shader->SetMat3("normalMatrix", _node->GetTransform().NormalMatrix());
	_shader->SetMat4("MVP", MVP);
}

void Model::ResetCount() {
	s_ModelNumber = 0;
}

void Model::AddMaterial() {
	materials.push_back(material::none);
}

void Model::AddMaterials(unsigned int _number)
{
	for (unsigned int i = 0; i < _number; i++)
		AddMaterial();
}

void Model::AddMaterial(Material _mat) {
	materials.push_back(_mat);
}

void Model::ChangeMaterial(Material _mat, uint32_t idx) {
	materials[idx] = _mat;
}