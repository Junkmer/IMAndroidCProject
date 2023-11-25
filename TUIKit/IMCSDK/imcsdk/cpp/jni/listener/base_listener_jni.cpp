//
// Created by EDY on 2022/8/23.
//

#include <iostream>
#include "base_listener_jni.h"

#include "../basic/jni_util.h"
#include "../basic/jni_helper.h"
#include "../util/LogUtil.h"
#include "../engine/tim_engine.h"
#include "java_basic_jni.h"
#include "user_full_info_jni.h"
#include "user_status_jni.h"
#include "receive_message_opt_info_jni.h"

namespace tim {
    namespace jni {

        void BaseListenerJni::initListener() {
            TIMSetNetworkStatusListenerCallback(ImplTIMNetworkStatusListenerCallback, &listener_imsdk_map);
            TIMSetKickedOfflineCallback(ImplTIMKickedOfflineCallback, &listener_imsdk_map);
            TIMSetUserSigExpiredCallback(ImplTIMUserSigExpiredCallback, &listener_imsdk_map);
            TIMSetSelfInfoUpdatedCallback(ImplTIMSelfInfoUpdatedCallback, &listener_imsdk_map);
            TIMSetUserInfoChangedCallback(ImplTIMUserInfoChangedCallback, &listener_imsdk_map);
            TIMSetUserStatusChangedCallback(ImplTIMUserStatusChangedCallback, &listener_imsdk_map);
            TIMSetMsgAllMessageReceiveOptionCallback(ImplTIMMsgAllMessageReceiveOptionCallback, &listener_imsdk_map);
            TIMSetExperimentalNotifyCallback(ImplTIMExperimentalNotifyCallback, &listener_imsdk_map);
        }

        void BaseListenerJni::unInitListener() {
            TIMSetNetworkStatusListenerCallback(nullptr, nullptr);
            TIMSetKickedOfflineCallback(nullptr, nullptr);
            TIMSetUserSigExpiredCallback(nullptr, nullptr);
            TIMSetSelfInfoUpdatedCallback(nullptr, nullptr);
            TIMSetUserInfoChangedCallback(nullptr, nullptr);
            TIMSetUserStatusChangedCallback(nullptr, nullptr);
            TIMSetMsgAllMessageReceiveOptionCallback(nullptr, nullptr);
            TIMSetExperimentalNotifyCallback(nullptr, nullptr);
        }

        void BaseListenerJni::AddSDKListener(JNIEnv *env, jobject listener_imsdk, jstring listenerPath) {
            if (nullptr == listener_imsdk) {
                LOGE("addSDKListener | listener_imsdk is null");
                return;
            }
            if (listener_imsdk_map.empty()) {
                tim::ObserverManager::getInstance().addListener(this);
            }

            std::string path = StringJni::Jstring2Cstring(env, listenerPath);
            for (auto &item: listener_imsdk_map) {
                if (path.empty() || path == item.first) {
                    return;
                }
            }

            jobject jni_object = env->NewGlobalRef(listener_imsdk);
            listener_imsdk_map.insert(std::make_pair(path, jni_object));
        }

        void BaseListenerJni::RemoveSDKListener(JNIEnv *env, jstring listenerPath) {
            listener_imsdk_map.erase(StringJni::Jstring2Cstring(env, listenerPath));
            if (listener_imsdk_map.empty()) {
                tim::ObserverManager::getInstance().removeListener(this);
            }
        }

        bool BaseListenerJni::Empty() {
            return listener_imsdk_map.empty();
        }

        void
        BaseListenerJni::ImplTIMNetworkStatusListenerCallback(enum TIMNetworkStatus status, int32_t code, const char *desc, const void *user_data) {
            auto _listener_ = (std::map<std::string, jobject> *) user_data;
            switch (status) {
                case TIMNetworkStatus::kTIMConnecting:
                    CallJavaMethod(*_listener_, "onConnecting");
                    break;
                case TIMNetworkStatus::kTIMConnected:
                    CallJavaMethod(*_listener_, "onConnectSuccess");
                    break;
                case TIMNetworkStatus::kTIMConnectFailed:
                case TIMNetworkStatus::kTIMDisconnected:
                    CallJavaMethod(*_listener_, "onConnectFailed", code, desc);
                    break;

            }
        }

        void BaseListenerJni::ImplTIMKickedOfflineCallback(const void *user_data) {
            CallJavaMethod(*(std::map<std::string, jobject> *) user_data, "onKickedOffline");
        }

        void BaseListenerJni::ImplTIMUserSigExpiredCallback(const void *user_data) {
            CallJavaMethod(*(std::map<std::string, jobject> *) user_data, "onUserSigExpired");
        }

        void BaseListenerJni::ImplTIMSelfInfoUpdatedCallback(const char *json_user_profile, const void *user_data) {
            const char *method2sig[] = {"onSelfInfoUpdated", "(Lcom/tencent/imsdk/v2/V2TIMUserFullInfo;)V"};
            jobject j_obj_info = UserFullInfoJni::Convert2JObject(json::Deserialize(json_user_profile));
            CallJavaMethod2Data(*(std::map<std::string, jobject> *) user_data, method2sig, j_obj_info);
        }

        void BaseListenerJni::ImplTIMUserInfoChangedCallback(const char *json_user_info_array, const void *user_data) {
            const char *method2sig[] = {"onUserInfoChanged", "(Ljava/util/List;)V"};
            jobject infoList = ArrayListJni::NewArrayList();
            json::Array user_info_array = json::Deserialize(json_user_info_array);
            for (const auto &item : user_info_array){
                jobject j_obj_info = UserFullInfoJni::Convert2JObject(item);
                ArrayListJni::Add(infoList,j_obj_info);

                ScopedJEnv scopedJEnv;
                auto *env = scopedJEnv.GetEnv();
                env->DeleteLocalRef(j_obj_info);
            }
            CallJavaMethod2Data(*(std::map<std::string, jobject> *) user_data, method2sig, infoList);
        }

