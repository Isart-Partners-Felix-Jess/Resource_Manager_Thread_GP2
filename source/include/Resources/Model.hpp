#pragma once

#include <Mesh.hpp>
#include <IResource.hpp>

#include <Material.hpp>

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

	static void ResetCount();

	// Inherited from IResource
	virtual void ResourceFileRead(const std::string _path) override;
	virtual void ResourceLoadOpenGL(const std::string _name) override;
	virtual void ResourceUnload() override;

private:
	std::mutex m_meshMtx;
	// Model data
	std::string m_directory;
	std::vector<uint32_t> m_indices;

	// Tmp index
	std::vector<Vertex> m_tmpVertices;
	std::vector<uint32_t> m_tmpIdxPositions;
	std::vector<uint32_t> m_tmpIdxUvs;
	std::vector<uint32_t> m_tmpIdxNormals;
};