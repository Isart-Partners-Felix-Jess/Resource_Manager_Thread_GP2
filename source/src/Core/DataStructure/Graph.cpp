#include <Graph.hpp>


bool SceneNode::UpdateChildren()
{
	for (Model* model : models)
	{
		// Update modeles specific to SceneNode
		// model->update();
	}
	for (Node* child : children)
	{
		SceneNode* sceneChild = dynamic_cast<SceneNode*>(child);
		if (sceneChild != nullptr)
		{
			// Perform scene-specific update for SceneNode child
			sceneChild->UpdateChildren();
		}
	}
	return true;
}

bool Node::UpdateChildren()
{
	for (Node* child : children)
	{
		if (child != nullptr)
		{
			child->global = global * child->local1;
			child->UpdateChildren();
		}
	}
	return true;
}

bool Graph::UpdateChildren()
{
	return m_RootNode.UpdateChildren();
}
