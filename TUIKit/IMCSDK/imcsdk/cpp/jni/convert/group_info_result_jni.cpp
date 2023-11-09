//
// Created by EDY on 2022/9/30.
//

#include "group_info_result_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "group_info_jni.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {
        jclass GroupInfoResultJni::j_cls_;
        jfieldID GroupInfoResultJni::j_field_array_[FieldIDMax];
        jmethodID GroupInfoResultJni::j_method_id_array_[MethodIDMax];

        bool GroupInfoResultJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupInfoResult");
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
            jfield = env->GetFieldID(j_cls_, "resultCode", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDResultCode] = jfield;

            jfield = env->GetFieldID(j_cls_, "resultMessage", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDResultMessage] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupInfo", "Lcom/tencent/imsdk/v2/V2TIMGroupInfo;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupInfo] = jfield;

            return true;
        }

        jobject GroupInfoResultJni::Convert2JObject(const json::Object &infoResult_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!jObj) {
                return nullptr;
            }

            env->SetIntField(jObj, j_field_array_[FieldIDResultCode], infoResult_json[kTIMGetGroupInfoResultCode]);

            env->SetObjectField(jObj, j_field_array_[FieldIDResultMessage], StringJni::Cstring2Jstring(env, infoResult_json[kTIMGetGroupInfoResultDesc]));

            env->SetObjectField(jObj,j_field_array_[FieldIDGroupInfo],GroupInfoJni::Convert2JObject(infoResult_json[kTIMGetGroupInfoResultInfo]));
            return jObj;
        }

    }//namespace jni
}//namespace tim