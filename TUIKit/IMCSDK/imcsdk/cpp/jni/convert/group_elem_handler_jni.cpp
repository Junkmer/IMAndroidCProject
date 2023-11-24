//
// Created by Junker on 2022/9/22.
//

#include <jni_helper.h>
#include "group_elem_handler_jni.h"
#include "java_basic_jni.h"
#include "group_member_info_jni.h"
#include "group_change_info_jni.h"
#include "group_member_change_info_jni.h"
#include "TIMGroupManager.h"
#include "TIMMessageManager.h"

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

            env->SetObjectField(jElemObj, j_field_array_[FieldIDGroupID], StringJni::Cstring2Jstring(env, elem_json[kTIMGroupTipsElemGroupId]));
            env->SetIntField(jElemObj, j_field_array_[FieldIDType], CTipType2JTipType(TIMGroupTipType(elem_json[kTIMGroupTipsElemTipType].ToInt())));

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

            if (elem_json.HasKey(kTIMGroupTipsElemMemberChangeInfoArray)){
                json::Array member_info_change_array = elem_json[kTIMGroupTipsElemMemberChangeInfoArray];
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
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_GroupTips || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object groupTipElem;
            groupTipElem[kTIMElemType] = TIMElemType::kTIMElem_GroupTips;

            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[FieldIDGroupID]);
            if (jStr) {
                groupTipElem[kTIMGroupTipsElemGroupId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }
            groupTipElem[kTIMGroupTipsElemTipType] = JTipType2CTipType(env->GetIntField(jElemObj, j_field_array_[FieldIDType]));

            jobject opMemberObj = env->GetObjectField(jElemObj, j_field_array_[FieldIDOpMember]);
            if (opMemberObj) {
                json::Object opMember_json;
                bool flag = GroupMemberInfoJni::Convert2CoreObject(opMemberObj,opMember_json);
                if (flag){
                    groupTipElem[kTIMGroupTipsElemOpGroupMemberInfo] = opMember_json;
                }
                env->DeleteLocalRef(opMemberObj);
            }

            jobject memberInfoListObj = env->GetObjectField(jElemObj, j_field_array_[FieldIDMemberList]);
            if (memberInfoListObj) {
                json::Array member_array;
                int size = ArrayListJni::Size(memberInfoListObj);
                for (int i = 0; i < size; ++i){
                    json::Object member_json;
                    jobject member_info_obj = ArrayListJni::Get(memberInfoListObj,i);
                    bool flag = GroupMemberInfoJni::Convert2CoreObject(member_info_obj,member_json);
                    if (flag){
                        member_array.push_back(member_json);
                    }
                    env->DeleteLocalRef(member_info_obj);
                }
                if (size > 0){
                    groupTipElem[kTIMGroupTipsElemChangedGroupMemberInfoArray] = member_array;
                }
                env->DeleteLocalRef(memberInfoListObj);
            }

//            jobject groupChangeInfoListObj = env->GetObjectField(jElemObj, j_field_array_[FieldIDGroupChangeInfoList]);
//            if (groupChangeInfoListObj) {
//                json::Array change_info_array;
//                int size = ArrayListJni::Size(groupChangeInfoListObj);
//                for (int i = 0; i < size; ++i){
//                    json::Object change_info_json;
//                    jobject change_info_obj = ArrayListJni::Get(groupChangeInfoListObj,i);
//                    bool flag = GroupChangeInfoJni::Convert2CoreObject(change_info_obj,change_info_json);
//                    if (flag){
//                        change_info_array.push_back(change_info_json);
//                    }
//                    env->DeleteLocalRef(change_info_obj);
//                }
//                if (size > 0){
//                    groupTipElem[kTIMGroupTipsElemGroupChangeInfoArray] = change_info_array;
//                }
//                env->DeleteLocalRef(groupChangeInfoListObj);
//            }

//            jobject memberChangeInfoListObj = env->GetObjectField(jElemObj, j_field_array_[FieldIDGroupChangeInfoList]);
//            if (memberChangeInfoListObj) {
//                json::Array member_change_info_array;
//                int size = ArrayListJni::Size(memberChangeInfoListObj);
//                for (int i = 0; i < size; ++i){
//                    json::Object member_change_info_json;
//                    jobject memberChangeInfoObj = ArrayListJni::Get(memberChangeInfoListObj,i);
//                    bool flag = GroupMemberChangeInfoJni::Convert2CoreObject(memberChangeInfoObj,member_change_info_json);
//                    if (flag){
//                        member_change_info_array.push_back(member_change_info_json);
//                    }
//                    env->DeleteLocalRef(memberChangeInfoObj);
//                }
//                if (size > 0){
//                    groupTipElem[kTIMGroupTipsElemChangedGroupMemberInfoArray] = member_change_info_array;
//                }
//                env->DeleteLocalRef(memberChangeInfoListObj);
//            }

            groupTipElem[kTIMGroupTipsElemMemberNum] = env->GetIntField(jElemObj, j_field_array_[FieldIDMemberCount]);

            return std::make_unique<json::Object>(groupTipElem);
        }

        TIMGroupTipType GroupElemHandlerJni::JTipType2CTipType(int jTipType) {
            //im native sdk 中的群tip类型 转换 im c sdk中的群tip类型
            switch (jTipType) {
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_JOIN:
                    return kTIMGroupTip_Invite;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_INVITE:
                    return kTIMGroupTip_Invite;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_QUIT:
                    return kTIMGroupTip_Quit;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_KICKED:
                    return kTIMGroupTip_Kick;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_SET_ADMIN:
                    return kTIMGroupTip_SetAdmin;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_CANCEL_ADMIN:
                    return kTIMGroupTip_CancelAdmin;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_GROUP_INFO_CHANGE:
                    return kTIMGroupTip_GroupInfoChange;
                case JavaTipType::V2TIM_GROUP_TIPS_TYPE_MEMBER_INFO_CHANGE:
                    return kTIMGroupTip_MemberInfoChange;
                default:
                    return kTIMGroupTip_None;
            }
        }

        int GroupElemHandlerJni::CTipType2JTipType(TIMGroupTipType cTipType) {
            //im c sdk中的群tip类型  转换 im native sdk 中的群tip类型
            switch (cTipType) {
                case kTIMGroupTip_Invite:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_JOIN;
                    //TODO: im sdk 目前暂没有区分用户加群方式
//                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_INVITE;
                case kTIMGroupTip_Quit:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_QUIT;
                case kTIMGroupTip_Kick:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_KICKED;
                case kTIMGroupTip_SetAdmin:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_SET_ADMIN;
                case kTIMGroupTip_CancelAdmin:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_CANCEL_ADMIN;
                case kTIMGroupTip_GroupInfoChange:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_GROUP_INFO_CHANGE;
                case kTIMGroupTip_MemberInfoChange:
                case kTIMGroupTip_MemberMarkChange:
                    return JavaTipType::V2TIM_GROUP_TIPS_TYPE_MEMBER_INFO_CHANGE;
                default:
                    return JavaTipType::GROUP_TIPS_TYPE_INVALID;
            }
        }
    }// namespace jni
}// namespace tim