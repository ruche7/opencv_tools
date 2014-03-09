cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# cmake common module
#===========================================================

#---------------------------------------
# �萔��`
#---------------------------------------

# �v���W�F�N�g�Ɋ܂߂�t�@�C���̊g���q
set(SRC_EXTENDS   "c;cc;cpp;cxx")
set(EXTRA_EXTENDS "h;hpp;hxx;inc;def")

# �R���p�C���ɒǉ��ݒ肷��I�v�V����
if(MSVC)
    # boost �̃R�[�h�ŏo�Ă��܂�W4�x���͖���
    set(ADDITIONAL_COMPILE_FLAGS "/MP /W4 /WX /wd4505 /wd4512")
    set(ADDITIONAL_COMPILE_FLAGS_RELEASE "/Ox /Oi /Ot /Oy")
else()
    set(ADDITIONAL_COMPILE_FLAGS "")
    set(ADDITIONAL_COMPILE_FLAGS_RELEASE "")
endif()

# OpenCV���C�u�����o�[�W��������l
set(OPENCV_VERSION_DEFAULT "248")

#---------------------------------------
# �ϐ����Ƃ��̓��e��\������B
#---------------------------------------
macro(show_variable V)
    message("${V}=${${V}}")
endmacro()

#---------------------------------------
# VC++�r���h�\���̐ݒ���s���B
# project �R�}���h�����O�ɌĂяo�����ƁB
#---------------------------------------
macro(setup_config_types)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endmacro()

#---------------------------------------
# ���ʐݒ���s���B
# 
# ���O�Ɉȉ��̕ϐ��̐ݒ肪�K�v�B
#  - PROJ_NAME     -- �v���W�F�N�g��
#  - PROJ_SRC_DIRS -- �\�[�X�̃��[�g�f�B���N�g���p�X�̃��X�g
#  - PROJ_INCLUDES -- �C���N���[�h�p�X�̃��X�g
#
# �ȉ��̕ϐ��͖��ݒ�Ȃ�Ί���l���ݒ肳���B
#  - PROJ_TYPE     -- �v���W�F�N�g�̎��
#  - PROJ_BASE_DIR -- �v���W�F�N�g�̃��[�g�f�B���N�g���p�X
#
# PROJ_TYPE �ɂ͉��L�̂����ꂩ�̒l���w�肷��B
#  - "program" -- ���s�v���O����(����l)
#  - "static"  -- �X�^�e�B�b�N���C�u����
#  - "shared"  -- ���L���C�u����(DLL)
#---------------------------------------
macro(setup_common)
    message("---- setup : ${PROJ_NAME}")

    # �K�v�ȕϐ���ݒ�
    if(NOT PROJ_TYPE)
        set(PROJ_TYPE "program")
    endif()
    if(NOT PROJ_BASE_DIR)
        set(PROJ_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    # �r���h�\���ݒ�
    setup_config_types()

    # �v���W�F�N�g�ݒ�
    project("${PROJ_NAME}")

    # �\�[�X�t�@�C�����X�g�쐬
    set(SRC_FILES "")
    foreach(SRC_DIR ${PROJ_SRC_DIRS})
        # �t�@�C�������X�g�A�b�v
        set(FILES_ON_DIR "")

        # �\�[�X�ȊO�̃t�@�C�����ɒǉ�
        foreach(EXTRA_EXT ${EXTRA_EXTENDS})
            file(GLOB_RECURSE TEMP_SRC_FILES
                "${PROJ_BASE_DIR}/${SRC_DIR}/*.${EXTRA_EXT}")
            list(APPEND FILES_ON_DIR ${TEMP_SRC_FILES})
        endforeach()

        # �R���p�C���ΏۊO�̃v���W�F�N�g�A�C�e���ɂ���
        set_source_files_properties(${FILES_ON_DIR} PROPERTIES HEADER_FILE_ONLY on)

        # �\�[�X�t�@�C����ǉ�
        foreach(SRC_EXT ${SRC_EXTENDS})
            file(GLOB_RECURSE TEMP_SRC_FILES
                "${PROJ_BASE_DIR}/${SRC_DIR}/*.${SRC_EXT}")
            list(APPEND FILES_ON_DIR ${TEMP_SRC_FILES})
        endforeach()

        # �t�@�C�����X�g�ɒǉ�
        list(APPEND SRC_FILES ${FILES_ON_DIR})
    endforeach()

    # �S�t�@�C���ɑ΂��鏈��
    foreach(SRC_FILE ${SRC_FILES})
        # �f�B���N�g���̑��΃p�X�擾
        file(RELATIVE_PATH REL_PATH "${PROJ_BASE_DIR}" "${SRC_FILE}")
        string(REGEX REPLACE "/[^/]+$" "" REL_PATH "${REL_PATH}")

        # �t�@�C���̑�����v���W�F�N�g�K�w��ݒ�(���VC++�p)
        string(REPLACE "/" "\\" PROJ_TREE "${REL_PATH}")
        source_group("${PROJ_TREE}" FILES "${SRC_FILE}")

        # �\�[�X�t�@�C�����o��
        message("INPUT: ${SRC_FILE}")
    endforeach()

    # �쐬�ݒ�
    if("${PROJ_TYPE}" STREQUAL "static")
        add_library("${PROJ_NAME}" STATIC ${SRC_FILES})
    elseif("${PROJ_TYPE}" STREQUAL "shared")
        add_library("${PROJ_NAME}" SHARED ${SRC_FILES})
    else()
        add_executable("${PROJ_NAME}" ${SRC_FILES})
    endif()

    # �f�o�b�O�ł̃|�X�g�t�B�b�N�X�ݒ�
    set_target_properties("${PROJ_NAME}" PROPERTIES DEBUG_POSTFIX "_d")

    # �C���N���[�h�p�X�ǉ�
    foreach(INC_PATH ${PROJ_INCLUDES})
        include_directories("${PROJ_BASE_DIR}/${INC_PATH}")
    endforeach()

    # �R���p�C���I�v�V�����̐ݒ�
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ${ADDITIONAL_COMPILE_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} ${ADDITIONAL_COMPILE_FLAGS_RELEASE}")
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} ${ADDITIONAL_COMPILE_FLAGS}")
    set(CMAKE_C_FLAGS_RELEASE
        "${CMAKE_C_FLAGS_RELEASE} ${ADDITIONAL_COMPILE_FLAGS_RELEASE}")
