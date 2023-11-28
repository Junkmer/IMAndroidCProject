//
// Created by EDY on 2022/9/13.
//

#include <jni_helper.h>

#include <utility>
#include "tim_callback_impl.h"
#include "im_callbak_jni.h"
#include "java_basic_jni.h"
#include "jni_helper.h"
#include "LogUtil.h"

namespace tim {
    TIMCallbackIMpl::TIMCallbackIMpl(jobject listener_callback) {
        jni::ScopedJEnv scopedJEnv;
        auto *env = scopedJEnv.GetEnv();
        if (nullptr != listener_callback){
            listener_callback_ = env->NewGlobalRef(listener_callback);
        }

    }

    TIMCallbackIMpl::~TIMCallbackIMpl() {
        jni::ScopedJEnv scopedJEnv;
        auto *env = scopedJEnv.GetEnv();
        if (env) {
            if (listener_callback_) {
//                LOGE("TIMCallbackIMpl ~init");
                env->DeleteGlobalRef(listener_callback_);
            }
        }
    }

    void TIMCallbackIMpl::onError(int code) {
        if (listener_callback_) {
            tim::jni::IMCallbackJNI::Fail(listener_callback_, code);
        }
    }

    void TIMCallbackIMpl::defaultCallBack(int32_t code, const char *desc, const char *json_params, const void *user_data) {
        if (listener_callback_) {
            if (TIM_SUCC == code){
                tim::jni::IMCallbackJNI::Success(listener_callback_,{});
            } else{
                tim::jni::IMCallbackJNI::Fail(listener_callback_, code, desc);
            }
        }
    }

}
