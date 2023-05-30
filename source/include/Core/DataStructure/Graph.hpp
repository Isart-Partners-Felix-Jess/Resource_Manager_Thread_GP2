#pragma once
#include <matrix.hpp>
#include <Transform.hpp>
#include <Mesh.hpp>

//Generic one
struct Node
{
	Node* parent;
	Matrix4x4 local1;
	Matrix4x4 local2;
	Matrix4x4 global;
	std::vector<Node*> children;

	virtual bool UpdateChildren();
};

struct SceneNode : public Node
{
	Transform transform;
	std::vector<Mesh*> meshes;

	bool UpdateChildren() override;
};

class Graph
{
	Node m_RootNode;
public:
	Graph() {};
	
	bool UpdateChildren();
	//bool Draw();
};

//template <typename N>
//bool Graph<N>::Draw()
//{
//	if (shape != nullptr)
//		shape->draw();
//	for (Node* child : children)
//	{
//		if (child != nullptr)
//			child->draw();
//	}
//	return true;
//}