//
// Created by Junker on 2022/9/22.
//

#include <jni_helper.h>
#include "group_member_info_jni.h"
#include "java_basic_jni.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {

        jclass GroupMemberInfoJni::j_cls_;
        jfieldID GroupMemberInfoJni::j_field_array_[FieldIDMax];
        jmethodID GroupMemberInfoJni::j_method_id_array[MethodIDMax];

        bool GroupMemberInfoJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupMemberInfo");
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
            j_method_id_array[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "nickName", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_array_[FieldIDNickname] = jfield;

            jfield = env->GetFieldID(j_cls_, "nameCard", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_array_[FieldIDNameCard] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendRemark", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_array_[FieldIDFriendRemark] = jfield;

            jfield = env->GetFieldID(j_cls_, "faceUrl", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_array_[FieldIDFaceUrl] = jfield;

            return true;
        }

        jobject GroupMemberInfoJni::Convert2JObject(const json::Object &member_info_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jMemberInfoObj = env->NewObject(j_cls_, j_method_id_array[MethodIDConstructor]);
            if (jMemberInfoObj == nullptr) return nullptr;

            env->SetObjectField(jMemberInfoObj, j_field_array_[FieldIDUserID], StringJni::Cstring2Jstring(env, member_info_json[kTIMGroupMemberInfoIdentifier].ToString()));
            env->SetObjectField(jMemberInfoObj, j_field_array_[FieldIDNameCard], StringJni::Cstring2Jstring(env, member_info_json[kTIMGroupMemberInfoNameCard].ToString()));
            env->SetObjectField(jMemberInfoObj, j_field_array_[FieldIDNickname], StringJni::Cstring2Jstring(env, member_info_json[kTIMGroupMemberInfoNickName].ToString()));
            env->SetObjectField(jMemberInfoObj, j_field_array_[FieldIDFriendRemark], StringJni::Cstring2Jstring(env, member_info_json[kTIMGroupMemberInfoFriendRemark].ToString()));
            env->SetObjectField(jMemberInfoObj, j_field_array_[FieldIDFaceUrl], StringJni::Cstring2Jstring(env, member_info_json[kTIMGroupMemberInfoFaceUrl].ToString()));

            return jMemberInfoObj;
        }

        bool GroupMemberInfoJni::Convert2CoreObject(const jobject &object, json::Object &member_info_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(object, j_field_array_[FieldIDUserID]);
            if (jStr) {
                member_info_json[kTIMGroupMemberInfoIdentifier] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_array_[FieldIDNickname]);
            if (jStr) {
                member_info_json[kTIMGroupMemberInfoNickName] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_array_[FieldIDNameCard]);
            if (jStr) {
                member_info_json[kTIMGroupMemberInfoNameCard] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_array_[FieldIDFriendRemark]);
            if (jStr) {
                member_info_json[kTIMGroupMemberInfoFriendRemark] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_array_[FieldIDFaceUrl]);
            if (jStr) {
                member_info_json[kTIMGroupMemberInfoFaceUrl] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            return true;
        }
    }
}