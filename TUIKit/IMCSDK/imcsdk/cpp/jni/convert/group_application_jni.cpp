//
// Created by EDY on 2022/9/30.
//

#include "group_application_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {
        jclass GroupApplicationJni::j_cls_;
        jfieldID GroupApplicationJni::j_field_array_[FieldIDMax];
        jmethodID GroupApplicationJni::j_method_id_array_[MethodIDMax];


        bool GroupApplicationJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupApplication");
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
            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "fromUser", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFromUser] = jfield;

            jfield = env->GetFieldID(j_cls_, "fromUserNickName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFromUserNickName] = jfield;

            jfield = env->GetFieldID(j_cls_, "fromUserFaceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFromUserFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "toUser", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDToUser] = jfield;

            jfield = env->GetFieldID(j_cls_, "addTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDAddTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "requestMsg", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDRequestMsg] = jfield;

            jfield = env->GetFieldID(j_cls_, "handledMsg", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDHandledMsg] = jfield;

            jfield = env->GetFieldID(j_cls_, "applicationType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDApplicationType] = jfield;

            jfield = env->GetFieldID(j_cls_, "handleStatus", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDHandleStatus] = jfield;

            jfield = env->GetFieldID(j_cls_, "handleResult", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDHandleResult] = jfield;

            return true;
        }
        
        jobject GroupApplicationJni::Convert2JObject(const json::Object &groupApplication_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!jObj) {
                return nullptr;
            }

            env->SetObjectField(jObj, j_field_array_[FieldIDGroupID], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendencyGroupId]));
            env->SetObjectField(jObj, j_field_array_[FieldIDFromUser], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendencyFromIdentifier]));
            //TODO::加群申请未决列表，缺少“用户头像和昵称”字段，待完善
//            //C目前没有此项能力
//            env->SetObjectField(jObj, j_field_array_[FieldIDFromUserNickName], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendency]));
//            env->SetObjectField(jObj, j_field_array_[FieldIDFromUserFaceUrl], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendency]));
            env->SetObjectField(jObj, j_field_array_[FieldIDToUser], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendencyToIdentifier]));
            env->SetLongField(jObj, j_field_array_[FieldIDAddTime], groupApplication_json[kTIMGroupPendencyAddTime].ToInt64());
            env->SetObjectField(jObj, j_field_array_[FieldIDRequestMsg], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendencyApplyInviteMsg]));
            env->SetObjectField(jObj, j_field_array_[FieldIDHandledMsg], StringJni::Cstring2Jstring(env, groupApplication_json[kTIMGroupPendencyApprovalMsg]));
            env->SetIntField(jObj, j_field_array_[FieldIDApplicationType], groupApplication_json[kTIMGroupPendencyPendencyType]);
            env->SetIntField(jObj, j_field_array_[FieldIDHandleStatus], groupApplication_json[kTIMGroupPendencyHandled]);
            env->SetIntField(jObj, j_field_array_[FieldIDHandleResult], groupApplication_json[kTIMGroupPendencyHandleResult]);

            return jObj;
        }

        bool GroupApplicationJni::Convert2CoreObject(jobject const &j_obj_groupApplication, json::Object &groupApplication_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;
            jStr = (jstring) env->GetObjectField(j_obj_groupApplication, j_field_array_[FieldIDGroupID]);
            if (jStr) {
                groupApplication_json[kTIMGroupPendencyGroupId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupApplication, j_field_array_[FieldIDFromUser]);
            if (jStr) {
                groupApplication_json[kTIMGroupPendencyFromIdentifier] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupApplication, j_field_array_[FieldIDToUser]);
            if (jStr) {
                groupApplication_json[kTIMGroupPendencyToIdentifier] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            groupApplication_json[kTIMGroupPendencyAddTime] =(long long) env->GetLongField(j_obj_groupApplication, j_field_array_[FieldIDAddTime]);

            jStr = (jstring) env->GetObjectField(j_obj_groupApplication, j_field_array_[FieldIDRequestMsg]);
            if (jStr) {
                groupApplication_json[kTIMGroupPendencyApplyInviteMsg] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupApplication, j_field_array_[FieldIDHandledMsg]);
            if (jStr) {
                groupApplication_json[kTIMGroupPendencyApprovalMsg] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            groupApplication_json[kTIMGroupPendencyPendencyType] = TIMGroupPendencyType(env->GetIntField(j_obj_groupApplication, j_field_array_[FieldIDApplicationType]));
            groupApplication_json[kTIMGroupPendencyHandled] = TIMGroupPendencyHandle(env->GetIntField(j_obj_groupApplication, j_field_array_[FieldIDHandleStatus]));
            groupApplication_json[kTIMGroupPendencyHandleResult] = TIMGroupPendencyHandleResult(env->GetIntField(j_obj_groupApplication, j_field_array_[FieldIDHandleResult]));
            
            return true;
        }
    }//namespace jni
}//namespace tim