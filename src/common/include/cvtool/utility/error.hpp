/// @file
/// @brief エラー関連のユーティリティ処理定義ヘッダ。
/// @author ruche

#pragma once

#include <string>
#include <exception>
#include <ostream>

namespace cvtool { namespace utility
{
    /// @brief エラー出力を行う。
    /// @param[in] message エラーメッセージ。
    /// @note print_help(std::cerr, message) と同義。
    void print_error(const std::string& message);

    /// @brief エラー出力を行う。
    /// @param[in,out] s 出力先ストリーム。
    /// @param[in] message エラーメッセージ。
    void print_error(std::ostream& s, const std::string& message);

    /// @brief エラー出力を行う。
    /// @param[in] ex エラー原因の例外値。
    /// @note print_help(std::cerr, ex) と同義。
    void print_error(const std::exception& ex);

    /// @brief エラー出力を行う。
    /// @param[in,out] s 出力先ストリーム。
    /// @param[in] ex エラー原因の例外値。
    void print_error(std::ostream& s, const std::exception& ex);
}}
