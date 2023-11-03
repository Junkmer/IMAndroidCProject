//
// Created by Junker on 2022/9/22.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_MEMBER_CHANGE_INFO_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_MEMBER_CHANGE_INFO_JNI_H

#include "jni.h"
#include "json.h"

namespace tim {
    namespace jni {
        class GroupMemberChangeInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &groupMemberChangeInfo_json);

        private:
            enum FieldID {
                FieldIDUserID = 0,
                FieldIDMuteTime,

                FieldIDMax,
            };

            enum MethodID {
                MethodIDConstruct = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_id_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}




#endif //IMCPLUSPLUSDEMO_GROUP_MEMBER_CHANGE_INFO_JNI_H
