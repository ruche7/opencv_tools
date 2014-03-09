/// @file
/// @brief エラー関連のユーティリティ処理定義ソース。
/// @author ruche

#include <cvtool/utility/error.hpp>

#include <iostream>

namespace cvtool { namespace utility
{
    void print_error(const std::string& message)
    {
        print_error(std::cerr, message);
    }

    void print_error(std::ostream& s, const std::string& message)
    {
        s << "ERROR: " << message << std::endl;
    }

    void print_error(const std::exception& ex)
    {
        print_error(std::cerr, ex);
    }

    void print_error(std::ostream& s, const std::exception& ex)
    {
        auto msg = ex.what();
        print_error(s, (msg == nullptr) ? "Unknown error." : msg);
    }
}}
