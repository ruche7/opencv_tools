cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# cmake module for opencv tool projects
#===========================================================

# ���ʃ��W���[�����C���N���[�h
include(common)

#---------------------------------------
# OpenCV���C�u�����o�[�W�������`�F�b�N����B
#---------------------------------------
macro(check_opencv_version VERSION)
    # �o�[�W�����`�F�b�N�p�\�[�X���R���p�C�������s
    try_run(
        RUN_RES
        COMPILE_RES
        "${CMAKE_MODULE_PATH}/mod_src"
        "${CMAKE_MODULE_PATH}/mod_src/opencv_version.cpp")

    # �G���[�`�F�b�N
    if(NOT COMPILE_RES)
        message(FATAL_ERROR "Cannot check the OpenCV version. (compile error)")
    endif()
    if("${RUN_RES}" STREQUAL "FAILED_TO_RUN")
        message(FATAL_ERROR "Cannot check the OpenCV version. (runtime error)")
    endif()

    # �o�[�W�����ݒ�
    set(${VERSION} "${RUN_RES}")
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
        check_opencv_version(OPENCV_VERSION)
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
