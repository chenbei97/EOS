cmake_minimum_required(VERSION 3.20)

project(EOSI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

message(STATUS "FAMILYPC = $ENV{MY_ENV_VAR}")
# （1）设置中文
if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(CMAKE_PREFIX_PATH "E:/Qt5.14.2/5.14.2/msvc2017_64")
else(GNU)
    set(CMAKE_PREFIX_PATH "E:/Qt5.14.2/5.14.2/mingw73_64")
endif ()
# E:/Qt5.14.2/5.14.2/msvc2017_64  C:/Qt/Qt5.14.2/5.14.2/msvc2017_64
# E:/Qt5.14.2/5.14.2/mingw73_64 C:/Qt/Qt5.14.2/5.14.2/mingw73_64

# （2）资源文件目录、插件目录
set(QRC_PATH bin/images.qrc bin/app.rc)
#set(THEME_PATH ${PROJECT_SOURCE_DIR}/bin/theme/dark/darkstyle.qrc)

set(MYSQL_INSTALL_PATH "C:/Program Files/MySQL/MySQL Server 8.0")
set(MYSQL_LIB_PATH "${MYSQL_INSTALL_PATH}/lib/libmysql.lib") # dll必须复制到bin下,lib可以不用
set(MYSQL_DLL_PATH "${MYSQL_INSTALL_PATH}/lib/libmysql.dll")

#set(OpenCV_DIR "C:/opencv-4.6.0")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
file(GLOB OPENCV_DLLs_PATH  "C:/opencv-4.6.0/x64/vc17/bin/*.dll")
file(GLOB OPENCV_LIBs_PATH  "C:/opencv-4.6.0/x64/vc17/lib/*.lib")
set(OPENCV_opencv_world_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_world460d.lib")
set(OPENCV_img_hash_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_img_hash460d.lib")
set(OPENCV_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/opencv2)

set(SQLDRIVERS_PATH "${CMAKE_PREFIX_PATH}/plugins/sqldrivers")
set(IMAGEFORMATS_PATH "${CMAKE_PREFIX_PATH}/plugins/imageformats")
set(MEDIA_PATH "${CMAKE_PREFIX_PATH}/plugins/mediaservice")

set(PYTHON310_PATH ${PROJECT_SOURCE_DIR}/bin/python310)
set(PYTHON310_LIB_PATH ${PROJECT_SOURCE_DIR}/bin/python310/libs)
set(PYTHON310_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/bin/python310/include)

set(COMPONENT_INCLUDE_PATH
        ${PROJECT_SOURCE_DIR}/component/include
        ${PROJECT_SOURCE_DIR}/component/include/widgets
        ${PROJECT_SOURCE_DIR}/component/include/datastructure
        ${PROJECT_SOURCE_DIR}/component/include/tool
        ${PROJECT_SOURCE_DIR}/component/include/socket)

set(INTERFACE_INCLUDE_PATH
        ${PROJECT_SOURCE_DIR}/interface/include
        ${PROJECT_SOURCE_DIR}/interface/include/mainwindow
        ${PROJECT_SOURCE_DIR}/interface/include/preview
        ${PROJECT_SOURCE_DIR}/interface/include/protocol)

file(GLOB MAIN_FILES  "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB TESTFILES  "${PROJECT_SOURCE_DIR}/test/*.h" "${PROJECT_SOURCE_DIR}/test/*.cpp")
# （3）2个子目录
add_subdirectory(component)
add_subdirectory(interface)
#（4）链接的库目录 必须add_executable之前
link_directories(${PYTHON310_LIB_PATH} ${PROJECT_SOURCE_DIR}/lib)

# （5）查找包 // C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\include
find_package(Qt5 COMPONENTS Core Gui Widgets Sql Network
        Concurrent Multimedia MultimediaWidgets OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${MAIN_FILES} ${QRC_PATH} ${TESTFILES}) # ${THEME_PATH}

#（6）包含头文件目录，必须add_executable之后
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPONENT_INCLUDE_PATH} ${INTERFACE_INCLUDE_PATH}
        ${PYTHON310_INCLUDE_PATH} ${PYTHON310_PATH} ${OPENCV_INCLUDE_PATH})
# （7）清理4个文件夹并重新复制
# 1. remove_directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/sqldrivers"
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats"
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/mediaservice"
        )
# 2. copy_directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SQLDRIVERS_PATH}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/sqldrivers"
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${IMAGEFORMATS_PATH}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats"
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${MEDIA_PATH}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/mediaservice"
        )
# 3. mysql软件的2个文件必须复制
if (EXISTS "${MYSQL_LIB_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${MYSQL_LIB_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
if (EXISTS "${MYSQL_DLL_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${MYSQL_DLL_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
# 4. 同理opencv的进行复制 OPENCV_DLL_PATH
foreach(opencv_lib ${OPENCV_LIBs_PATH})
    if (EXISTS "${opencv_lib}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${opencv_lib}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
    endif ()
endforeach(opencv_lib)

foreach(opencv_dll ${OPENCV_DLLs_PATH}) # 必须复制dll不然无法单独运行
    if (EXISTS "${opencv_dll}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${opencv_dll}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
    endif ()
endforeach(opencv_dll)

# （8）清理lib、bin不必要的上次编译生成的文件
#file(GLOB lastFiles
#        "bin/*.dll"
#        "bin/*.exe"
#        "bin/*.exp"
#        "bin/*.pdb"
#        "bin/*.ilk"
#        "bin/*.lib"
#        "lib/*.lib"
#        "lib/*.exp"
#        "lib/*.dll.a"
#        )
#foreach(file ${lastFiles})
#    file(REMOVE ${file})
#endforeach(file)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql Qt5::Network Qt5::Concurrent
        Qt5::Multimedia Qt5::MultimediaWidgets Qt5::OpenGL
        component interface python310  # mingw必须显示指定python310,msvc指定与否都可
        ${OPENCV_LIBs_PATH} # ${OPENCV_opencv_world_LIB_PATH} ${OPENCV_img_hash_LIB_PATH} #不行,还没复制这 ${OPENCV_LIBs_PATH}
        ) # opencv连接libs就可以,不需要dll


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets Sql Network Concurrent Multimedia MultimediaWidgets OpenGl)
        if(MSVC)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"# bin/Qt5Cored.dll拷贝进来
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>") # 拷贝到bin目录
        else()
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different # mingw没有尾缀d
                    "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}.dll"# bin/Qt5Core.dll拷贝进来
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>") # 拷贝到bin目录
        endif ()
    endforeach (QT_LIB)
endif ()