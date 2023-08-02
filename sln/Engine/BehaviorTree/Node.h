#pragma once
#include <functional>

// ノード結果
enum class NodeResult : uint8_t
{
	Failed,// 失敗
	Succeeded,// 成功
	Running,// 実行中
};

class Node
{
	std::function<NodeResult()> func;

public:
	Node(const std::function<NodeResult()>& func) : func(func) {}

	inline NodeResult Run() { return func(); }
};

