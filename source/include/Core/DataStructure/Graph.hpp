#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include <Transform.hpp>
#include <assertion.hpp>

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
	Assert((std::is_base_of_v<Node, T>),"T must be a specialization of Node");
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
	Transform transform;
public:
	SceneNode(SceneNode* _parent,const Scene* _scene);

	//Here for component list/vector (WIP)
	Model* model;
	const Scene* scene;

	void InitDefaultShader(Shader& _shader);
	bool UpdateChildren();
	void SetParent(SceneNode* _parent, bool _keepGlobalTransform = false); 
	void AddChild(SceneNode* _child, bool _keepGlobalTransform = false); 
	SceneNode* GetChild(size_t _index = 0);
	size_t GetChildNumber();
	SceneNode* GetParent();
	Transform& SetTransform();
	Transform GetTransform();
	void Draw();
};
//Like a gameobject
class SceneGraph : public Graph<SceneNode>
{
public:
	std::vector<SceneNode*> entities;
	const Scene* scene;
	SceneGraph(Scene* _scene);
	~SceneGraph();

	void AddEntity(Model* _model, SceneNode* _parent = nullptr, Transform _transform = Transform());
	//Think to do this before drawing and first update
	void InitDefaultShader(Shader& _shader);
	//Check if all shaders are initialized first
	void Update();
	void Draw();
};
#endif //GRAPH_H