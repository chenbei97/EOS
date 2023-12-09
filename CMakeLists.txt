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
    set(CMAKE_PREFIX_PATH "E:/Qt5.14.2/5.14.2/msvc2017_64") # 这个目录取决于电脑Qt的安装目录,每台电脑不同,只能使用绝对路径编译
else(GNU)  # 再者,cmake使用绝对路径也不影响打包的结果,cmake不是真实的业务代码只是用于编译,业务代码当然不能使用绝对路径
    set(CMAKE_PREFIX_PATH "E:/Qt5.14.2/5.14.2/mingw73_64") # 这也是为何cmake种我想要把Qt安装目录下的一些文件复制到bin下这样可以使用相对路径,但是我不可能知道Qt安装目录本身
endif () # qmake的编译配置也是用的个人的安装目录,只不过qtcreator帮用户做好了,cmake需要自己修改而已
# E:/Qt5.14.2/5.14.2/msvc2017_64  C:/Qt/Qt5.14.2/5.14.2/msvc2017_64
# E:/Qt5.14.2/5.14.2/mingw73_64 C:/Qt/Qt5.14.2/5.14.2/mingw73_64

# （2）资源文件目录、插件目录
set(QRC_PATH bin/images.qrc bin/app.rc)
#set(THEME_PATH ${PROJECT_SOURCE_DIR}/bin/theme/dark/darkstyle.qrc)

#set(USE_MYSQL 0)
#if (${USE_MYSQL})
#set(MYSQL_INSTALL_PATH "C:/Program Files/MySQL/MySQL Server 8.0")
#set(MYSQL_LIB_PATH "${MYSQL_INSTALL_PATH}/lib/libmysql.lib") # dll必须复制到bin下,lib可以不用
#set(MYSQL_DLL_PATH "${MYSQL_INSTALL_PATH}/lib/libmysql.dll")
#endif ()

#set(USE_OPENCV 0) # interface的cmakelist设置是否引用opencv不影响main.cpp,这里单独去控制
#if (${USE_OPENCV})
#file(GLOB OPENCV_DLLs_PATH  "C:/opencv-4.6.0/x64/vc17/bin/*.dll")
#file(GLOB OPENCV_LIBs_PATH  "C:/opencv-4.6.0/x64/vc17/lib/*.lib")
#set(OPENCV_opencv_world_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_world460d.lib")
#set(OPENCV_img_hash_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/opencv_img_hash460d.lib")
#set(OPENCV_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/interface/opencv2)
#endif ()

set(SQLDRIVERS_PATH "${CMAKE_PREFIX_PATH}/plugins/sqldrivers")
set(IMAGEFORMATS_PATH "${CMAKE_PREFIX_PATH}/plugins/imageformats")
set(MEDIA_PATH "${CMAKE_PREFIX_PATH}/plugins/mediaservice")
set(ICONENGINE_PATH "${CMAKE_PREFIX_PATH}/plugins/iconengines")
set(QML_PATH "${CMAKE_PREFIX_PATH}/qml")
#set(USE_PYTHON 0)
#if (${USE_PYTHON})
#set(PYTHON310_PATH ${PROJECT_SOURCE_DIR}/bin/python310)
#set(PYTHON310_LIB_PATH ${PROJECT_SOURCE_DIR}/bin/python310/libs)
#set(PYTHON310_LIB_PATH_python310 ${PROJECT_SOURCE_DIR}/bin/python310/libs/python310.lib)
#set(PYTHON310_LIB_PATH_python310d ${PROJECT_SOURCE_DIR}/bin/python310/libs/python310_d.lib)
#set(PYTHON310_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/bin/python310/include)
#endif ()

set(TOUPCAM_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/toupcam)
set(TOUPCAM_LIB_PATH "${PROJECT_SOURCE_DIR}/bin/toupcam.lib")

set(OPENGL32_PATH ${CMAKE_PREFIX_PATH}/bin/opengl32sw.dll)
set(D3DCOMPILER_PATH ${CMAKE_PREFIX_PATH}/bin/d3dcompiler_47.dll)
set(LIBEGL_PATH ${CMAKE_PREFIX_PATH}/bin/libEGL.dll)
set(LIBGLESV2_PATH ${CMAKE_PREFIX_PATH}/bin/libGLESv2.dll)
set(LIBEGLD_PATH ${CMAKE_PREFIX_PATH}/bin/libEGLd.dll)
set(LIBGLESV2D_PATH ${CMAKE_PREFIX_PATH}/bin/libGLESv2d.dll)

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
        ${PROJECT_SOURCE_DIR}/interface/include/setting)

