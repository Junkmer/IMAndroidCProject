//
// Created by EDY on 2022/8/22.
//

#include <android/log.h>
#include <jni.h>
#include <bits/sysconf.h>
#include <string>
#include <iostream>
#include <im_callbak_jni.h>
#include "jni_util.h"
#include "LogUtil.h"
#include "value_callback_impl.h"

#include "../convert/sdk_config_jni.h"
#include "../basic/jni_helper.h"
#include "../listener/base_listener_jni.h"
#include "tim_engine.h"
#include "simple_listener_jni.h"
#include "java_basic_jni.h"
#include "group_listener_jni.h"
#include "user_full_info_jni.h"
#include "user_status_jni.h"
#include "tim_callback_impl.h"
#include "call_experimental_api_jni.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...)  \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<tim::jni::BaseListenerJni> base_listener_jni;
static std::shared_ptr<tim::jni::SimpleListenerJni> simple_listener_jni;
static std::shared_ptr<tim::jni::GroupListenerJni> group_listener_jni;

DEFINE_NATIVE_FUNC(void, NativeAddSDKListener, jobject sdkListener, jstring listenerPath) {
    if (!base_listener_jni) {
        base_listener_jni = std::make_shared<tim::jni::BaseListenerJni>();
    }
    base_listener_jni->AddSDKListener(env, sdkListener, listenerPath);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveSDKListener, jstring listenerPath) {
    if (base_listener_jni) {
        base_listener_jni->RemoveSDKListener(env, listenerPath);
        if (base_listener_jni->Empty()) {
            base_listener_jni.reset();
        }
    }
}

DEFINE_NATIVE_FUNC(bool, NativeInitSDK, const jint sdkAppID, jobject sdkConfig) {
    std::string config = tim::jni::SDKConfigJni::ConvertToCoreObject(env, sdkConfig);
    int flag = TIMInit(sdkAppID, config.c_str());
    return flag == TIM_SUCC;
}

DEFINE_NATIVE_FUNC(void, NativeUninitSDK) {
    TIMUninit();
}

