//
// Created by EDY on 2022/9/28.
//

#include "friend_info_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "user_full_info_jni.h"
#include "TIMFriendshipManager.h"
#include "TIMManager.h"
#include "LogUtil.h"

namespace tim {
    namespace jni {
        jclass FriendInfoJni::j_cls_;
        jfieldID FriendInfoJni::j_field_id_array_[FieldIDMax];
        jmethodID FriendInfoJni::j_method_id_array_[MethodIDMax];

        bool FriendInfoJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendInfo");
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
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addFriendGroup", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddFriendGroup] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "putFriendCustomInfo", "(Ljava/lang/String;[B)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDPutFriendCustomInfo] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendRemark", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDFriendRemark] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendAddTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDFriendAddTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendGroups", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDFriendGroups] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendCustomInfo", "Ljava/util/HashMap;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDFriendCustomInfo] = jfield;

            jfield = env->GetFieldID(j_cls_, "userFullInfo", "Lcom/tencent/imsdk/v2/V2TIMUserFullInfo;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDUserFullInfo] = jfield;

            return true;
        }

        jobject FriendInfoJni::Convert2JObject(const json::Object &friend_info_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject friendInfoObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);
            if (nullptr == friendInfoObj) {
                return nullptr;
            }

            jstring jStr = nullptr;

            jStr = StringJni::Cstring2Jstring(env, friend_info_json[kTIMFriendProfileIdentifier]);
            if (jStr) {
                env->SetObjectField(friendInfoObj, j_field_id_array_[FieldIDUserID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, friend_info_json[kTIMFriendProfileRemark]);
            if (jStr) {
                env->SetObjectField(friendInfoObj, j_field_id_array_[FieldIDFriendRemark], jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetLongField(friendInfoObj, j_field_id_array_[FieldIDFriendAddTime], friend_info_json[kTIMFriendProfileAddTime].ToInt64());


            if (friend_info_json.HasKey(kTIMFriendProfileGroupNameArray)){
                json::Array group_array_json = friend_info_json[kTIMFriendProfileGroupNameArray];
                for (const auto & i : group_array_json) {
                    jstring groupItem = StringJni::Cstring2Jstring(env, i);
                    if (groupItem) {
                        env->CallVoidMethod(friendInfoObj, j_method_id_array_[MethodIDAddFriendGroup], groupItem);
                        env->DeleteLocalRef(groupItem);
                    }
                }
            }

            if (friend_info_json.HasKey(kTIMFriendProfileCustomStringArray)){
                json::Array custom_info_json_array = friend_info_json[kTIMFriendProfileCustomStringArray];
                for (int i = 0; i < custom_info_json_array.size(); ++i) {
                    json::Object item_custom_info = custom_info_json_array[i];
                    jstring keyStr = StringJni::Cstring2Jstring(env, item_custom_info[kTIMUserProfileCustomStringInfoKey]);
                    if (keyStr) {
                        jbyteArray valueByte = StringJni::Cstring2JbyteArray(env, item_custom_info[kTIMUserProfileCustomStringInfoValue]);
                        if (valueByte) {
                            env->CallVoidMethod(friendInfoObj, j_method_id_array_[MethodIDAddFriendGroup], keyStr, valueByte);
                            env->DeleteLocalRef(valueByte);
                        }
                        env->DeleteLocalRef(keyStr);
                    }
                }
            }

            jobject userProfileObj = UserFullInfoJni::Convert2JObject(friend_info_json[kTIMFriendProfileUserProfile]);
            if (userProfileObj) {
                env->SetObjectField(friendInfoObj, j_field_id_array_[FieldIDUserFullInfo], userProfileObj);
                env->DeleteLocalRef(userProfileObj);
            }
            return friendInfoObj;
        }

        jobject FriendInfoJni::Convert2JObject_UpdateFriendInfoCallback(const json::Object &friend_info_item_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject friendInfoObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);
            if (nullptr == friendInfoObj) {
                return nullptr;
            }

            jstring jStr = nullptr;

            std::string userID = friend_info_item_json[kTIMFriendProfileUpdateIdentifier];

            jStr = StringJni::Cstring2Jstring(env, userID);
            if (jStr) {
                env->SetObjectField(friendInfoObj, j_field_id_array_[FieldIDUserID], jStr);
                env->DeleteLocalRef(jStr);
            }

            json::Object item_info_json = friend_info_item_json[kTIMFriendProfileUpdateItem];

            if (item_info_json.HasKey(kTIMFriendProfileItemRemark)) {
                jStr = StringJni::Cstring2Jstring(env, item_info_json[kTIMFriendProfileItemRemark]);
                env->SetObjectField(friendInfoObj, j_field_id_array_[FieldIDFriendRemark], jStr);
                env->DeleteLocalRef(jStr);
            }

            if (item_info_json.HasKey(kTIMFriendProfileItemGroupNameArray)) {
                json::Array group_array_json = item_info_json[kTIMFriendProfileItemGroupNameArray];
                for (int i = 0; i < group_array_json.size(); ++i) {
                    jstring groupItem = StringJni::Cstring2Jstring(env, group_array_json[i]);
                    env->CallVoidMethod(friendInfoObj, j_method_id_array_[MethodIDAddFriendGroup], groupItem);
                    env->DeleteLocalRef(groupItem);
                }
            }

            if (item_info_json.HasKey(kTIMFriendProfileItemCustomStringArray)) {
                json::Array custom_info_json_array = item_info_json[kTIMFriendProfileItemCustomStringArray];
                for (const auto & i : custom_info_json_array) {
                    json::Object item_custom_info = i;
                    jstring keyStr = StringJni::Cstring2Jstring(env, item_custom_info[kTIMFriendProfileCustomStringInfoKey]);
                    jbyteArray valueByte = StringJni::Cstring2JbyteArray(env, item_custom_info[kTIMFriendProfileCustomStringInfoValue]);

                    env->CallVoidMethod(friendInfoObj, j_method_id_array_[MethodIDAddFriendGroup], keyStr, valueByte);
                    env->DeleteLocalRef(valueByte);
                    env->DeleteLocalRef(keyStr);
                }
            }

            jobject userProfileObj = UserFullInfoJni::Convert2JObject_UpdateCallback(friend_info_item_json);
            if (userProfileObj) {
                env->SetObjectField(friendInfoObj, j_field_id_array_[FieldIDUserFullInfo], userProfileObj);
                env->DeleteLocalRef(userProfileObj);
            }

            return friendInfoObj;
        }

        bool FriendInfoJni::Convert2CoreObject(jobject const &j_obj_friendInfo, json::Object &json_modify_friend_profile_item) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            auto friendRemark_jStr = (jstring) env->GetObjectField(j_obj_friendInfo, j_field_id_array_[FieldIDFriendRemark]);
            if (friendRemark_jStr) {
                json_modify_friend_profile_item[kTIMFriendProfileItemRemark] = StringJni::Jstring2Cstring(env, friendRemark_jStr).c_str();
                env->DeleteLocalRef(friendRemark_jStr);
            }

            json::Array json_group_name_array;
            jobject friendGroupList = env->GetObjectField(j_obj_friendInfo, j_field_id_array_[FieldIDFriendGroups]);
            if (friendGroupList) {
                int size = ArrayListJni::Size(friendGroupList);
                for (int i = 0; i < size; ++i) {
                    auto friendGroupObj = (jstring) ArrayListJni::Get(friendGroupList, i);
                    json_group_name_array.push_back(StringJni::Jstring2Cstring(env, friendGroupObj).c_str());
                    env->DeleteLocalRef(friendGroupObj);
                }
                env->DeleteLocalRef(friendGroupList);
            }
            json_modify_friend_profile_item[kTIMFriendProfileItemGroupNameArray] = json_group_name_array;  // 修改好友分组名称列表

            json::Array json_custom_array;
            jobject j_obj_customHashMap = env->GetObjectField(j_obj_friendInfo, j_field_id_array_[FieldIDFriendCustomInfo]);
            jobject entry_set = HashMapJni::entrySet(j_obj_customHashMap);
            jobject iterator = HashMapJni::iterator(entry_set);
            while (HashMapJni::hasNext(iterator)) {
                jobject object = HashMapJni::next(iterator);
                if (nullptr == object) {
                    continue;
                }
                auto jStr_key = (jstring) HashMapJni::getKey(object);
                if (nullptr != jStr_key) {
                    json::Object json_custom_object;
                    json_custom_object[kTIMFriendProfileCustomStringInfoKey] = StringJni::Jstring2Cstring(env, jStr_key);
                    auto jByte_value = (jbyteArray) HashMapJni::getValue(object);
                    json_custom_object[kTIMFriendProfileCustomStringInfoValue] = StringJni::JbyteArray2Cstring(env, jByte_value);
                    json_custom_array.push_back(json_custom_object);

                    env->DeleteLocalRef(jByte_value);
                    env->DeleteLocalRef(jStr_key);
                }
            }
            json_modify_friend_profile_item[kTIMFriendProfileItemCustomStringArray] = json_custom_array;

            json::Object json_modify_friend_info_param;
            auto userID_jStr = (jstring) env->GetObjectField(j_obj_friendInfo, j_field_id_array_[FieldIDUserID]);
            if (userID_jStr) {
                json_modify_friend_info_param[kTIMFriendshipModifyFriendProfileParamIdentifier] = StringJni::Jstring2Cstring(env, userID_jStr).c_str();
                env->DeleteLocalRef(userID_jStr);
            }
            json_modify_friend_info_param[kTIMFriendshipModifyFriendProfileParamItem] = json_modify_friend_profile_item;

            return true;
        }
    }// namespace jni
}// namespace tim