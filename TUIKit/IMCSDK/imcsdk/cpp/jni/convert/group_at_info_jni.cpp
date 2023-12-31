//
// Created by Junker on 2022/9/24.
//

#include <jni_helper.h>
#include "group_at_info_jni.h"
#include "TIMGroupManager.h"
#include "TIMConversationManager.h"

namespace tim {
    namespace jni {

        jclass GroupAtInfoJni::j_cls_;
        jfieldID GroupAtInfoJni::j_field_id_array_[FieldIDMax];
        jmethodID GroupAtInfoJni::j_method_id_array_[MethodIDMax];

        bool GroupAtInfoJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupAtInfo");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "seq", "J");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDSeq] = jfield;

            jfield = env->GetFieldID(j_cls_, "atType", "I");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDAtType] = jfield;

            return true;
        }

        jobject GroupAtInfoJni::Convert2JObject(const json::Object &groupAtInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject j_obj_topicInfo = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);

            env->SetLongField(j_obj_topicInfo, j_field_id_array_[FieldIDSeq], groupAtInfo_json[kTIMGroupAtInfoSeq].ToInt64());
            env->SetIntField(j_obj_topicInfo, j_field_id_array_[FieldIDAtType], groupAtInfo_json[kTIMGroupAtInfoAtType]);

            return j_obj_topicInfo;
        }

        bool GroupAtInfoJni::Convert2CoreObject(const jobject &object,json::Object &groupAtInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            groupAtInfo_json[kTIMGroupAtInfoSeq] = (long long)env->GetLongField(object, j_field_id_array_[FieldIDSeq]);
            groupAtInfo_json[kTIMGroupAtInfoAtType] = TIMGroupAtType(env->GetIntField(object, j_field_id_array_[FieldIDAtType]));
            return true;
        }
    }
}