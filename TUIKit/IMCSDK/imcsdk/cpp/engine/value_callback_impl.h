//
// Created by EDY on 2022/9/26.
//

#ifndef IMCPLUSPLUSDEMO_VALUE_CALLBACK_IMPL_H
#define IMCPLUSPLUSDEMO_VALUE_CALLBACK_IMPL_H

#include <functional>
#include <jni.h>
#include "TIMErrorCode.h"
#include "functional"
#include "tim_common.h"

namespace tim {
    template<class T>
    class ValueTIMCallbackIMpl {
    public:
        using ValueCallback = std::function<void(const int, const char*, const T &)>;

        ValueTIMCallbackIMpl() = default;
        ~ValueTIMCallbackIMpl()  = default;

        void setCallback(ValueCallback value_callback) {
            _value_callback_ = std::move(value_callback);
        }

    protected:

        // V2TIMValueCallback
        void OnSuccess(const T &value) override {
            if (_value_callback_) {
                _value_callback_(TIMErrCode::ERR_SUCC, "", value);
            }
        }

        void OnError(const int error_code, const char *error_message) {
            if (_value_callback_) {
                _value_callback_(error_code, error_message, {});
            }
        }

    private:
        ValueCallback _value_callback_;
    };
}// namespace tim

#endif //IMCPLUSPLUSDEMO_VALUE_CALLBACK_IMPL_H
