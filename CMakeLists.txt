cmake_minimum_required(VERSION 2.8.12)

project(FrpApp)
# 导入头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
# 指定第三方库
link_libraries(curl)
link_libraries(json-c)
# 指定库路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
# 指定源文件
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")
# 添加可执行文件
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# 拷贝到bin目录
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    ${PROJECT_SOURCE_DIR}/bin/
)