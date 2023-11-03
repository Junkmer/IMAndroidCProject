//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_MEMBER_OPERATION_RESULT_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_MEMBER_OPERATION_RESULT_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupMemberOperationResultJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject_Invite(const json::Object &operationResult_json);
            static jobject Convert2JObject_Delete(const json::Object &operationResult_json);

        private:
            enum FieldID{
                FieldIDMemberID = 0,
                FieldIDResult,

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


#endif //IMCPLUSPLUSDEMO_GROUP_MEMBER_OPERATION_RESULT_JNI_H
