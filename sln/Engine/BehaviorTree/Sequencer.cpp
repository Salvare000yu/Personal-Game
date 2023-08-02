#include "Sequencer.h"

NodeResult Sequencer::MainFunc()
{
	for (size_t i = currentNode;
		 i < childNode.size();
		 ++i)
	{
		// ノードの実行結果
		const NodeResult result = childNode[i].Run();

		// 実行中なら現在ノードを記録して終了
		if (result == NodeResult::Running)
		{
			currentNode = i;
			return NodeResult::Running;
		}

		// 実行終了で記録していた現在ノードをリセット
		currentNode = 0u;

		// シーケンサーなので失敗で終了
		if (result == NodeResult::Failed)
		{
			return NodeResult::Failed;
		}
	}
	// すべて成功なら成功を返す
	return NodeResult::Succeeded;
}