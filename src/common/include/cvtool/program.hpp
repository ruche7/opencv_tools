/// @file
/// @brief �c�[���̊�{�I�ȏ������s���N���X program �̃w�b�_�B
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
    /// �c�[���̊�{�I�ȏ������s���N���X�B
    class program
    {
    public:
        /// @brief �R���X�g���N�^�B
        /// @tparam TArgs �v���O����������`�̒l�^�B
        /// @param[in] argc main �֐��̑�1�����B
        /// @param[in] argv main �֐��̑�2�����B
        /// @param[in] positional_arg_name
        ///     �I�v�V�������𔺂�Ȃ��v���O���������̃w���v�\�����B
        ///     ���p���Ȃ��Ȃ�� nullptr �܂��͋󕶎���B
        /// @param[in] positional_arg_count
        ///     �I�v�V�������𔺂�Ȃ��v���O���������̍ő勖�e���B
        ///     �����݂��Ȃ��Ȃ�� -1 �B���p���Ȃ��Ȃ�� 0 �B
        /// @param[in] arg_defs �v���O����������`�B
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

        /// @brief �R���X�g���N�^�B
        /// @tparam TArgs �v���O����������`�̒l�^�B
        /// @param[in] argc main �֐��̑�1�����B
        /// @param[in] argv main �֐��̑�2�����B
        /// @param[in] arg_defs �v���O����������`�B
        template<class ...TArgs>
        program(
            int argc,
            const char* const argv[],
            const program_arg<TArgs>&... arg_defs)
            :
            program(argc, argv, nullptr, 0, arg_defs...)
        {
        }

        // ���蓮��
        ~program() = default;

        // �Öْ�`�̍폜
        program(const program&) = delete;
        program& operator=(const program&) = delete;
#ifndef _MSC_VER
        program(program&&) = delete;
        program& operator=(program&&) = delete;
#endif // _MSC_VER

        /// @brief �v���O�����������p�[�X����B
        /// @exception std::exception �p�[�X���s���B
        void parse_args();

        /// @brief �I�v�V�������𔺂�Ȃ��v���O�����������w��\���ۂ����擾����B
        /// @retval true  �w��\�B
        /// @retval false �w��s�\�B
        bool allow_positional() const;

        /// @brief �I�v�V�������𔺂�Ȃ��v���O���������̒��o�����擾����B
        /// @return ���o���B parse_args �֐��Ăяo���O�� 0 �B
        std::size_t parsed_positional_arg_count() const;

        /// @brief �I�v�V�������𔺂�Ȃ��v���O���������̎w�蕶����l���擾����B
        /// @param[in] index �C���f�b�N�X�B
        /// @return �w�蕶����l�B index ���͈͊O�Ȃ�� nullptr �B
        const char* parsed_positional_arg(std::size_t index) const;

        /// @brief �w���v�\����v�����ꂽ���ۂ����擾����B
        /// @return true  �w���v�\����v�����ꂽ�ꍇ�B
        /// @return false �w���v�\����v������Ȃ������ꍇ�B
        ///
        /// parse_args �֐��Ăяo���O�͕K�� false ��Ԃ��B
        bool help_required() const;

        /// @brief �w���v�o�͂��s���B
        /// @note print_help(std::cout) �Ɠ��`�B
        void print_help() const;

        /// @brief �w���v�o�͂��s���B
        /// @param[in,out] s �o�͐�X�g���[���B
        void print_help(std::ostream& s) const;

        /// @brief �G���[�o�͂��s���B
        /// @param[in] ex �G���[�����̗�O�l�B
        /// @note print_help(std::cerr, ex) �Ɠ��`�B
        void print_error(const std::exception& ex) const;

        /// @brief �G���[�o�͂��s���B
        /// @param[in,out] s �o�͐�X�g���[���B
        /// @param[in] ex �G���[�����̗�O�l�B
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
