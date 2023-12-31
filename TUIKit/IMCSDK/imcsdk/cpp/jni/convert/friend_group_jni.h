//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_FRIEND_GROUP_JNI_H
#define IMCPLUSPLUSDEMO_FRIEND_GROUP_JNI_H

#include <jni.h>
#include <jni_helper.h>
#include "java_basic_jni.h"
#include "json.h"

namespace tim {
    namespace jni {
        class FriendGroupJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &friendGroup_json);

        private:
            enum FieldID{
                FieldIDName = 0,
                FieldIDFriendCount,
                FieldIDFriendIDList,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,
                MethodIDAddFriendID,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_FRIEND_GROUP_JNI_H
