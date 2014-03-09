/// @file
/// @brief ファイル関連のユーティリティ処理定義ヘッダ。
/// @author ruche

#pragma once

#include <string>

namespace cvtool { namespace utility
{
    /// @brief ファイルの親ディレクトリ階層を作成する。
    /// @param[in] filename ファイル名。
    /// @retval true  作成に成功したか、作成不要である場合。
    /// @retval false 作成に失敗した場合。
    bool create_parent_directories(const std::string& filename);

    /// @brief ファイル名のプレフィクスを変更する。
    /// @param[in] filename ファイル名。
    /// @param[in] prefix プレフィクス文字列。
    /// @return 変更されたファイル名。
    ///
    /// prefix がディレクトリパスを含む場合は filename のファイル名部分より前を置換する。
    /// そうでなければ filename のファイル名部分先頭に prefix を挿入する。
    std::string change_filename_prefix(
        const std::string& filename,
        const std::string& prefix);

    /// @brief ファイル名のサフィックスを変更する。
    /// @param[in] filename ファイル名。
    /// @param[in] prefix サフィックス文字列。
    /// @return 変更されたファイル名。
    ///
    /// suffix が拡張子を含む場合は filename の拡張子以降を置換する。
    /// そうでなければ filename の拡張子手前に suffix を挿入する。
    std::string change_filename_suffix(
        const std::string& filename,
        const std::string& suffix);

    /// @brief ファイル名のプレフィクスおよびサフィックスを変更する。
    /// @param[in] filename ファイル名。
    /// @param[in] prefix プレフィクス文字列。
    /// @param[in] prefix サフィックス文字列。
    /// @return 変更されたファイル名。
    ///
    /// change_filename_prefix と change_filename_suffix を両方呼ぶことと同義。
    std::string change_filename(
        const std::string& filename,
        const std::string& prefix,
        const std::string& suffix);

    /// @brief ファイル名プレフィクス置換用プログラム引数の説明文を取得する。
    /// @return 説明文。
    std::string filename_prefix_arg_description();

    /// @brief ファイル名サフィックス置換用プログラム引数の説明文を取得する。
    /// @return 説明文。
    std::string filename_suffix_arg_description();
}}
