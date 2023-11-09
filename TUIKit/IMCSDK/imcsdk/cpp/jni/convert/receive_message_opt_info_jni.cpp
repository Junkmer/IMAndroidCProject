//
// Created by EDY on 2022/10/8.
//

#include "receive_message_opt_info_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMMessageManager.h"

namespace tim {
    namespace jni {
        jclass ReceiveMessageOptInfoJni::j_cls_;
        jfieldID ReceiveMessageOptInfoJni::j_field_array_[FieldIDMax];
        jmethodID ReceiveMessageOptInfoJni::j_method_id_array_[MethodIDMax];

        bool ReceiveMessageOptInfoJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMReceiveMessageOptInfo");
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
            jfield = env->GetFieldID(cls, "startHour", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDStartHour] = jfield;

            jfield = env->GetFieldID(cls, "startMinute", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDStartMinute] = jfield;

            jfield = env->GetFieldID(cls, "startSecond", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDStartSecond] = jfield;

            jfield = env->GetFieldID(cls, "startTimeStamp", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDStartTimeStamp] = jfield;

            jfield = env->GetFieldID(cls, "duration", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDDuration] = jfield;

            jfield = env->GetFieldID(cls, "allReceiveMessageOpt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDAllReceiveMessageOpt] = jfield;

            jfield = env->GetFieldID(j_cls_, "c2CReceiveMessageOpt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDC2CReceiveMessageOpt] = jfield;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            return true;
        }

        jobject ReceiveMessageOptInfoJni::Convert2JObject(const json::Object &messageOptInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject userStatusObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!userStatusObj) {
                return nullptr;
            }

            env->SetIntField(userStatusObj, j_field_array_[FieldIDC2CReceiveMessageOpt], messageOptInfo_json[kTIMMsgGetC2CRecvMsgOptResultOpt]);
            env->SetObjectField(userStatusObj, j_field_array_[FieldIDUserID], StringJni::Cstring2Jstring(env, messageOptInfo_json[kTIMMsgGetC2CRecvMsgOptResultIdentifier]));
            return userStatusObj;
        }

        jobject ReceiveMessageOptInfoJni::Convert2JObject_AllRecvMsg(const json::Object &messageOptInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject userStatusObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!userStatusObj) {
                return nullptr;
            }

            env->SetIntField(userStatusObj, j_field_array_[FieldIDStartHour], messageOptInfo_json[kTIMMsgAllRecvMsgOptStartHour]);
            env->SetIntField(userStatusObj, j_field_array_[FieldIDStartMinute], messageOptInfo_json[kTIMMsgAllRecvMsgOptStartMinute]);
            env->SetIntField(userStatusObj, j_field_array_[FieldIDStartSecond], messageOptInfo_json[kTIMMsgAllRecvMsgOptStartSecond]);
            env->SetLongField(userStatusObj, j_field_array_[FieldIDStartTimeStamp], messageOptInfo_json[kTIMMsgAllRecvMsgOptStartTimeStamp].ToInt64());
            env->SetLongField(userStatusObj, j_field_array_[FieldIDDuration], messageOptInfo_json[kTIMMsgAllRecvMsgDuration].ToInt64());
            env->SetIntField(userStatusObj, j_field_array_[FieldIDAllReceiveMessageOpt], messageOptInfo_json[kTIMMsgAllRecvMsgOptLevel]);
            return userStatusObj;
        }

    }// namespace jni
}// namespace tim