//
// Created by Junker on 2022/9/22.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_MEMBER_INFO_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_MEMBER_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupMemberInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &member_info_json);
            static bool Convert2CoreObject(const jobject &object, json::Object &member_info_json);

        private:
            enum FieldID {
                FieldIDUserID = 0,
                FieldIDNickname,
                FieldIDNameCard,
                FieldIDFriendRemark,
                FieldIDFaceUrl,

                FieldIDMax,
            };

            enum MethodID {
                MethodIDConstructor = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array[MethodIDMax];
        };
    }
}




#endif //IMCPLUSPLUSDEMO_GROUP_MEMBER_INFO_JNI_H
