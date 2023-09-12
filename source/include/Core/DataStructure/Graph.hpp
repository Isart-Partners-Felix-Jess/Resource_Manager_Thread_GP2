#pragma once

#include <Transform.hpp>
#include <assertion.hpp>

#include <Material.hpp>
#include <Camera.hpp>

#include <Model.hpp>
#include <Shader.hpp>
#include <Light.hpp>

// Precompile
class Scene;

// Generic one
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
	inline Graph() {
		Assert((std::is_base_of_v<Node, T>), "T must be a specialization of Node");
	}

	inline bool Update() {
		bool success = true;
		for (Node* child : m_RootNode->children)
			success &= child->UpdateChildren();
		return success;
	}
};

struct SceneNode : public Node
{
protected:
	SceneNode() : material(material::none) {}

	Transform transform;

public:
	SceneNode(SceneNode* _parent, const Scene* _scene);

	std::string name;
	// Here for component list/vector (WIP)
	Model* model = nullptr;
	// In model is best
	Material material;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;
	Camera* camera = nullptr;
	Shader* shader = nullptr;
	const Scene* scene{};

	void InitDefaultShader(Shader& _shader);
	bool UpdateChildren() override;
	void SetParent(SceneNode* _parent, bool _keepGlobalTransform = false);
	void AddChild(SceneNode* _child, bool _keepGlobalTransform = false);
	SceneNode* GetChild(size_t _index = 0);
	size_t GetChildNumber();
	SceneNode* GetParent();
	Transform& SetTransform();
	Transform GetTransform();
	void Draw();
};

// Like a gameobject
class SceneGraph : public Graph<SceneNode>
{
public:
	std::vector<SceneNode*> entities;
	const Scene* scene{};

	SceneGraph(Scene* _scene);
	~SceneGraph();

	void AddEntity(Model* _model, SceneNode* _parent = nullptr, Transform _transform = Transform());
	// Think to do this before drawing and first update
	void InitDefaultShader(Shader& _shader);
	// Check if all shaders are initialized first
	void Update(const float& _deltaTime);
	void Draw();
};