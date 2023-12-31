//
// Created by EDY on 2022/10/8.
//

#include "message_search_result_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "message_search_result_item_jni.h"
#include "TIMMessageManager.h"

namespace tim {
    namespace jni {
        jclass MessageSearchResultJni::j_cls_;
        jfieldID MessageSearchResultJni::j_field_array_[FieldIDMax];
        jmethodID MessageSearchResultJni::j_method_id_array_[MethodIDMax];

        bool MessageSearchResultJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMessageSearchResult");
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

            jmethod = env->GetMethodID(j_cls_, "addMessageSearchResultItem", "(Lcom/tencent/imsdk/v2/V2TIMMessageSearchResultItem;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddMessageSearchResultItem] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "totalCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDTotalCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "messageSearchResultItems", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMessageSearchResultItems] = jfield;

            jfield = env->GetFieldID(j_cls_, "searchCursor", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSearchCursor] = jfield;

            return true;
        }

        jobject MessageSearchResultJni::Convert2JObject(const json::Object &searchResult_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject j_obj_searchResult = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (nullptr == j_obj_searchResult) {
                return nullptr;
            }

            env->SetIntField(j_obj_searchResult, j_field_array_[FieldIDTotalCount], searchResult_json[kTIMMsgSearchResultTotalCount]);

            if (searchResult_json.HasKey(kTIMMsgSearchResultItemArray)) {
                json::Array item_array = searchResult_json[kTIMMsgSearchResultItemArray];
                for (int i = 0; i < item_array.size(); ++i) {
                    jobject j_obj_resultItem = MessageSearchResultItemJni::Convert2JObject(item_array[i]);
                    if (j_obj_resultItem) {
                        env->CallVoidMethod(j_obj_searchResult, j_method_id_array_[MethodIDAddMessageSearchResultItem], j_obj_resultItem);
                        env->DeleteLocalRef(j_obj_resultItem);
                    }
                }
            }

            if (searchResult_json.HasKey(kTIMMsgSearchResultSearchCursor)){
                jobject searchCursor_obj = StringJni::Cstring2Jstring(env,searchResult_json[kTIMMsgSearchResultSearchCursor]);
                if (searchCursor_obj){
                    env->SetObjectField(j_obj_searchResult, j_field_array_[FieldIDSearchCursor], searchCursor_obj);
                    env->DeleteLocalRef(searchCursor_obj);
                }
            }

            return j_obj_searchResult;
        }

    }// namespace jni
}// namespace tim