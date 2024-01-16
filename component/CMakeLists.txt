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

set(USE_OPENCV 1) # 要和window.h的use_opencv2一致,如果引入这里要设置1
if (${USE_OPENCV})
#    set(OPENCV_opencv_world_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_world460d.lib")
#    set(OPENCV_img_hash_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_img_hash460d.lib")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(OPENCV_opencv_world_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_world490d.lib")
    set(OPENCV_img_hash_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_img_hash490d.lib")
else ()
    set(OPENCV_opencv_world_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_world490.lib")
    set(OPENCV_img_hash_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_img_hash490.lib")
endif ()

endif ()
if (${USE_OPENCV})
    link_directories(${OPENCV_opencv_world_LIB_PATH} ${OPENCV_img_hash_LIB_PATH})
endif ()# opencv 2个都需要指定

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
        ${COMPONENT_HEADERS})


target_include_directories(${DLLNAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/component/include/widgets
        ${PROJECT_SOURCE_DIR}/component/include/datastructure
        ${PROJECT_SOURCE_DIR}/component/include/tool
        ${PROJECT_SOURCE_DIR}/component/include/socket
        ${PROJECT_SOURCE_DIR}/component/include # 这里要包含,否则main.cpp不能引用目录
        )
if (${USE_OPENCV})
    target_include_directories(${DLLNAME} PUBLIC ${PROJECT_SOURCE_DIR}/component/include/opencv2)
endif()

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
        DbgHelp
        )
if (${USE_OPENCV})
target_link_libraries(${DLLNAME}
        ${OPENCV_opencv_world_LIB_PATH}
        ${OPENCV_img_hash_LIB_PATH}
        )
endif()
