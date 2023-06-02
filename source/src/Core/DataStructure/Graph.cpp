#include <Graph.hpp>
#include <Model.hpp>

SceneNode::SceneNode(SceneNode* _parent)
{
	parent = _parent;
}

void SceneNode::InitDefaultShader(Shader& _shader, Scene* _scene)
{
	for (Node* child : children)
	{
		SceneNode* sceneChild = dynamic_cast<SceneNode*>(child);
		for (Model* model : sceneChild->models)
		{
			model->ProcessNode(sceneChild, _scene);
			model->shader = &_shader;
		}
	}
}
bool SceneNode::UpdateChildren()
{
	if (changed)
	{
		transform.ComputeAll(dynamic_cast<SceneNode*>(parent)->transform.ModelMatrix());
	}
	return true;
}

void SceneNode::Draw()
{
	for (Model* model : models)
	{
		model->Draw();
	}
}

bool Node::UpdateChildren()
{
	for (Node* child : children)
	{
		if (child != nullptr)
		{
			if (changed)
				child->changed = true;
			child->UpdateChildren();
		}
	}
	changed = false;
	return true;
}

bool Graph::Update()
{
	bool success = true;
	for (Node* child : m_RootNode.children)
	{
		success &= child->UpdateChildren();
	}
	return success;
}
SceneGraph::SceneGraph(Scene* _scene)
{
	scene = _scene;
}
void SceneGraph::Draw()
{
	for (Node* child : m_RootNode.children)
	{
		SceneNode* sceneChild = dynamic_cast<SceneNode*>(child);
		sceneChild->Draw();
	}
	//Stop condition
	return;
}

void SceneGraph::InitDefaultShader(Shader& _shader)
{
	for (Node* child : m_RootNode.children)
	{
		SceneNode* sceneChild = dynamic_cast<SceneNode*>(child);
		sceneChild->InitDefaultShader(_shader, scene);
	}
	//Stop condition
	return;
}
