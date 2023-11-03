//
// Created by Junker on 2022/8/22.
//

#include "sdk_config_jni.h"
#include <string>
#include "jni_util.h"
#include "json.h"
#include "TIMCloud.h"

namespace tim {
    namespace jni {

        jclass SDKConfigJni::jcls_;
        jmethodID SDKConfigJni::j_method_id_array_[MethodIDMax];

        bool SDKConfigJni::InitIDs(JNIEnv *env) {
            if (jcls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMSDKConfig");

            if (nullptr == cls) {
                return false;
            }

            jcls_ = (jclass) env->NewGlobalRef(cls);

            env->DeleteLocalRef(cls);

            jmethodID j_mod = nullptr;
            j_mod = env->GetMethodID(jcls_, "<init>", "()V");
            if (j_mod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = j_mod;
            return true;
        }

        std::string SDKConfigJni::ConvertToCoreObject(JNIEnv *env, const jobject &object) {
            jclass clazz = env->GetObjectClass(object);

//            config.initPath = "/data/user/0/com.junker.im.ndk.im.ndk.demo/files";
//            config.logPath = "/storage/emulated/0/Android/data/com.junker.im.ndk.im.ndk.demo/files/log/tencent/imsdk/";

            json::Object filter;
            filter[kTIMSdkConfigLogFilePath] = util::getStringValue(env, clazz, object, "sdkInitPath");
            filter[kTIMSdkConfigConfigFilePath] = util::getStringValue(env, clazz, object, "sdkLogPath");
            env->DeleteLocalRef(clazz);

            return json::Serialize(filter);
        }
    } // namespace jni
} // namespace tim

