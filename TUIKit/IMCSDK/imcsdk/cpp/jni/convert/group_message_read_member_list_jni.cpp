//
// Created by EDY on 2022/10/8.
//

#include "group_message_read_member_list_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "group_member_info_jni.h"

namespace tim {
    namespace jni {

        jclass GroupMessageReadMemberListJni::j_cls_;
        jfieldID GroupMessageReadMemberListJni::j_field_array_[FieldIDMax];
        jmethodID GroupMessageReadMemberListJni::j_method_id_array_[MethodIDMax];

        bool GroupMessageReadMemberListJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupMessageReadMemberList");
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

            jmethod = env->GetMethodID(j_cls_, "addMemberInfo", "(Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddMemberInfo] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "nextSeq", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDNextSeq] = jfield;

            jfield = env->GetFieldID(j_cls_, "finished", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFinished] = jfield;

            jfield = env->GetFieldID(j_cls_, "memberInfoList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMemberInfoList] = jfield;

            return true;
        }

        jobject GroupMessageReadMemberListJni::Convert2JObject(const json::Array &readMemberList_json,const uint64_t &next_seq,const bool &is_finished) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject j_obj_readMemberList = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!j_obj_readMemberList) {
                return nullptr;
            }
            env->SetLongField(j_obj_readMemberList, j_field_array_[FieldIDNextSeq], (long)next_seq);
            env->SetBooleanField(j_obj_readMemberList, j_field_array_[FieldIDFinished], is_finished);

            for (int i = 0; i < readMemberList_json.size(); ++i) {
                jobject j_obj_readMember = GroupMemberInfoJni::Convert2JObject(readMemberList_json[i]);
                env->CallVoidMethod(j_obj_readMemberList, j_method_id_array_[MethodIDAddMemberInfo], j_obj_readMember);
                env->DeleteLocalRef(j_obj_readMember);
            }

            return j_obj_readMemberList;
        }
    }// namespace jni
}// namespace tim