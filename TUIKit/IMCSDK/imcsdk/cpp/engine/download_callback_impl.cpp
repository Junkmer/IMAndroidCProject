//
// Created by EDY on 2022/10/18.
//

#include <jni_helper.h>

#include <utility>
#include "download_callback_impl.h"
#include "im_callbak_jni.h"
#include "json.h"
#include "TIMErrorCode.h"
#include "TIMMessageManager.h"
#include "LogUtil.h"

namespace tim {
    DownloadTIMCallbackIMpl::DownloadTIMCallbackIMpl(jobject download_callback_) {
        jni::ScopedJEnv scopedJEnv;
        auto *env = scopedJEnv.GetEnv();
        if (nullptr != download_callback_){
            _download_callback_ = env->NewGlobalRef(download_callback_);
        }
    }

    DownloadTIMCallbackIMpl::~DownloadTIMCallbackIMpl() {
        jni::ScopedJEnv scopedJEnv;
        auto *env = scopedJEnv.GetEnv();
        if (env) {
            if (_download_callback_) {
                env->DeleteGlobalRef(_download_callback_);
            }
        }
    }

    void DownloadTIMCallbackIMpl::OnError(int code) {
        if (_download_callback_) {
            tim::jni::IMCallbackJNI::Fail(_download_callback_, code);
        }
    }

    void DownloadTIMCallbackIMpl::defaultCallBack(int32_t code, const char *desc, const char *json_params, const void *user_data) {
        if (_download_callback_) {
            if (code == TIMErrCode::ERR_SUCC){
                json::Object result_json = json::Deserialize(json_params);
                int totalSize = result_json[kTIMMsgDownloadElemResultTotalSize];
                int currentSize = result_json[kTIMMsgDownloadElemResultCurrentSize];
                if (currentSize == totalSize && totalSize != 0){
                    tim::jni::IMCallbackJNI::Success(_download_callback_,{});
                } else{
                    tim::jni::IMCallbackJNI::DownloadProgress(_download_callback_, currentSize, totalSize);
                }
            } else{
                tim::jni::IMCallbackJNI::Fail(_download_callback_, code, desc);
            }
        }
    }

}