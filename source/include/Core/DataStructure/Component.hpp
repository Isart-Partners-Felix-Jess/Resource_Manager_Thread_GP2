#pragma once

struct Node;

class Component
{
protected:
	Node* m_node;
	Component(Node* _node) { m_node = _node; };
	virtual void Update(Node* _node) = 0;
	void Draw() {}; //Useless ?
};