cmake_minimum_required(VERSION 3.20)

set(DLLNAME "interface")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # windows下生成.lib文件
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_definitions(-DINTERFACE_LIBRARY=0) # 添加预定义宏,=号不能有空格,用于指定类导出或导入

file(GLOB_RECURSE INTERFACE_HEADERS  "include/*.h")
file(GLOB_RECURSE INTERFACE_SOURCES "src/*.cpp")

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
        ${INTERFACE_SOURCES}
        ${INTERFACE_HEADERS})
set_target_properties(${DLLNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${DLLNAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/component/include
        ${PROJECT_SOURCE_DIR}/interface/include/preview
        ${PROJECT_SOURCE_DIR}/interface/include/setting
        ${PROJECT_SOURCE_DIR}/interface/include/mainwindow
        ${PROJECT_SOURCE_DIR}/interface/include
        )

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
        component
        )
