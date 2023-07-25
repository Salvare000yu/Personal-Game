#pragma once
/// <<<<<<<<
/// @brief イージングをするクラス
/// >>>>>>>>
#include <cmath>
#include <DirectXMath.h>

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
	static  DirectX::XMFLOAT3 EaseInSine(
		const float t,
		const  DirectX::XMFLOAT3 startPos,
		const  DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Sine)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseOutSine(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Sine)の複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInOutSine(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズイン(Quad)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static  DirectX::XMFLOAT3 EaseInQuad(
		const float t,
		const  DirectX::XMFLOAT3 startPos,
		const  DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Quad)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static  DirectX::XMFLOAT3 EaseOutQuad(
		const float t,
		const  DirectX::XMFLOAT3 startPos,
		const  DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Quad)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static  DirectX::XMFLOAT3 EaseInOutQuad(
		const float t,
		const  DirectX::XMFLOAT3 startPos,
		const  DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズイン(Cubic)3次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInCubic(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Cubic)3次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static  DirectX::XMFLOAT3 EaseOutCubic(
		const float t,
		const  DirectX::XMFLOAT3 startPos,
		const  DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Cubic)3次 複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInOutCubic(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズイン(Quart)4次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInQuart(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズアウト(Quart)4次
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseOutQuart(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインアウト(Quart)4次 複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInOutQuart(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインバック(下がってから速くなる感じ、チョロQ)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInBack(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズアウトバック(だんだん遅くなる時に少し前に出てから戻って止まる)
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseOutBack(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインアウトバック(チョロQしてからはみ出して戻る)複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在の位置
	static DirectX::XMFLOAT3 EaseInOutBack(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief 揺れてから速くなる感じ
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseInElastic(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief 最終地点で揺れる感じ
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseOutElastic(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief 揺れてから速くなる最終地点で揺れる複合
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseInOutElastic(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインバウンス（だんだん跳ねていく）
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseInBounce(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズアウトバウンス（最後にはねる）
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseOutBounce(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズインアウトバウンス（最初と最後にはねる）
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseInOutBounce(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);

	/// @brief イーズカレントバウンス（その場でバウンドし戻る、ずれる）
	/// @param t 時間
	/// @param startPos 開始位置
	/// @param endPos 最大位置
	/// @param totalTime 移動にかける時間
	/// @return 現在位置
	static DirectX::XMFLOAT3 EaseCurrentBounce(
		const float t,
		const DirectX::XMFLOAT3 startPos,
		const DirectX::XMFLOAT3 endPos,
		const float totalTime
	);
};

