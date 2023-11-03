//
// Created by Junker on 2022/8/22.
//

#ifndef IMCPLUSPLUSDEMO_SDK_CONFIG_JNI_H
#define IMCPLUSPLUSDEMO_SDK_CONFIG_JNI_H

#include <jni.h>
#include <string>

namespace tim {
    namespace jni {

        class SDKConfigJni {
        public:
            static bool InitIDs(JNIEnv *env);

            static std::string ConvertToCoreObject(JNIEnv *env, const jobject &object);

        private:
            enum MethodID {
                MethodIDConstruct = 0,

                MethodIDMax,
            };

            static jclass jcls_;
            static jmethodID j_method_id_array_[MethodIDMax];
        };

    } // namespace tim
} // namespace jni


#endif //IMCPLUSPLUSDEMO_SDK_CONFIG_JNI_H
