#include "Easing.h"
#include <DirectXMath.h>

using namespace DirectX;

float Easing::EaseIn(const float t, const float startPos, const float endPos, const float totalTime)
{
	// 間隔
	const float dist = t / totalTime;
	// 加速度の変化
	const float acceleration = std::pow(dist, 2.f);
	//現在位置
	const float currentPos = startPos + (endPos - startPos) * acceleration;

	return currentPos;
}

float Easing::EaseInElastic(const float t, const float startPos, const float endPos, const float totalTime)
{
	// 間隔
	const float distTime = t / totalTime;

	const float t2 = std::pow(distTime, 2.f);

	// 計算結果一時格納
	const float result = t2 * t2 * std::sin(distTime * XM_PI * 4.5f);

	// 場所の間隔
	const float distPos = endPos - startPos;
	// 現在位置
	const float currentPos = startPos+distPos * result;

	return currentPos;
}

float Easing::EaseOutElastic(const float t, const float startPos, const float endPos, const float totalTime)
{
	// 間隔
	const float distTime = t / totalTime;

	const float t2 = std::pow(distTime-1, 2.f);

	// 計算結果一時格納
	const float result = 1 - t2 * t2 * std::cos(distTime * XM_PI * 4.5f);

	// 場所の間隔
	const float distPos = endPos - startPos;
	// 現在位置 -result
	const float currentPos= startPos+distPos + distPos * -result;

	return currentPos;
}
