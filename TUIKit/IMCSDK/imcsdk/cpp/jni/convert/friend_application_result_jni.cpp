//
// Created by EDY on 2022/9/30.
//

#include "friend_application_result_jni.h"
#include <jni_helper.h>
#include "java_basic_jni.h"
#include "friend_application_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass FriendApplicationResultJni::j_cls_;
        jfieldID FriendApplicationResultJni::j_field_array_[FieldIDMax];
        jmethodID FriendApplicationResultJni::j_method_id_array_[MethodIDMax];

        bool FriendApplicationResultJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendApplicationResult");
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

            jmethod = env->GetMethodID(j_cls_, "addFriendApplication", "(Lcom/tencent/imsdk/v2/V2TIMFriendApplication;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddFriendApplication] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "unreadCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUnreadCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendApplicationList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFriendApplicationList] = jfield;

            return true;
        }

        jobject FriendApplicationResultJni::Convert2JObject(const json::Object &applicationResult_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject applicationResultObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (nullptr == applicationResultObj) {
                return nullptr;
            }

            env->SetIntField(applicationResultObj, j_field_array_[FieldIDUnreadCount], applicationResult_json[kTIMPendencyPageUnReadNum]);

            if (applicationResult_json.HasKey(kTIMPendencyPagePendencyInfoArray)){
                json::Array pendency_array = applicationResult_json[kTIMPendencyPagePendencyInfoArray];
                for (const auto & i : pendency_array) {
                    jobject applicationObj = FriendApplicationJni::Convert2JObject(i);
                    env->CallVoidMethod(applicationResultObj,j_method_id_array_[MethodIDAddFriendApplication],applicationObj);
                    env->DeleteLocalRef(applicationObj);
                }
            }
            return applicationResultObj;
        }
    }// namespace jni
}// namespace tim
