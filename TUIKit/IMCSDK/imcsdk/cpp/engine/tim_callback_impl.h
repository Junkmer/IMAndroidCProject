//
// Created by EDY on 2022/9/13.
//

#ifndef IMCPLUSPLUSDEMO_CALLBACK_IMPL_H
#define IMCPLUSPLUSDEMO_CALLBACK_IMPL_H

#include <functional>
#include <jni.h>

namespace tim {
    class TIMCallbackIMpl final {
    public://构造函数 和 回调函数
        TIMCallbackIMpl(jobject listener_callback_);

        ~TIMCallbackIMpl();

    public:
        void onError(int error_code);

        //实现 TIMCommCallback
        void defaultCallBack(int32_t code, const char* desc, const char* json_params, const void* user_data);

    private:
        jobject listener_callback_;
    };
}


#endif //IMCPLUSPLUSDEMO_CALLBACK_IMPL_H
