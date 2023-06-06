#pragma once
#ifndef MODEL_H
#define MODEL_H

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
    void ProcessNode(SceneNode* _node,const Scene* _scene);
    void ProcessNode(SceneNode* _node, const Scene* _scene, Shader* _shader);
private:

    // model data
    std::string directory;
    Mesh BuildMesh(const std::vector<Vertex>& _temp_Vertices, const std::vector<uint32_t>& _temp_idx_Positions, const std::vector<uint32_t>& _temp_idx_Uvs, const std::vector<uint32_t>& _temp_idx_Normals);
    // Inherited from IResource
    virtual void UnloadResource() override;
    void LoadResource(const char* path) override;
};
#endif // !MODEL_H
