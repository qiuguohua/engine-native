# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(XComponent)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/common
                    ${NATIVERENDER_ROOT_PATH}/napi
                    ${NATIVERENDER_ROOT_PATH}/render
                    ${NATIVERENDER_ROOT_PATH}/add
                    ${NATIVERENDER_ROOT_PATH}/include
                    ${NATIVERENDER_ROOT_PATH}/include/uv
                    )

add_library(nativerender SHARED
            render/egl_core.cpp
            render/plugin_render.cpp
            plugin_manager.cpp
            napi/napi_init.cpp
            message_queue.cpp
            )
add_library(uv STATIC IMPORTED GLOBAL)
set_target_properties(uv PROPERTIES
    IMPORTED_LOCATION ${NATIVERENDER_ROOT_PATH}/../../../libs/arm64-v8a/libuv_a.a
)

add_library(hello SHARED add/hello.cpp)
target_link_libraries(hello PUBLIC uv libhilog_ndk.z.so libace_napi.z.so)

find_library( # Sets the name of the path variable.
              EGL-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              EGL )
find_library( # Sets the name of the path variable.
              GLESv3-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              GLESv3 )

find_library( hilog-lib
              hilog_ndk.z )

find_library( libace-lib
              libace_ndk.z )

find_library( # Sets the name of the path variable.
              zgraphic.z-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              zgraphic.z )

target_link_libraries(nativerender PUBLIC uv ${EGL-lib} ${GLESv3-lib} ${hilog-lib} ${zgraphic.z-lib} libace_napi.z.so libace_ndk.z.so libc++.a)