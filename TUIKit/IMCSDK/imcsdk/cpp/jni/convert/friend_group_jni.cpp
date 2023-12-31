//
// Created by EDY on 2022/9/30.
//

#include "friend_group_jni.h"
#include <jni_helper.h>
#include "java_basic_jni.h"
#include "TIMFriendshipManager.h"

namespace tim {
    namespace jni {
        jclass FriendGroupJni::j_cls_;
        jfieldID FriendGroupJni::j_field_array_[FieldIDMax];
        jmethodID FriendGroupJni::j_method_id_array_[MethodIDMax];

        bool FriendGroupJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendGroup");
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

            jmethod = env->GetMethodID(j_cls_, "addFriendID", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddFriendID] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "name", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDName] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendCount", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFriendCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendIDList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFriendIDList] = jfield;

            return true;
        }

        jobject FriendGroupJni::Convert2JObject(const json::Object &friendGroup_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject friendGroupObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (nullptr == friendGroupObj) {
                return nullptr;
            }

            jstring name_jStr = StringJni::Cstring2Jstring(env, friendGroup_json[kTIMFriendGroupInfoName]);
            if (name_jStr) {
                env->SetObjectField(friendGroupObj, j_field_array_[FieldIDName], name_jStr);
                env->DeleteLocalRef(name_jStr);
            }

            env->SetLongField(friendGroupObj, j_field_array_[FieldIDFriendCount], friendGroup_json[kTIMFriendGroupInfoCount].ToInt64());

            if (friendGroup_json.HasKey(kTIMFriendGroupInfoIdentifierArray)){
                json::Array friendList = friendGroup_json[kTIMFriendGroupInfoIdentifierArray];
                for (const auto & i : friendList) {
                    jstring userId = StringJni::Cstring2Jstring(env,i.ToString());
                    if (userId){
                        env->CallVoidMethod(friendGroupObj,j_method_id_array_[MethodIDAddFriendID],userId);
                        env->DeleteLocalRef(userId);
                    }
                }
            }

            return friendGroupObj;
        }
    }// namespace jni
}// namespace tim