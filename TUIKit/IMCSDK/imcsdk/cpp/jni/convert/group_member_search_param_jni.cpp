//
// Created by EDY on 2022/9/30.
//

#include "group_member_search_param_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass GroupMemberSearchParamJni::j_cls_;
        jfieldID GroupMemberSearchParamJni::j_field_array_[FieldIDMax];
        jmethodID GroupMemberSearchParamJni::j_method_id_array_[MethodIDMax];

        bool GroupMemberSearchParamJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupMemberSearchParam");
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
            jfield = env->GetFieldID(j_cls_, "groupIDList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupIDList] = jfield;

            jfield = env->GetFieldID(j_cls_, "keywordList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDKeywordList] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchMemberUserID", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchMemberUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchMemberNickName", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchMemberNickName] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchMemberRemark", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchMemberRemark] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchMemberNameCard", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchMemberNameCard] = jfield;

            return true;
        }

        bool GroupMemberSearchParamJni::Convert2CoreObject(jobject const &j_obj_memberSearchParam,json::Object &memberSearchParam_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jobject groupIDListObj = env->GetObjectField(j_obj_memberSearchParam,j_field_array_[FieldIDGroupIDList]);
            if (groupIDListObj){
                json::Array groupID_array;
                int size = ArrayListJni::Size(groupIDListObj);
                for (int i = 0; i < size; ++i) {
                    auto groupIDStr = (jstring) ArrayListJni::Get(groupIDListObj,i);
                    groupID_array.push_back(StringJni::Jstring2Cstring(env,groupIDStr));
                    env->DeleteLocalRef(groupIDStr);
                }
                memberSearchParam_json[TIMGroupMemberSearchParamGroupidList] = groupID_array;
                env->DeleteLocalRef(groupIDListObj);
            }

            jobject keywordListObj = env->GetObjectField(j_obj_memberSearchParam,j_field_array_[FieldIDKeywordList]);
            if (keywordListObj){
                json::Array keyword_array;
                int size = ArrayListJni::Size(keywordListObj);
                for (int i = 0; i < size; ++i) {
                    auto keywordStr = (jstring) ArrayListJni::Get(keywordListObj,i);
                    keyword_array.push_back(StringJni::Jstring2Cstring(env,keywordStr));
                    env->DeleteLocalRef(keywordStr);
                }
                memberSearchParam_json[TIMGroupMemberSearchParamKeywordList] = keyword_array;
                env->DeleteLocalRef(keywordListObj);
            }

            json::Array field_key_array;
            bool isSearchUserID = env->GetBooleanField(j_obj_memberSearchParam, j_field_array_[FieldIDIsSearchMemberUserID]);
            if (isSearchUserID){
                field_key_array.push_back(kTIMGroupMemberSearchFieldKey_Identifier);
            }
            bool isSearchNickName  = env->GetBooleanField(j_obj_memberSearchParam, j_field_array_[FieldIDIsSearchMemberNickName]);
            if (isSearchNickName){
                field_key_array.push_back(kTIMGroupMemberSearchFieldKey_NickName);
            }
            bool isSearchRemark  = env->GetBooleanField(j_obj_memberSearchParam, j_field_array_[FieldIDIsSearchMemberRemark]);
            if (isSearchRemark){
                field_key_array.push_back(kTIMGroupMemberSearchFieldKey_Remark);
            }
            bool isSearchNameCard  = env->GetBooleanField(j_obj_memberSearchParam, j_field_array_[FieldIDIsSearchMemberNameCard]);
            if (isSearchNameCard){
                field_key_array.push_back(kTIMGroupMemberSearchFieldKey_NameCard);
            }

            return true;
        }
    }//namespace jni
}//namespace tim