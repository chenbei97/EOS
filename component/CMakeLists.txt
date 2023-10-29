cmake_minimum_required(VERSION 3.20)

set(DLLNAME "component")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # windows下生成.lib文件
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_definitions(-DCOMPONENT_LIBRARY=0) # 添加预定义宏,=号不能有空格,用于指定类导出或导入

file(GLOB_RECURSE COMPONENT_HEADERS "include/*.h")
file(GLOB_RECURSE COMPONENT_SOURCES "src/*.cpp")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Network
        Concurrent
        Multimedia
        MultimediaWidgets
        OpenGL
        REQUIRED)

add_library(${DLLNAME} SHARED
        ${COMPONENT_SOURCES}
        ${COMPONENT_HEADERS} include/widgets/inscribedrect.h src/widgets/inscribedrect.cpp)

target_link_libraries(${DLLNAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
        Qt5::Network
        Qt5::Concurrent
        Qt5::Multimedia
        Qt5::MultimediaWidgets
        Qt5::OpenGL
        )

target_include_directories(${DLLNAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/component/include/widgets
        ${PROJECT_SOURCE_DIR}/component/include/datastructure
        ${PROJECT_SOURCE_DIR}/component/include/tool
        ${PROJECT_SOURCE_DIR}/component/include/socket
#        ${PROJECT_SOURCE_DIR}/component/inc/video
        ${PROJECT_SOURCE_DIR}/component/include # 这里要包含,否则main.cpp不能引用目录
        )