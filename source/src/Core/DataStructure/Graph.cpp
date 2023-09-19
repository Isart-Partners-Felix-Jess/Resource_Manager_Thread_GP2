#include <Graph.hpp>

#include <Model.hpp>
#include <Scene.hpp>

SceneNode::SceneNode(SceneNode* _parent, const Scene* _scene) : material(material::none)
{
	if (!_parent)
		parent = _parent;   // nullptr
	else
		SetParent(_parent); // Set parent child as well
	scene = _scene;
	changed = true;
	shader = scene->shadlight;
}

// Set parent child as well
void SceneNode::SetParent(SceneNode* _parent, bool _keepGlobalPosition)
{
	if (parent)
		std::erase(parent->children, this);

	parent = _parent;
	_parent->children.push_back(this);
	if (_keepGlobalPosition)
		transform.SetNewLocalFrom(_parent->transform.ModelMatrix());
	else
		changed = true;
}

//Set child's parent as well
void SceneNode::AddChild(SceneNode* _child, bool _keepGlobalPosition)
{
	_child->SetParent(this, _keepGlobalPosition);
	if (_keepGlobalPosition)
		_child->transform.SetNewLocalFrom(this->transform.ModelMatrix());
	else
		changed = true;
	children.push_back(_child);
}

// 0 by default
SceneNode* SceneNode::GetChild(size_t _index)
{
	// "Safe" out of array
	if (_index > children.size())
		return nullptr;
	return dynamic_cast<SceneNode*>(children[_index]);
}

size_t SceneNode::GetChildNumber() {
	return children.size();
}

// Setter, so each time transform is touched, we update the dirty flag
Transform& SceneNode::SetTransform()
{
	changed = true;
	return transform;
}

Transform SceneNode::GetTransform() {
	return transform;
}

SceneNode* SceneNode::GetParent() {
	return dynamic_cast<SceneNode*>(parent);
}

void SceneNode::InitDefaultShader(Shader& _shader)
{
	// Here for all components
	if (model)
		model->shader = &_shader;
	shader = &_shader;
	for (Node* child : children)
	{
		SceneNode* sceneChild = dynamic_cast<SceneNode*>(child);
		sceneChild->InitDefaultShader(_shader);
	}
}

bool SceneNode::UpdateChildren()
{
	if (changed)
	{
		transform.ComputeAll(dynamic_cast<SceneNode*>(parent)->transform.ModelMatrix());
		// Update Component
		if (pointLight)
			pointLight->position = transform.ModelMatrix() * pointLight->position;
		if (spotLight)
			spotLight->point.position = transform.ModelMatrix() * spotLight->point.position;
		if (camera)
		{
			camera->eye *= transform.ModelMatrix();
			camera->zCamera *= transform.ModelMatrix().Inversion().Transposed(); // NormalMatrix
			camera->ComputeViewProjection();
		}
	}
	bool success = Node::UpdateChildren();
	return success;
}

void SceneNode::Draw()
{
	// Here for all components
	if (model)
	{
		Assert(shader, std::string("No Shader for object " + name).c_str());
		shader->Use();
		material.InitShader(*shader);
		model->ProcessNode(this, scene);

		model->Draw();
	}
	for (Node* child : children)
	{
		SceneNode* sceneChild = dynamic_cast<SceneNode*>(child);
		sceneChild->Draw();
	}
}

bool Node::UpdateChildren()
{
	for (Node* child : children)
		if (child != nullptr)
		{
			if (changed)
				child->changed = true;
			child->UpdateChildren();
		}

	changed = false;
	return true;
}

SceneGraph::SceneGraph(Scene* _scene)
{
	m_RootNode = new SceneNode(nullptr, _scene);
	scene = _scene;
}

SceneGraph::~SceneGraph()
{
	Destroy();
	delete m_RootNode;
}

void SceneGraph::Update(const float& _deltaTime)
{
	for (size_t i = 0; i < m_RootNode->GetChildNumber(); i++)
		m_RootNode->GetChild(i)->UpdateChildren();
}

void SceneGraph::Draw()
{
	for (size_t i = 0; i < m_RootNode->GetChildNumber(); i++)
		m_RootNode->GetChild(i)->Draw();
}

void SceneGraph::Destroy()
{
	for (SceneNode* entity : entities)
		delete entity;
	m_RootNode->children.clear();
	entities.clear();
}

void SceneGraph::InitDefaultShader(Shader& _shader)
{
	for (size_t i = 0; i < m_RootNode->GetChildNumber(); i++)
		m_RootNode->GetChild(i)->InitDefaultShader(_shader);
}

void  SceneGraph::AddEntity(Model* _model, SceneNode* _parent, Transform _transform)
{
	SceneNode* entity = new SceneNode(m_RootNode, scene);
	entity->SetTransform() = _transform;
	if (_model != nullptr)
		entity->model = _model;
	if (_parent != nullptr)
		entity->SetParent(_parent);
	entities.push_back(entity);
}