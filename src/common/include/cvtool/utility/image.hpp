/// @file
/// @brief 画像関連のユーティリティ処理定義ヘッダ。
/// @author ruche

#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <string>

namespace cvtool { namespace image
{
    /// @brief 例外を投げずエラー出力を行わない cv::imread ラップ関数。
    cv::Mat read(const std::string& filename, int flags = cv::IMREAD_COLOR);

    /// @brief 例外を投げずエラー出力を行わない cv::imwrite ラップ関数。
    bool write(
        const std::string& filename,
        cv::InputArray img,
        const cv::vector<int>& params = cv::vector<int>());
}}
