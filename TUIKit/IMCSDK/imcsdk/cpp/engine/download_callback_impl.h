//
// Created by Junker on 2022/10/17.
//

#ifndef IMCPLUSPLUSDEMO_DOWNLOAD_CALLBACK_IMPL_H
#define IMCPLUSPLUSDEMO_DOWNLOAD_CALLBACK_IMPL_H

#include "tim_common.h"
#include <functional>

namespace tim {
    class DownloadTIMCallbackIMpl{
    public:
        DownloadTIMCallbackIMpl(jobject download_callback_);
        ~DownloadTIMCallbackIMpl();
    public:
        void OnError(const int error_code);

        //实现 TIMCommCallback
        void defaultCallBack(int32_t code, const char* desc, const char* json_params, const void* user_data);

    private:
        jobject _download_callback_;
    };
}// namespace tim

#endif //IMCPLUSPLUSDEMO_DOWNLOAD_CALLBACK_IMPL_H
