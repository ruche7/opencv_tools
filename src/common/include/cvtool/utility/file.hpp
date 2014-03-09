/// @file
/// @brief �t�@�C���֘A�̃��[�e�B���e�B������`�w�b�_�B
/// @author ruche

#pragma once

#include <string>

namespace cvtool { namespace utility
{
    /// @brief �t�@�C���̐e�f�B���N�g���K�w���쐬����B
    /// @param[in] filename �t�@�C�����B
    /// @retval true  �쐬�ɐ����������A�쐬�s�v�ł���ꍇ�B
    /// @retval false �쐬�Ɏ��s�����ꍇ�B
    bool create_parent_directories(const std::string& filename);

    /// @brief �t�@�C�����̃v���t�B�N�X��ύX����B
    /// @param[in] filename �t�@�C�����B
    /// @param[in] prefix �v���t�B�N�X������B
    /// @return �ύX���ꂽ�t�@�C�����B
    ///
    /// prefix ���f�B���N�g���p�X���܂ޏꍇ�� filename �̃t�@�C�����������O��u������B
    /// �����łȂ���� filename �̃t�@�C���������擪�� prefix ��}������B
    std::string change_filename_prefix(
        const std::string& filename,
        const std::string& prefix);

    /// @brief �t�@�C�����̃T�t�B�b�N�X��ύX����B
    /// @param[in] filename �t�@�C�����B
    /// @param[in] prefix �T�t�B�b�N�X������B
    /// @return �ύX���ꂽ�t�@�C�����B
    ///
    /// suffix ���g���q���܂ޏꍇ�� filename �̊g���q�ȍ~��u������B
    /// �����łȂ���� filename �̊g���q��O�� suffix ��}������B
    std::string change_filename_suffix(
        const std::string& filename,
        const std::string& suffix);

    /// @brief �t�@�C�����̃v���t�B�N�X����уT�t�B�b�N�X��ύX����B
    /// @param[in] filename �t�@�C�����B
    /// @param[in] prefix �v���t�B�N�X������B
    /// @param[in] prefix �T�t�B�b�N�X������B
    /// @return �ύX���ꂽ�t�@�C�����B
    ///
    /// change_filename_prefix �� change_filename_suffix �𗼕��ĂԂ��ƂƓ��`�B
    std::string change_filename(
        const std::string& filename,
        const std::string& prefix,
        const std::string& suffix);

    /// @brief �t�@�C�����v���t�B�N�X�u���p�v���O���������̐��������擾����B
    /// @return �������B
    std::string filename_prefix_arg_description();

    /// @brief �t�@�C�����T�t�B�b�N�X�u���p�v���O���������̐��������擾����B
    /// @return �������B
    std::string filename_suffix_arg_description();
}}