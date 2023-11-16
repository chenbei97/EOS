cmake_minimum_required(VERSION 3.20)

set(DLLNAME "interface")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # windows下生成.lib文件
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(TOUPCAM_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/toupcam)
set(TOUPCAM_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/toupcam.lib")

#set(USE_PYTHON 0) # 要和public的use_python一致
#if (${USE_PYTHON})
#set(PYTHON310_PATH ${PROJECT_SOURCE_DIR}/bin/python310)
#set(PYTHON310_LIB_PATH ${PROJECT_SOURCE_DIR}/bin/python310/libs)
#set(PYTHON310_LIB_PATH_python310 ${PROJECT_SOURCE_DIR}/bin/python310/libs/python310.lib)
#set(PYTHON310_LIB_PATH_python310d ${PROJECT_SOURCE_DIR}/bin/python310/libs/python310_d.lib)
#set(PYTHON310_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/bin/python310/include)
#endif ()

set(TOUPCAM_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/toupcam)
set(TOUPCAM_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/toupcam.lib")

#set(USE_OPENCV 1) # 要和window.h的use_opencv2一致,如果引入这里要设置1
#if (${USE_OPENCV})
#set(OPENCV_opencv_world_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_world460d.lib")
#set(OPENCV_img_hash_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_img_hash460d.lib")
#endif ()

add_definitions(-DINTERFACE_LIBRARY=0) # 添加预定义宏,=号不能有空格,用于指定类导出或导入

file(GLOB_RECURSE INTERFACE_HEADERS  "include/*.h")
file(GLOB_RECURSE INTERFACE_SOURCES "src/*.cpp")

#if (${USE_PYTHON})
#link_directories(${PYTHON310_LIB_PATH})# python引入lib只能在这里
#endif ()
#if (${USE_OPENCV})
#link_directories(${OPENCV_opencv_world_LIB_PATH} ${OPENCV_img_hash_LIB_PATH})
#endif ()# opencv 2个都需要指定

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
        ${PROJECT_SOURCE_DIR}/interface/include
        ${PROJECT_SOURCE_DIR}/interface/include/preview
        ${PROJECT_SOURCE_DIR}/interface/include/setting
        ${PROJECT_SOURCE_DIR}/interface/include/mainwindow
        ${TOUPCAM_INCLUDE_PATH}
        )
#if (${USE_PYTHON})
#target_include_directories(${DLLNAME} PUBLIC ${PYTHON310_INCLUDE_PATH})
#endif ()
#if (${USE_OPENCV})
#target_include_directories(${DLLNAME} PUBLIC ${PROJECT_SOURCE_DIR}/interface/include/opencv2)
#endif()

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
        ${TOUPCAM_LIB_PATH}
        )
#if (${USE_PYTHON})
#target_link_libraries(${DLLNAME}
#        ${PYTHON310_LIB_PATH_python310}
#        ${PYTHON310_LIB_PATH_python310d}) # mingw必须显示指定python310,msvc指定与否都可
#endif ()
#if (${USE_OPENCV})
#target_link_libraries(${DLLNAME}
#    ${OPENCV_opencv_world_LIB_PATH}
#    ${OPENCV_img_hash_LIB_PATH}
#)
#endif()
