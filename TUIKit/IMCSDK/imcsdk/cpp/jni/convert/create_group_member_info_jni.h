//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_CREATE_GROUP_MEMBER_INFO_JNI_H
#define IMCPLUSPLUSDEMO_CREATE_GROUP_MEMBER_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class CreateGroupMemberInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static bool Convert2CoreObject(const jobject &j_obj_createMemberInfo,json::Object &createMemberInfo_json);

        private:
            enum FieldID{
                FieldIDUserID = 0,
                FieldIDRole,

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
    }//namespace jni
}//namespace tim

#endif //IMCPLUSPLUSDEMO_CREATE_GROUP_MEMBER_INFO_JNI_H
