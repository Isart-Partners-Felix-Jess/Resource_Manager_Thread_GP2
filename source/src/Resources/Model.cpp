#include <Model.hpp>
#include <Log.hpp>

Model::~Model()
{
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
		m_VertexBuffer.clear();
		m_IndexBuffer.clear();

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
				m_VertexBuffer.push_back(Vertex{ Vectorf3{ x, y, z }, Vectorf3(), Vectorf2() }); //Assumes that vertex position is the first of the 3 coordinates
			}
			else if (type == "vt") // Texture position
			{
				iss >> x >> y;
				m_VertexBuffer[vtIdx].Uv = Vectorf2(x, y);
				vtIdx++;
			}
			else if (type == "vn")// Normal position
			{
				iss >> x >> y >> z;
				m_VertexBuffer[vnIdx].Normal = Vectorf3(x, y, z);
				vnIdx++;
			}
			else if (type == "f")// Face indices (assumes that model is an assembly of triangles only)
			{
				char separator;  // To ignore the texture and normal indices
				for (uint32_t idx = 0; idx < 9; idx++) //3coord x3 Vertices = 9
				{
					iss >> idx >> separator;
					if (idx == '/')// ignore if no texture coord
						continue;
					m_IndexBuffer.push_back(idx - 1); // Vertex n°1 is at index 0
				}
				//Hard way:
				//uint32_t i1, i2, i3,i4,i5,i6,i7,i8,i9;
				//iss >> i1 >> separator >> i2 >> separator >> i3 >> separator >> i4 >> separator >> i5 >> separator >> i6 >> separator >> i7 >> separator >> i8 >> separator >> i9;
				//indices.push_back(i1 - 1);  // Subtract 1 to convert to 0-based indexing
				//indices.push_back(i2 - 1);
				//indices.push_back(i3 - 1);

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
	m_VertexBuffer.clear();
	m_IndexBuffer.clear();
}