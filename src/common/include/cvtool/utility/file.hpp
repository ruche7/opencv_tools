/// @file
/// @brief ファイル関連のユーティリティ処理定義ヘッダ。
/// @author ruche

#pragma once

#include <cvtool/program_arg.hpp>

#include <string>

namespace cvtool { namespace file
{
    /// @brief ファイルの親ディレクトリ階層を作成する。
    /// @param[in] filename ファイル名。
    /// @retval true  作成に成功したか、作成不要である場合。
    /// @retval false 作成に失敗した場合。
    bool create_parent_directories(const std::string& filename);

    /// @brief ファイル名から拡張子部分を取得する。
    /// @param[in] filename ファイル名。
    /// @return ピリオド抜きの拡張子文字列。拡張子がない場合は空文字列。
    std::string filename_extension(const std::string& filename);

    /// @brief ファイル名を書式文字列によって変換する。
    /// @param[in] format 書式文字列。
    /// @param[in] filename ファイル名。
    /// @return 変換されたファイル名。 format が空文字列の場合は filename そのもの。
    ///
    /// 書式文字列には通常の文字列以外に下記の変数を利用できる。
    /// なおいずれも大文字と小文字は区別しない。
    ///
    /// - $f または %f -- 拡張子を含むファイル名。
    /// - $n または %n -- 拡張子を含まないファイル名。
    /// - $e または %e -- ピリオドを含まない拡張子。
    /// - $p または %p -- ファイル名部分以外。
    /// - $0 または %0 -- filename そのもの。
    std::string format_filename(
        const std::string& format,
        const std::string& filename);

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

    /// @brief ファイル名フォーマット文字列指定用のプログラム引数を作成する。
    /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
    /// @param[in] store フォーマット文字列の設定先。
    /// @return プログラム引数。
    program_arg<std::string> make_filename_format_program_arg(
        const std::string& name,
        std::string& store);

    /// @brief ファイル名プレフィクス置換用のプログラム引数を作成する。
    /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
    /// @param[in] store プレフィクス文字列の設定先。
    /// @return プログラム引数。
    program_arg<std::string> make_filename_prefix_program_arg(
        const std::string& name,
        std::string& store);

    /// @brief ファイル名サフィックス置換用のプログラム引数を作成する。
    /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
    /// @param[in] store サフィックス文字列の設定先。
    /// @return プログラム引数。
    program_arg<std::string> make_filename_suffix_program_arg(
        const std::string& name,
        std::string& store);
}}
