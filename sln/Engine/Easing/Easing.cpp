#include "Easing.h"
#include <DirectXMath.h>

using namespace DirectX;

float Easing::EaseInSine(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納				1.5707963f
	const float result= std::cos(distTime*XM_PIDIV2);

	// 現在位置
	const float currentPos= -distPos * result+distPos+startPos;

	return currentPos;
}

float Easing::EaseOutSine(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納				1.5707963f
	const float result = std::sin(distTime * XM_PIDIV2);

	// 現在位置
	const float currentPos = distPos * result + startPos;

	return currentPos;
}

float Easing::EaseOutQuad(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	const float result = distTime*(distTime-2.f);

	// 現在位置
	const float currentPos =  - distPos * result+startPos;

	return currentPos;
}

float Easing::EaseInElastic(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	const float t2 = std::pow(distTime, 2.f);

	// 計算結果一時格納
	const float result = t2 * t2 * std::sin(distTime * XM_PI * 4.5f);

	// 場所の間隔
	const float distPos = endPos - startPos;
	// 現在位置
	const float currentPos = startPos + distPos * result;

	return currentPos;
}

float Easing::EaseOutElastic(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	const float t2 = std::pow(distTime - 1, 2.f);

	// 計算結果一時格納
	const float result = 1 - t2 * t2 * std::cos(distTime * XM_PI * 4.5f);

	// 場所の間隔
	const float distPos = endPos - startPos;
	// 現在位置 -result
	const float currentPos = startPos + distPos * result;

	return currentPos;
}

float Easing::EaseInOutElastic(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	float t2;

	float result;// 計算結果一時格納

	// 場所の間隔
	const float distPos = endPos - startPos;

	if (distTime < 0.45) {
		t2 = std::pow(distTime, 2.f);
		// 計算結果
		result = 8 * t2 * t2 * std::sin(distTime * XM_PI * 9.f);
	}
	else if (distTime < 0.55f) {
		// 計算結果
		result = 0.5f + 0.75f * std::sin(distTime * XM_PI * 4.f);
	}
	else {
		t2 = std::pow(distTime-1,2.f);
		// 計算結果
		result = 1 - 8 * t2 * t2 * std::sin(distTime * XM_PI * 9.f);
	}

	// 現在位置
	const float currentPos = startPos + distPos * result;

	return currentPos;
}
