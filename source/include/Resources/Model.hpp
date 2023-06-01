#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <Graph.hpp>
#include <Scene.hpp>
#include <Mesh.hpp>
class Model : public IResource
{
public:
    void Draw(Shader& _shader);
    std::vector<Mesh*> meshes;
private:
    // model data
    std::string directory;

    //void ProcessNode(SceneNode* node, const Scene* scene);
    //Mesh ProcessMesh(Mesh* mesh, const Scene* scene);
    //std::vector<Texture> LoadMaterialTextures(Material* mat, TextureType type,
    //    std::string typeName);
    Mesh BuildMesh(const std::vector<Vertex>& _temp_Vertices, const std::vector<uint32_t>& _temp_idx_Positions, const std::vector<uint32_t>& _temp_idx_Uvs, const std::vector<uint32_t>& _temp_idx_Normals);

    // Inherited from IResource
    virtual void UnloadResource() override;
    void LoadResource(const char* path) override;
};
#endif // !MODEL_H
