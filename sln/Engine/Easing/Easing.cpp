#include "Easing.h"
#include <DirectXMath.h>

using namespace DirectX;

XMFLOAT3 Easing::EaseInSine(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納				1.5707963f(π/2)
	const float result = std::cos(distTime * XM_PIDIV2);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = -distPos.x * result + distPos.x + startPos.x;
	currentPos.y = -distPos.y * result + distPos.y + startPos.y;
	currentPos.z = -distPos.z * result + distPos.z + startPos.z;

	return currentPos;
}

XMFLOAT3 Easing::EaseOutSine(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納				1.5707963f(π/2)
	const float result = std::sin(distTime * XM_PIDIV2);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;

	return currentPos;
}

XMFLOAT3 Easing::EaseInOutSine(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = std::cos(distTime * XM_PI) - 1.f;

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = -distPos.x / 2.f * result + startPos.x;
	currentPos.y = -distPos.y / 2.f * result + startPos.y;
	currentPos.z = -distPos.z / 2.f * result + startPos.z;

	return currentPos;
}

XMFLOAT3 Easing::EaseInQuad(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = std::pow(distTime, 2.f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;

	return currentPos;
}

XMFLOAT3 Easing::EaseOutQuad(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = distTime * (distTime - 2.f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = -distPos.x * result + startPos.x;
	currentPos.y = -distPos.y * result + startPos.y;
	currentPos.z = -distPos.z * result + startPos.z;

	return currentPos;
}

XMFLOAT3 Easing::EaseInOutQuad(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	float result = 0;

	if (distTime < 0.5f) {// EaseInQuad → EaseOutQuad
		result = 2.f * std::pow(distTime, 2.f);
	}
	else {
		result = distTime * (4.f - 2.f * distTime) - 1.f;
	}

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInCubic(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = std::pow(distTime, 3.f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;

	return currentPos;
}

XMFLOAT3 Easing::EaseOutCubic(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime - 1.f;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = 1.f + std::pow(distTime, 3.f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInOutCubic(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / (totalTime / 2.f);

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	float result = 0;

	if (distTime < 1.f) {
		result = 0.5f * std::pow(distTime, 3.f);
	}
	else {
		distTime -= 2.f;
		result = 0.5f * (std::pow(distTime, 3.f) + 2.f);
	}

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInQuart(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = std::pow(distTime, 4.f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseOutQuart(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime - 1.f;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result = std::pow(distTime, 4.f) - 1.f;

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = -distPos.x * result + startPos.x;
	currentPos.y = -distPos.y * result + startPos.y;
	currentPos.z = -distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInOutQuart(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	float result = 0;

	if (distTime < 0.5f) {
		distTime *= distTime;
		result = 8.f * distTime * distTime;
	}
	else {
		distTime = (distTime-1.f) * distTime;
		result = (1.f - 8.f * distTime * distTime);
	}

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInBack(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納 2.70158f-1.70158f=1
	const float result = std::pow(distTime, 2.f) * (2.70158f * distTime - 1.70158f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseOutBack(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime-1.f;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納 2.70158f-1.70158f=1
	const float result = 1.f + std::pow(distTime, 2.f) * (2.70158f * distTime + 1.70158f);

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInOutBack(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	float result = 0;

	if (distTime < 0.5f) {
		result = std::pow(distTime, 2.f) * (7.f * distTime - 2.5f) * 2.f;
	}
	else {
		distTime-=1.f;
		result = 1.f + std::pow(distTime, 2.f) * 2.f * (7.f * distTime + 2.5f);
	}

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = distPos.x * result + startPos.x;
	currentPos.y = distPos.y * result + startPos.y;
	currentPos.z = distPos.z * result + startPos.z;
	return currentPos;
}

XMFLOAT3 Easing::EaseInElastic(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	const float t2 = std::pow(distTime, 2.f);

	// 計算結果一時格納
	const float result = t2 * t2 * std::sin(distTime * XM_PI * 4.5f);

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = startPos.x + distPos.x * result;
	currentPos.y = startPos.y + distPos.y * result;
	currentPos.z = startPos.z + distPos.z * result;

	return currentPos;
}

XMFLOAT3 Easing::EaseOutElastic(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	const float t2 = std::pow(distTime - 1.f, 2.f);

	// 計算結果一時格納
	const float result = 1.f - t2 * t2 * std::cos(distTime * XM_PI * 4.5f);

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = startPos.x + distPos.x * result;
	currentPos.y = startPos.y + distPos.y * result;
	currentPos.z = startPos.z + distPos.z * result;

	return currentPos;
}

XMFLOAT3 Easing::EaseInOutElastic(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	float t2;

	float result = 0;// 計算結果一時格納

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	if (distTime < 0.45) {
		t2 = std::pow(distTime, 2.f);
		// 計算結果
		result = 8.f * t2 * t2 * std::sin(distTime * XM_PI * 9.f);
	}
	else if (distTime < 0.55f) {
		// 計算結果
		result = 0.5f + 0.75f * std::sin(distTime * XM_PI * 4.f);
	}
	else {
		t2 = std::pow(distTime - 1, 2.f);
		// 計算結果
		result = 1.f - 8.f * t2 * t2 * std::sin(distTime * XM_PI * 9.f);
	}

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = startPos.x + distPos.x * result;
	currentPos.y = startPos.y + distPos.y * result;
	currentPos.z = startPos.z + distPos.z * result;

	return currentPos;
}

XMFLOAT3 Easing::EaseInBounce(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result =
		std::pow(2.f, 6.f * (distTime - 1.f)) *
		std::abs(std::sin(distTime * XM_PI * 3.5f));// 複素数の絶対値

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = startPos.x + distPos.x * result;
	currentPos.y = startPos.y + distPos.y * result;
	currentPos.z = startPos.z + distPos.z * result;
	return currentPos;
}

XMFLOAT3 Easing::EaseOutBounce(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	const float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	const float result =
		1.f - std::pow(2.f, -6.f * distTime) *
		std::abs(std::cos(distTime * XM_PI * 3.5f));// 複素数の絶対値

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = startPos.x + distPos.x * result;
	currentPos.y = startPos.y + distPos.y * result;
	currentPos.z = startPos.z + distPos.z * result;
	return currentPos;
}

XMFLOAT3 Easing::EaseInOutBounce(
	const float t,
	const XMFLOAT3& startPos,
	const XMFLOAT3& endPos,
	const float totalTime
)
{
	// 間隔
	float distTime = t / totalTime;

	// 場所の間隔
	XMFLOAT3 distPos{};
	distPos.x = endPos.x - startPos.x;
	distPos.y = endPos.y - startPos.y;
	distPos.z = endPos.z - startPos.z;

	// 計算結果一時格納
	float result = 0;

	if (distTime < 0.5f) {
		result =
			8.f * std::pow(2.f, 8.f * (distTime - 1.f)) *
			std::abs(std::sin(distTime * XM_PI * 7.f));
	}
	else {
		result =
			1.f - 8.f * std::pow(2.f, -8.f * distTime) *
			std::abs(std::sin(distTime * XM_PI * 7.f));
	}

	// 現在位置
	XMFLOAT3 currentPos{};
	currentPos.x = startPos.x + distPos.x * result;
	currentPos.y = startPos.y + distPos.y * result;
	currentPos.z = startPos.z + distPos.z * result;
	return currentPos;
}

//XMFLOAT3 Easing::EaseCurrentBounce(
//	const float t,
//	const XMFLOAT3& startPos,
//	const XMFLOAT3& endPos,
//	const float totalTime
//)
//{
//	// 間隔
//	float distTime = t / totalTime;
//
//	// 場所の間隔
//	XMFLOAT3 distPos;
//	distPos.x = endPos.x - startPos.x;
//	distPos.y = endPos.y - startPos.y;
//	distPos.z = endPos.z - startPos.z;
//
//	// 計算結果一時格納
//	const float result =
//		std::pow(2.f, -6.f * distTime) *
//		std::abs(std::sin(distTime * XM_PI * 3.5f));// 複素数の絶対値
//
//	// 現在位置
//	XMFLOAT3 currentPos;
//	currentPos.x = startPos.x + distPos.x * result;
//	currentPos.y = startPos.y + distPos.y * result;
//	currentPos.z = startPos.z + distPos.z * result;
//	return currentPos;
//}
