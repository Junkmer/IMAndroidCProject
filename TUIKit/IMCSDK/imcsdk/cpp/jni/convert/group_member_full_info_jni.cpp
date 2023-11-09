//
// Created by EDY on 2022/9/19.
//

#include "group_member_full_info_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {

        jclass GroupMemberFullInfoJNI::j_cls_;
        jfieldID GroupMemberFullInfoJNI::j_field_array_[FieldIDMax];
        jmethodID GroupMemberFullInfoJNI::j_method_id_array_[MethodIDMax];

        bool GroupMemberFullInfoJNI::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupMemberFullInfo");
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

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "nickName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDNickName] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendRemark", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFriendRemark] = jfield;

            jfield = env->GetFieldID(j_cls_, "nameCard", "Ljava/lang/String;");
            if (nullptr == jfield){
                return false;
            }
            j_field_array_[FieldIDNameCard] = jfield;

            jfield = env->GetFieldID(j_cls_, "faceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "role", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDRole] = jfield;

            jfield = env->GetFieldID(j_cls_, "muteUntil", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMuteUntil] = jfield;

            jfield = env->GetFieldID(j_cls_, "joinTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDJoinTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "customInfo", "Ljava/util/Map;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDCustomInfo] = jfield;

            return true;
        }

        jobject GroupMemberFullInfoJNI::Convert2JObject(const json::Object &memberFullInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jmemberFullInfoObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);
            if (nullptr == jmemberFullInfoObj) {
                return nullptr;
            }

            env->SetObjectField(jmemberFullInfoObj, j_field_array_[FieldIDUserID], StringJni::Cstring2Jstring(env, memberFullInfo_json[kTIMGroupMemberInfoIdentifier]));
            env->SetObjectField(jmemberFullInfoObj, j_field_array_[FieldIDNickName],
                                StringJni::Cstring2Jstring(env, memberFullInfo_json[kTIMGroupMemberInfoNickName]));
            env->SetObjectField(jmemberFullInfoObj, j_field_array_[FieldIDFriendRemark],
                                StringJni::Cstring2Jstring(env, memberFullInfo_json[kTIMGroupMemberInfoFriendRemark]));
            env->SetObjectField(jmemberFullInfoObj, j_field_array_[FieldIDNameCard],
                                StringJni::Cstring2Jstring(env, memberFullInfo_json[kTIMGroupMemberInfoNameCard]));
            env->SetObjectField(jmemberFullInfoObj, j_field_array_[FieldIDFaceUrl],
                                StringJni::Cstring2Jstring(env, memberFullInfo_json[kTIMGroupMemberInfoFaceUrl]));
            env->SetIntField(jmemberFullInfoObj, j_field_array_[FieldIDRole], memberFullInfo_json[kTIMGroupMemberInfoMemberRole]);
            env->SetLongField(jmemberFullInfoObj, j_field_array_[FieldIDMuteUntil], memberFullInfo_json[kTIMGroupMemberInfoShutupTime].ToInt64());
            env->SetLongField(jmemberFullInfoObj, j_field_array_[FieldIDJoinTime], memberFullInfo_json[kTIMGroupMemberInfoJoinTime].ToInt64());

            if (memberFullInfo_json.HasKey(kTIMGroupMemberInfoCustomInfo)){
                json::Array custom_array = memberFullInfo_json[kTIMGroupMemberInfoCustomInfo];
                jobject mapObj = HashMapJni::NewHashMap();
                for (int i = 0; i < custom_array.size(); ++i) {
                    json::Object item = custom_array[i];
                    jstring key = StringJni::Cstring2Jstring(env, item[kTIMGroupMemberInfoCustomStringInfoKey]);
                    jstring value = StringJni::Cstring2Jstring(env, item[kTIMGroupMemberInfoCustomStringInfoValue]);
                    HashMapJni::Put(mapObj, key, value);

                    env->DeleteLocalRef(key);
                    env->DeleteLocalRef(value);
                }
                env->SetObjectField(jmemberFullInfoObj, j_field_array_[FieldIDCustomInfo], mapObj);
                env->DeleteLocalRef(mapObj);
            }

            return jmemberFullInfoObj;
        }

        bool GroupMemberFullInfoJNI::Convert2CoreObject(jobject const &member_info_obj, json::Object &memberFullInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            auto userIDJStr = (jstring) env->GetObjectField(member_info_obj, j_field_array_[FieldIDUserID]);
            if (userIDJStr) {
                memberFullInfo_json[kTIMGroupModifyMemberInfoParamIdentifier] = StringJni::Jstring2Cstring(env, userIDJStr).c_str();
                env->DeleteLocalRef(userIDJStr);
            }

            int modifyFlag = 0;//修改标记

            auto nameCardJStr = (jstring) env->GetObjectField(member_info_obj, j_field_array_[FieldIDNameCard]);
            if (nameCardJStr) {
                modifyFlag = modifyFlag | TIMGroupMemberModifyInfoFlag::kTIMGroupMemberModifyFlag_NameCard;
                memberFullInfo_json[kTIMGroupModifyMemberInfoParamNameCard] = StringJni::Jstring2Cstring(env, nameCardJStr).c_str();
                env->DeleteLocalRef(nameCardJStr);
            }

            jobject j_obj_customHashMap = env->GetObjectField(member_info_obj, j_field_array_[FieldIDCustomInfo]);
            if (j_obj_customHashMap){
                jobject entry_set = HashMapJni::entrySet(j_obj_customHashMap);
                jobject iterator = HashMapJni::iterator(entry_set);
                int size = HashMapJni::Size(j_obj_customHashMap);
                if (size > 0){
                    json::Array custom_array;
                    while (HashMapJni::hasNext(iterator)) {
                        jobject object = HashMapJni::next(iterator);
                        if (nullptr == object) {
                            continue;
                        }
                        auto jStr_key = (jstring) HashMapJni::getKey(object);
                        if (nullptr != jStr_key) {
                            json::Object item;

                            auto jByte_value = (jbyteArray) HashMapJni::getValue(object);

                            item[kTIMGroupMemberInfoCustomStringInfoKey] = StringJni::Jstring2Cstring(env, jStr_key).c_str();
                            item[kTIMGroupMemberInfoCustomStringInfoValue] = StringJni::JbyteArray2Cstring(env, jByte_value);
                            custom_array.push_back(item);

                            env->DeleteLocalRef(jByte_value);
                            env->DeleteLocalRef(jStr_key);
                        }
                    }

                    if (custom_array.size() > 0){
                        modifyFlag = modifyFlag | TIMGroupMemberModifyInfoFlag::kTIMGroupMemberModifyFlag_Custom;
                        memberFullInfo_json[kTIMGroupModifyMemberInfoParamCustomInfo] = custom_array;
                    }
                }

            }

            //修改群成员资料标记位
            memberFullInfo_json[kTIMGroupModifyMemberInfoParamModifyFlag] = modifyFlag;
            return true;
        }
    }// namespace jni
}// namespace tim
