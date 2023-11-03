//
// Created by Junker on 2022/9/19.
//

#include "send_callback_impl.h"

#include "jni_helper.h"
#include "LogUtil.h"
#include "im_callbak_jni.h"
#include "java_basic_jni.h"
#include "message_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    SendTIMCallbackIMpl::SendTIMCallbackIMpl(jobject callback_) {
        jni::ScopedJEnv scopedJEnv;
        auto *env = scopedJEnv.GetEnv();
        send_callback_ = env->NewGlobalRef(callback_);
    }

    SendTIMCallbackIMpl::~SendTIMCallbackIMpl(){
        jni::ScopedJEnv scopedJEnv;
        auto *env = scopedJEnv.GetEnv();
        if (env) {
            if (send_callback_) {
                LOGE("SendTIMCallbackIMpl ~init");
                env->DeleteGlobalRef(send_callback_);
            }
        }
    }

    void SendTIMCallbackIMpl::OnError(int error_code) {
        if (send_callback_) {
            tim::jni::IMCallbackJNI::Fail(send_callback_,error_code);
        }
    }

    void SendTIMCallbackIMpl::TIMCommCallbackImpl(int32_t code, const char *desc, const char *json_params, const void *user_data) {
        auto sendTimCallbackIMpl = (SendTIMCallbackIMpl *)user_data;
        if (sendTimCallbackIMpl->send_callback_){
            if (TIM_SUCC == code){
                LOGE("sendMessage success, json_params = %s",json_params);
                json::Object msg_obj = json::Deserialize(json_params);
                jni::IMCallbackJNI::Success(sendTimCallbackIMpl->send_callback_,jni::MessageJni::Convert2JObject(msg_obj));
            } else{
                tim::jni::IMCallbackJNI::Fail(sendTimCallbackIMpl->send_callback_,code,desc);
            }
        }
    }
}// namespace tim