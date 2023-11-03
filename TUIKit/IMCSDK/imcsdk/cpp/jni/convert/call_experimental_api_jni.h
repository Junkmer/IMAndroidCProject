//
// Created by EDY on 2023/10/19.
//

#ifndef IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H
#define IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H

#include <jni.h>
#include <string>
#include "json.h"

namespace tim {
    namespace jni {

        class CallExperimentalAPIJni {
        public:
            static std::string ConvertToCoreObject(JNIEnv *env,const jstring &api,const jobject &param);
        };

    } // namespace tim
} // namespace jni


#endif //IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H
