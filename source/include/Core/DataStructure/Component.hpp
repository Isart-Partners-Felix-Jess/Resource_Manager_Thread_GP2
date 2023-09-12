#pragma once

struct Node;
class Component
{
protected:
	Node* node;
	Component(Node* _node) { node = _node; };
	virtual void Update(Node* _node) = 0;
	void Draw() {}; //Useless ?
};