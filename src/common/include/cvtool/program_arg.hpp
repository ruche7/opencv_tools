/// @file
/// @brief �v���O�����������`����N���X program_arg{T} �̃w�b�_�B
/// @author ruche

#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <utility>
#include <string>

namespace cvtool
{
    /// �v���O�����������`����N���X�B
    template<class T = void>
    class program_arg
    {
    public:
        /// @brief �R���X�g���N�^�B
        /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
        /// @param[in] description �������B
        /// @param[in] store �����l�ݒ��Q�ƁB����l�ɂ��Ȃ�B
        /// @param[in] required �K���w�肷�ׂ������Ȃ�� true �B
        ///
        /// required �� true �Ȃ�� store �͊���l�ɂ͂Ȃ�Ȃ��B
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

        // ���蓮��
        program_arg(const program_arg&) = default;
        ~program_arg() = default;
        program_arg& operator=(const program_arg&) = default;

#ifdef _MSC_VER
        /// ���[�u�R���X�g���N�^�B
        program_arg(program_arg&& src)
            :
            name_(std::move(src.name_)),
            description_(std::move(src.description_)),
            store_(src.store_),
            required_(src.required_)
        {
        }

        /// ���[�u������Z�q�B
        program_arg& operator=(program_arg&& r)
        {
            name_ = std::move(r.name_);
            description_ = std::move(r.description_);
            store_ = r.store_;
            required_ = r.required_;
        }
#else
        // ���蓮��
        program_arg(program_arg&&) = default;
        program_arg& operator=(program_arg&&) = default;
#endif // _MSC_VER

        /// @brief ������ options_description �ɒǉ�����B
        /// @param[in,out] desc �ǉ���� options_description �B
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

    /// �l�����Ȃ��v���O�����������`����N���X�B
    template<>
    class program_arg<void>
    {
    public:
        /// @brief �R���X�g���N�^�B
        /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
        /// @param[in] description �������B
        /// @param[in] presented_store �������w�肳�ꂽ���ۂ��̐ݒ��B
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

        // ���蓮��
        program_arg(const program_arg&) = default;
        ~program_arg() = default;
        program_arg& operator=(const program_arg&) = default;

#ifdef _MSC_VER
        /// ���[�u�R���X�g���N�^�B
        program_arg(program_arg&& src)
            :
            name_(std::move(src.name_)),
            description_(std::move(src.description_)),
            presented_store_(src.presented_store_)
        {
        }

        /// ���[�u������Z�q�B
        program_arg& operator=(program_arg&& r)
        {
            name_ = std::move(r.name_);
            description_ = std::move(r.description_);
            presented_store_ = r.presented_store_;
        }
#else
        // ���蓮��
        program_arg(program_arg&&) = default;
        program_arg& operator=(program_arg&&) = default;
#endif // _MSC_VER

        /// @brief ������ options_description �ɒǉ�����B
        /// @param[in,out] desc �ǉ���� options_description �B
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

    /// @brief �v���O�����������쐬����B
    /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
    /// @param[in] description �������B
    /// @param[in] store �����l�ݒ��Q�ƁB����l�ɂ��Ȃ�B
    /// @param[in] required �K���w�肷�ׂ������Ȃ�� true �B
    /// @return �v���O���������B
    ///
    /// required �� true �Ȃ�� store �͊���l�ɂ͂Ȃ�Ȃ��B
    template<class T>
    inline program_arg<T> make_program_arg(
        const std::string& name,
        const std::string& description,
        T& store,
        bool required = false)
    {
        return program_arg<T>(name, description, store, required);
    }

    /// @brief �l�����Ȃ��v���O�����������쐬����B
    /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
    /// @param[in] description �������B
    /// @param[in] presented_store �������w�肳�ꂽ���ۂ��̐ݒ��B
    /// @return �v���O���������B
    inline program_arg<void> make_switch_program_arg( 
        const std::string& name,
        const std::string& description,
        bool& presented_store)
    {
        return program_arg<void>(name, description, presented_store);
    }
}
