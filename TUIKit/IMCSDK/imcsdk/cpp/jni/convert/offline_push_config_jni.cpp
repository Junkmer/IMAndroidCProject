//
// Created by EDY on 2022/9/30.
//

#include <jni_helper.h>
#include "offline_push_config_jni.h"
#include "java_basic_jni.h"
#include "TIMOfflinePushManager.h"

namespace tim {
    namespace jni {
        jclass OfflinePushConfigJni::j_cls_;
        jfieldID OfflinePushConfigJni::j_field_array_[FieldIDMax];
        jmethodID OfflinePushConfigJni::j_method_id_array_[MethodIDMax];

        bool OfflinePushConfigJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMOfflinePushConfig");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "(JLjava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "businessID", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDBusinessID] = jfield;

            jfield = env->GetFieldID(j_cls_, "token", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDToken] = jfield;

            jfield = env->GetFieldID(j_cls_, "isTPNSToken", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsTPNSToken] = jfield;
            return true;
        }

        bool OfflinePushConfigJni::Convert2CoreObject(const jobject &pushConfigObj, json::Object &pushConfig_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            pushConfig_json[kTIMOfflinePushTokenBusinessID] = (int) env->GetLongField(pushConfigObj, j_field_array_[FieldIDBusinessID]);

            auto tokenStr = (jstring) env->GetObjectField(pushConfigObj,j_field_array_[FieldIDToken]);
            if (tokenStr){
                pushConfig_json[kTIMOfflinePushTokenToken] = StringJni::Jstring2Cstring(env,tokenStr);
                env->DeleteLocalRef(tokenStr);
            }

            pushConfig_json[kTIMOfflinePushTokenType] = TIMOfflinePushTokenType(env->GetBooleanField(pushConfigObj,j_field_array_[FieldIDIsTPNSToken]));
            return true;
        }
    }
}