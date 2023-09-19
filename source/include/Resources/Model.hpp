#pragma once

#include <Mesh.hpp>

class Scene;
struct SceneNode;

class Model : public IResource
{
public:
	void Draw(Shader& _shader);
	void Draw();

	std::vector<Mesh*> meshes;
	Shader* shader = nullptr;
	std::vector<Material> materials = { material::none };

	void AddMaterial();
	void AddMaterials(unsigned int _number);
	void AddMaterial(Material _mat);
	void ChangeMaterial(Material _mat, uint32_t idx);
	void ProcessNode(SceneNode* _node, const Scene* _scene);
	void ProcessNode(SceneNode* _node, const Scene* _scene, Shader* _shader);

	virtual void LoadResourceThreadJoined(const std::string _name) override;

private:
	// Model data
	std::string directory;
	std::vector<uint32_t> indices;

	// Tmp index
	std::vector<Vertex> tmpVertices;
	std::vector<uint32_t> tmpIdxPositions;
	std::vector<uint32_t> tmpIdxUvs;
	std::vector<uint32_t> tmpIdxNormals;

	// Inherited from IResource
	virtual void UnloadResource() override;
	void LoadResource(const std::string path, bool isMultiThread = false) override;
	void FileRead(const std::string path);
};