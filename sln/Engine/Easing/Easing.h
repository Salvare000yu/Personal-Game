#pragma once
/// <<<<<<<<
/// @brief イージングをするクラス
/// >>>>>>>>
#include <cmath>

class Easing final//継承禁止
{
private:
	Easing() = default;
	~Easing() = default;

public:

	/// @brief イーズイン(Sine)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInSine(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Sine)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseOutSine(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Sine)の複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInOutSine(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズイン(Quad)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInQuad(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Quad)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseOutQuad(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Quad)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInOutQuad(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズイン(Cubic)3次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInCubic(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Cubic)3次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseOutCubic(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Cubic)3次 複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInOutCubic(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズイン(Quart)4次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInQuart(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Quart)4次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseOutQuart(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Quart)4次 複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInOutQuart(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズインバック(下がってから速くなる感じ、チョロQ)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInBack(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズアウトバック(だんだん遅くなる時に少し前に出てから戻って止まる)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseOutBack(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief イーズインアウトバック(チョロQしてからはみ出して戻る)複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static float EaseInOutBack(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief 揺れてから速くなる感じ
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

	/// @brief 最終地点で揺れる感じ
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static float EaseOutElastic(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

	/// @brief 揺れてから速くなる最終地点で揺れる複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static float EaseInOutElastic(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
	);

	/// @brief イーズインバウンス（だんだん跳ねていく）
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static float EaseInBounce(
		const float t,
		const float startPos,
		const float endPos,
		const float totalTime
	);

};