file(GLOB MAIN_FILES  "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB TESTFILES  "${PROJECT_SOURCE_DIR}/test/*.h" "${PROJECT_SOURCE_DIR}/test/*.cpp")

# （3）2个子目录
add_subdirectory(component)
add_subdirectory(interface)

#（4）链接的库目录 必须add_executable之前

#set(A1 ${PROJECT_SOURCE_DIR}/bin/swscale-lav-3.dll)
#set(A2 ${PROJECT_SOURCE_DIR}/bin/libbluray.dll)
#set(A3 ${PROJECT_SOURCE_DIR}/bin/IntelQuickSyncDecoder.dll)
#set(A4 ${PROJECT_SOURCE_DIR}/bin/swscale-lav-3.dll)
#set(A5 ${PROJECT_SOURCE_DIR}/bin/avutil-lav-54.dll)
#set(A6 ${PROJECT_SOURCE_DIR}/bin/avresample-lav-2.dll)
#set(A7 ${PROJECT_SOURCE_DIR}/bin/avformat-lav-56.dll)
#set(A8 ${PROJECT_SOURCE_DIR}/bin/avfilter-lav-5.dll)
#set(A9 ${PROJECT_SOURCE_DIR}/bin/avcodec-lav-56.dll)


link_directories(${TOUPCAM_LIB_PATH} ${PROJECT_SOURCE_DIR}/lib
        #${A1}${A2}${A3}${A4}${A5}${A6}${A7}${A8}${A9}
        )
#if (${USE_PYTHON})
#    link_directories(${PYTHON310_LIB_PATH})
#endif ()
# ${PYTHON310_LIB_PATH}只能在这里,不能在target_link_libraries
# ${OPENCV_LIBs_PATH}不能放link_directories 只能在target_link_libraries, ${TOUPCAM_LIB_PATH}测试是2个都可以放

# （5）查找包 // C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\include
find_package(Qt5 COMPONENTS Core Gui Widgets Sql Network
        Qml QuickWidgets Quick QmlModels QuickControls2 QuickTemplates2 QmlWorkerScript
        Concurrent Multimedia MultimediaWidgets OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${MAIN_FILES} ${QRC_PATH} ${TESTFILES}) # ${THEME_PATH}

#（6）包含头文件目录，必须add_executable之后
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPONENT_INCLUDE_PATH} ${INTERFACE_INCLUDE_PATH}
          ${TOUPCAM_INCLUDE_PATH})
#if (${USE_PYTHON})
#target_include_directories(${PROJECT_NAME} PUBLIC ${PYTHON310_INCLUDE_PATH} ${PYTHON310_PATH})
#endif ()
#if (${USE_OPENCV})
#target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INCLUDE_PATH})
#endif ()
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
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/iconengines"
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/qml"
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
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ICONENGINE_PATH}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/iconengines"
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${QML_PATH}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/qml"
        )
# 3. mysql软件的2个文件必须复制
#if (${USE_MYSQL})
#if (EXISTS "${MYSQL_LIB_PATH}")
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy
#            "${MYSQL_LIB_PATH}"
#            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
#endif ()
#if (EXISTS "${MYSQL_DLL_PATH}")
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy
#            "${MYSQL_DLL_PATH}"
#            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
#endif ()
#endif ()
# 4. 同理opencv的进行复制 OPENCV_DLL_PATH
#if (${USE_OPENCV})
#foreach(opencv_lib ${OPENCV_LIBs_PATH})
#    if (EXISTS "${opencv_lib}")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${opencv_lib}"
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
#    endif ()
#endforeach(opencv_lib)
#
#foreach(opencv_dll ${OPENCV_DLLs_PATH}) # 必须复制dll不然无法单独运行
#    if (EXISTS "${opencv_dll}")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${opencv_dll}"
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
#    endif ()
#endforeach(opencv_dll)
#endif ()

# 5. 其它依赖dll
if (EXISTS "${OPENGL32_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${OPENGL32_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
if (EXISTS "${D3DCOMPILER_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${D3DCOMPILER_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
if (EXISTS "${LIBEGL_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${LIBEGL_PATH}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
if (EXISTS "${LIBEGLD_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${LIBEGLD_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
if (EXISTS "${LIBGLESV2_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${LIBGLESV2_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
if (EXISTS "${LIBGLESV2D_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${LIBGLESV2D_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
endif ()
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
        Qt5::QuickTemplates2 Qt5::Qml Qt5::QuickWidgets Qt5::Quick Qt5::QmlModels Qt5::QuickControls2 Qt5::QmlWorkerScript
        component interface  # mingw必须显示指定python310,msvc指定与否都可
        ${TOUPCAM_LIB_PATH}# opencv显示连接libs就可以,不需要dll,打赏dll文件需要放在bin下

        )
#if (${USE_PYTHON})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${PYTHON310_LIB_PATH_python310} ${PYTHON310_LIB_PATH_python310d})
#endif ()
#if (${USE_OPENCV})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENCV_opencv_world_LIB_PATH} ${OPENCV_img_hash_LIB_PATH})
# # bin下已经放好了2个文件 ${OPENCV_LIBs_PATH},所以可以不使用OPENCV_LIBs_PATH
#endif ()

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
    foreach (QT_LIB Core Gui Widgets Sql Network
            Qml QuickWidgets Quick QmlModels QuickControls2 QuickTemplates2 QmlWorkerScript
            Concurrent Multimedia MultimediaWidgets OpenGl)
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