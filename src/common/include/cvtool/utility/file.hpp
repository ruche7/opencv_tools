/// @file
/// @brief �t�@�C���֘A�̃��[�e�B���e�B������`�w�b�_�B
/// @author ruche

#pragma once

#include <cvtool/program_arg.hpp>

#include <string>

namespace cvtool { namespace file
{
    /// @brief �t�@�C���̐e�f�B���N�g���K�w���쐬����B
    /// @param[in] filename �t�@�C�����B
    /// @retval true  �쐬�ɐ����������A�쐬�s�v�ł���ꍇ�B
    /// @retval false �쐬�Ɏ��s�����ꍇ�B
    bool create_parent_directories(const std::string& filename);

    /// @brief �t�@�C��������g���q�������擾����B
    /// @param[in] filename �t�@�C�����B
    /// @return �s���I�h�����̊g���q������B�g���q���Ȃ��ꍇ�͋󕶎���B
    std::string filename_extension(const std::string& filename);

    /// @brief �t�@�C����������������ɂ���ĕϊ�����B
    /// @param[in] format ����������B
    /// @param[in] filename �t�@�C�����B
    /// @return �ϊ����ꂽ�t�@�C�����B format ���󕶎���̏ꍇ�� filename ���̂��́B
    ///
    /// ����������ɂ͒ʏ�̕�����ȊO�ɉ��L�̕ϐ��𗘗p�ł���B
    /// �Ȃ���������啶���Ə������͋�ʂ��Ȃ��B
    ///
    /// - $f �܂��� %f -- �g���q���܂ރt�@�C�����B
    /// - $n �܂��� %n -- �g���q���܂܂Ȃ��t�@�C�����B
    /// - $e �܂��� %e -- �s���I�h���܂܂Ȃ��g���q�B
    /// - $p �܂��� %p -- �t�@�C���������ȊO�B
    /// - $0 �܂��� %0 -- filename ���̂��́B
    std::string format_filename(
        const std::string& format,
        const std::string& filename);

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

    /// @brief �t�@�C�����t�H�[�}�b�g������w��p�̃v���O�����������쐬����B
    /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
    /// @param[in] store �t�H�[�}�b�g������̐ݒ��B
    /// @return �v���O���������B
    program_arg<std::string> make_filename_format_program_arg(
        const std::string& name,
        std::string& store);

    /// @brief �t�@�C�����v���t�B�N�X�u���p�̃v���O�����������쐬����B
    /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
    /// @param[in] store �v���t�B�N�X������̐ݒ��B
    /// @return �v���O���������B
    program_arg<std::string> make_filename_prefix_program_arg(
        const std::string& name,
        std::string& store);

    /// @brief �t�@�C�����T�t�B�b�N�X�u���p�̃v���O�����������쐬����B
    /// @param[in] name �������B�J���}�ŋ�؂��ĒZ�k�����w��ł���B
    /// @param[in] store �T�t�B�b�N�X������̐ݒ��B
    /// @return �v���O���������B
    program_arg<std::string> make_filename_suffix_program_arg(
        const std::string& name,
        std::string& store);
}}
