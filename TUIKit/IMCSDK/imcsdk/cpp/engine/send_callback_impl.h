//
// Created by Junker on 2022/9/19.
//

#ifndef IMCPLUSPLUSDEMO_SEND_CALLBACK_IMPL_H
#define IMCPLUSPLUSDEMO_SEND_CALLBACK_IMPL_H

#include <functional>

#include "jni.h"

namespace tim {
    class SendTIMCallbackIMpl{
    public:
        SendTIMCallbackIMpl(jobject callback_);

        ~SendTIMCallbackIMpl();
    public:
        void OnError(int error_code);
        static void TIMCommCallbackImpl(int32_t code, const char *desc, const char *json_params, const void *user_data);

    protected:
        jobject send_callback_;
    };
}

#endif //IMCPLUSPLUSDEMO_SEND_CALLBACK_IMPL_H
