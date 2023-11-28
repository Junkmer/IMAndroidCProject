//
// Created by EDY on 2022/8/23.
//

#ifndef IMCPLUSPLUSDEMO_BASE_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_BASE_LISTENER_JNI_H

#include <jni.h>
#include <string>
#include <set>
#include <map>
#include "TIMCloud.h"
#include "json.h"
#include "observer_init.h"

namespace tim {
    namespace jni {
        class BaseListenerJni final:public Observer {
        private:
        //实现Observer 纯虚函数
        void initListener() override;

        void unInitListener() override;

        public:
            BaseListenerJni() = default;

            ~BaseListenerJni() = default;

            void AddSDKListener(JNIEnv *env, jobject listener_imsdk, jstring listenerPath);

            void RemoveSDKListener(JNIEnv *env, jstring listenerPath);

            bool Empty();//判断监听是否为空

        private:
            static void ImplTIMNetworkStatusListenerCallback(enum TIMNetworkStatus status, int32_t code, const char* desc, const void* user_data);

            static void ImplTIMKickedOfflineCallback(const void* user_data);

            static void ImplTIMUserSigExpiredCallback(const void* user_data);

            static void ImplTIMSelfInfoUpdatedCallback(const char* json_user_profile, const void* user_data);

            static void ImplTIMUserInfoChangedCallback(const char* json_user_info_array, const void* user_data);

            static void ImplTIMUserStatusChangedCallback(const char* json_user_status_array, const void* user_data);

            static void ImplTIMMsgAllMessageReceiveOptionCallback(const char* json_receive_message_option_info, const void* user_data);

            static void ImplTIMExperimentalNotifyCallback(const char* key, const char* data, const void* user_data);

        private:
            static void CallJavaMethod(const std::map<std::string, jobject> &_listener_,const char *method_name);

            static void CallJavaMethod(const std::map<std::string, jobject> &_listener_,const char *method_name, int code, const char *message);

            static void CallJavaMethod2Data(const std::map<std::string, jobject> &_listener_, const char *method2sig[2], const jobject &data_obj);

        private:
            std::map<std::string, jobject> listener_imsdk_map;//map 容器不允许有重复的key值
        };
    }// namespace tim
}// namespace jni
#endif //IMCPLUSPLUSDEMO_BASE_LISTENER_JNI_H
