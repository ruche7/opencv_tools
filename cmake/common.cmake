cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# cmake common module
#===========================================================

#---------------------------------------
# 定数定義
#---------------------------------------

# プロジェクトに含めるファイルの拡張子
set(SRC_EXTENDS   "c;cc;cpp;cxx")
set(EXTRA_EXTENDS "h;hpp;hxx;inc;def")

# コンパイラに追加設定するオプション
if(MSVC)
    # boost のコードで出てしまうW4警告は無視
    set(ADDITIONAL_COMPILE_FLAGS "/MP /W4 /WX /wd4505 /wd4512")
    set(ADDITIONAL_COMPILE_FLAGS_RELEASE "/Ox /Og /Oi /Ot /Oy")
else()
    set(ADDITIONAL_COMPILE_FLAGS "")
    set(ADDITIONAL_COMPILE_FLAGS_RELEASE "")
endif()

# OpenCVライブラリバージョン既定値
set(OPENCV_VERSION_DEFAULT "248")

#---------------------------------------
# 変数名とその内容を表示する。
#---------------------------------------
macro(show_variable V)
    message("${V}=${${V}}")
endmacro()

#---------------------------------------
# VC++ビルド構成の設定を行う。
# project コマンドよりも前に呼び出すこと。
#---------------------------------------
macro(setup_config_types)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endmacro()

#---------------------------------------
# 共通設定を行う。
# 
# 事前に以下の変数の設定が必要。
#  - PROJ_NAME     -- プロジェクト名
#  - PROJ_TYPE     -- プロジェクトの種別
#  - PROJ_BASE_DIR -- プロジェクトのルートディレクトリパス
#  - PROJ_SRC_DIRS -- ソースディレクトリパスのリスト
#  - PROJ_INCLUDES -- インクルードパスのリスト
#
# PROJ_TYPE には下記のいずれかの値を指定する。
# 
#  - "program" -- 実行プログラム(既定値)
#  - "static"  -- スタティックライブラリ
#  - "shared"  -- 共有ライブラリ(DLL)
#---------------------------------------
macro(setup_common)
    message("---- setup : ${PROJ_NAME}")

    # ビルド構成設定
    setup_config_types()

    # サフィックス設定
    set(CMAKE_DEBUG_POSTFIX "_d")

    # プロジェクト設定
    project("${PROJ_NAME}")

    # ソースファイルリスト作成
    set(SRC_FILES "")
    foreach(SRC_DIR ${PROJ_SRC_DIRS})
        # ファイルをリストアップ
        set(FILES_ON_DIR "")

        # ソース以外のファイルを先に追加
        foreach(EXTRA_EXT ${EXTRA_EXTENDS})
            file(GLOB TEMP_SRC_FILES
                "${PROJ_BASE_DIR}/${SRC_DIR}/*.${EXTRA_EXT}")
            list(APPEND FILES_ON_DIR ${TEMP_SRC_FILES})
        endforeach()

        # コンパイル対象外のプロジェクトアイテムにする
        set_source_files_properties(${FILES_ON_DIR} PROPERTIES HEADER_FILE_ONLY on)

        # ソースファイルを追加
        foreach(SRC_EXT ${SRC_EXTENDS})
            file(GLOB TEMP_SRC_FILES
                "${PROJ_BASE_DIR}/${SRC_DIR}/*.${SRC_EXT}")
            list(APPEND FILES_ON_DIR ${TEMP_SRC_FILES})
        endforeach()

        # ファイルの属するプロジェクト階層を設定(VC++用)
        string(REPLACE "/" "\\" PROJ_DIR "${SRC_DIR}")
        source_group("${PROJ_DIR}" FILES ${FILES_ON_DIR})

        # ファイルリストに追加
        list(APPEND SRC_FILES ${FILES_ON_DIR})
    endforeach()

    # ソースファイル名出力
    foreach(SRC_FILE ${SRC_FILES})
        message("INPUT: ${SRC_FILE}")
    endforeach()

    # 作成設定
    if("${PROJ_TYPE}" STREQUAL "static")
        add_library("${PROJ_NAME}" STATIC ${SRC_FILES})
    elseif("${PROJ_TYPE}" STREQUAL "shared")
        add_library("${PROJ_NAME}" SHARED ${SRC_FILES})
    else()
        add_executable("${PROJ_NAME}" ${SRC_FILES})
    endif()

    # インクルードパス追加
    foreach(INC_PATH ${PROJ_INCLUDES})
        include_directories("${PROJ_BASE_DIR}/${INC_PATH}")
    endforeach()

    # コンパイルオプションの設定
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
# ツール作成の共通設定を行う。
# 
# 事前に以下の変数の設定が必要。
#  - PROJ_NAME      -- プロジェクト名
#  - OPENCV_LIBS    -- リンクするOpenCVライブラリ名リスト
#
# 以下の変数は未設定ならば既定値が設定される。
#  - PROJ_BASE_DIR  -- プロジェクトのルートディレクトリパス
#  - PROJ_SRC_DIRS  -- ソースディレクトリパスのリスト
#  - PROJ_INCLUDES  -- インクルードパスのリスト
#  - OPENCV_VERSION -- OpenCVライブラリバージョン値 (ex. "248")
#---------------------------------------
macro(setup_cvtool)
    # 必要な変数を設定
    set(PROJ_TYPE "program")
    if(NOT PROJ_BASE_DIR)
        set(PROJ_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
    if(NOT PROJ_SRC_DIRS)
        set(PROJ_SRC_DIRS "src")
    endif()
    if(NOT PROJ_INCLUDES)
        set(PROJ_INCLUDES "../common/include")
    endif()
    if(NOT OPENCV_VERSION)
        set(OPENCV_VERSION "${OPENCV_VERSION_DEFAULT}")
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
