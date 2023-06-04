#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include <matrix.hpp>
#include <Transform.hpp>
#include <assertion.hpp>
#include <type_traits>

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

template<typename T>
class Graph
{
protected:
T* m_RootNode;
public:
	Graph();

	bool Update();
};

template<typename T>
Graph<T>::Graph()
{
	static_assert( std::is_base_of_v<Node, T>, "T must be a specialization of Node");
	//Assert(std::is_base_of_v<Node, T>,"T must be a specialization of Node");

	//m_RootNode = nullptr;
}

template<typename T>
bool Graph<T>::Update()
{
	bool success = true;
	for (Node* child : m_RootNode->children)
	{
		success &= child->UpdateChildren();
	}
	return success;
}

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
class SceneGraph : public Graph<SceneNode>
{
	static SceneNode m_RootSceneNode;
public:
	std::vector<SceneNode> entities;
	Scene* scene;
	SceneGraph(Scene* _scene);

	//Check if all shaders are initialized first
	void Draw();
	//Think to do this before drawing and first update
	void InitDefaultShader(Shader& _shader);
	void AddEntity(Model& _model, SceneNode& _parent = m_RootSceneNode, Transform _transform = Transform());
	//void AddEntity(std::vector<Model*> _models, SceneNode& _parent = m_RootSceneNode, Transform _transform = Transform());
};
#endif //GRAPH_H