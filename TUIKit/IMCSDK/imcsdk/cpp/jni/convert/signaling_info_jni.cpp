//
// Created by EDY on 2022/9/30.
//

#include <jni_helper.h>
#include "signaling_info_jni.h"
#include "java_basic_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass SignalingInfoJni::j_cls;
        jfieldID SignalingInfoJni::j_field_array_[FieldIDMax];
        jmethodID SignalingInfoJni::j_method_id_array_[MethodIDMax];

        bool SignalingInfoJni::InitIDs(JNIEnv *env) {
            if (j_cls != nullptr) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMSignalingInfo");
            if (nullptr == cls) {
                return false;
            }

            j_cls = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jmethod = env->GetMethodID(j_cls, "addInvitee", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddInvitee] = jmethod;

            jfieldID jfield = nullptr;


            jfield = env->GetFieldID(j_cls, "inviteID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDInviteID] = jfield;

            jfield = env->GetFieldID(j_cls, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls, "inviter", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDInviter] = jfield;

            jfield = env->GetFieldID(j_cls, "inviteeList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDInviteeList] = jfield;

            jfield = env->GetFieldID(j_cls, "data", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDData] = jfield;

            jfield = env->GetFieldID(j_cls, "timeout", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDTimeout] = jfield;

            jfield = env->GetFieldID(j_cls, "actionType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDActionType] = jfield;
            return true;
        }

        jobject SignalingInfoJni::Convert2JObject(const json::Object &signalingInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject signalingInfoObj = env->NewObject(j_cls, j_method_id_array_[MethodIDConstruct]);
            if (!signalingInfoObj) {
                return nullptr;
            }
            jstring jStr = nullptr;

            jStr = StringJni::Cstring2Jstring(env, signalingInfo_json[kTIMSignalingInfoInviteID]);
            if (jStr){
                env->SetObjectField(signalingInfoObj, j_field_array_[FieldIDInviteID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, signalingInfo_json[kTIMSignalingInfoGroupID]);
            if (jStr){
                env->SetObjectField(signalingInfoObj, j_field_array_[FieldIDGroupID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, signalingInfo_json[kTIMSignalingInfoInviter]);
            if (jStr){
                env->SetObjectField(signalingInfoObj, j_field_array_[FieldIDInviter],jStr);
                env->DeleteLocalRef(jStr);
            }

            if (signalingInfo_json.HasKey(kTIMSignalingInfoInviteeList)){
                json::Array invitee_array = signalingInfo_json[kTIMSignalingInfoInviteeList];
                for (int i = 0; i < invitee_array.size(); ++i) {
                    jstring inviteeStr = StringJni::Cstring2Jstring(env, invitee_array[i]);
                    env->CallVoidMethod(signalingInfoObj,j_method_id_array_[MethodIDAddInvitee],inviteeStr);
                    env->DeleteLocalRef(inviteeStr);
                }
            }

            jStr = StringJni::Cstring2Jstring(env, signalingInfo_json[kTIMSignalingInfoData]);
            if (jStr){
                env->SetObjectField(signalingInfoObj, j_field_array_[FieldIDData],jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetIntField(signalingInfoObj, j_field_array_[FieldIDTimeout], signalingInfo_json[kTIMSignalingInfoTimeout]);
            env->SetIntField(signalingInfoObj, j_field_array_[FieldIDActionType], signalingInfo_json[kTIMSignalingInfoActionType]);

            return signalingInfoObj;
        }

        bool SignalingInfoJni::Convert2CoreObject(const jobject &j_obj_signalingInfo, json::Object &signalingInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;
            jStr = (jstring) env->GetObjectField(j_obj_signalingInfo, j_field_array_[FieldIDInviteID]);
            if (jStr) {
                signalingInfo_json[kTIMSignalingInfoInviteID] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_signalingInfo, j_field_array_[FieldIDGroupID]);
            if (jStr) {
                signalingInfo_json[kTIMSignalingInfoGroupID] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_signalingInfo, j_field_array_[FieldIDInviter]);
            if (jStr) {
                signalingInfo_json[kTIMSignalingInfoInviter] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jobject inviteeListObj = env->GetObjectField(j_obj_signalingInfo,j_field_array_[FieldIDInviteeList]);
            if (inviteeListObj){
                int size = ArrayListJni::Size(inviteeListObj);
                for (int i = 0; i < size; ++i) {
                    auto inviteeStr = (jstring)ArrayListJni::Get(inviteeListObj,i);
                    signalingInfo_json[kTIMSignalingInfoInviteeList].ToArray().push_back(StringJni::Jstring2Cstring(env,inviteeStr));
                    env->DeleteLocalRef(inviteeStr);
                }
                env->DeleteLocalRef(inviteeListObj);
            }

            jStr = (jstring) env->GetObjectField(j_obj_signalingInfo, j_field_array_[FieldIDData]);
            if (jStr) {
                signalingInfo_json[kTIMSignalingInfoData] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            signalingInfo_json[kTIMSignalingInfoActionType] = TIMSignalingActionType(env->GetIntField(j_obj_signalingInfo, j_field_array_[FieldIDActionType]));
            signalingInfo_json[kTIMSignalingInfoTimeout] =  env->GetIntField(j_obj_signalingInfo, j_field_array_[FieldIDTimeout]);
            return true;
        }
    }// namespace jni
}// namespace tim