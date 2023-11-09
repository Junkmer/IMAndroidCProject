//
// Created by EDY on 2023/11/2.
//

#include "message_reaction_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "user_info_jni.h"
#include "TIMMessageManager.h"

namespace tim {
    namespace jni {

        jclass MessageReactionJni::j_cls_;
        jfieldID MessageReactionJni::j_field_array_[FieldIDMax];
        jmethodID MessageReactionJni::j_method_id_array[MethodIDMax];

        bool MessageReactionJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMessageReaction");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_,"reactionID", "Ljava/lang/String;");
            if (nullptr == jfield){
                return false;
            }
            j_field_array_[FieldIDReactionID] = jfield;

            jfield = env->GetFieldID(j_cls_,"totalUserCount", "I");
            if (nullptr == jfield){
                return false;
            }
            j_field_array_[FieldIDTotalUserCount] = jfield;

            jfield = env->GetFieldID(j_cls_,"partialUserList", "Ljava/util/List;");
            if (nullptr == jfield){
                return false;
            }
            j_field_array_[FieldIDPartialUserList] = jfield;

            jfield = env->GetFieldID(j_cls_,"reactedByMyself", "Z");
            if (nullptr == jfield){
                return false;
            }
            j_field_array_[FieldIDReactedByMyself] = jfield;

            return true;
        }

        jobject MessageReactionJni::Convert2JObject(const json::Object &messageReaction_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject messageReactionObj = env->NewObject(j_cls_, j_method_id_array[MethodIDConstructor]);
            if (nullptr == messageReactionObj) {
                return nullptr;
            }

            jstring jStr;
            jStr = StringJni::Cstring2Jstring(env, messageReaction_json[kTIMMsgReactionID]);
            if (jStr) {
                env->SetObjectField(messageReactionObj, j_field_array_[FieldIDReactionID], jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetIntField(messageReactionObj, j_field_array_[FieldIDTotalUserCount], messageReaction_json[kTIMMsgReactionTotalUserCount]);

            if (messageReaction_json.HasKey(kTIMMsgReactionPartialUserInfoList)){
                json::Array user_array = messageReaction_json[kTIMMsgReactionPartialUserInfoList];
                jobject userListObj =  ArrayListJni::NewArrayList();
                for (const auto &item : user_array){
                    jobject user_obj = UserInfoJni::Convert2JObject(item);
                    if (user_obj){
                        ArrayListJni::Add(userListObj,user_obj);
                        env->DeleteLocalRef(user_obj);
                    }
                }
                env->SetObjectField(messageReactionObj,j_field_array_[FieldIDPartialUserList],userListObj);
                env->DeleteLocalRef(userListObj);
            }
            //TODO::im c sdk 当前暂为有该字段，待实现。
//            env->GetBooleanField(messageReactionObj,j_field_array_[FieldIDReactedByMyself], false);
            return messageReactionObj;
        }
    }
}