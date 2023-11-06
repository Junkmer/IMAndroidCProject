//
// Created by EDY on 2022/9/30.
//

#include "friend_search_param_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass FriendSearchParamJni::j_cls_;
        jfieldID FriendSearchParamJni::j_field_array_[FieldIDMax];
        jmethodID FriendSearchParamJni::j_method_id_array_[MethodIDMax];

        bool FriendSearchParamJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendSearchParam");
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

            jfield = env->GetFieldID(j_cls_, "keywordList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDKeywordList] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchUserID", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchNickName", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchNickName] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchRemark", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchRemark] = jfield;
            return true;
        }

        bool FriendSearchParamJni::Convert2CoreObject(const jobject &j_obj_searchParam,json::Object &searchParam_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jobject keywordListObj = env->GetObjectField(j_obj_searchParam,j_field_array_[FieldIDKeywordList]);
            int size = ArrayListJni::Size(keywordListObj);
            if (size > 0){
                json::Array keyword_array;
                for (int i = 0; i < size; ++i) {
                    auto keywordJStr = (jstring) ArrayListJni::Get(keywordListObj,i);
                    if (keywordJStr){
                        keyword_array.push_back(StringJni::Jstring2Cstring(env,keywordJStr).c_str());
                        env->DeleteLocalRef(keywordJStr);
                    }
                }
                env->DeleteLocalRef(keywordListObj);
                searchParam_json[kTIMFriendshipSearchParamKeywordList] = keyword_array;
            }

            json::Array search_field_array;
            bool isSearchUserID = env->GetBooleanField(j_obj_searchParam,j_field_array_[FieldIDIsSearchUserID]);
            if (isSearchUserID){
                search_field_array.push_back(TIMFriendshipSearchFieldKey::kTIMFriendshipSearchFieldKey_Identifier);
            }
            bool isSearchNickName = env->GetBooleanField(j_obj_searchParam,j_field_array_[FieldIDIsSearchNickName]);
            if (isSearchNickName){
                search_field_array.push_back(TIMFriendshipSearchFieldKey::kTIMFriendshipSearchFieldKey_NickName);
            }
            bool isSearchRemark = env->GetBooleanField(j_obj_searchParam,j_field_array_[FieldIDIsSearchRemark]);
            if (isSearchRemark){
                search_field_array.push_back(TIMFriendshipSearchFieldKey::kTIMFriendshipSearchFieldKey_Remark);
            }

            searchParam_json[kTIMFriendshipSearchParamSearchFieldList] = search_field_array;
            return true;
        }
    }
}