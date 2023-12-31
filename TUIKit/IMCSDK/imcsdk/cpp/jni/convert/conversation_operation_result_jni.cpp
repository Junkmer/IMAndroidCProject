//
// Created by Junker on 2022/9/29.
//

#include <jni_helper.h>
#include "conversation_operation_result_jni.h"
#include "java_basic_jni.h"
#include "TIMConversationManager.h"

namespace tim {
    namespace jni {
        jclass ConversationOperationResultJni::j_cls_;
        jfieldID ConversationOperationResultJni::j_field_array_[FieldIDMax];
        jmethodID ConversationOperationResultJni::j_method_id_array_[MethodIDMax];

        bool ConversationOperationResultJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMConversationOperationResult");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "conversationID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDConversationID] = jfield;

            jfield = env->GetFieldID(j_cls_, "resultCode", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDResultCode] = jfield;

            jfield = env->GetFieldID(j_cls_, "resultInfo", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDResultInfo] = jfield;

            return true;
        }

        jobject ConversationOperationResultJni::Convert2JObject(const json::Object &operationResult_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject operationResultObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (nullptr == operationResultObj) {
                return nullptr;
            }

            jstring conversationID_jStr = StringJni::Cstring2Jstring(env, operationResult_json[kTIMConversationOperationResultConversationID].ToString());
            if (conversationID_jStr) {
                env->SetObjectField(operationResultObj, j_field_array_[FieldIDConversationID], conversationID_jStr);
                env->DeleteLocalRef(conversationID_jStr);
            }

            env->SetIntField(operationResultObj, j_field_array_[FieldIDResultCode], operationResult_json[kTIMConversationOperationResultResultCode]);

            jstring resultInfo_jStr = StringJni::Cstring2Jstring(env, operationResult_json[kTIMConversationOperationResultResultInfo].ToString());
            if (resultInfo_jStr) {
                env->SetObjectField(operationResultObj, j_field_array_[FieldIDResultInfo], resultInfo_jStr);
                env->DeleteLocalRef(resultInfo_jStr);
            }

            return operationResultObj;
        }
    }
}