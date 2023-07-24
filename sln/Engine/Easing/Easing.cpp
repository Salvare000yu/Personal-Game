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

	// 計算結果一時格納				1.5707963f(π/2)
	const float result = std::cos(distTime * XM_PIDIV2);

	// 現在位置
	const float currentPos = -distPos * result + distPos + startPos;

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

	// 計算結果一時格納				1.5707963f(π/2)
	const float result = std::sin(distTime * XM_PIDIV2);

	// 現在位置
	const float currentPos = distPos * result + startPos;

	return currentPos;
}

float Easing::EaseInOutSine(
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
	const float result = std::cos(distTime * XM_PI) - 1;

	// 現在位置
	const float currentPos = -distPos / 2 * result + startPos;

	return currentPos;
}

float Easing::EaseInQuad(
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
	const float result = std::pow(distTime, 2.f);

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
	const float result = distTime * (distTime - 2.f);

	// 現在位置
	const float currentPos = -distPos * result + startPos;

	return currentPos;
}

float Easing::EaseInOutQuad(
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
	float result;

	if (distTime < 0.5f) {// EaseInQuad → EaseOutQuad
		result = 2 * std::pow(distTime, 2.f);
	}
	else {
		result = distTime * (4 - 2 * distTime) - 1;
	}

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

XMFLOAT3 Easing::EaseInCubic(
	const float t,
	const XMFLOAT3 startPos,
	const XMFLOAT3 endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	const float distPosX = endPos.x - startPos.x;
	const float distPosY = endPos.y - startPos.y;
	const float distPosZ = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = std::pow(distTime, 3.f);

	// 現在位置
	const float currentPosX = distPosX * result + startPos.x;
	const float currentPosY = distPosY * result + startPos.y;
	const float currentPosZ = distPosZ * result + startPos.z;
	const XMFLOAT3 currentPos{ currentPosX ,currentPosY ,currentPosZ };

	return currentPos;
}

float Easing::EaseOutCubic(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime - 1.f;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	const float result = 1 + std::pow(distTime, 3.f);

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseInOutCubic(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / (totalTime / 2.f);

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	float result;

	if (distTime < 1.f) {
		result = 0.5f * std::pow(distTime, 3.f);
	}
	else {
		distTime -= 2.f;
		result = 0.5f * (std::pow(distTime, 3.f) + 2.f);
	}

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseInQuart(
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
	const float result = std::pow(distTime, 4.f);

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseOutQuart(
	const float t, 
	const float startPos, 
	const float endPos, 
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime-1.f;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	const float result = std::pow(distTime, 4.f)-1.f;

	// 現在位置
	const float currentPos = -distPos * result + startPos;
	return currentPos;
}

float Easing::EaseInOutQuart(
	const float t,
	const float startPos,
	const float endPos, 
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	float result;

	if (distTime < 0.5f ) {
		distTime *= distTime;
		result = 8 * distTime * distTime;
	}
	else {
		distTime = (--distTime)*distTime;
		result= (1 - 8 * distTime * distTime);
	}

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseInBack(
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

	// 計算結果一時格納 2.70158f-1.70158f=1
	const float result = std::pow(distTime, 2.f) *(2.70158f*distTime-1.70158f);

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseOutBack(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;
	--distTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納 2.70158f-1.70158f=1
	const float result = 1+std::pow(distTime, 2.f) * (2.70158f * distTime + 1.70158f);

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseInOutBack(
	const float t,
	const float startPos,
	const float endPos, 
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;
	
	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	float result;

	if (distTime < 0.5f) {
		result = std::pow(distTime, 2.f) * (7.f * distTime - 2.5f) * 2.f;
	}
	else {
		--distTime;
		result = 1 +std::pow(distTime,2.f) * 2.f * (7.f * distTime + 2.5f);
	}

	// 現在位置
	const float currentPos = distPos * result + startPos;
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
		t2 = std::pow(distTime - 1, 2.f);
		// 計算結果
		result = 1 - 8 * t2 * t2 * std::sin(distTime * XM_PI * 9.f);
	}

	// 現在位置
	const float currentPos = startPos + distPos * result;

	return currentPos;
}

float Easing::EaseInBounce(
	const float t, 
	const float startPos, 
	const float endPos, 
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	const float result=
		std::pow(2.f,6*(distTime-1))*
		std::abs(std::sin(distTime*XM_PI*3.5f));// 複素数の絶対値

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseOutBounce(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	const float result =
		1-std::pow(2.f, -6.f * distTime) *
		std::abs(std::cos(distTime * XM_PI * 3.5f));// 複素数の絶対値

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseInOutBounce(
	const float t, 
	const float startPos,
	const float endPos, 
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	float result;

	if (distTime < 0.5f) {
		result= 
			8.f * std::pow(2.f, 8.f * (distTime - 1.f)) *
			std::abs(std::sin(distTime * XM_PI * 7.f));
	}
	else {
		result= 
			1.f - 8.f *std:: pow(2.f, -8.f * distTime) *
			std::abs(std::sin(distTime * XM_PI * 7.f));
	}

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}

float Easing::EaseCurrentBounce(
	const float t,
	const float startPos,
	const float endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	const float distPos = endPos - startPos;

	// 計算結果一時格納
	const float result =
		std::pow(2.f, -6.f * distTime) *
		std::abs(std::sin(distTime * XM_PI * 3.5f));// 複素数の絶対値

	// 現在位置
	const float currentPos = distPos * result + startPos;
	return currentPos;
}
