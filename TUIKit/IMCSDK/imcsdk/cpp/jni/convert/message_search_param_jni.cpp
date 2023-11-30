//
// Created by EDY on 2022/10/8.
//

#include "message_search_param_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMMessageManager.h"

namespace tim {
    namespace jni {
        jclass MessageSearchParamJni::j_cls_;
        jfieldID MessageSearchParamJni::j_field_array_[FieldIDMax];
        jmethodID MessageSearchParamJni::j_method_id_array_[MethodIDMax];

        bool MessageSearchParamJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMessageSearchParam");
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
            jfield = env->GetFieldID(j_cls_, "conversationID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDConversationID] = jfield;

            jfield = env->GetFieldID(j_cls_, "keywordList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDKeywordList] = jfield;

            jfield = env->GetFieldID(j_cls_, "keywordListMatchType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDKeywordListMatchType] = jfield;

            jfield = env->GetFieldID(j_cls_, "senderUserIDList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSenderUserIDList] = jfield;

            jfield = env->GetFieldID(j_cls_, "messageTypeList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMessageTypeList] = jfield;

            jfield = env->GetFieldID(j_cls_, "searchTimePosition", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSearchTimePosition] = jfield;

            jfield = env->GetFieldID(j_cls_, "searchTimePeriod", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSearchTimePeriod] = jfield;

            jfield = env->GetFieldID(j_cls_, "pageSize", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDPageSize] = jfield;

            jfield = env->GetFieldID(j_cls_, "pageIndex", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDPageIndex] = jfield;

            jfield = env->GetFieldID(j_cls_, "searchCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSearchCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "searchCursor", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSearchCursor] = jfield;

            return true;
        }

        bool MessageSearchParamJni::Convert2CoreObject(jobject const &j_obj_searchParam,json::Object &searchParam_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;
            jStr = (jstring) env->GetObjectField(j_obj_searchParam, j_field_array_[FieldIDConversationID]);
            if (jStr) {
                std::string conversationID_str = tim::jni::StringJni::Jstring2Cstring(env, jStr);
                if (conversationID_str.find("c2c_") != std::string::npos) {//单聊会话 c2c_
                    searchParam_json[kTIMMsgSearchParamConvType] = kTIMConv_C2C;
                    searchParam_json[kTIMMsgSearchParamConvId] = conversationID_str.substr(4);
                } else if (conversationID_str.find("group_") != std::string::npos) {//群聊会话 group_xxx
                    searchParam_json[kTIMMsgSearchParamConvType] = kTIMConv_Group;
                    searchParam_json[kTIMMsgSearchParamConvId] = conversationID_str.substr(6);
                } else {
                    searchParam_json[kTIMMsgSearchParamConvType] = kTIMConv_C2C;
                    searchParam_json[kTIMMsgSearchParamConvId] = conversationID_str;
                }
                env->DeleteLocalRef(jStr);
            }

            int size;

            jobject j_obj_keywordList = env->GetObjectField(j_obj_searchParam, j_field_array_[FieldIDKeywordList]);
            if (j_obj_keywordList){
                size = ArrayListJni::Size(j_obj_keywordList);
                if (size > 0){
                    json::Array keyword_array;
                    for (int i = 0; i < size; ++i) {
                        auto j_str_keyword = (jstring) ArrayListJni::Get(j_obj_keywordList, i);
                        std::string keywordStr = StringJni::Jstring2Cstring(env, j_str_keyword);
                        keyword_array.push_back(keywordStr);
                        env->DeleteLocalRef(j_str_keyword);
                    }
                    if (keyword_array.size() > 0){
                        searchParam_json[kTIMMsgSearchParamKeywordArray] = keyword_array;
                    }
                }
                env->DeleteLocalRef(j_obj_keywordList);
            }

            searchParam_json[kTIMMsgSearchParamKeywordListMatchType] = TIMKeywordListMatchType(env->GetIntField(j_obj_searchParam, j_field_array_[FieldIDKeywordListMatchType]));

            jobject j_obj_userIDList = env->GetObjectField(j_obj_searchParam, j_field_array_[FieldIDSenderUserIDList]);
            if (j_obj_userIDList){
                size = ArrayListJni::Size(j_obj_userIDList);
                if (size > 0){
                    json::Array identifier_array;
                    for (int i = 0; i < size; ++i) {
                        auto j_str_userID = (jstring) ArrayListJni::Get(j_obj_keywordList, i);
                        std::string keywordStr = StringJni::Jstring2Cstring(env, j_str_userID);
                        identifier_array.push_back(keywordStr);
                        env->DeleteLocalRef(j_str_userID);
                    }
                    if (identifier_array.size() > 0){
                        searchParam_json[kTIMMsgSearchParamSenderIdentifierArray] = identifier_array;
                    }
                }
                env->DeleteLocalRef(j_obj_userIDList);
            }

            jobject j_obj_messageTypeList = env->GetObjectField(j_obj_searchParam, j_field_array_[FieldIDMessageTypeList]);
            if (j_obj_messageTypeList){
                size = ArrayListJni::Size(j_obj_messageTypeList);
                if (size > 0){
                    json::Array msg_type_array;
                    for (int i = 0; i < size; ++i) {
                        int messageType = IntegerJni::IntValue(ArrayListJni::Get(j_obj_messageTypeList, i));
                        msg_type_array.push_back(messageType);
                    }
                    if (msg_type_array.size() > 0){
                        searchParam_json[kTIMMsgSearchParamMessageTypeArray] = msg_type_array;
                    }
                }
                env->DeleteLocalRef(j_obj_messageTypeList);
            }

            searchParam_json[kTIMMsgSearchParamSearchTimePosition] = (long long) env->GetLongField(j_obj_searchParam, j_field_array_[FieldIDSearchTimePosition]);
            searchParam_json[kTIMMsgSearchParamSearchTimePeriod] = (long long) env->GetLongField(j_obj_searchParam, j_field_array_[FieldIDSearchTimePeriod]);
            searchParam_json[kTIMMsgSearchParamPageSize] = env->GetIntField(j_obj_searchParam, j_field_array_[FieldIDPageSize]);
            searchParam_json[kTIMMsgSearchParamPageIndex] = env->GetIntField(j_obj_searchParam, j_field_array_[FieldIDPageIndex]);

            searchParam_json[kTIMMsgSearchParamSearchCount] = env->GetIntField(j_obj_searchParam, j_field_array_[FieldIDSearchCount]);
            jStr = (jstring) env->GetObjectField(j_obj_searchParam, j_field_array_[FieldIDSearchCursor]);
            if (jStr) {
                searchParam_json[kTIMMsgSearchParamSearchCursor] = tim::jni::StringJni::Jstring2Cstring(env, jStr);;
                env->DeleteLocalRef(jStr);
            }
            return true;
        }

    }// namespace jni
}// namespace tim