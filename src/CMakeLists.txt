# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (my_svr_util CXX)

# 查找当前目录下的所有源文件
# 并将名称保存到 ALL_SRC_FILE 变量
file(GLOB_RECURSE ALL_SRC_FILE  ${PROJECT_SOURCE_DIR} "*.cc")
file(GLOB_RECURSE ALL_SRC_FILE2  ${PROJECT_SOURCE_DIR} "*.cpp")

# 指定生成目标
ADD_LIBRARY(svr_util STATIC ${ALL_SRC_FILE} ${ALL_SRC_FILE2} )
#cotire(base)
