//
// Created by EDY on 2022/9/16.
//

#include "user_full_info_jni.h"

#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {

        jclass UserFullInfoJni::j_cls;
        jfieldID UserFullInfoJni::j_field_array_[FieldIDMax];
        jmethodID UserFullInfoJni::j_method_id_array_[MethodIDMax];

        bool UserFullInfoJni::InitIDs(JNIEnv *env) {
            if (j_cls != nullptr) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMUserFullInfo");
            if (nullptr == cls) {
                return false;
            }

            j_cls = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls, "nickName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDNickname] = jfield;

            jfield = env->GetFieldID(j_cls, "faceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls, "selfSignature", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSelfSignature] = jfield;

            jfield = env->GetFieldID(j_cls, "gender", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGender] = jfield;

            jfield = env->GetFieldID(j_cls, "role", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDRole] = jfield;

            jfield = env->GetFieldID(j_cls, "level", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDLevel] = jfield;

            jfield = env->GetFieldID(j_cls, "birthday", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDBirthday] = jfield;

            jfield = env->GetFieldID(j_cls, "allowType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDAllowType] = jfield;

            jfield = env->GetFieldID(j_cls, "customHashMap", "Ljava/util/HashMap;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDCustomHashMap] = jfield;

            return true;
        }

        jobject UserFullInfoJni::Convert2JObject(const json::Object &userFullInfo_json_obj) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject juserFullInfoObj = env->NewObject(j_cls, j_method_id_array_[MethodIDConstruct]);
            if (!juserFullInfoObj) {
                return nullptr;
            }

            env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDUserID], StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileIdentifier]));
            env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDNickname], StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileNickName]));
            env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDFaceUrl], StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileFaceUrl]));
            env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDSelfSignature],StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileSelfSignature]));
            env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDGender], userFullInfo_json_obj[kTIMUserProfileGender]);
            env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDRole], userFullInfo_json_obj[kTIMUserProfileRole]);
            env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDLevel], userFullInfo_json_obj[kTIMUserProfileLevel]);
            env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDAllowType], userFullInfo_json_obj[kTIMUserProfileAddPermission]);
            env->SetLongField(juserFullInfoObj, j_field_array_[FieldIDBirthday], userFullInfo_json_obj[kTIMUserProfileBirthDay].ToInt64());

            if (userFullInfo_json_obj.HasKey(kTIMUserProfileCustomStringArray)){
                json::Array custom_array_json = userFullInfo_json_obj[kTIMUserProfileCustomStringArray];
                jobject mapObj = HashMapJni::NewHashMap();
                for (auto & i : custom_array_json) {
                    jstring key = StringJni::Cstring2Jstring(env, i[kTIMUserProfileCustomStringInfoKey]);
                    jbyteArray value = StringJni::Cstring2JbyteArray(env,i[kTIMUserProfileCustomStringInfoValue]);
                    HashMapJni::Put(mapObj, key, value);

                    env->DeleteLocalRef(key);
                    env->DeleteLocalRef(value);
                }
                env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDCustomHashMap], mapObj);
                env->DeleteLocalRef(mapObj);
            }

            return juserFullInfoObj;
        }

        jobject UserFullInfoJni::Convert2JObject_UpdateCallback(const json::Object &friend_info_item_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject juserFullInfoObj = env->NewObject(j_cls, j_method_id_array_[MethodIDConstruct]);
            if (!juserFullInfoObj) {
                return nullptr;
            }

            env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDUserID], StringJni::Cstring2Jstring(env, friend_info_item_json[kTIMFriendProfileUpdateIdentifier]));

            if (friend_info_item_json.HasKey(kTIMFriendProfileUpdateItem)){
                json::Object userFullInfo_json_obj = friend_info_item_json[kTIMFriendProfileUpdateItem];

                env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDNickname], StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileItemNickName]));
                env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDFaceUrl], StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileItemFaceUrl]));
                env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDSelfSignature],StringJni::Cstring2Jstring(env, userFullInfo_json_obj[kTIMUserProfileItemSelfSignature]));
                env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDGender], userFullInfo_json_obj[kTIMUserProfileItemGender]);
                env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDRole], userFullInfo_json_obj[kTIMUserProfileItemRole]);
                env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDLevel], userFullInfo_json_obj[kTIMUserProfileItemLevel]);
                env->SetIntField(juserFullInfoObj, j_field_array_[FieldIDAllowType], userFullInfo_json_obj[kTIMUserProfileItemAddPermission]);
                env->SetLongField(juserFullInfoObj, j_field_array_[FieldIDBirthday], userFullInfo_json_obj[kTIMUserProfileItemBirthDay].ToInt64());

                if (userFullInfo_json_obj.HasKey(kTIMUserProfileItemCustomStringArray)){
                    json::Array custom_array_json = userFullInfo_json_obj[kTIMUserProfileItemCustomStringArray];
                    jobject mapObj = HashMapJni::NewHashMap();
                    for (auto & i : custom_array_json) {
                        jstring key = StringJni::Cstring2Jstring(env, i[kTIMUserProfileCustomStringInfoKey]);
                        jbyteArray value = StringJni::Cstring2JbyteArray(env,i[kTIMUserProfileCustomStringInfoValue]);
                        HashMapJni::Put(mapObj, key, value);

                        env->DeleteLocalRef(key);
                        env->DeleteLocalRef(value);
                    }
                    env->SetObjectField(juserFullInfoObj, j_field_array_[FieldIDCustomHashMap], mapObj);
                    env->DeleteLocalRef(mapObj);
                }
            }

            return juserFullInfoObj;
        }

        bool UserFullInfoJni::Convert2CoreObject(const jobject &j_obj_userFullInfo,json::Object &userFullInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            auto userID_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDUserID]);
            if (userID_jStr) {
                userFullInfo_json[kTIMUserProfileIdentifier] = StringJni::Jstring2Cstring(env, userID_jStr).c_str();
                env->DeleteLocalRef(userID_jStr);
            }

            auto nickName_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDNickname]);
            if (nickName_jStr) {
                userFullInfo_json[kTIMUserProfileNickName] = StringJni::Jstring2Cstring(env, nickName_jStr).c_str();
                env->DeleteLocalRef(nickName_jStr);
            }

            auto faceUrl_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDFaceUrl]);
            if (faceUrl_jStr) {
                userFullInfo_json[kTIMUserProfileFaceUrl] = StringJni::Jstring2Cstring(env, faceUrl_jStr).c_str();
                env->DeleteLocalRef(faceUrl_jStr);
            }

            auto selfSignature_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDSelfSignature]);
            if (selfSignature_jStr) {
                userFullInfo_json[kTIMUserProfileSelfSignature] = StringJni::Jstring2Cstring(env, selfSignature_jStr).c_str();
                env->DeleteLocalRef(selfSignature_jStr);
            }

            int genderInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDGender]);
            if (genderInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileGender] = TIMGenderType(genderInt);
            }

            int roleInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDRole]);
            if (roleInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileRole] = roleInt;
            }

            int levelInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDLevel]);
            if (levelInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileLevel] = levelInt;
            }

            int allowTypeInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDAllowType]);
            if (allowTypeInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileAddPermission] = TIMProfileAddPermission(allowTypeInt);
            }

            long long birthdayLong = env->GetLongField(j_obj_userFullInfo, j_field_array_[FieldIDBirthday]);
            if (birthdayLong != LongJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileBirthDay] = birthdayLong;
            }

            jobject j_obj_customHashMap = env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDCustomHashMap]);
            jobject entry_set = HashMapJni::entrySet(j_obj_customHashMap);
            jobject iterator = HashMapJni::iterator(entry_set);
            while (HashMapJni::hasNext(iterator)) {
                jobject object = HashMapJni::next(iterator);
                if (nullptr == object) {
                    continue;
                }
                auto jStr_key = (jstring) HashMapJni::getKey(object);
                if (nullptr != jStr_key) {
                    auto jByte_value = (jbyteArray) HashMapJni::getValue(object);
                    if (nullptr != jByte_value) {

                        json::Object custom_item;
                        custom_item[kTIMFriendProfileCustomStringInfoKey] = StringJni::Jstring2Cstring(env, jStr_key);
                        custom_item[kTIMFriendProfileCustomStringInfoValue] = StringJni::JbyteArray2Cstring(env, jByte_value);
                        userFullInfo_json[kTIMUserProfileCustomStringArray].ToArray().push_back(custom_item);

                        env->DeleteLocalRef(jByte_value);
                    }
                    env->DeleteLocalRef(jStr_key);
                }
            }

            return true;
        }

        bool UserFullInfoJni::Convert2CoreObject_SetSelfInfo(jobject const &j_obj_userFullInfo, json::Object &userFullInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            auto nickName_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDNickname]);
            if (nickName_jStr) {
                userFullInfo_json[kTIMUserProfileItemNickName] = StringJni::Jstring2Cstring(env, nickName_jStr).c_str();
                env->DeleteLocalRef(nickName_jStr);
            }

            auto faceUrl_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDFaceUrl]);
            if (faceUrl_jStr) {
                userFullInfo_json[kTIMUserProfileItemFaceUrl] = StringJni::Jstring2Cstring(env, faceUrl_jStr).c_str();
                env->DeleteLocalRef(faceUrl_jStr);
            }

            auto selfSignature_jStr = (jstring) env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDSelfSignature]);
            if (selfSignature_jStr) {
                userFullInfo_json[kTIMUserProfileItemSelfSignature] = StringJni::Jstring2Cstring(env, selfSignature_jStr).c_str();
                env->DeleteLocalRef(selfSignature_jStr);
            }

            int genderInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDGender]);
            if (genderInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileItemGender] = TIMGenderType(genderInt);
            }

            int roleInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDRole]);
            if (roleInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileItemRole] = roleInt;
            }

            int levelInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDLevel]);
            if (levelInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileItemLevel] = levelInt;
            }

            int allowTypeInt = env->GetIntField(j_obj_userFullInfo, j_field_array_[FieldIDAllowType]);
            if (allowTypeInt != IntegerJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileItemAddPermission] = TIMProfileAddPermission(allowTypeInt);
            }

            long long birthdayLong = env->GetLongField(j_obj_userFullInfo, j_field_array_[FieldIDBirthday]);
            if (birthdayLong != LongJni::MIN_VALUE()){
                userFullInfo_json[kTIMUserProfileItemBirthDay] = birthdayLong;
            }

            jobject j_obj_customHashMap = env->GetObjectField(j_obj_userFullInfo, j_field_array_[FieldIDCustomHashMap]);
            jobject entry_set = HashMapJni::entrySet(j_obj_customHashMap);
            jobject iterator = HashMapJni::iterator(entry_set);
            while (HashMapJni::hasNext(iterator)) {
                jobject object = HashMapJni::next(iterator);
                if (nullptr == object) {
                    continue;
                }
                auto jStr_key = (jstring) HashMapJni::getKey(object);
                if (nullptr != jStr_key) {
                    auto jByte_value = (jbyteArray) HashMapJni::getValue(object);
                    if (nullptr != jByte_value) {

                        json::Object custom_item;
                        custom_item[kTIMUserProfileCustomStringInfoKey] = StringJni::Jstring2Cstring(env, jStr_key);
                        custom_item[kTIMUserProfileCustomStringInfoValue] = StringJni::JbyteArray2Cstring(env, jByte_value);
                        userFullInfo_json[kTIMUserProfileItemCustomStringArray].ToArray().push_back(custom_item);

                        env->DeleteLocalRef(jByte_value);
                    }
                    env->DeleteLocalRef(jStr_key);
                }
            }

            return true;
        }

    }
}
