//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_FRIEND_CHECK_RESULT_JNI_H
#define IMCPLUSPLUSDEMO_FRIEND_CHECK_RESULT_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class FriendCheckResultJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &friendCheckResult_json);

        private:
            enum FieldID{
                FieldIDUserID = 0,
                FieldIDResultCode,
                FieldIDResultInfo,
                FieldIDResultType,

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

#endif //IMCPLUSPLUSDEMO_FRIEND_CHECK_RESULT_JNI_H
