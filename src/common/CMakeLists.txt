cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# CMakeLists.txt for common library
#===========================================================

# 共通モジュール設定
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
include(common)

# ライブラリ名
set(PROJ_NAME "common")

# ライブラリ種別
set(PROJ_TYPE "static")

# ベースディレクトリパス
set(PROJ_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# ソースディレクトリ
set(PROJ_SRC_DIRS
    "src"
    "src/utility"
    "include"
    "include/cvtool"
    "include/cvtool/utility")

# インクルードパス
set(PROJ_INCLUDES "include" "src/include")

#===========================================================

# 共通設定
setup_common()

#===========================================================
# EOF
