//
// Created by EDY on 2022/9/30.
//

#include "friend_add_application_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass FriendAddApplicationJni::j_cls_;
        jfieldID FriendAddApplicationJni::j_field_array_[FieldIDMax];
        jmethodID FriendAddApplicationJni::j_method_id_array_[MethodIDMax];

        bool FriendAddApplicationJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendAddApplication");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "remark", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDRemark] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendGroup", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFriendGroup] = jfield;

            jfield = env->GetFieldID(j_cls_, "addWording", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDAddWording] = jfield;

            jfield = env->GetFieldID(j_cls_, "addSource", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDAddSource] = jfield;

            jfield = env->GetFieldID(j_cls_, "addType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDAddType] = jfield;

            return true;
        }

        bool FriendAddApplicationJni::Convert2CoreObject(jobject const &j_obj_addApplication, json::Object &addApplication_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;

            jStr = (jstring)env->GetObjectField(j_obj_addApplication,j_field_array_[FieldIDUserID]);
            if (jStr){
                addApplication_json[kTIMFriendshipAddFriendParamIdentifier] = StringJni::Jstring2Cstring(env,jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring)env->GetObjectField(j_obj_addApplication,j_field_array_[FieldIDRemark]);
            if (jStr){
                addApplication_json[kTIMFriendshipAddFriendParamRemark] = StringJni::Jstring2Cstring(env,jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring)env->GetObjectField(j_obj_addApplication,j_field_array_[FieldIDFriendGroup]);
            if (jStr){
                addApplication_json[kTIMFriendshipAddFriendParamGroupName] = StringJni::Jstring2Cstring(env,jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring)env->GetObjectField(j_obj_addApplication,j_field_array_[FieldIDAddWording]);
            if (jStr){
                addApplication_json[kTIMFriendshipAddFriendParamAddWording] = StringJni::Jstring2Cstring(env,jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring)env->GetObjectField(j_obj_addApplication,j_field_array_[FieldIDAddSource]);
            if (jStr){
                addApplication_json[kTIMFriendshipAddFriendParamAddSource] = StringJni::Jstring2Cstring(env,jStr);
                env->DeleteLocalRef(jStr);
            }

            addApplication_json[kTIMFriendshipAddFriendParamFriendType] = TIMFriendType(env->GetIntField(j_obj_addApplication,j_field_array_[FieldIDAddType]));

            return true;
        }
    }
}