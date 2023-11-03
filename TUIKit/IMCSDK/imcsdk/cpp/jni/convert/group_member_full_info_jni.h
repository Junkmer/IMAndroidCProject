//
// Created by EDY on 2022/9/19.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_MEMBER_FULL_INFO_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_MEMBER_FULL_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupMemberFullInfoJNI {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &memberFullInfo_json);
            static bool Convert2CoreObject(const jobject &member_info_obj, json::Object &memberFullInfo_json);

        private:
            enum FieldID {
                FieldIDUserID = 0,
                FieldIDNickName,
                FieldIDFriendRemark,
                FieldIDNameCard,
                FieldIDFaceUrl,
                FieldIDRole,
                FieldIDMuteUntil,
                FieldIDJoinTime,
                FieldIDCustomInfo,

                FieldIDMax,
            };

            enum MethodID {
                MethodIDConstruct = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_GROUP_MEMBER_FULL_INFO_JNI_H
