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

# ソースのルートディレクトリパス
set(PROJ_SRC_DIRS "src" "include")

# インクルードパス
set(PROJ_INCLUDES "include" "src/include")

#===========================================================

# 共通設定
message("-- setup : ${PROJ_NAME}")
setup_common()
message("-- setup end")

#===========================================================
# EOF
