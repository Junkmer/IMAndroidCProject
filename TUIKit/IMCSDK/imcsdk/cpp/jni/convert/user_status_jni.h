//
// Created by EDY on 2022/9/26.
//

#ifndef IMCPLUSPLUSDEMO_USER_STATUS_JNI_H
#define IMCPLUSPLUSDEMO_USER_STATUS_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class UserStatusJni{
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &userStatus_json);
            static bool Convert2CoreObject(const jobject &userStatusObj,json::Object &userStatus_json);

        private:
            enum FieldID{
                FieldIDUserID = 0,
                FieldIDStatusType,
                FieldIDCustomStatus,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_USER_STATUS_JNI_H
