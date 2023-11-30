//
// Created by EDY on 2022/8/22.
//

#include <jni.h>
#include <LogUtil.h>
#include "jni_util.h"
#include "java_basic_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "tim_callback_impl.h"
#include "value_callback_impl.h"
#include "im_callbak_jni.h"
#include "group_topic_info_jni.h"
#include "group_member_info_jni.h"
#include "group_info_jni.h"
#include "group_application_jni.h"
#include "group_member_change_info_jni.h"
#include "group_member_full_info_jni.h"
#include "group_info_result_jni.h"
#include "group_member_info_result_jni.h"
#include "group_application_result_jni.h"
#include "group_member_operation_result_jni.h"
#include "group_member_search_param_jni.h"
#include "create_group_member_info_jni.h"
#include "group_search_param.h"
#include "topic_operation_result_jni.h"
#include "topic_info_result_jni.h"
#include "TIMGroupManager.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_NATIVE_FUNC(void, NativeCreateGroupToParame, jobject info, jobject member_list, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_object_param;
    tim::jni::GroupInfoJni::Convert2CoreObject_Create(info, json_object_param);

    json::Array member_array;
    int size = tim::jni::ArrayListJni::Size(member_list);
    for (int i = 0; i < size; ++i) {
        json::Object memberInfo;
        jobject j_obj_member = tim::jni::ArrayListJni::Get(member_list, i);
        bool flag = tim::jni::CreateGroupMemberInfoJni::Convert2CoreObject(j_obj_member, memberInfo);
        if (flag) {
            member_array.push_back(memberInfo);
        }
        env->DeleteLocalRef(j_obj_member);
    }
    json_object_param[kTIMCreateGroupParamGroupMemberArray] = member_array;

    std::string paramStr = json::Serialize(json_object_param);

    tim::TIMEngine::GetInstance()->CreateGroup(paramStr.c_str(), [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Object param;
            param = json::Deserialize(json_params);
            std::string groupId = param[kTIMCreateGroupResultGroupId];

            tim::jni::IMCallbackJNI::Success(_callback, tim::jni::StringJni::Cstring2Jstring(scopedJEnv.GetEnv(), groupId));
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetJoinedGroupList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetJoinedGroupList([](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array group_array = json::Deserialize(json_params);
            jobject joinedGroupList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < group_array.size(); ++i) {
                jobject groupInfoObj = tim::jni::GroupInfoJni::Convert2JObject(group_array[i]);
                tim::jni::ArrayListJni::Add(joinedGroupList, groupInfoObj);
                _env->DeleteLocalRef(groupInfoObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, joinedGroupList);
            _env->DeleteLocalRef(joinedGroupList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupsInfo, jobject group_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array groupIDList;
    int size = tim::jni::ArrayListJni::Size(group_idlist);
    for (int i = 0; i < size; ++i) {
        std::string groupID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(group_idlist, i));
        groupIDList.push_back(groupID);
    }
    std::string groupIDListStr = json::Serialize(groupIDList);

    tim::TIMEngine::GetInstance()->GetGroupsInfo(groupIDListStr.c_str(),
                                                 [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                     tim::jni::ScopedJEnv scopedJEnv;
                                                     auto _env = scopedJEnv.GetEnv();
                                                     auto _callback = (jobject) user_data;

                                                     if (TIMErrCode::ERR_SUCC == code) {
                                                         json::Array result_array = json::Deserialize(json_params);
                                                         jobject groupInfoResultList = tim::jni::ArrayListJni::NewArrayList();
                                                         for (int i = 0; i < result_array.size(); ++i) {
                                                             jobject groupInfoResultObj = tim::jni::GroupInfoResultJni::Convert2JObject(
                                                                     result_array[i]);
                                                             tim::jni::ArrayListJni::Add(groupInfoResultList, groupInfoResultObj);
                                                             _env->DeleteLocalRef(groupInfoResultObj);
                                                         }
                                                         tim::jni::IMCallbackJNI::Success(_callback, groupInfoResultList);
                                                         _env->DeleteLocalRef(groupInfoResultList);

                                                     } else {
                                                         tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                     }
                                                     _env->DeleteGlobalRef(_callback);
                                                 }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSearchGroups, jobject search_param, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object searchParam;
    tim::jni::GroupSearchParam::Convert2CoreObject(search_param, searchParam);
    std::string searchParamStr = json::Serialize(searchParam);

    tim::TIMEngine::GetInstance()->SearchGroups(searchParamStr.c_str(),
                                                [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                    tim::jni::ScopedJEnv scopedJEnv;
                                                    auto _env = scopedJEnv.GetEnv();
                                                    auto _callback = (jobject) user_data;

                                                    if (TIMErrCode::ERR_SUCC == code) {
                                                        json::Array group_array = json::Deserialize(json_params);
                                                        jobject joinedGroupList = tim::jni::ArrayListJni::NewArrayList();
                                                        for (int i = 0; i < group_array.size(); ++i) {
                                                            jobject groupInfoObj = tim::jni::GroupInfoJni::Convert2JObject(group_array[i]);
                                                            tim::jni::ArrayListJni::Add(joinedGroupList, groupInfoObj);
                                                            _env->DeleteLocalRef(groupInfoObj);
                                                        }
                                                        tim::jni::IMCallbackJNI::Success(_callback, joinedGroupList);
                                                        _env->DeleteLocalRef(joinedGroupList);

                                                    } else {
                                                        tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                    }
                                                    _env->DeleteGlobalRef(_callback);
                                                }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupInfo, jobject info, jobject callback) {
    json::Object groupInfo;
    tim::jni::GroupInfoJni::Convert2CoreObject_Update(info, groupInfo);

    tim::TIMEngine::GetInstance()->SetGroupInfo(json::Serialize(groupInfo).c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeInitGroupAttributes, jstring group_id, jobject attributes, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array attributeMap;
    jobject entry_set = tim::jni::HashMapJni::entrySet(attributes);
    jobject iterator = tim::jni::HashMapJni::iterator(entry_set);
    while (tim::jni::HashMapJni::hasNext(iterator)) {
        jobject object = tim::jni::HashMapJni::next(iterator);
        if (nullptr == object) {
            continue;
        }
        auto jStr_key = (jstring) tim::jni::HashMapJni::getKey(object);
        if (nullptr != jStr_key) {
            auto jByte_value = (jstring) tim::jni::HashMapJni::getValue(object);
            std::string c_key_str = tim::jni::StringJni::Jstring2Cstring(env, jStr_key);
            std::string c_value_str = tim::jni::StringJni::Jstring2Cstring(env, jByte_value);
            json::Object json_obj;
            json_obj[TIMGroupAttributeKey] = c_key_str;
            json_obj[TIMGroupAttributeValue] = c_value_str;
            attributeMap.push_back(json_obj);

            env->DeleteLocalRef(jByte_value);
            env->DeleteLocalRef(jStr_key);
        }
    }
    std::string attributeMapStr = json::Serialize(attributeMap);

    tim::TIMEngine::GetInstance()->InitGroupAttributes(groupIDStr.c_str(), attributeMapStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupAttributes, jstring group_id, jobject attributes, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array attributeMap;
    jobject entry_set = tim::jni::HashMapJni::entrySet(attributes);
    jobject iterator = tim::jni::HashMapJni::iterator(entry_set);
    while (tim::jni::HashMapJni::hasNext(iterator)) {
        jobject object = tim::jni::HashMapJni::next(iterator);
        if (nullptr == object) {
            continue;
        }
        auto jStr_key = (jstring) tim::jni::HashMapJni::getKey(object);
        if (nullptr != jStr_key) {
            auto jByte_value = (jstring) tim::jni::HashMapJni::getValue(object);
            std::string c_key_str = tim::jni::StringJni::Jstring2Cstring(env, jStr_key);
            std::string c_value_str = tim::jni::StringJni::Jstring2Cstring(env, jByte_value);
            json::Object json_obj;
            json_obj[TIMGroupAttributeKey] = c_key_str;
            json_obj[TIMGroupAttributeValue] = c_value_str;
            attributeMap.push_back(json_obj);

            env->DeleteLocalRef(jByte_value);
            env->DeleteLocalRef(jStr_key);
        }
    }
    std::string attributeMapStr = json::Serialize(attributeMap);

    tim::TIMEngine::GetInstance()->SetGroupAttributes(groupIDStr.c_str(), attributeMapStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDeleteGroupAttributes, jstring group_id, jobject keys, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array keyList;
    int size = tim::jni::ArrayListJni::Size(keys);
    for (int i = 0; i < size; ++i) {
        std::string groupID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(keys, i));
        keyList.push_back(groupID);
    }
    std::string keyListStr = json::Serialize(keyList);

    tim::TIMEngine::GetInstance()->DeleteGroupAttributes(groupIDStr.c_str(), keyListStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupAttributes, jstring group_id, jobject keys, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array keyList;
    int size = tim::jni::ArrayListJni::Size(keys);
    for (int i = 0; i < size; ++i) {
        std::string groupID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(keys, i));
        keyList.push_back(groupID);
    }
    std::string keyListStr = json::Serialize(keyList);

    tim::TIMEngine::GetInstance()->GetGroupAttributes(groupIDStr.c_str(), keyListStr.c_str(),
                                                      [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                          tim::jni::ScopedJEnv scopedJEnv;
                                                          auto _env = scopedJEnv.GetEnv();
                                                          auto _callback = (jobject) user_data;

                                                          if (TIMErrCode::ERR_SUCC == code) {
                                                              json::Array attribute_array = json::Deserialize(json_params);
                                                              jobject j_obj_customHashMap = tim::jni::HashMapJni::NewHashMap();
                                                              for (const auto &item: attribute_array) {
                                                                  jstring keyStr = tim::jni::StringJni::Cstring2Jstring(_env,
                                                                                                                        item[TIMGroupAttributeKey]);
                                                                  if (keyStr) {
                                                                      jstring valueStr = tim::jni::StringJni::Cstring2Jstring(_env,
                                                                                                                              item[TIMGroupAttributeValue]);
                                                                      if (valueStr) {
                                                                          tim::jni::HashMapJni::Put(j_obj_customHashMap, keyStr, valueStr);
                                                                          _env->DeleteLocalRef(valueStr);
                                                                      }
                                                                      _env->DeleteLocalRef(keyStr);
                                                                  }
                                                              }
                                                              tim::jni::IMCallbackJNI::Success(_callback, j_obj_customHashMap);
                                                              _env->DeleteLocalRef(j_obj_customHashMap);
                                                          } else {
                                                              tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                          }
                                                          _env->DeleteGlobalRef(_callback);
                                                      }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupOnlineMemberCount, jstring group_id, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    tim::TIMEngine::GetInstance()->GetGroupOnlineMemberCount(groupIDStr.c_str(),
                                                             [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                 tim::jni::ScopedJEnv scopedJEnv;
                                                                 auto _env = scopedJEnv.GetEnv();
                                                                 auto _callback = (jobject) user_data;

                                                                 if (TIMErrCode::ERR_SUCC == code) {
                                                                     json::Object result_obj = json::Deserialize(json_params);
                                                                     int onlineMemberCount = result_obj[TIMGroupGetOnlineMemberCountResult];
                                                                     tim::jni::IMCallbackJNI::Success(_callback, tim::jni::IntegerJni::NewIntegerObj(
                                                                             onlineMemberCount));
                                                                 } else {
                                                                     tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                                 }
                                                                 _env->DeleteGlobalRef(_callback);
                                                             }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupCounters, jstring group_id, jobject counters, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array json_group_counter_array;
    jobject entry_set = tim::jni::HashMapJni::entrySet(counters);
    jobject iterator = tim::jni::HashMapJni::iterator(entry_set);
    while (tim::jni::HashMapJni::hasNext(iterator)) {
        jobject object = tim::jni::HashMapJni::next(iterator);
        if (nullptr == object) {
            continue;
        }
        auto jStr_key = (jstring) tim::jni::HashMapJni::getKey(object);
        if (nullptr != jStr_key) {
            auto jLong_value = (jlong) tim::jni::HashMapJni::getValue(object);

            json::Object json_group_counter;
            json_group_counter[kTIMGroupCounterKey] = tim::jni::StringJni::Jstring2Cstring(env, jStr_key);
            json_group_counter[kTIMGroupCounterValue] = (long long ) jLong_value;
            json_group_counter_array.push_back(json_group_counter);

            env->DeleteLocalRef(jStr_key);
        }
    }
    std::string paramStr = json::Serialize(json_group_counter_array);
    tim::TIMEngine::GetInstance()->GroupSetGroupCounters(groupStr.c_str(),paramStr.c_str(),[](int32_t code, const char* desc, const char*
    json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array counter_array = json::Deserialize(json_params);
            jobject j_obj_hash_map = tim::jni::HashMapJni::NewHashMap();
            for (const auto &item: counter_array) {
                jstring keyStr = tim::jni::StringJni::Cstring2Jstring(_env,item[kTIMGroupCounterKey]);
                if (keyStr) {
                    jobject valueLong = tim::jni::LongJni::NewLongObj(item[kTIMGroupCounterValue].ToInt64());
                    if (valueLong) {
                        tim::jni::HashMapJni::Put(j_obj_hash_map, keyStr, valueLong);
                        _env->DeleteLocalRef(valueLong);
                    }
                    _env->DeleteLocalRef(keyStr);
                }
            }
            tim::jni::IMCallbackJNI::Success(_callback, j_obj_hash_map);
            _env->DeleteLocalRef(j_obj_hash_map);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupCounters, jstring group_id, jobject keys, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array key_array;
    int size = tim::jni::ArrayListJni::Size(keys);
    for (int i = 0; i < size; ++i) {
        std::string key = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(keys, i));
        key_array.push_back(key);
    }
    std::string paramStr = json::Serialize(key_array);
    tim::TIMEngine::GetInstance()->GroupGetGroupCounters(groupStr.c_str(),paramStr.c_str(),[](int32_t code, const char* desc, const char*
    json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array counter_array = json::Deserialize(json_params);
            jobject j_obj_hash_map = tim::jni::HashMapJni::NewHashMap();
            for (const auto &item: counter_array) {
                jstring keyStr = tim::jni::StringJni::Cstring2Jstring(_env,item[kTIMGroupCounterKey]);
                if (keyStr) {
                    jobject valueLong = tim::jni::LongJni::NewLongObj(item[kTIMGroupCounterValue].ToInt64());
                    if (valueLong) {
                        tim::jni::HashMapJni::Put(j_obj_hash_map, keyStr, valueLong);
                        _env->DeleteLocalRef(valueLong);
                    }
                    _env->DeleteLocalRef(keyStr);
                }
            }
            tim::jni::IMCallbackJNI::Success(_callback, j_obj_hash_map);
            _env->DeleteLocalRef(j_obj_hash_map);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeIncreaseGroupCounter, jstring group_id, jstring key, jlong value,jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    std::string keyStr = tim::jni::StringJni::Jstring2Cstring(env, key);

    tim::TIMEngine::GetInstance()->GroupIncreaseGroupCounter(groupStr.c_str(),keyStr.c_str(),value,[](int32_t code, const char* desc, const char*
    json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array counter_array = json::Deserialize(json_params);
            jobject j_obj_hash_map = tim::jni::HashMapJni::NewHashMap();
            for (const auto &item: counter_array) {
                jstring keyStr = tim::jni::StringJni::Cstring2Jstring(_env,item[kTIMGroupCounterKey]);
                if (keyStr) {
                    jobject valueLong = tim::jni::LongJni::NewLongObj(item[kTIMGroupCounterValue].ToInt64());
                    if (valueLong) {
                        tim::jni::HashMapJni::Put(j_obj_hash_map, keyStr, valueLong);
                        _env->DeleteLocalRef(valueLong);
                    }
                    _env->DeleteLocalRef(keyStr);
                }
            }
            tim::jni::IMCallbackJNI::Success(_callback, j_obj_hash_map);
            _env->DeleteLocalRef(j_obj_hash_map);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDecreaseGroupCounter, jstring group_id, jstring key, jlong value,jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    std::string keyStr = tim::jni::StringJni::Jstring2Cstring(env, key);

    tim::TIMEngine::GetInstance()->GroupDecreaseGroupCounter(groupStr.c_str(),keyStr.c_str(),value,[](int32_t code, const char* desc, const char*
    json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array counter_array = json::Deserialize(json_params);
            jobject j_obj_hash_map = tim::jni::HashMapJni::NewHashMap();
            for (const auto &item: counter_array) {
                jstring keyStr = tim::jni::StringJni::Cstring2Jstring(_env,item[kTIMGroupCounterKey]);
                if (keyStr) {
                    jobject valueLong = tim::jni::LongJni::NewLongObj(item[kTIMGroupCounterValue].ToInt64());
                    if (valueLong) {
                        tim::jni::HashMapJni::Put(j_obj_hash_map, keyStr, valueLong);
                        _env->DeleteLocalRef(valueLong);
                    }
                    _env->DeleteLocalRef(keyStr);
                }
            }
            tim::jni::IMCallbackJNI::Success(_callback, j_obj_hash_map);
            _env->DeleteLocalRef(j_obj_hash_map);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupMemberList, jstring group_id, jint filter, jlong next_seq, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object member_list_json;
    member_list_json[kTIMGroupGetMemberInfoListParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    member_list_json[kTIMGroupGetMemberInfoListParamNextSeq] = (long long) next_seq;
    json::Object filter_json;
    filter_json[kTIMGroupMemberGetInfoOptionRoleFlag] = TIMGroupMemberRoleFlag(filter);
    member_list_json[kTIMGroupGetMemberInfoListParamOption] = filter_json;
    std::string paramStr = json::Serialize(member_list_json);

    tim::TIMEngine::GetInstance()->GetGroupMemberList(paramStr.c_str(),
                                                      [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                          tim::jni::ScopedJEnv scopedJEnv;
                                                          auto _env = scopedJEnv.GetEnv();
                                                          auto _callback = (jobject) user_data;

                                                          if (TIMErrCode::ERR_SUCC == code) {
                                                              json::Object result_json = json::Deserialize(json_params);
                                                              jobject memberInfoResultObj = tim::jni::GroupMemberInfoResultJni::Convert2JObject(
                                                                      result_json);
                                                              tim::jni::IMCallbackJNI::Success(_callback, memberInfoResultObj);
                                                              _env->DeleteLocalRef(memberInfoResultObj);

                                                          } else {
                                                              tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                          }
                                                          _env->DeleteGlobalRef(_callback);
                                                      }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupMembersInfo, jstring group_id, jobject member_list, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Object get_member_info_opt;
    get_member_info_opt[kTIMGroupGetMemberInfoListParamGroupId] = group_id;
    json::Array identifiers;
    int size = tim::jni::ArrayListJni::Size(member_list);
    for (int i = 0; i < size; ++i) {
        std::string memberID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(member_list, i));
        identifiers.push_back(memberID);
    }
    get_member_info_opt[kTIMGroupGetMemberInfoListParamIdentifierArray] = identifiers;
    std::string paramStr = json::Serialize(get_member_info_opt);

    tim::TIMEngine::GetInstance()->GetGroupMemberList(paramStr.c_str(),
                                                      [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                          tim::jni::ScopedJEnv scopedJEnv;
                                                          auto _env = scopedJEnv.GetEnv();
                                                          auto _callback = (jobject) user_data;

                                                          if (TIMErrCode::ERR_SUCC == code) {
                                                              json::Object result_json = json::Deserialize(json_params);
                                                              json::Array member_array = result_json[kTIMGroupGetMemberInfoListResultInfoArray];
                                                              jobject memberFullInfoObjList = tim::jni::ArrayListJni::NewArrayList();
                                                              for (const auto &item: member_array) {
                                                                  jobject memberFullInfoObj = tim::jni::GroupMemberFullInfoJNI::Convert2JObject(item);
                                                                  tim::jni::ArrayListJni::Add(memberFullInfoObjList, memberFullInfoObj);
                                                                  _env->DeleteLocalRef(memberFullInfoObj);
                                                              }
                                                              tim::jni::IMCallbackJNI::Success(_callback, memberFullInfoObjList);
                                                              _env->DeleteLocalRef(memberFullInfoObjList);

                                                          } else {
                                                              tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                          }
                                                          _env->DeleteGlobalRef(_callback);
                                                      }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSearchGroupMembers, jobject param, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object searchParam;
    tim::jni::GroupMemberSearchParamJni::Convert2CoreObject(param, searchParam);
    std::string paramStr = json::Serialize(searchParam);

    tim::TIMEngine::GetInstance()->SearchGroupMembers(paramStr.c_str(),
                                                      [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                          tim::jni::ScopedJEnv scopedJEnv;
                                                          auto _env = scopedJEnv.GetEnv();
                                                          auto _callback = (jobject) user_data;

                                                          if (TIMErrCode::ERR_SUCC == code) {
                                                              json::Array result_array = json::Deserialize(json_params);
                                                              jobject j_obj_membersMapMap = tim::jni::HashMapJni::NewHashMap();
                                                              for (const auto &item: result_array) {
                                                                  jstring keyStr = tim::jni::StringJni::Cstring2Jstring(_env,
                                                                                                                        item[TIMGroupSearchGroupMembersdResultGroupID]);
                                                                  if (keyStr) {
                                                                      json::Array member_array = item[TIMGroupSearchGroupMembersdResultMemberInfoList];
                                                                      jobject valueObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                      for (const auto &i: member_array) {
                                                                          jobject memberObj = tim::jni::GroupMemberFullInfoJNI::Convert2JObject(i);
                                                                          tim::jni::ArrayListJni::Add(valueObjList, memberObj);
                                                                          _env->DeleteLocalRef(memberObj);
                                                                      }
                                                                      tim::jni::HashMapJni::Put(j_obj_membersMapMap, keyStr, valueObjList);
                                                                      _env->DeleteLocalRef(keyStr);
                                                                      _env->DeleteLocalRef(valueObjList);
                                                                  }
                                                              }
                                                              tim::jni::IMCallbackJNI::Success(_callback, j_obj_membersMapMap);
                                                              _env->DeleteLocalRef(j_obj_membersMapMap);

                                                          } else {
                                                              tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                          }
                                                          _env->DeleteGlobalRef(_callback);
                                                      }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupMemberInfo, jstring group_id, jobject info, jobject callback) {
    json::Object set_group_member_info_param;
    set_group_member_info_param[kTIMGroupModifyMemberInfoParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    tim::jni::GroupMemberFullInfoJNI::Convert2CoreObject(info, set_group_member_info_param);
    std::string paramStr = json::Serialize(set_group_member_info_param);

    tim::TIMEngine::GetInstance()->SetGroupMemberInfo(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeMuteGroupMember, jstring group_id, jstring user_id, jint seconds, jobject callback) {
    json::Object mute_json;
    mute_json[kTIMGroupModifyMemberInfoParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    mute_json[kTIMGroupModifyMemberInfoParamIdentifier] = tim::jni::StringJni::Jstring2Cstring(env, user_id);
    mute_json[kTIMGroupModifyMemberInfoParamModifyFlag] = TIMGroupMemberModifyInfoFlag::kTIMGroupMemberModifyFlag_ShutupTime;
    mute_json[kTIMGroupModifyMemberInfoParamShutupTime] = seconds;
    std::string paramStr = json::Serialize(mute_json);

    tim::TIMEngine::GetInstance()->SetGroupMemberInfo(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeMuteAllGroupMembers, jstring group_id, jboolean is_mute, jobject callback) {
    json::Object all_mute_json;
    all_mute_json[kTIMGroupModifyInfoParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env,group_id);
    all_mute_json[kTIMGroupModifyInfoParamModifyFlag] = TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_ShutupAll;
    all_mute_json[kTIMGroupModifyInfoParamIsShutupAll] = (bool) is_mute;
    std::string paramStr = json::Serialize(all_mute_json);
    tim::TIMEngine::GetInstance()->SetGroupInfo(paramStr.c_str(),new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeInviteUserToGroup, jstring group_id, jobject user_list, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object param_json;
    param_json[kTIMGroupInviteMemberParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    json::Array userList;
    int size = tim::jni::ArrayListJni::Size(user_list);
    for (int i = 0; i < size; ++i) {
        std::string memberID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(user_list, i));
        userList.push_back(memberID);
    }
    param_json[kTIMGroupInviteMemberParamIdentifierArray] = userList;
    std::string paramStr = json::Serialize(param_json);

    tim::TIMEngine::GetInstance()->InviteUserToGroup(paramStr.c_str(),
                                                     [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                         tim::jni::ScopedJEnv scopedJEnv;
                                                         auto _env = scopedJEnv.GetEnv();
                                                         auto _callback = (jobject) user_data;

                                                         if (TIMErrCode::ERR_SUCC == code) {
                                                             json::Array result_array = json::Deserialize(json_params);
                                                             jobject memberOperationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                             for (const auto &i: result_array) {
                                                                 jobject memberOperationResult = tim::jni::GroupMemberOperationResultJni::Convert2JObject_Invite(
                                                                         i);
                                                                 tim::jni::ArrayListJni::Add(memberOperationResultList, memberOperationResult);
                                                                 _env->DeleteLocalRef(memberOperationResult);
                                                             }
                                                             tim::jni::IMCallbackJNI::Success(_callback, memberOperationResultList);
                                                             _env->DeleteLocalRef(memberOperationResultList);

                                                         } else {
                                                             tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                         }
                                                         _env->DeleteGlobalRef(_callback);
                                                     }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeKickGroupMember, jstring group_id, jobject member_list, jstring reason, jint duration, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object param_json;
    param_json[kTIMGroupDeleteMemberParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    param_json[kTIMGroupDeleteMemberParamUserData] = tim::jni::StringJni::Jstring2Cstring(env, reason);
    param_json[kTIMGroupDeleteMemberParamDuration] = duration;
    json::Array userList;
    int size = tim::jni::ArrayListJni::Size(member_list);
    for (int i = 0; i < size; ++i) {
        std::string memberID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(member_list, i));
        userList.push_back(memberID);
    }
    param_json[kTIMGroupDeleteMemberParamIdentifierArray] = userList;
    std::string paramStr = json::Serialize(param_json);

    tim::TIMEngine::GetInstance()->KickGroupMember(paramStr.c_str(),
                                                   [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                       tim::jni::ScopedJEnv scopedJEnv;
                                                       auto _env = scopedJEnv.GetEnv();
                                                       auto _callback = (jobject) user_data;

                                                       if (TIMErrCode::ERR_SUCC == code) {
                                                           json::Array result_array = json::Deserialize(json_params);
                                                           jobject memberOperationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                           for (const auto &i: result_array) {
                                                               jobject memberOperationResult = tim::jni::GroupMemberOperationResultJni::Convert2JObject_Delete(
                                                                       i);
                                                               tim::jni::ArrayListJni::Add(memberOperationResultList, memberOperationResult);
                                                               _env->DeleteLocalRef(memberOperationResult);
                                                           }
                                                           tim::jni::IMCallbackJNI::Success(_callback, memberOperationResultList);
                                                           _env->DeleteLocalRef(memberOperationResultList);

                                                       } else {
                                                           tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                       }
                                                       _env->DeleteGlobalRef(_callback);
                                                   }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupMemberRole, jstring group_id, jstring user_id, jint role, jobject callback) {
    json::Object set_member_role_json;
    set_member_role_json[kTIMGroupModifyMemberInfoParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    set_member_role_json[kTIMGroupModifyMemberInfoParamIdentifier] = tim::jni::StringJni::Jstring2Cstring(env, user_id);
    set_member_role_json[kTIMGroupModifyMemberInfoParamModifyFlag] = TIMGroupMemberModifyInfoFlag::kTIMGroupMemberModifyFlag_MemberRole;
    set_member_role_json[kTIMGroupModifyMemberInfoParamMemberRole] = TIMGroupMemberRole(role);
    std::string paramStr = json::Serialize(set_member_role_json);

    tim::TIMEngine::GetInstance()->SetGroupMemberInfo(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeMarkGroupMemberList, jstring group_id, jobject member_idlist, jint mark_type, jboolean enable_mark, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array memberIdList;
    int size = tim::jni::ArrayListJni::Size(member_idlist);
    for (int i = 0; i < size; ++i) {
        std::string memberID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(member_idlist, i));
        memberIdList.push_back(memberID);
    }
    std::string memberIdListStr = json::Serialize(memberIdList);

    tim::TIMEngine::GetInstance()->MarkGroupMemberList(groupIDStr.c_str(), memberIdListStr.c_str(), (uint32_t) mark_type, enable_mark,
                                                       new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeTransferGroupOwner, jstring group_id, jstring user_id, jobject callback) {
    json::Object transfer_group_owner_json;
    transfer_group_owner_json[kTIMGroupModifyMemberInfoParamGroupId] = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    transfer_group_owner_json[kTIMGroupModifyMemberInfoParamIdentifier] = tim::jni::StringJni::Jstring2Cstring(env, user_id);
    transfer_group_owner_json[kTIMGroupModifyMemberInfoParamModifyFlag] = TIMGroupMemberModifyInfoFlag::kTIMGroupMemberModifyFlag_MemberRole;
    transfer_group_owner_json[kTIMGroupModifyMemberInfoParamMemberRole] = TIMGroupMemberRole::kTIMMemberRole_Owner;
    std::string paramStr = json::Serialize(transfer_group_owner_json);

    tim::TIMEngine::GetInstance()->SetGroupMemberInfo(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetGroupApplicationList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object get_pendency_option;
    get_pendency_option[kTIMGroupPendencyOptionStartTime] = 0;
    get_pendency_option[kTIMGroupPendencyOptionMaxLimited] = 0;
    std::string paramStr = json::Serialize(get_pendency_option);

    tim::TIMEngine::GetInstance()->GetGroupApplicationList(paramStr.c_str(),
                                                           [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                               tim::jni::ScopedJEnv scopedJEnv;
                                                               auto _env = scopedJEnv.GetEnv();
                                                               auto _callback = (jobject) user_data;

                                                               if (TIMErrCode::ERR_SUCC == code) {
                                                                   json::Object result_json = json::Deserialize(json_params);
                                                                   jobject applicationResultObj = tim::jni::GroupApplicationResultJni::Convert2JObject(
                                                                           result_json);
                                                                   tim::jni::IMCallbackJNI::Success(_callback, applicationResultObj);
                                                                   _env->DeleteLocalRef(applicationResultObj);

                                                               } else {
                                                                   tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                               }
                                                               _env->DeleteGlobalRef(_callback);
                                                           }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeAcceptGroupApplication, jobject application, jstring reason, jobject callback) {
    json::Object handle_pendency;
    handle_pendency[kTIMGroupHandlePendencyParamIsAccept] = true;
    handle_pendency[kTIMGroupHandlePendencyParamHandleMsg] = tim::jni::StringJni::Jstring2Cstring(env, reason);
    json::Object pendency; //构造 GroupPendency
    tim::jni::GroupApplicationJni::Convert2CoreObject(application, pendency);
    handle_pendency[kTIMGroupHandlePendencyParamPendency] = pendency;
    std::string paramStr = json::Serialize(handle_pendency);

    tim::TIMEngine::GetInstance()->ProcessGroupApplication(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeRefuseGroupApplication, jobject application, jstring reason, jobject callback) {

    json::Object handle_pendency;
    handle_pendency[kTIMGroupHandlePendencyParamIsAccept] = false;
//    handle_pendency[kTIMGroupHandlePendencyParamHandleMsg] = tim::jni::StringJni::Jstring2Cstring(env, reason);
    handle_pendency[kTIMGroupHandlePendencyParamHandleMsg] = "gsgf";
    json::Object pendency; //构造 GroupPendency
    tim::jni::GroupApplicationJni::Convert2CoreObject(application, pendency);
    handle_pendency[kTIMGroupHandlePendencyParamPendency] = pendency;
    std::string paramStr = json::Serialize(handle_pendency);

    tim::TIMEngine::GetInstance()->ProcessGroupApplication(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupApplicationRead, jobject callback) {
    tim::TIMEngine::GetInstance()->SetGroupApplicationRead(new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetJoinedCommunityList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetJoinedCommunityList([](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array community_array = json::Deserialize(json_params);
            jobject joinedGroupList = tim::jni::ArrayListJni::NewArrayList();
            for (const auto &i: community_array) {
                jobject groupInfoObj = tim::jni::GroupInfoJni::Convert2JObject(i);
                tim::jni::ArrayListJni::Add(joinedGroupList, groupInfoObj);
                _env->DeleteLocalRef(groupInfoObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, joinedGroupList);
            _env->DeleteLocalRef(joinedGroupList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeCreateTopicInCommunity, jstring group_id, jobject topic_info, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    json::Object topicInfo;
    tim::jni::GroupTopicInfoJni::Convert2CoreObject(topic_info, topicInfo);
    std::string topicInfoStr = json::Serialize(topicInfo);

    tim::TIMEngine::GetInstance()->CreateTopicInCommunity(groupIDStr.c_str(), topicInfoStr.c_str(),
                                                          [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                              tim::jni::ScopedJEnv scopedJEnv;
                                                              auto _env = scopedJEnv.GetEnv();
                                                              auto _callback = (jobject) user_data;

                                                              if (TIMErrCode::ERR_SUCC == code) {
                                                                  json::Object result_json = json::Deserialize(json_params);
                                                                  jstring groupID = tim::jni::StringJni::Cstring2Jstring(_env,
                                                                                                                         result_json[kTIMGroupTopicOperationResultTopicID]);
                                                                  tim::jni::IMCallbackJNI::Success(_callback, groupID);
                                                                  _env->DeleteLocalRef(groupID);
                                                              } else {
                                                                  tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                              }
                                                              _env->DeleteGlobalRef(_callback);
                                                          }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteTopicFromCommunity, jstring group_id, jobject topic_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    json::Array topicIdList;
    int size = tim::jni::ArrayListJni::Size(topic_idlist);
    for (int i = 0; i < size; ++i) {
        std::string memberID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(topic_idlist, i));
        topicIdList.push_back(memberID);
    }
    std::string topicIdListStr = json::Serialize(topicIdList);

    tim::TIMEngine::GetInstance()->DeleteTopicFromCommunity(groupIDStr.c_str(), topicIdListStr.c_str(),
                                                            [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                tim::jni::ScopedJEnv scopedJEnv;
                                                                auto _env = scopedJEnv.GetEnv();
                                                                auto _callback = (jobject) user_data;

                                                                if (TIMErrCode::ERR_SUCC == code) {
                                                                    json::Array result_array = json::Deserialize(json_params);
                                                                    jobject operationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                                    for (const auto &i: result_array) {
                                                                        jobject operationResultObj = tim::jni::TopicOperationResultJni::Convert2JObject(
                                                                                i);
                                                                        tim::jni::ArrayListJni::Add(operationResultList, operationResultObj);
                                                                        _env->DeleteLocalRef(operationResultObj);
                                                                    }
                                                                    tim::jni::IMCallbackJNI::Success(_callback, operationResultList);
                                                                    _env->DeleteLocalRef(operationResultList);

                                                                } else {
                                                                    tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                                }
                                                                _env->DeleteGlobalRef(_callback);
                                                            }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetTopicInfo, jobject topic_info, jobject callback) {
    json::Object topicInfo;
    tim::jni::GroupTopicInfoJni::Convert2CoreObject(topic_info, topicInfo);
    std::string topicInfoStr = json::Serialize(topicInfo);

    tim::TIMEngine::GetInstance()->SetTopicInfo(topicInfoStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetTopicInfoList, jstring group_id, jobject topic_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array topicIdList;
    int size = tim::jni::ArrayListJni::Size(topic_idlist);
    for (int i = 0; i < size; ++i) {
        std::string memberID = tim::jni::StringJni::Jstring2Cstring(env, (jstring) tim::jni::ArrayListJni::Get(topic_idlist, i));
        topicIdList.push_back(memberID);
    }
    std::string topicIdListStr = json::Serialize(topicIdList);

    tim::TIMEngine::GetInstance()->GetTopicInfoList(groupIDStr.c_str(), topicIdListStr.c_str(),
                                                    [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                        tim::jni::ScopedJEnv scopedJEnv;
                                                        auto _env = scopedJEnv.GetEnv();
                                                        auto _callback = (jobject) user_data;

                                                        if (TIMErrCode::ERR_SUCC == code) {
                                                            json::Array result_array = json::Deserialize(json_params);
                                                            jobject topicInfoResultList = tim::jni::ArrayListJni::NewArrayList();
                                                            for (int i = 0; i < result_array.size(); ++i) {
                                                                jobject topicInfoResultObj = tim::jni::TopicInfoResultJni::Convert2JObject(
                                                                        result_array[i]);
                                                                tim::jni::ArrayListJni::Add(topicInfoResultList, topicInfoResultObj);
                                                                _env->DeleteLocalRef(topicInfoResultObj);
                                                            }
                                                            tim::jni::IMCallbackJNI::Success(_callback, topicInfoResultList);
                                                            _env->DeleteLocalRef(topicInfoResultList);
                                                        } else {
                                                            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                        }
                                                        _env->DeleteGlobalRef(_callback);
                                                    }, jni_callback);
}

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeCreateGroup",               "(Lcom/tencent/imsdk/v2/V2TIMGroupInfo;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",             (void *) NativeCreateGroupToParame},
        {"nativeGetJoinedGroupList",        "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                                  (void *) NativeGetJoinedGroupList},
        {"nativeGetGroupsInfo",             "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                                  (void *) NativeGetGroupsInfo},
        {"nativeSearchGroups",              "(Lcom/tencent/imsdk/v2/V2TIMGroupSearchParam;Lcom/tencent/imsdk/common/IMCallback;)V",                      (void *) NativeSearchGroups},
        {"nativeSetGroupInfo",              "(Lcom/tencent/imsdk/v2/V2TIMGroupInfo;Lcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeSetGroupInfo},
        {"nativeInitGroupAttributes",       "(Ljava/lang/String;Ljava/util/HashMap;Lcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeInitGroupAttributes},
        {"nativeSetGroupAttributes",        "(Ljava/lang/String;Ljava/util/HashMap;Lcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeSetGroupAttributes},
        {"nativeDeleteGroupAttributes",     "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeDeleteGroupAttributes},
        {"nativeGetGroupAttributes",        "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeGetGroupAttributes},
        {"nativeGetGroupOnlineMemberCount", "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                                (void *) NativeGetGroupOnlineMemberCount},
        {"nativeSetGroupCounters",          "(Ljava/lang/String;Ljava/util/HashMap;Lcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeSetGroupCounters},
        {"nativeGetGroupCounters",          "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeGetGroupCounters},
        {"nativeIncreaseGroupCounter",      "(Ljava/lang/String;Ljava/lang/String;JLcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeIncreaseGroupCounter},
        {"nativeDecreaseGroupCounter",      "(Ljava/lang/String;Ljava/lang/String;JLcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeDecreaseGroupCounter},
        {"nativeGetGroupMemberList",        "(Ljava/lang/String;IJLcom/tencent/imsdk/common/IMCallback;)V",                                              (void *) NativeGetGroupMemberList},
        {"nativeGetGroupMembersInfo",       "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeGetGroupMembersInfo},
        {"nativeSearchGroupMembers",        "(Lcom/tencent/imsdk/v2/V2TIMGroupMemberSearchParam;Lcom/tencent/imsdk/common/IMCallback;)V",                (void *) NativeSearchGroupMembers},
        {"nativeSetGroupMemberInfo",        "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberFullInfo;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeSetGroupMemberInfo},
        {"nativeMuteGroupMember",           "(Ljava/lang/String;Ljava/lang/String;ILcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeMuteGroupMember},
        {"nativeMuteAllGroupMembers",       "(Ljava/lang/String;ZLcom/tencent/imsdk/common/IMCallback;)V",                                               (void *) NativeMuteAllGroupMembers},
        {"nativeInviteUserToGroup",         "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeInviteUserToGroup},
        {"nativeKickGroupMember",           "(Ljava/lang/String;Ljava/util/List;Ljava/lang/String;ILcom/tencent/imsdk/common/IMCallback;)V",             (void *) NativeKickGroupMember},
        {"nativeSetGroupMemberRole",        "(Ljava/lang/String;Ljava/lang/String;ILcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeSetGroupMemberRole},
        {"nativeMarkGroupMemberList",       "(Ljava/lang/String;Ljava/util/List;IZLcom/tencent/imsdk/common/IMCallback;)V",                              (void *) NativeMarkGroupMemberList},
        {"nativeTransferGroupOwner",        "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                              (void *) NativeTransferGroupOwner},
        {"nativeGetGroupApplicationList",   "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                                  (void *) NativeGetGroupApplicationList},
        {"nativeAcceptGroupApplication",    "(Lcom/tencent/imsdk/v2/V2TIMGroupApplication;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",    (void *) NativeAcceptGroupApplication},
        {"nativeRefuseGroupApplication",    "(Lcom/tencent/imsdk/v2/V2TIMGroupApplication;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",    (void *) NativeRefuseGroupApplication},
        {"nativeSetGroupApplicationRead",   "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                                  (void *) NativeSetGroupApplicationRead},
        {"nativeGetJoinedCommunityList",    "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                                  (void *) NativeGetJoinedCommunityList},
        {"nativeCreateTopicInCommunity",    "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMTopicInfo;Lcom/tencent/imsdk/common/IMCallback;)V",           (void *) NativeCreateTopicInCommunity},
        {"nativeDeleteTopicFromCommunity",  "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeDeleteTopicFromCommunity},
        {"nativeSetTopicInfo",              "(Lcom/tencent/imsdk/v2/V2TIMTopicInfo;Lcom/tencent/imsdk/common/IMCallback;)V",                             (void *) NativeSetTopicInfo},
        {"nativeGetTopicInfoList",          "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeGetTopicInfoList},
};

//注册 native 方法
int RegisterNativeMethodsForV2TIMGroupManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMGroupManager", gMethods,
                                                 sizeof(gMethods) / sizeof(gMethods[0]));
}

#ifdef __cplusplus
}
#endif
