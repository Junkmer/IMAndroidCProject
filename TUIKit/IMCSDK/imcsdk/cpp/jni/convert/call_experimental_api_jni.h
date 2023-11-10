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
            static std::string ConvertToCoreObject(JNIEnv *env, const jstring &api, const jobject &param);

        private:
            static jobject getJObject(JNIEnv *env, const jobject &obj, const jstring &key);

            static int getJInt(JNIEnv *env, const jobject &obj, const jstring &key);

            static long getJLong(JNIEnv *env, const jobject &obj, const jstring &key);

            static double getJDouble(JNIEnv *env, const jobject &obj, const jstring &key);

            static std::string getJString(JNIEnv *env, const jobject &obj, const jstring &key);
        };

        JNIEnv *jniEnv;
    } // namespace tim
} // namespace jni


#endif //IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H
