#include <Model.hpp>
#include <Material.hpp>
#include <Shader.hpp>
#include <Graph.hpp>
#include <Scene.hpp>

static unsigned int s_ModelNumber = 0;

void Model::LoadResource(const std::string _name, bool isMultiThread)
{
	FileRead(_name);

	if (!isMultiThread)
		LoadResourceThreaded(_name);

	isLoaded = true;
}

void Model::FileRead(const std::string _name)
{
	m_ResourceId = s_ModelNumber++;
	std::ifstream file;
	std::filesystem::path path = "assets/meshes/";
	path += _name + std::string(".obj");
	file.open(path);
	// If we want to have the full path
	//m_ResourcePath = path.generic_string();
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
		tmpVertices.clear();
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
					if (vIdx < tmpVertices.size())
						tmpVertices[vIdx].Position = Vectorf3{ x, y, z };
					else
						tmpVertices.push_back({ Vectorf3(x, y,z) });
					vIdx++;
				}
				else if (type[1] == 't') // Texture position
				{
					if (vtIdx < tmpVertices.size())
						tmpVertices[vtIdx].Uv = Vectorf2(x, y);
					else
						tmpVertices.push_back({ {}, Vectorf2(x, y) });
					vtIdx++;
				}
				else if (type[1] == 'n') // Normal position
				{
					if (vnIdx < tmpVertices.size())
						tmpVertices[vnIdx].Normal = Vectorf3(x, y, z);
					else
						tmpVertices.push_back({ {},{}, Vectorf3(x, y, z) });
					vnIdx++;
				}
			}
			else if (type == "g"
				&& line.find("default") != -1
				&& !tmpVertices.empty()) // Group
			{
				Mesh* next_mesh = new Mesh(tmpVertices, tmpIdxPositions, tmpIdxUvs, tmpIdxNormals);
				next_mesh->SetIndices(indices);
				meshes.push_back(next_mesh);
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
							tmpIdxNormals.push_back(i);

						if (elementsToAdd == 2)
							tmpIdxUvs.push_back(i);

						if (elementsToAdd == 3)
						{
							tmpIdxPositions.push_back(i);
							if (vertexIdx / 2) // New triangle
							{
								indices.push_back(faceIdx);
								indices.push_back(faceIdx + vertexIdx - 1);
								indices.push_back(faceIdx + vertexIdx);
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
		}
		file.close();
	}
}

void Model::LoadResourceThreaded(const std::string _name)
{
	if (meshes.empty())
	{
		Mesh* next_mesh = new Mesh(tmpVertices, tmpIdxPositions, tmpIdxUvs, tmpIdxNormals);
		next_mesh->SetIndices(indices);
		meshes.push_back(next_mesh);
	}

	for (Mesh* mesh : meshes)
		mesh->SetupMesh();

	tmpVertices.clear();
	tmpIdxPositions.clear();
	tmpIdxUvs.clear();
	tmpIdxNormals.clear();
	indices.clear();
}

void Model::Draw(Shader& _shader)
{
	uint32_t i = 0;
	for (Mesh* mesh : meshes)
	{
		//if (i < materials.size())
			//materials[i++].InitShader(_shader);
		mesh->Draw();
	}
}

void Model::Draw() {
	Draw(*shader);
}

void Model::UnloadResource()
{
	for (Mesh* mesh : meshes)
	{
		mesh->Unload();
		delete mesh;
	}
	//meshes.clear();
}

void Model::ProcessNode(SceneNode* _node, const Scene* _scene) {
	ProcessNode(_node, _scene, _node->shader);
}

void Model::ProcessNode(SceneNode* _node, const Scene* _scene, Shader* _shader)
{
	Assert(_shader, "No Shader for Model nb%i.", m_ResourceId);
	_shader->Use();
	Matrix4x4 model = _node->GetTransform().ModelMatrix();
	Matrix4x4 MVP = _scene->camera.viewProjection * model;

	_shader->SetMat4("model", model);
	_shader->SetMat3("normalMatrix", _node->GetTransform().NormalMatrix());
	_shader->SetMat4("MVP", MVP);
}

void Model::ResetCount()
{
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