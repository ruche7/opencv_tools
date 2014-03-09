/// @file
/// @brief プログラム引数を定義するクラス program_arg{T} のヘッダ。
/// @author ruche

#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <utility>
#include <string>

namespace cvtool
{
    /// プログラム引数を定義するクラス。
    template<class T = void>
    class program_arg
    {
    public:
        /// @brief コンストラクタ。
        /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
        /// @param[in] description 説明文。
        /// @param[in] store 引数値設定先参照。既定値にもなる。
        /// @param[in] required 必ず指定すべき引数ならば true 。
        ///
        /// required が true ならば store は既定値にはならない。
        program_arg(
            const std::string& name,
            const std::string& description,
            T& store,
            bool required = false)
            :
            name_(name),
            description_(description),
            store_(&store),
            required_(required)
        {
        }

        // 既定動作
        program_arg(const program_arg&) = default;
        ~program_arg() = default;
        program_arg& operator=(const program_arg&) = default;

#ifdef _MSC_VER
        /// ムーブコンストラクタ。
        program_arg(program_arg&& src)
            :
            name_(std::move(src.name_)),
            description_(std::move(src.description_)),
            store_(src.store_),
            required_(src.required_)
        {
        }

        /// ムーブ代入演算子。
        program_arg& operator=(program_arg&& r)
        {
            name_ = std::move(r.name_);
            description_ = std::move(r.description_);
            store_ = r.store_;
            required_ = r.required_;
        }
#else
        // 既定動作
        program_arg(program_arg&&) = default;
        program_arg& operator=(program_arg&&) = default;
#endif // _MSC_VER

        /// @brief 引数を options_description に追加する。
        /// @param[in,out] desc 追加先の options_description 。
        void add_to(boost::program_options::options_description& desc) const
        {
            desc.add_options()(
                name_.c_str(),
                required_ ?
                    boost::program_options::value(store_)->required() :
                    boost::program_options::value(store_)->default_value(*store_),
                description_.c_str());
        }

    private:
        std::string name_;
        std::string description_;
        T* store_;
        bool required_;
    };

    /// 値を取らないプログラム引数を定義するクラス。
    template<>
    class program_arg<void>
    {
    public:
        /// @brief コンストラクタ。
        /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
        /// @param[in] description 説明文。
        /// @param[in] presented_store 引数が指定されたか否かの設定先。
        program_arg(
            const std::string& name,
            const std::string& description,
            bool& presented_store)
            :
            name_(name),
            description_(description),
            presented_store_(&presented_store)
        {
        }

        // 既定動作
        program_arg(const program_arg&) = default;
        ~program_arg() = default;
        program_arg& operator=(const program_arg&) = default;

#ifdef _MSC_VER
        /// ムーブコンストラクタ。
        program_arg(program_arg&& src)
            :
            name_(std::move(src.name_)),
            description_(std::move(src.description_)),
            presented_store_(src.presented_store_)
        {
        }

        /// ムーブ代入演算子。
        program_arg& operator=(program_arg&& r)
        {
            name_ = std::move(r.name_);
            description_ = std::move(r.description_);
            presented_store_ = r.presented_store_;
        }
#else
        // 既定動作
        program_arg(program_arg&&) = default;
        program_arg& operator=(program_arg&&) = default;
#endif // _MSC_VER

        /// @brief 引数を options_description に追加する。
        /// @param[in,out] desc 追加先の options_description 。
        void add_to(boost::program_options::options_description& desc) const
        {
            desc.add_options()(
                name_.c_str(),
                boost::program_options::bool_switch(presented_store_),
                description_.c_str());
        }

    private:
        std::string name_;
        std::string description_;
        bool* presented_store_;
    };

    /// @brief プログラム引数を作成する。
    /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
    /// @param[in] description 説明文。
    /// @param[in] store 引数値設定先参照。既定値にもなる。
    /// @param[in] required 必ず指定すべき引数ならば true 。
    /// @return プログラム引数。
    ///
    /// required が true ならば store は既定値にはならない。
    template<class T>
    inline program_arg<T> make_program_arg(
        const std::string& name,
        const std::string& description,
        T& store,
        bool required = false)
    {
        return program_arg<T>(name, description, store, required);
    }

    /// @brief 値を取らないプログラム引数を作成する。
    /// @param[in] name 引数名。カンマで区切って短縮名も指定できる。
    /// @param[in] description 説明文。
    /// @param[in] presented_store 引数が指定されたか否かの設定先。
    /// @return プログラム引数。
    inline program_arg<void> make_switch_program_arg( 
        const std::string& name,
        const std::string& description,
        bool& presented_store)
    {
        return program_arg<void>(name, description, presented_store);
    }
}
