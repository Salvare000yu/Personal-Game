#include "Selector.h"

NodeResult Selector::MainFunc()
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

		// セレクターなので成功で終了
		if (result == NodeResult::Succeeded)
		{
			return NodeResult::Succeeded;
		}
	}
	// すべて失敗なら失敗を返す
	return NodeResult::Failed;
}