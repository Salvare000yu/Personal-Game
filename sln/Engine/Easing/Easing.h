#pragma once
/// <<<<<<<<
/// @briefイージングをするクラス
/// >>>>>>>>
#include <cmath>

class Easing final//継承禁止
{
private:
	Easing() = default;
	~Easing() = default;

public:

	/// @brief イーズイン
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseIn(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief 揺れてから速くなっていく
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static float EaseInElastic(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	static float EaseOutElastic(
const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

};

