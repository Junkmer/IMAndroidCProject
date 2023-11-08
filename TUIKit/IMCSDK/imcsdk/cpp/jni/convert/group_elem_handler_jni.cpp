//
// Created by Junker on 2022/9/22.
//

#include <jni_helper.h>
#include "group_elem_handler_jni.h"
#include "java_basic_jni.h"
#include "group_member_info_jni.h"
#include "group_change_info_jni.h"
#include "group_member_change_info_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {

        std::string GroupElemHandlerJni::GetHandlerType() {
            return "GroupElemHandlerJni";
        }

        bool GroupElemHandlerJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupTipsElem");
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

            jmethod = env->GetMethodID(j_cls_, "addMember", "(Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddMember] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addGroupChangeInfo", "(Lcom/tencent/imsdk/v2/V2TIMGroupChangeInfo;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddGroupChangeInfo] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addMemberChangeInfo", "(Lcom/tencent/imsdk/v2/V2TIMGroupMemberChangeInfo;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddMemberChangeInfo] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "type", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDType] = jfield;

            jfield = env->GetFieldID(j_cls_, "elementType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDElemType] = jfield;

            jfield = env->GetFieldID(j_cls_, "opMember", "Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDOpMember] = jfield;

            jfield = env->GetFieldID(j_cls_, "memberList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMemberList] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupChangeInfoList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupChangeInfoList] = jfield;

            jfield = env->GetFieldID(j_cls_, "memberChangeInfoList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMemberChangeInfoList] = jfield;

            jfield = env->GetFieldID(j_cls_, "memberCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMemberCount] = jfield;

            return true;
        }

        jobject GroupElemHandlerJni::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_GroupTips) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            auto *jElemObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetIntField(jElemObj, j_field_array_[FieldIDElemType], elem_json[kTIMElemType]);
            env->SetObjectField(jElemObj, j_field_array_[FieldIDGroupID], StringJni::Cstring2Jstring(env, elem_json[kTIMGroupTipsElemGroupId]));
            env->SetIntField(jElemObj, j_field_array_[FieldIDType], elem_json[kTIMGroupTipsElemTipType]);

            if (elem_json.HasKey(kTIMGroupTipsElemOpGroupMemberInfo)) {
                jobject opMemberObj = GroupMemberInfoJni::Convert2JObject(elem_json[kTIMGroupTipsElemOpGroupMemberInfo]);
                env->SetObjectField(jElemObj, j_field_array_[FieldIDOpMember], opMemberObj);
                env->DeleteLocalRef(opMemberObj);
            }

            if (elem_json.HasKey(kTIMGroupTipsElemChangedGroupMemberInfoArray)) {
                json::Array member_array = elem_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                for (int i = 0; i < member_array.size(); ++i) {
                    jobject memberObj = GroupMemberInfoJni::Convert2JObject(member_array[i]);
                    env->CallVoidMethod(jElemObj, j_method_id_array_[MethodIDAddMember], memberObj);
                    env->DeleteLocalRef(memberObj);
                }
            }

            if (elem_json.HasKey(kTIMGroupTipsElemGroupChangeInfoArray)) {
                json::Array info_change_array = elem_json[kTIMGroupTipsElemGroupChangeInfoArray];
                for (int i = 0; i < info_change_array.size(); ++i) {
                    jobject infoChangeObj = GroupChangeInfoJni::Convert2JObject(info_change_array[i]);
                    env->CallVoidMethod(jElemObj, j_method_id_array_[MethodIDAddGroupChangeInfo], infoChangeObj);
                    env->DeleteLocalRef(infoChangeObj);
                }
            }

            if (elem_json.HasKey(kTIMGroupTipsElemChangedGroupMemberInfoArray)){
                json::Array member_info_change_array = elem_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                for (int i = 0; i < member_info_change_array.size(); ++i) {
                    jobject memberInfoChangeObj = GroupMemberChangeInfoJni::Convert2JObject(member_info_change_array[i]);
                    env->CallVoidMethod(jElemObj, j_method_id_array_[MethodIDAddMemberChangeInfo], memberInfoChangeObj);
                    env->DeleteLocalRef(memberInfoChangeObj);
                }
            }

            env->SetIntField(jElemObj, j_field_array_[FieldIDMemberCount], elem_json[kTIMGroupTipsElemMemberNum]);

            return jElemObj;
        }

        std::unique_ptr<json::Object> GroupElemHandlerJni::Convert2CoreObject(int elemType, jobject jElemObj) {
            return nullptr;
        }
    }// namespace jni
}// namespace tim