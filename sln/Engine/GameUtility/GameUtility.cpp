#include "GameUtility.h"
#include <fstream>
#include <sstream>
#include <iostream>	// std::cerr

auto GameUtility::UtilSLerp(const DirectX::XMFLOAT3& startPos, const DirectX::XMFLOAT3& endPos, float t)
{
	const float startLength = std::sqrt(startPos.x * startPos.x + startPos.y * startPos.y + startPos.z * startPos.z);
	const float endLength = std::sqrt(endPos.x * endPos.x + endPos.y * endPos.y + endPos.z * endPos.z);

	const float len = std::lerp(startLength, endLength, t);

	auto vec = UtilLerp(startPos, endPos, t);

	// vec を正規化
	UtilNormalize(vec);

	vec.x *= len;
	vec.y *= len;
	vec.z *= len;

	return vec;
}

std::vector<std::vector<std::string>> GameUtility::LoadCsv(const std::string& csvFilePath, bool commentFlag, char divChar, const std::string& commentStartStr)
{
	std::vector<std::vector<std::string>> csvData{}; // csvの中身を格納

	std::ifstream ifs(csvFilePath);
	if (!ifs) {
		//標準エラー出力
		std::cerr << "!Error! Could not open file... : " << csvFilePath << std::endl;
		return csvData;
	}

	std::string line{};
	// 開いたファイルを一行読み込む(カーソルも動く)
	while (std::getline(ifs, line))
	{
		// 空行の場合は無視する
		if (line.empty()) {
			continue;
		}

		// コメントが有効かつ行頭が指定文字なら、その行は無視する
		if (commentFlag && line.find(commentStartStr) == 0U) {
			continue;
		}

		// 行数を増やす
		csvData.emplace_back();

		std::istringstream stream(line);
		std::string field;
		// 読み込んだ行を','区切りで分割
		while (std::getline(stream, field, divChar)) {
			csvData.back().emplace_back(field);
		}
	}

	return csvData;
}