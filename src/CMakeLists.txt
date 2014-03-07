cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# CMakeLists.txt for opencv_tools
#===========================================================

# 共通モジュール設定
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(common)

# ビルド構成設定
setup_config_types()

# プロジェクト名設定
project("opencv_tools")

# 共通ライブラリ追加
add_subdirectory("common")

# ツール群追加
add_subdirectory("cv_flip")

#===========================================================
# EOF
