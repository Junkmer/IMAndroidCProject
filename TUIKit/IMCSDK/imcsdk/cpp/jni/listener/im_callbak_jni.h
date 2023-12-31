//
// Created by Junker on 2022/8/26.
//

#ifndef IMCPLUSPLUSDEMO_IM_CALLBAK_JNI_H
#define IMCPLUSPLUSDEMO_IM_CALLBAK_JNI_H

#include <jni.h>
#include <string>
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        class IMCallbackJNI {

        public:
            static bool InitIDs(JNIEnv *env);

            static void Success(const jobject &callback, const jobject &obj);
            static void Fail(const jobject &callback, const int &code);
            static void Fail(const jobject &callback, const int &code, const char *desc);
            static void FailOrComplete(const jobject &callback, const int &code, const char *desc, const jobject &params);
            static void Progress(const jobject &callback, const int &progress);
            static void DownloadProgress(const jobject &callback, const uint64_t &currentSize,const uint64_t &totalSize);
            static void Success2signalInfo(const jobject &callback, const jobject &obj);
            static void Fail2signalInfo(const jobject &callback, const int &code, const char *desc);

        private:
            enum MethodID {
                MethodIDSuccess = 0,
                MethodIDFail,
                MethodIDFailOrComplete,
                MethodIDProgress,
                MethodIDDownloadProgress,
                MethodIDSuccess2signalInfo,
                MethodIDFail2signalInfo,

                MethodIDMax,
            };
        public:
            static jclass j_cls_callback;
            static jmethodID j_method_id_array[MethodIDMax];
        };
    } // namespace jni
} // namespace tim

#endif //IMCPLUSPLUSDEMO_IM_CALLBAK_JNI_H

