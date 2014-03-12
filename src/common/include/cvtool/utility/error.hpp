/// @file
/// @brief エラー関連のユーティリティ処理定義ヘッダ。
/// @author ruche

#pragma once

#include <utility>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>

namespace cvtool { namespace error
{
    namespace detail
    {
        template<class T>
        void print_values(std::ostream& s, T&& value)
        {
            s << value;
        }

        template<class T, class ...TArgs>
        void print_values(std::ostream& s, T&& value, TArgs&&... values)
        {
            print_values(s, std::forward<T>(value));
            print_values(s, std::forward<TArgs>(values)...);
        }
    }

    /// @brief エラー出力を行う。
    /// @param[in] message エラーメッセージ。
    /// @note print(std::cerr, message) と同義。
    void print(const std::string& message);

    /// @brief エラー出力を行う。
    /// @param[in,out] s 出力先ストリーム。
    /// @param[in] message エラーメッセージ。
    void print(std::ostream& s, const std::string& message);

    /// @brief エラー出力を行う。
    /// @param[in] ex エラー原因の例外値。
    /// @note print(std::cerr, ex) と同義。
    void print(const std::exception& ex);

    /// @brief エラー出力を行う。
    /// @param[in,out] s 出力先ストリーム。
    /// @param[in] ex エラー原因の例外値。
    void print(std::ostream& s, const std::exception& ex);

    /// @brief エラー出力を行う。
    /// @tparam T 1つ目の出力要素の型。
    /// @tparam TArgs 2つ目以降の出力要素の型。
    /// @param[in,out] s 出力先ストリーム。
    /// @param[in] value 1つ目の出力要素。
    /// @param[in] values 2つ目以降の出力要素。
    template<class T, class ...TArgs>
    void prints(std::ostream& s, T&& value, TArgs&&... values)
    {
        std::ostringstream ss;
        detail::print_values(ss, std::forward<T>(value), std::forward<TArgs>(values)...);
        print(s, ss.str());
    }

    /// @brief エラー出力を行う。
    /// @tparam T 1つ目の出力要素の型。
    /// @tparam TArgs 2つ目以降の出力要素の型。
    /// @param[in] value 1つ目の出力要素。
    /// @param[in] values 2つ目以降の出力要素。
    /// @note print(std::cerr, ...) と同義。
    template<class T, class ...TArgs>
    void prints(T&& value, TArgs&&... values)
    {
        prints(std::cerr, std::forward<T>(value), std::forward<TArgs>(values)...);
    }
}}
