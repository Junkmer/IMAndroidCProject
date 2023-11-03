cmake_minimum_required(VERSION 3.18.1)
project("ImCSDKDemo")

set(IM_C_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_PATH ${CMAKE_SOURCE_DIR}/thirdparty)

include(${IM_C_SDK_PATH}/im_c_sdk_jni.cmake)
include(${THIRD_PARTY_PATH}/thirdparty.cmake)

set(IMCSDKJNI_FILES
        ${IM_C_SDK_JNI_SRC}
        ${THIRD_PARTY_SRC}
)

add_library(
        ImCSDKDemo

        SHARED

        ${IMCSDKJNI_FILES}
)

find_library(
        log-lib

        log)

target_link_libraries(
        ImCSDKDemo

        ${log-lib}

        #将第三方库与目标库进行关联
        imcsdk
)