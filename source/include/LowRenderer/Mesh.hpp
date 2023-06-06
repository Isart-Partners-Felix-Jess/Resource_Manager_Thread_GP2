
#pragma once

#ifndef MESH_H
#define MESH_H

#include <matrix.hpp>
#include <vector>
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
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    Matrix4x4 local = Matrix4x4(true);
public:
    ~Mesh();
    Mesh() {};

    void Set_Vertices(const std::vector<Vertex>& _Vertices);
    void Set_Indices(const std::vector<unsigned int>& _Indices);

    void SetupMesh();
    void Draw();

};
#endif // MESH_H