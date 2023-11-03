//
// Created by EDY on 2022/8/22.
//

#include <jni.h>
#include "jni_util.h"
#include "java_basic_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "tim_callback_impl.h"
#include "value_callback_impl.h"
#include "im_callbak_jni.h"
#include "offline_push_config_jni.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_NATIVE_FUNC(void, NativeSetOfflinePushConfig, jobject config, jobject callback) {
    json::Object pushConfig;
    bool flag = tim::jni::OfflinePushConfigJni::Convert2CoreObject(config, pushConfig);
    if (flag) {
        tim::TIMEngine::GetInstance()->SetOfflinePushConfig(json::Serialize(pushConfig).c_str(), new tim::TIMCallbackIMpl(callback));
    }
}

DEFINE_NATIVE_FUNC(void, NativeDoBackground, jint unread_count, jobject callback) {
    tim::TIMEngine::GetInstance()->DoBackground(unread_count, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDoForeground, jobject callback) {
    tim::TIMEngine::GetInstance()->DoForeground(new tim::TIMCallbackIMpl(callback));
}

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeSetOfflinePushConfig", "(Lcom/tencent/imsdk/v2/V2TIMOfflinePushConfig;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeSetOfflinePushConfig},
        {"nativeDoBackground",         "(ILcom/tencent/imsdk/common/IMCallback;)V",                                                                 (void *) NativeDoBackground},
        {"nativeDoForeground",         "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                                  (void *) NativeDoForeground},
};

//注册 native 方法
int RegisterNativeMethodsForV2TIMOfflinePushManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMOfflinePushManager", gMethods,
                                                  sizeof(gMethods) / sizeof(gMethods[0]));
}

#ifdef __cplusplus
}
#endif