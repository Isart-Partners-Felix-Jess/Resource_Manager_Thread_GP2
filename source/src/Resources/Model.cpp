#include<Model.hpp>
#include <Material.hpp>
#include <Shader.hpp>
#include <Graph.hpp>
#include <Scene.hpp>

static unsigned int s_ModelNumber = 0;

void Model::LoadResource(const std::string _name)
{
	FileRead(_name);

	Mesh* next_mesh = new Mesh;
	*next_mesh = BuildMesh(temp_Vertices, temp_idx_Positions, temp_idx_Uvs, temp_idx_Normals);
	next_mesh->Set_Indices(indices);
	next_mesh->SetupMesh();
	meshes.push_back(next_mesh);
}

void Model::FileRead(const std::string _name)
{
	m_ResourceId = s_ModelNumber++;
	std::ifstream file;
	std::filesystem::path path = "assets/meshes/";
	path += _name + std::string(".obj");
	file.open(path);
	//If we want to have the full path
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

		//clear in case of double load
		temp_Vertices.clear();
		//load .obj
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
					if (vIdx < temp_Vertices.size())
						temp_Vertices[vIdx].Position = Vectorf3{ x, y, z };
					else
						temp_Vertices.push_back({ Vectorf3(x, y,z) });
					vIdx++;
				}
				else if (type[1] == 't') // Texture position
				{
					if (vtIdx < temp_Vertices.size())
						temp_Vertices[vtIdx].Uv = Vectorf2(x, y);
					else
						temp_Vertices.push_back({ {}, Vectorf2(x, y) });
					vtIdx++;
				}
				else if (type[1] == 'n')// Normal position
				{
					if (vnIdx < temp_Vertices.size())
						temp_Vertices[vnIdx].Normal = Vectorf3(x, y, z);
					else
						temp_Vertices.push_back({ {},{}, Vectorf3(x, y, z) });
					vnIdx++;
				}
			}
			else if (type == "g"
				&& line.find("default") != -1
				&& !temp_Vertices.empty()) // group
			{
				Mesh* next_mesh = new Mesh;
				*next_mesh = BuildMesh(temp_Vertices, temp_idx_Positions, temp_idx_Uvs, temp_idx_Normals);
				next_mesh->Set_Indices(indices);
				next_mesh->SetupMesh();
				meshes.push_back(next_mesh);
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
							if (vertexIdx / 2) // NewTriangle
							{
								indices.push_back(faceIdx);
								indices.push_back(faceIdx + vertexIdx - 1);
								indices.push_back(faceIdx + vertexIdx);
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
}

std::thread Model::LoadResourceStartThread(const std::string _name)
{
	return std::thread([this, _name]() { FileRead(_name); });
}

void Model::LoadResourceThreadJoined(const std::string _name)
{
	Mesh* next_mesh = new Mesh;
	*next_mesh = BuildMesh(temp_Vertices, temp_idx_Positions, temp_idx_Uvs, temp_idx_Normals);
	next_mesh->Set_Indices(indices);
	next_mesh->SetupMesh();
	meshes.push_back(next_mesh);

	temp_Vertices.clear();
	temp_idx_Positions.clear();
	temp_idx_Uvs.clear();
	temp_idx_Normals.clear();
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

Mesh Model::BuildMesh(const std::vector<Vertex>& _temp_Vertices, const std::vector<uint32_t>& _temp_idx_Positions, const std::vector<uint32_t>& _temp_idx_Uvs, const std::vector<uint32_t>& _temp_idx_Normals)
{
	Mesh result;
	//Build final VAO (Mesh)
	size_t total_size = std::max({ _temp_idx_Positions.size(), _temp_idx_Uvs.size(), _temp_idx_Normals.size() });
	std::vector<Vertex> vertices;
	vertices.resize(total_size);
	for (size_t i = 0; i < total_size; i++)
	{
		vertices[i].Position = _temp_Vertices[_temp_idx_Positions[i] - 1].Position;
		vertices[i].Uv = _temp_Vertices[_temp_idx_Uvs[i] - 1].Uv;
		vertices[i].Normal = _temp_Vertices[_temp_idx_Normals[i] - 1].Normal;
	}
	result.Set_Vertices(vertices);
	return result;
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