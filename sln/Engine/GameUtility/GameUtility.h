/// <<<<<<<<
/// @brief 汎用的機能使いやすくをまとめるためのクラス
/// >>>>>>>>

#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
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

    /// @brief　CSVの読み込み
    /// @param csvFilePath　ファイルパス
    /// @param divChar　行の区切り文字
    /// @param commentFlag　ファイルコメント有効かどうか
    /// @param commentStartStr　コメントの先頭文字
    static std::vector<std::vector<std::string>> LoadCsv(
        const std::string& csvFilePath,
        bool commentFlag,
        char divChar,
        const std::string& commentStartStr
    )
    {
        std::vector<std::vector<std::string>> csvData{}; // csvの中身を格納

        std::ifstream ifs(csvFilePath);
        if (!ifs)
        {
            //標準エラー出力
            std::cerr << "!Error! Could not open file... : " << csvFilePath << std::endl;
            return csvData;
        }

        std::string line{};
        // 開いたファイルを一行読み込む(カーソルも動く)
        while (std::getline(ifs, line))
        {
            // コメントが有効かつ行頭が指定文字なら、その行は無視する
            if (commentFlag && line.find(commentStartStr) == 0U)
            {
                continue;
            }

            // 空行の場合は無視する
            if (line.empty())
            {
                continue;
            }

            // 行数を増やす
            csvData.emplace_back();

            std::istringstream stream(line);
            std::string field;
            // 読み込んだ行を','区切りで分割
            while (std::getline(stream, field, divChar))
            {
                csvData.back().emplace_back(field);
            }
        }

        return csvData;
    }
};