        void BaseListenerJni::ImplTIMUserStatusChangedCallback(const char *json_user_status_array, const void *user_data) {
            const char *method2sig[] = {"onUserStatusChanged", "(Ljava/util/List;)V"};
            jobject userStatusList = tim::jni::ArrayListJni::NewArrayList();
            json::Array status_array = json::Deserialize(json_user_status_array);
            for (const auto & i : status_array) {
                jobject userStatus = tim::jni::UserStatusJni::Convert2JObject(i);
                tim::jni::ArrayListJni::Add(userStatusList, userStatus);

                ScopedJEnv scopedJEnv;
                auto *env = scopedJEnv.GetEnv();
                env->DeleteLocalRef(userStatus);
            }
            CallJavaMethod2Data(*(std::map<std::string, jobject> *) user_data, method2sig, userStatusList);
        }

        void BaseListenerJni::ImplTIMMsgAllMessageReceiveOptionCallback(const char* json_receive_message_option_info, const void* user_data) {
            const char *method2sig[] = {"onAllReceiveMessageOptChanged", "(Lcom/tencent/imsdk/v2/V2TIMReceiveMessageOptInfo;)V"};
            jobject j_obj_info = ReceiveMessageOptInfoJni::Convert2JObject_AllRecvMsg(json::Deserialize(json_receive_message_option_info));
            CallJavaMethod2Data(*(std::map<std::string, jobject> *) user_data, method2sig, j_obj_info);
        }

        void BaseListenerJni::ImplTIMExperimentalNotifyCallback(const char* key, const char* data, const void* user_data) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();
            const std::map<std::string, jobject> _listener_ = *(std::map<std::string, jobject> *) user_data;

            std::string keyStr = key;
            jstring key_jStr = StringJni::Cstring2Jstring(env,key);
            for (auto &item: _listener_) {
                jclass j_cls = env->GetObjectClass(item.second);
                if (nullptr == j_cls) {
                    LOGE("get object class faild");
                }

                jmethodID method = env->GetMethodID(j_cls, "onExperimentalNotify", "((Ljava/lang/String;Ljava/lang/Object;)V");
                if (nullptr != method) {
                    env->CallVoidMethod(item.second, method,key_jStr, nullptr);
                } else {
                    LOGE("get onExperimentalNotify method failed");
                    return;
                }
                env->DeleteLocalRef(j_cls);
            }
        }

        void BaseListenerJni::CallJavaMethod(const std::map<std::string, jobject> &_listener_, const char *method_name) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();
            if (nullptr == env || scopedJEnv.CheckAndClearException()) {
                return;
            }

            if (nullptr == method_name) {
                LOGE("invalid method_name");
                return;
            }

            if (_listener_.empty()) {
                LOGE("listener_imsdk_list is empty");
                return;
            }

            for (auto &item: _listener_) {
                jclass j_cls = env->GetObjectClass(item.second);
                if (nullptr == j_cls) {
                    LOGE("get object class faild");
                }

                jmethodID method = env->GetMethodID(j_cls, method_name, "()V");
                if (nullptr != method) {
                    env->CallVoidMethod(item.second, method);
                } else {
                    LOGE("get %s method failed", method_name);
                    return;
                }
                env->DeleteLocalRef(j_cls);
            }
        }

        void
        BaseListenerJni::CallJavaMethod(const std::map<std::string, jobject> &_listener_, const char *method_name, int code, const char *message) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (nullptr == env || scopedJEnv.CheckAndClearException()) {
                return;
            }

            if (nullptr == method_name) {
                LOGE("invalid method_name");
                return;
            }

            if (_listener_.empty()) {
                LOGE("listener_imsdk_list is empty");
                return;
            }

            for (auto &item: _listener_) {
                jclass j_cls = env->GetObjectClass(item.second);
                if (nullptr == j_cls) {
                    LOGE("get object class failed");
                    return;
                }

                jmethodID method = env->GetMethodID(j_cls, method_name, "(ILjava/lang/String;)V");
                if (nullptr != method) {
                    env->CallVoidMethod(item.second, method, code, StringJni::charToJstring(env, message));
                } else {
                    LOGE("get %s method failed", method_name);
                    return;
                }
                env->DeleteLocalRef(j_cls);
            }
        }

        void BaseListenerJni::CallJavaMethod2Data(const std::map<std::string, jobject> &_listener_, const char *method2sig[2],
                                                  const jobject &data_obj) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (nullptr == env || scopedJEnv.CheckAndClearException()) {
                return;
            }

            if (nullptr == method2sig) {
                LOGE("invalid method_name or sig");
                return;
            }

            if (_listener_.empty()) {
                LOGE("listener_imsdk_list is empty");
                return;
            }

            for (auto &item: _listener_) {
                jclass j_cls = env->GetObjectClass(item.second);
                if (nullptr == j_cls) {
                    LOGE("get object class failed");
                    return;
                }

                jmethodID method = env->GetMethodID(j_cls, method2sig[0], method2sig[1]);
                if (nullptr != method) {
                    env->CallVoidMethod(item.second, method, data_obj);
                } else {
                    LOGE("get %s method failed", method2sig[0]);
                    return;
                }
                env->DeleteLocalRef(j_cls);
            }
            env->DeleteLocalRef(data_obj);
        }

    }
}
