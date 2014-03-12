cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# CMakeLists.txt for cv_flip
#===========================================================

# 共通モジュール設定
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
include(cvtool)

# ツール名
set(PROJ_NAME "cv_rename")

# OpenCVライブラリ
set(OPENCV_LIBS "core" "highgui")

#===========================================================

# ツール共通設定
message("-- setup : ${PROJ_NAME}")
setup_cvtool()
message("-- setup end")

#===========================================================
# EOF
