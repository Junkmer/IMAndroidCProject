//
// Created by EDY on 2022/9/30.
//

#include "topic_info_result_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "group_topic_info_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass TopicInfoResultJni::j_cls_;
        jfieldID TopicInfoResultJni::j_field_array_[FieldIDMax];
        jmethodID TopicInfoResultJni::j_method_id_array_[MethodIDMax];

        bool TopicInfoResultJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMTopicInfoResult");
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
            jfield = env->GetFieldID(j_cls_, "errorCode", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDErrorCode] = jfield;

            jfield = env->GetFieldID(j_cls_, "errorMessage", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDErrorMessage] = jfield;

            jfield = env->GetFieldID(j_cls_, "topicInfo", "Lcom/tencent/imsdk/v2/V2TIMTopicInfo;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDTopicInfo] = jfield;

            return true;
        }

        jobject TopicInfoResultJni::Convert2JObject(const json::Object &topicInfoResult_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!jObj) {
                return nullptr;
            }

            env->SetIntField(jObj, j_field_array_[FieldIDErrorCode], topicInfoResult_json[kTIMGroupTopicInfoResultErrorCode]);
            env->SetObjectField(jObj, j_field_array_[FieldIDErrorMessage],
                                StringJni::Cstring2Jstring(env, topicInfoResult_json[kTIMGroupTopicInfoResultErrorMessage].ToString()));

            jobject topicInfoObj = GroupTopicInfoJni::Convert2JObject(topicInfoResult_json[kTIMGroupTopicInfoResultTopicInfo]);
            if (topicInfoObj) {
                env->SetObjectField(jObj, j_field_array_[FieldIDTopicInfo], topicInfoObj);
                env->DeleteLocalRef(topicInfoObj);
            }

            return jObj;
        }

    }//namespace jni
}//namespace tim