cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# cmake module for opencv tool projects
#===========================================================

# 共通モジュールをインクルード
include(common)

#---------------------------------------
# OpenCVライブラリバージョンをチェックする。
#---------------------------------------
macro(check_opencv_version VERSION)
    # バージョンチェック用ソースをコンパイル＆実行
    try_run(
        RUN_RES
        COMPILE_RES
        "${CMAKE_MODULE_PATH}/mod_src"
        "${CMAKE_MODULE_PATH}/mod_src/opencv_version.cpp")

    # エラーチェック
    if(NOT COMPILE_RES)
        message(FATAL_ERROR "Cannot check the OpenCV version. (compile error)")
    endif()
    if("${RUN_RES}" STREQUAL "FAILED_TO_RUN")
        message(FATAL_ERROR "Cannot check the OpenCV version. (runtime error)")
    endif()

    # バージョン設定
    set(${VERSION} "${RUN_RES}")
endmacro()

#---------------------------------------
# ツール作成の共通設定を行う。
# 
# 事前に以下の変数の設定が必要。
#  - PROJ_NAME   -- プロジェクト名
#  - OPENCV_LIBS -- リンクするOpenCVライブラリ名リスト
#
# 以下の変数は未設定ならば既定値が設定される。
#  - PROJ_BASE_DIR  -- プロジェクトのルートディレクトリパス
#  - PROJ_SRC_DIRS  -- ソースのルートディレクトリパスのリスト
#  - PROJ_INCLUDES  -- インクルードパスのリスト
#  - OPENCV_VERSION -- OpenCVライブラリバージョン値 (ex. "248")
#---------------------------------------
macro(setup_cvtool)
    # 必要な変数を設定
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

    # 共通設定
    setup_common()

    # OpenCVライブラリをリンク
    foreach(CVLIB ${OPENCV_LIBS})
        # 完全名作成
        set(CVLIB_NAME "opencv_${CVLIB}${OPENCV_VERSION}")
        message("LINK: ${CVLIB_NAME}")

        # リンクライブラリ追加
        target_link_libraries("${PROJ_NAME}" debug     "${CVLIB_NAME}d")
        target_link_libraries("${PROJ_NAME}" optimized "${CVLIB_NAME}")
    endforeach()

    # common ライブラリを依存プロジェクト＆リンクライブラリに追加
    target_link_libraries("${PROJ_NAME}" "common")

    # インストール設定
    install(TARGETS "${PROJ_NAME}" DESTINATION "${PROJ_BASE_DIR}/../../bin")
endmacro()

#===========================================================
cmake_policy(POP)
# EOF
