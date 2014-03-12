/// @file
/// @brief �G���[�֘A�̃��[�e�B���e�B������`�w�b�_�B
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

    /// @brief �G���[�o�͂��s���B
    /// @param[in] message �G���[���b�Z�[�W�B
    /// @note print(std::cerr, message) �Ɠ��`�B
    void print(const std::string& message);

    /// @brief �G���[�o�͂��s���B
    /// @param[in,out] s �o�͐�X�g���[���B
    /// @param[in] message �G���[���b�Z�[�W�B
    void print(std::ostream& s, const std::string& message);

    /// @brief �G���[�o�͂��s���B
    /// @param[in] ex �G���[�����̗�O�l�B
    /// @note print(std::cerr, ex) �Ɠ��`�B
    void print(const std::exception& ex);

    /// @brief �G���[�o�͂��s���B
    /// @param[in,out] s �o�͐�X�g���[���B
    /// @param[in] ex �G���[�����̗�O�l�B
    void print(std::ostream& s, const std::exception& ex);

    /// @brief �G���[�o�͂��s���B
    /// @tparam T 1�ڂ̏o�͗v�f�̌^�B
    /// @tparam TArgs 2�ڈȍ~�̏o�͗v�f�̌^�B
    /// @param[in,out] s �o�͐�X�g���[���B
    /// @param[in] value 1�ڂ̏o�͗v�f�B
    /// @param[in] values 2�ڈȍ~�̏o�͗v�f�B
    template<class T, class ...TArgs>
    void prints(std::ostream& s, T&& value, TArgs&&... values)
    {
        std::ostringstream ss;
        detail::print_values(ss, std::forward<T>(value), std::forward<TArgs>(values)...);
        print(s, ss.str());
    }

    /// @brief �G���[�o�͂��s���B
    /// @tparam T 1�ڂ̏o�͗v�f�̌^�B
    /// @tparam TArgs 2�ڈȍ~�̏o�͗v�f�̌^�B
    /// @param[in] value 1�ڂ̏o�͗v�f�B
    /// @param[in] values 2�ڈȍ~�̏o�͗v�f�B
    /// @note print(std::cerr, ...) �Ɠ��`�B
    template<class T, class ...TArgs>
    void prints(T&& value, TArgs&&... values)
    {
        prints(std::cerr, std::forward<T>(value), std::forward<TArgs>(values)...);
    }
}}
