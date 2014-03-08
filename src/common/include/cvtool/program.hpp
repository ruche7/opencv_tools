/// @file
/// @brief ツールの基本的な処理を行うクラス program のヘッダ。
/// @author ruche

#pragma once

#include <cvtool/program_arg.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>
#include <exception>
#include <ostream>

namespace cvtool
{
    /// ツールの基本的な処理を行うクラス。
    class program
    {
    public:
        /// @brief コンストラクタ。
        /// @tparam TArgs プログラム引数定義の値型。
        /// @param[in] argc main 関数の第1引数。
        /// @param[in] argv main 関数の第2引数。
        /// @param[in] positional_arg_name
        ///     オプション名を伴わないプログラム引数のヘルプ表示名。
        ///     利用しないならば nullptr または空文字列。
        /// @param[in] positional_arg_count
        ///     オプション名を伴わないプログラム引数の最大許容数。
        ///     上限を設けないならば -1 。利用しないならば 0 。
        /// @param[in] arg_defs プログラム引数定義。
        template<class ...TArgs>
        program(
            int argc,
            const char* const argv[],
            const char* positional_arg_name,
            int positional_arg_count,
            const program_arg<TArgs>&... arg_defs)
            :
            argc_(argc),
            argv_(argv),
            positional_arg_name_(
                (positional_arg_name == nullptr) ? "" : positional_arg_name),
            positional_arg_count_(positional_arg_count),
            program_name_(make_program_name(argc, argv)),
            desc_("Option"),
            vm_()
        {
            make_options(desc_, arg_defs...);
        }

        /// @brief コンストラクタ。
        /// @tparam TArgs プログラム引数定義の値型。
        /// @param[in] argc main 関数の第1引数。
        /// @param[in] argv main 関数の第2引数。
        /// @param[in] arg_defs プログラム引数定義。
        template<class ...TArgs>
        program(
            int argc,
            const char* const argv[],
            const program_arg<TArgs>&... arg_defs)
            :
            program(argc, argv, nullptr, 0, arg_defs...)
        {
        }

        // 既定動作
        ~program() = default;

        // 暗黙定義の削除
        program(const program&) = delete;
        program& operator=(const program&) = delete;
#ifndef _MSC_VER
        program(program&&) = delete;
        program& operator=(program&&) = delete;
#endif // _MSC_VER

        /// @brief プログラム引数をパースする。
        /// @exception std::exception パース失敗時。
        void parse_args();

        /// @brief オプション名を伴わないプログラム引数を指定可能か否かを取得する。
        /// @retval true  指定可能。
        /// @retval false 指定不可能。
        bool allow_positional() const;

        /// @brief オプション名を伴わないプログラム引数の抽出数を取得する。
        /// @return 抽出数。 parse_args 関数呼び出し前は 0 。
        std::size_t parsed_positional_arg_count() const;

        /// @brief オプション名を伴わないプログラム引数の指定文字列値を取得する。
        /// @param[in] index インデックス。
        /// @return 指定文字列値。 index が範囲外ならば nullptr 。
        const char* parsed_positional_arg(std::size_t index) const;

        /// @brief ヘルプ表示を要求されたか否かを取得する。
        /// @return true  ヘルプ表示を要求された場合。
        /// @return false ヘルプ表示を要求されなかった場合。
        ///
        /// parse_args 関数呼び出し前は必ず false を返す。
        bool help_required() const;

        /// @brief ヘルプ出力を行う。
        /// @note print_help(std::cout) と同義。
        void print_help() const;

        /// @brief ヘルプ出力を行う。
        /// @param[in,out] s 出力先ストリーム。
        void print_help(std::ostream& s) const;

        /// @brief エラー出力を行う。
        /// @param[in] ex エラー原因の例外値。
        /// @note print_help(std::cerr, ex) と同義。
        void print_error(const std::exception& ex) const;

        /// @brief エラー出力を行う。
        /// @param[in,out] s 出力先ストリーム。
        /// @param[in] ex エラー原因の例外値。
        void print_error(std::ostream& s, const std::exception& ex) const;

    private:
        std::string make_usage() const;

        static std::string make_program_name(int argc, const char* const argv[]);

        static void make_options(boost::program_options::options_description& desc);

        template<class ...TArgs>
        static void make_options(
            boost::program_options::options_description& desc,
            const program_arg<TArgs>&... arg_defs)
        {
            make_options(desc);
            add_options(desc, arg_defs...);
        }

        template<class T>
        static void add_to_options(
            boost::program_options::options_description& desc,
            const program_arg<T>& arg_def)
        {
            arg_def.add_to(desc);
        }

        template<class T, class ...TArgs>
        static void add_to_options(
            boost::program_options::options_description& desc,
            const program_arg<T>& arg_def,
            const program_arg<TArgs>&... arg_defs)
        {
            add_to_options(desc, arg_def);
            add_to_options(desc, arg_defs...);
        }

    private:
        int argc_;
        const char* const* argv_;
        std::string positional_arg_name_;
        int positional_arg_count_;

        std::string program_name_;
        boost::program_options::options_description desc_;
        boost::program_options::variables_map vm_;
    };
}
