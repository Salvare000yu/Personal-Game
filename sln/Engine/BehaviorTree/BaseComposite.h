#pragma once
#include "Node.h"

class BaseComposite :
	public Node
{
protected:
	// 子ノード
	std::vector<Node> childNode;

	size_t currentNode = 0u;

	virtual NodeResult MainFunc() = 0;

public:
	BaseComposite() : Node(std::bind(&BaseComposite::MainFunc, this)) {}

	// 子ノードを後ろに追加
	inline void AddChild(const Node& func)
	{
		childNode.emplace_back(func);
	}

	inline const std::vector<Node>& GetChildNode()
	{
		return childNode;
	}
};
