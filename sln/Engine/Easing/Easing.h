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
	/// @param totalTime 移動にかける時間
	/// @param maxPos 最大位置
	/// @param nowPos 現在位置
	/// @return 現在位置から時間内で進んだ位置
	static inline float EaseIn(float t, float startPos,float endPos, float totalTime) {
		float acceleration =std::pow(t/totalTime,2.f);
		float currentPos = startPos+(endPos-startPos)*acceleration;

		return currentPos;
	}
};

