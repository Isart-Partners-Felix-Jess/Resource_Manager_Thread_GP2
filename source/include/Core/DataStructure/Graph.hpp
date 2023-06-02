#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include <matrix.hpp>
#include <Transform.hpp>
class Model;

class Scene;
class Shader;

//Generic one
struct Node
{
	Node* parent = nullptr;
	std::vector<Node*> children;
	//Dirty Flag
	bool changed = false;

	virtual bool UpdateChildren();
};


class Graph
{
protected:
const Node m_RootNode;
public:
	Graph() {};

	bool Update();
};

struct SceneNode : public Node
{
protected:
	SceneNode() {};
public:
	SceneNode(SceneNode* _parent);

	Transform transform;
	std::vector<Model*> models;

	void InitDefaultShader(Shader& _shader, Scene* _scene);
	bool UpdateChildren() override;
	void Draw();
};
//Like a gameobject
class SceneGraph : public Graph
{
public:
	std::vector<SceneNode*> entities;
	Scene* scene;
	SceneGraph(Scene* _scene);

	//Check if all shaders are initialized first
	void Draw();
	//Think to do this before drawing and first update
	void InitDefaultShader(Shader& _shader);
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
#endif//GRAPH_H