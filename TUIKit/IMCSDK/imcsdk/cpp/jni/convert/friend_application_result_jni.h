//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_FRIEND_APPLICATION_RESULT_JNI_H
#define IMCPLUSPLUSDEMO_FRIEND_APPLICATION_RESULT_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class FriendApplicationResultJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &applicationResult_json);

        private:
            enum FieldID{
                FieldIDUnreadCount = 0,
                FieldIDFriendApplicationList,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,
                MethodIDAddFriendApplication,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim


#endif //IMCPLUSPLUSDEMO_FRIEND_APPLICATION_RESULT_JNI_H
