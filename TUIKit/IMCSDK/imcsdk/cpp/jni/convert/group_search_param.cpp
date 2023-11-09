//
// Created by Junker on 2022/10/4.
//

#include "group_search_param.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {
        jclass GroupSearchParam::j_cls_;
        jfieldID GroupSearchParam::j_field_array_[FieldIDMax];
        jmethodID GroupSearchParam::j_method_id_array_[MethodIDMax];

        bool GroupSearchParam::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupSearchParam");
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

            jfield = env->GetFieldID(j_cls_, "isSearchGroupID", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSearchGroupName", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsSearchGroupName] = jfield;

            return true;
        }

        bool GroupSearchParam::Convert2CoreObject(jobject const &j_obj_groupSearchParam,json::Object &groupSearchParam_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jobject keywordListObj = env->GetObjectField(j_obj_groupSearchParam,j_field_array_[FieldIDKeywordList]);
            if (keywordListObj){
                json::Array keyword_array;
                int size = ArrayListJni::Size(keywordListObj);
                for (int i = 0; i < size; ++i) {
                    auto keywordStr = (jstring) ArrayListJni::Get(keywordListObj,i);
                    keyword_array.push_back(StringJni::Jstring2Cstring(env,keywordStr));
                    env->DeleteLocalRef(keywordStr);
                }
                groupSearchParam_json[TIMGroupSearchParamKeywordList] = keyword_array;
                env->DeleteLocalRef(keywordListObj);
            }

            json::Array field_array;
            bool isSearchID = (bool) env->GetBooleanField(j_obj_groupSearchParam, j_field_array_[FieldIDIsSearchGroupID]);
            if (isSearchID){
                field_array.push_back(kTIMGroupSearchFieldKey_GroupId);
            }
            bool isSearchName = (bool) env->GetBooleanField(j_obj_groupSearchParam, j_field_array_[FieldIDIsSearchGroupName]);
            if (isSearchName){
                field_array.push_back(kTIMGroupSearchFieldKey_GroupName);
            }
            return true;
        }
    }//namespace jni
}//namespace tim