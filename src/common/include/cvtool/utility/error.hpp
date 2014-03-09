/// @file
/// @brief �G���[�֘A�̃��[�e�B���e�B������`�w�b�_�B
/// @author ruche

#pragma once

#include <string>
#include <exception>
#include <ostream>

namespace cvtool { namespace utility
{
    /// @brief �G���[�o�͂��s���B
    /// @param[in] message �G���[���b�Z�[�W�B
    /// @note print_help(std::cerr, message) �Ɠ��`�B
    void print_error(const std::string& message);

    /// @brief �G���[�o�͂��s���B
    /// @param[in,out] s �o�͐�X�g���[���B
    /// @param[in] message �G���[���b�Z�[�W�B
    void print_error(std::ostream& s, const std::string& message);

    /// @brief �G���[�o�͂��s���B
    /// @param[in] ex �G���[�����̗�O�l�B
    /// @note print_help(std::cerr, ex) �Ɠ��`�B
    void print_error(const std::exception& ex);

    /// @brief �G���[�o�͂��s���B
    /// @param[in,out] s �o�͐�X�g���[���B
    /// @param[in] ex �G���[�����̗�O�l�B
    void print_error(std::ostream& s, const std::exception& ex);
}}
