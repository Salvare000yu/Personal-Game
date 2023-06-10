/// <<<<<<<<
/// @brief 汎用的機能使いやすくをまとめるためのクラス
/// >>>>>>>>

#pragma once
#include <vector>
#include <cmath>
#include <DirectXMath.h>

class GameUtility
{
public:

	/// @brief　std::lerp(線形補間)
	/// @param startPos　開始座標
	/// @param endPos　開始座標
	/// @param t　現在補間値
	inline static DirectX::XMFLOAT3 UtilLerp(const DirectX::XMFLOAT3& startPos,
		const DirectX::XMFLOAT3& endPos,
		float t)
	{
		DirectX::XMFLOAT3 result;
		result.x = std::lerp(startPos.x, endPos.x, t);
		result.y = std::lerp(startPos.y, endPos.y, t);
		result.z = std::lerp(startPos.z, endPos.z, t);
		return result;
	}
};