DEFINE_NATIVE_FUNC(void, NativeLogin, const jstring userID, const jstring userSig, jobject callback) {
    char *user_id = tim::jni::StringJni::jstringToChar(env, userID);
    char *user_sig = tim::jni::StringJni::jstringToChar(env, userSig);
    tim::TIMEngine::GetInstance()->Login(user_id, user_sig, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeLogout, jobject callback) {
    tim::TIMEngine::GetInstance()->Logout(new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(jstring, NativeGetVersion) {
    const char *sdk_version = tim::TIMEngine::GetInstance()->GetVersion();
    return tim::jni::StringJni::Cstring2Jstring(env, sdk_version);
}

DEFINE_NATIVE_FUNC(jlong, NativeGetServerTime) {
    return (jlong) tim::TIMEngine::GetInstance()->GetServerTime();
}

DEFINE_NATIVE_FUNC(jstring, NativeGetLoginUser) {
    return tim::jni::StringJni::Cstring2Jstring(env, tim::TIMEngine::GetInstance()->GetLoginUser());
}

DEFINE_NATIVE_FUNC(jint, NativeGetLoginStatus) {
    return tim::TIMEngine::GetInstance()->GetLoginStatus();
}

DEFINE_NATIVE_FUNC(void, NativeAddSimpleMsgListener, jobject simple_listener) {
    if (!simple_listener_jni) {
        simple_listener_jni = std::make_shared<tim::jni::SimpleListenerJni>();
    }
    simple_listener_jni->AddListener(env, simple_listener);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveSimpleMsgListener, jobject simple_listener) {
    if (simple_listener_jni) {
        simple_listener_jni->RemoveListener(simple_listener);
        if (simple_listener_jni->empty()) {
            simple_listener_jni.reset();
        }
    }
}

std::string CreateTextMessage(const std::string &text) {
    json::Object json_obj_text;
    json_obj_text[kTIMElemType] = TIMElemType::kTIMElem_Text;
    json_obj_text[kTIMTextElemContent] = text;
    json::Array elem_array;
    elem_array.push_back(json_obj_text);
    json::Object json_obj_msg;
    json_obj_msg[kTIMMsgElemArray] = elem_array;
    return json::Serialize(json_obj_msg);
}

std::string CreateCustomMessage(const std::string &data) {
    json::Object json_obj_custom;
    json_obj_custom[kTIMElemType] = TIMElemType::kTIMElem_Custom;
    json_obj_custom[kTIMCustomElemData] = data;
    json::Array elem_array;
    elem_array.push_back(json_obj_custom);
    json::Object json_obj_msg;
    json_obj_msg[kTIMMsgElemArray] = elem_array;
    return json::Serialize(json_obj_msg);
}

DEFINE_NATIVE_FUNC(jstring, NativeSendC2CTextMessage, jstring text, jstring user_id, jobject callback) {
    std::string textCStr = tim::jni::StringJni::Jstring2Cstring(env, text);
    std::string userIdCStr = tim::jni::StringJni::Jstring2Cstring(env, user_id);

    std::string msgStr = CreateTextMessage(textCStr);

    const size_t kMessageIDLength = 128;
    char message_id_buffer[kMessageIDLength] = {0};

    tim::TIMEngine::GetInstance()->SendMessage(userIdCStr.c_str(), TIMConvType::kTIMConv_C2C, msgStr.c_str(),
                                               message_id_buffer, new tim::SendTIMCallbackIMpl(callback));
    return tim::jni::StringJni::Cstring2Jstring(env, message_id_buffer);
}

DEFINE_NATIVE_FUNC(jstring, NativeSendC2CCustomMessage, jbyteArray custom_data, jstring user_id, jobject callback) {
    std::string textCStr = tim::jni::StringJni::JbyteArray2Cstring(env, custom_data);
    std::string userIdCStr = tim::jni::StringJni::Jstring2Cstring(env, user_id);

    std::string msgStr = CreateCustomMessage(textCStr);

    const size_t kMessageIDLength = 128;
    char message_id_buffer[kMessageIDLength] = {0};

    tim::TIMEngine::GetInstance()->SendMessage(userIdCStr.c_str(), TIMConvType::kTIMConv_C2C, msgStr.c_str(),
                                               message_id_buffer, new tim::SendTIMCallbackIMpl(callback));
    return tim::jni::StringJni::Cstring2Jstring(env, message_id_buffer);
}

DEFINE_NATIVE_FUNC(jstring, NativeSendGroupTextMessage, jstring text, jstring group_id, jint priority, jobject callback) {
    std::string textCStr = tim::jni::StringJni::Jstring2Cstring(env, text);
    std::string groupIdCStr = tim::jni::StringJni::jstringToChar(env, group_id);

    std::string msgStr = CreateTextMessage(textCStr);

    const size_t kMessageIDLength = 128;
    char message_id_buffer[kMessageIDLength] = {0};

    tim::TIMEngine::GetInstance()->SendMessage(groupIdCStr.c_str(), TIMConvType::kTIMConv_Group, msgStr.c_str(),
                                               message_id_buffer, new tim::SendTIMCallbackIMpl(callback));
    return tim::jni::StringJni::Cstring2Jstring(env, message_id_buffer);
}

DEFINE_NATIVE_FUNC(jstring, NativeSendGroupCustomMessage, jbyteArray custom_data, jstring group_id, jint priority, jobject callback) {
    std::string textCStr = tim::jni::StringJni::JbyteArray2Cstring(env, custom_data);
    std::string groupIdCStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    std::string msgStr = CreateCustomMessage(textCStr);

    const size_t kMessageIDLength = 128;
    char message_id_buffer[kMessageIDLength] = {0};

    tim::TIMEngine::GetInstance()->SendMessage(groupIdCStr.c_str(), TIMConvType::kTIMConv_Group, msgStr.c_str(),
                                               message_id_buffer, new tim::SendTIMCallbackIMpl(callback));
    return tim::jni::StringJni::Cstring2Jstring(env, message_id_buffer);
}

DEFINE_NATIVE_FUNC(void, NativeAddGroupListener, jobject listener, jstring listenerPath) {
    if (!group_listener_jni) {
        group_listener_jni = std::make_shared<tim::jni::GroupListenerJni>();
    }
    group_listener_jni->AddListener(env, listener, listenerPath);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveGroupListener, jstring listenerPath) {
    if (group_listener_jni) {
        group_listener_jni->RemoveListener(env, listenerPath);
        if (group_listener_jni->empty()) {
            group_listener_jni.reset();
        }
    }
}

DEFINE_NATIVE_FUNC(void, NativeCreateGroup, jstring group_type, jstring group_id, jstring group_name, jobject callback) {
    if (nullptr == group_id || nullptr == group_type) {
        return;
    }
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupTypeStr = tim::jni::StringJni::Jstring2Cstring(env, group_type);
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    std::string groupNameStr = tim::jni::StringJni::Jstring2Cstring(env, group_name);

    json::Object json_object_param;
    json_object_param[kTIMCreateGroupParamGroupId] = groupIDStr;
    json_object_param[kTIMCreateGroupParamGroupName] = groupNameStr;

    if (groupTypeStr == "Work") {
        json_object_param[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_Private;
    } else if (groupTypeStr == "Public") {
        json_object_param[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_Public;
    } else if (groupTypeStr == "Meeting") {
        json_object_param[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_ChatRoom;
    } else if (groupTypeStr == "AVChatRoom") {
        json_object_param[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_AVChatRoom;
    } else if (groupTypeStr == "Community") {
        json_object_param[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_Community;
    }

    std::string stringVector_json_str = json::Serialize(json_object_param);

    tim::TIMEngine::GetInstance()->CreateGroup(stringVector_json_str.c_str(),
                                               [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                   tim::jni::ScopedJEnv scopedJEnv;
                                                   auto _callback = (jobject) user_data;

                                                   if (TIMErrCode::ERR_SUCC == code) {
                                                       json::Object param;
                                                       param = json::Deserialize(json_params);
                                                       std::string groupId = param[kTIMCreateGroupResultGroupId];

                                                       tim::jni::IMCallbackJNI::Success(_callback,
                                                                                        tim::jni::StringJni::Cstring2Jstring(scopedJEnv.GetEnv(),
                                                                                                                             groupId));
                                                   } else {
                                                       tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                   }

                                                   scopedJEnv.GetEnv()->DeleteGlobalRef(_callback);

                                               }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeJoinGroup, jstring group_id, jstring message, jobject callback) {
    if (nullptr == group_id) {
        return;
    }

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    std::string messageStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    tim::TIMEngine::GetInstance()->JoinGroup(groupIDStr.c_str(), messageStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeQuitGroup, jstring group_id, jobject callback) {
    if (nullptr == group_id) {
        return;
    }

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    tim::TIMEngine::GetInstance()->QuitGroup(groupIDStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDismissGroup, jstring group_id, jobject callback) {
    if (nullptr == group_id) {
        return;
    }

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);
    tim::TIMEngine::GetInstance()->DismissGroup(groupIDStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetUsersInfo, jobject user_id_list, jobject callback) {
    if (nullptr == user_id_list) {
        return;
    }

    jobject jni_callback = env->NewGlobalRef(callback);

    int size = tim::jni::ArrayListJni::Size(user_id_list);
    json::Array user_id_array;
    for (int i = 0; i < size; ++i) {
        auto userID = (jstring) tim::jni::ArrayListJni::Get(user_id_list, i);
        if (nullptr != userID) {
            user_id_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID));
        }
        env->DeleteLocalRef(userID);
    }

    json::Object json_get_user_profile_list_param;
    json_get_user_profile_list_param[kTIMFriendShipGetProfileListParamForceUpdate] = false;
    json_get_user_profile_list_param[kTIMFriendShipGetProfileListParamIdentifierArray] = user_id_array;

    std::string paramStr = json::Serialize(json_get_user_profile_list_param);

    tim::TIMEngine::GetInstance()->GetUsersInfo(paramStr.c_str(), [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();

        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array user_full_info_array = json::Deserialize(json_params);
            if (user_full_info_array.size() > 0) {
                jobject userFullInfoList = tim::jni::ArrayListJni::NewArrayList();
                for (int i = 0; i < user_full_info_array.size(); ++i) {
                    jobject userFullInfo = tim::jni::UserFullInfoJni::Convert2JObject(user_full_info_array[i]);
                    tim::jni::ArrayListJni::Add(userFullInfoList, userFullInfo);
                    _env->DeleteLocalRef(userFullInfo);
                }
                tim::jni::IMCallbackJNI::Success(_callback, userFullInfoList);
                _env->DeleteLocalRef(userFullInfoList);
            }
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }

        scopedJEnv.GetEnv()->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetSelfInfo, jobject info, jobject callback) {
    if (nullptr == info) {
        return;
    }

    json::Object userFullInfo;
    bool flag = tim::jni::UserFullInfoJni::Convert2CoreObject_SetSelfInfo(info, userFullInfo);
    if (flag) {
        std::string userFullInfo_str = json::Serialize(userFullInfo);
        tim::TIMEngine::GetInstance()->SetSelfInfo(userFullInfo_str.c_str(), new tim::TIMCallbackIMpl(callback));
    }
}

DEFINE_NATIVE_FUNC(void, NativeSubscribeUserInfo, jobject user_id_list, jobject callback) {
    if (nullptr == user_id_list) {
        return;
    }

    json::Array user_id_array;
    int size = tim::jni::ArrayListJni::Size(user_id_list);
    for (int i = 0; i < size; ++i){
        auto userId = (jstring)tim::jni::ArrayListJni::Get(user_id_list,i);
        user_id_array.push_back(tim::jni::StringJni::Jstring2Cstring(env,userId));
    }
    std::string paramStr = json::Serialize(user_id_array);

    tim::TIMEngine::GetInstance()->SubscribeUserInfo(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}


DEFINE_NATIVE_FUNC(void, NativeUnsubscribeUserInfo, jobject user_id_list, jobject callback) {
    if (nullptr == user_id_list) {
        return;
    }

    json::Array user_id_array;
    int size = tim::jni::ArrayListJni::Size(user_id_list);
    for (int i = 0; i < size; ++i){
        auto userId = (jstring)tim::jni::ArrayListJni::Get(user_id_list,i);
        user_id_array.push_back(tim::jni::StringJni::Jstring2Cstring(env,userId));
    }
    std::string paramStr = json::Serialize(user_id_array);

    tim::TIMEngine::GetInstance()->UnsubscribeUserInfo(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetUserStatus, jobject user_id_list, jobject callback) {
    if (nullptr == user_id_list) {
        return;
    }

    jobject jni_callback = env->NewGlobalRef(callback);

    int size = tim::jni::ArrayListJni::Size(user_id_list);
    json::Array stringVector;
    for (int i = 0; i < size; ++i) {
        auto userID = (jstring) tim::jni::ArrayListJni::Get(user_id_list, i);
        stringVector.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID).c_str());
    }

    std::string stringVector_json = json::Serialize(stringVector);

    tim::TIMEngine::GetInstance()->GetUserStatus(stringVector_json.c_str(),
                                                 [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                     tim::jni::ScopedJEnv scopedJEnv;
                                                     auto _env = scopedJEnv.GetEnv();

                                                     auto _callback = (jobject) user_data;

                                                     if (TIMErrCode::ERR_SUCC == code) {
                                                         json::Array userStatusVector = json::Deserialize(json_params);
                                                         if (userStatusVector.size() > 0) {
                                                             jobject userStatusList = tim::jni::ArrayListJni::NewArrayList();
                                                             for (int i = 0; i < userStatusVector.size(); ++i) {
                                                                 jobject userStatus = tim::jni::UserStatusJni::Convert2JObject(userStatusVector[i]);
                                                                 tim::jni::ArrayListJni::Add(userStatusList, userStatus);
                                                                 _env->DeleteLocalRef(userStatus);
                                                             }
                                                             tim::jni::IMCallbackJNI::Success(_callback, userStatusList);
                                                             _env->DeleteLocalRef(userStatusList);
                                                         }
                                                     } else {
                                                         tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                     }

                                                     _env->DeleteGlobalRef(_callback);
                                                 }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetSelfStatus, jobject status, jobject callback) {
    if (nullptr == status) {
        return;
    }

    json::Object userStatus;
    bool flag = tim::jni::UserStatusJni::Convert2CoreObject(status, userStatus);
    if (flag) {
        tim::TIMEngine::GetInstance()->SetSelfStatus(json::Serialize(userStatus).c_str(), new tim::TIMCallbackIMpl(callback));
    }
}

DEFINE_NATIVE_FUNC(void, NativeSubscribeUserStatus, jobject user_id_list, jobject callback) {
    if (nullptr == user_id_list) {
        return;
    }

    int size = tim::jni::ArrayListJni::Size(user_id_list);
    json::Array stringVector;
    for (int i = 0; i < size; ++i) {
        auto userID = (jstring) tim::jni::ArrayListJni::Get(user_id_list, i);
        stringVector.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID).c_str());
    }

    tim::TIMEngine::GetInstance()->SubscribeUserStatus(json::Serialize(stringVector).c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeUnsubscribeUserStatus, jobject user_id_list, jobject callback) {
    if (nullptr == user_id_list) {
        return;
    }

    int size = tim::jni::ArrayListJni::Size(user_id_list);
    json::Array stringVector;
    for (int i = 0; i < size; ++i) {
        auto userID = (jstring) tim::jni::ArrayListJni::Get(user_id_list, i);
        stringVector.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID).c_str());
    }

    tim::TIMEngine::GetInstance()->UnsubscribeUserStatus(json::Serialize(stringVector).c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeCallExperimentalAPI, jstring api, jobject param, jobject callback) {
    if (nullptr == api || nullptr == param) {
        return;
    }

    jobject jni_callback = env->NewGlobalRef(callback);

    std::string json_param = tim::jni::CallExperimentalAPIJni::ConvertToCoreObject(env, api, param);

    tim::TIMEngine::GetInstance()->CallExperimentalAPI(json_param.c_str(),
                                                       [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                           tim::jni::ScopedJEnv scopedJEnv;

                                                           auto _callback = (jobject) user_data;

                                                           if (TIMErrCode::ERR_SUCC == code) {
                                                               LOGE("json_params = %s",json_params);
                                                               json::Object base_json = json::Deserialize(json_params);
                                                               if (base_json.size() > 0) {
                                                                   if (base_json.HasKey(kTIMCommercialAbilityResultEnabled)){
                                                                       bool isEnabled = base_json[kTIMCommercialAbilityResultEnabled];
                                                                       tim::jni::IMCallbackJNI::Success(_callback, tim::jni::IntegerJni::NewIntegerObj
                                                                               (isEnabled));
                                                                   }
                                                               }
                                                           } else {
                                                               tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                           }

                                                           scopedJEnv.GetEnv()->DeleteGlobalRef(_callback);
                                                       }, jni_callback);

}

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeAddSDKListener",          "(Lcom/tencent/imsdk/v2/V2TIMSDKListener;Ljava/lang/String;)V",                                   (void *) NativeAddSDKListener},
        {"nativeRemoveSDKListener",       "(Ljava/lang/String;)V",                                                                          (void *) NativeRemoveSDKListener},
        {"nativeInitSDK",                 "(ILcom/tencent/imsdk/v2/V2TIMSDKConfig;)Z",                                                      (bool *) NativeInitSDK},
        {"nativeUninitSDK",               "()V",                                                                                            (void *) NativeUninitSDK},
        {"nativeLogin",                   "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeLogin},
        {"nativeLogout",                  "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                       (void *) NativeLogout},
        {"nativeGetVersion",              "()Ljava/lang/String;",                                                                           (jstring *) NativeGetVersion},
        {"nativeGetServerTime",           "()J",                                                                                            (jlong *) NativeGetServerTime},
        {"nativeGetLoginUser",            "()Ljava/lang/String;",                                                                           (jstring *) NativeGetLoginUser},
        {"nativeGetLoginStatus",          "()I",                                                                                            (jint *) NativeGetLoginStatus},
        {"nativeAddSimpleMsgListener",    "(Lcom/tencent/imsdk/v2/V2TIMSimpleMsgListener;Ljava/lang/String;)V",                             (void *) NativeAddSimpleMsgListener},
        {"nativeRemoveSimpleMsgListener", "(Ljava/lang/String;)V",                                                                          (void *) NativeRemoveSimpleMsgListener},
        {"nativeSendC2CTextMessage",      "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;",  (jstring *) NativeSendC2CTextMessage},
        {"nativeSendC2CCustomMessage",    "([BLjava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;",                  (jstring *) NativeSendC2CCustomMessage},
        {"nativeSendGroupTextMessage",    "(Ljava/lang/String;Ljava/lang/String;ILcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;", (jstring *) NativeSendGroupTextMessage},
        {"nativeSendGroupCustomMessage",  "([BLjava/lang/String;ILcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;",                 (jstring *) NativeSendGroupCustomMessage},
        {"nativeAddGroupListener",        "(Lcom/tencent/imsdk/v2/V2TIMGroupListener;Ljava/lang/String;)V",                                 (void *) NativeAddGroupListener},
        {"nativeRemoveGroupListener",     "(Ljava/lang/String;)V",                                                                          (void *) NativeRemoveGroupListener},
        {"nativeCreateGroup",             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeCreateGroup},
        {"nativeJoinGroup",               "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeJoinGroup},
        {"nativeQuitGroup",               "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                     (void *) NativeQuitGroup},
        {"nativeDismissGroup",            "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                     (void *) NativeDismissGroup},
        {"nativeGetUsersInfo",            "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                       (void *) NativeGetUsersInfo},
        {"nativeSetSelfInfo",             "(Lcom/tencent/imsdk/v2/V2TIMUserFullInfo;Lcom/tencent/imsdk/common/IMCallback;)V",               (void *) NativeSetSelfInfo},
        {"nativeSubscribeUserInfo",       "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                       (void *) NativeSubscribeUserInfo},
        {"nativeUnsubscribeUserInfo",     "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                       (void *) NativeUnsubscribeUserInfo},
        {"nativeGetUserStatus",           "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                       (void *) NativeGetUserStatus},
        {"nativeSetSelfStatus",           "(Lcom/tencent/imsdk/v2/V2TIMUserStatus;Lcom/tencent/imsdk/common/IMCallback;)V",                 (void *) NativeSetSelfStatus},
        {"nativeSubscribeUserStatus",     "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                       (void *) NativeSubscribeUserStatus},
        {"nativeUnsubscribeUserStatus",   "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                       (void *) NativeUnsubscribeUserStatus},
        {"nativeCallExperimentalAPI",     "(Ljava/lang/String;Ljava/lang/Object;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeCallExperimentalAPI},
};

// 注册 native 方法
int RegisterNativeMethodsForV2TIMManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMManager", gMethods,
                                                 sizeof(gMethods) / sizeof(gMethods[0]));
}


#ifdef __cplusplus
}
#endif