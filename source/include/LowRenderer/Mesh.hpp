
#pragma once

#ifndef MESH_H
#define MESH_H

#include <matrix.hpp>
#include <vector>
//#include <ResourcesManager.hpp>
#include <Material.hpp>

class Shader;


struct Vertex {
    Vectorf3 Position;
    Vectorf2 Uv;
    Vectorf3 Normal;
};

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    //public: ?
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    Matrix4x4 local = Matrix4x4(true);
public:
    ~Mesh();
    Material material = material::none;
    Mesh() {};

    //void Set_OpenGLOutput(unsigned int _VAO, unsigned int _VBO, unsigned int _EBO);
    void Set_Vertices(const std::vector<Vertex>& _Vertices);
    void Set_Indices(const std::vector<unsigned int>& _Indices);

    void SetupMesh();
    void Draw(Shader& _shader);

};
#endif // MESH_H