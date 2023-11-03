//
// Created by Junker on 2022/9/29.
//

#include <jni_helper.h>
#include "conversation_list_filter_jni.h"
#include "java_basic_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass ConversationListFilterJni::j_cls_;
        jfieldID ConversationListFilterJni::j_field_array_[FieldIDMax];
        jmethodID ConversationListFilterJni::j_method_id_array_[MethodIDMax];

        bool ConversationListFilterJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMConversationListFilter");
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

            jfieldID jfield;

            jfield = env->GetFieldID(j_cls_, "conversationType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDConversationType] = jfield;

            jfield = env->GetFieldID(j_cls_, "conversationGroup", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDConversationGroup] = jfield;

            jfield = env->GetFieldID(j_cls_, "markType", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMarkType] = jfield;

            jfield = env->GetFieldID(j_cls_, "hasUnreadCount", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDHasUnreadCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "hasGroupAtInfo", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDHasGroupAtInfo] = jfield;

            return true;
        }

        jobject ConversationListFilterJni::Convert2JObject(const json::Object &listFilter) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject listFilterObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (nullptr == listFilterObj) {
                return nullptr;
            }

            env->SetIntField(listFilterObj, j_field_array_[FieldIDConversationType], listFilter[kTIMConversationListFilterConvType]);
            env->SetLongField(listFilterObj, j_field_array_[FieldIDMarkType], listFilter[kTIMConversationListFilterMarkType].ToInt64());
            env->SetBooleanField(listFilterObj, j_field_array_[FieldIDHasUnreadCount], listFilter[kTIMConversationListFilterHasUnreadCount].ToBool());
            env->SetBooleanField(listFilterObj, j_field_array_[FieldIDHasGroupAtInfo], listFilter[kTIMConversationListFilterHasGroupAtInfo].ToBool());

            jstring groupName = StringJni::Cstring2Jstring(env, listFilter[kTIMConversationListFilterConversationGroup]);
            if (groupName) {
                env->SetObjectField(listFilterObj, j_field_array_[FieldIDConversationGroup], groupName);
                env->DeleteLocalRef(groupName);
            }

            return listFilterObj;
        }

        bool ConversationListFilterJni::Convert2CoreObject(const jobject &filterObj,json::Object &filter_json_obj) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            filter_json_obj[kTIMConversationListFilterConvType] = env->GetIntField(filterObj, j_field_array_[FieldIDConversationType]);
            auto jStr = (jstring) env->GetObjectField(filterObj,j_field_array_[FieldIDConversationGroup]);
            if (jStr){
                filter_json_obj[kTIMConversationListFilterConversationGroup] = StringJni::Jstring2Cstring(env,jStr).c_str();
                env->DeleteLocalRef(jStr);
            }
            filter_json_obj[kTIMConversationListFilterMarkType] = (long long)(env->GetLongField(filterObj, j_field_array_[FieldIDMarkType]));
            filter_json_obj[kTIMConversationListFilterHasUnreadCount] = env->GetBooleanField(filterObj,j_field_array_[FieldIDHasUnreadCount]);
            filter_json_obj[kTIMConversationListFilterHasGroupAtInfo] = env->GetBooleanField(filterObj, j_field_array_[FieldIDHasGroupAtInfo]);

            return true;
        }
    }
}