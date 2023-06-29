/// <<<<<<<<
/// @brief 汎用的機能使いやすくをまとめるためのクラス
/// >>>>>>>>

#pragma once
#include <vector>
#include <string>
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

	/// @brief 正規化
	/// @param vec 対象のベクトル
	inline static void UtilNormalize(DirectX::XMFLOAT3& vec)
	{
		const float maxNum = std::max(std::max(vec.x, vec.y), vec.z);

		vec.x /= maxNum;
		vec.y /= maxNum;
		vec.z /= maxNum;
	}

	/// @brief　球面線形補間
	/// @param startPos　開始座標
	/// @param endPos　開始座標
	/// @param t　現在補間値
	static DirectX::XMFLOAT3 UtilSLerp(const DirectX::XMFLOAT3& startPos,
		const DirectX::XMFLOAT3& endPos,
		float t);

	/// @brief　CSVの読み込み
	/// @param csvFilePath　ファイルパス
	/// @param divChar　行の区切り文字
	/// @param commentFlag　ファイルコメント有効かどうか
	/// @param commentStartStr　コメントの先頭文字
	static std::vector<std::vector<std::string>> LoadCsv(
		const std::string& csvFilePath,
		bool commentFlag,
		char divChar,
		const std::string& commentStartStr);

	/// @brief String変数からCSV読み込み
	/// @param source csvFilePath　ファイルパス
	/// @param commentFlag ファイルコメント有効かどうか
	/// @param divChar 行の区切り文字
	/// @param commentStartStr コメントの先頭文字
	static std::vector<std::vector<std::string>> LoadCsvFromString(
		const std::string& source,
		bool commentFlag,
		char divChar,
		const std::string& commentStartStr);
};
