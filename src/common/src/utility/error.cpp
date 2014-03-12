/// @file
/// @brief エラー関連のユーティリティ処理定義ソース。
/// @author ruche

#include <cvtool/utility/error.hpp>

#include <iostream>

namespace cvtool { namespace error
{
    void print(const std::string& message)
    {
        print(std::cerr, message);
    }

    void print(std::ostream& s, const std::string& message)
    {
        s << "ERROR: " << message << std::endl;
    }

    void print(const std::exception& ex)
    {
        print(std::cerr, ex);
    }

    void print(std::ostream& s, const std::exception& ex)
    {
        auto msg = ex.what();
        print(s, (msg == nullptr) ? "Unknown error." : msg);
    }
}}
