#pragma once

#include <memory>

struct Node {
	enum Type {
		VALUE,
		PAIR,
		NONE
	};
	typedef Node* NodePointer;

	Node(NodePointer parent, Type t = NONE);
	NodePointer base();
	NodePointer previousValue(NodePointer x) const;
	NodePointer nextValue(NodePointer x) const;

	bool isPair() const;
	void clear();
	void explode();
	void split();

	size_t calcMagnitude() const;

	Type type;
	int v;
	NodePointer parent;
	NodePointer left;
	NodePointer right;

	void print() const;
};

typedef Node* NodePointer;

NodePointer makeNode(NodePointer parent, Node::Type type = Node::NONE);
NodePointer addPair(NodePointer a, NodePointer b, NodePointer parent = nullptr);