endmacro()

#---------------------------------------
# �c�[���쐬�̋��ʐݒ���s���B
# 
# ���O�Ɉȉ��̕ϐ��̐ݒ肪�K�v�B
#  - PROJ_NAME   -- �v���W�F�N�g��
#  - OPENCV_LIBS -- �����N����OpenCV���C�u���������X�g
#
# �ȉ��̕ϐ��͖��ݒ�Ȃ�Ί���l���ݒ肳���B
#  - PROJ_BASE_DIR  -- �v���W�F�N�g�̃��[�g�f�B���N�g���p�X
#  - PROJ_SRC_DIRS  -- �\�[�X�̃��[�g�f�B���N�g���p�X�̃��X�g
#  - PROJ_INCLUDES  -- �C���N���[�h�p�X�̃��X�g
#  - OPENCV_VERSION -- OpenCV���C�u�����o�[�W�����l (ex. "248")
#---------------------------------------
macro(setup_cvtool)
    # �K�v�ȕϐ���ݒ�
    set(PROJ_TYPE "program")
    if(NOT PROJ_SRC_DIRS)
        set(PROJ_SRC_DIRS "src")
    endif()
    if(NOT PROJ_INCLUDES)
        set(PROJ_INCLUDES "../common/include")
    endif()
    if(NOT OPENCV_VERSION)
        set(OPENCV_VERSION "${OPENCV_VERSION_DEFAULT}")
    endif()

    # ���ʐݒ�
    setup_common()

    # OpenCV���C�u�����������N
    foreach(CVLIB ${OPENCV_LIBS})
        # ���S���쐬
        set(CVLIB_NAME "opencv_${CVLIB}${OPENCV_VERSION}")
        message("LINK: ${CVLIB_NAME}")

        # �����N���C�u�����ǉ�
        target_link_libraries("${PROJ_NAME}" debug     "${CVLIB_NAME}d")
        target_link_libraries("${PROJ_NAME}" optimized "${CVLIB_NAME}")
    endforeach()

    # common ���C�u�������ˑ��v���W�F�N�g�������N���C�u�����ɒǉ�
    target_link_libraries("${PROJ_NAME}" "common")

    # �C���X�g�[���ݒ�
    install(TARGETS "${PROJ_NAME}" DESTINATION "${PROJ_BASE_DIR}/../../bin")
endmacro()

#===========================================================
cmake_policy(POP)
# EOF
