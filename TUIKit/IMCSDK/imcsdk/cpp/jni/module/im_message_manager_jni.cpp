/**
 * Created by EDY on 2022/8/22.
 */

#include <jni.h>
#include "jni_util.h"
#include "java_basic_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "advanced_msg_listener_jni.h"
#include "tim_callback_impl.h"
#include "value_callback_impl.h"
#include "im_callbak_jni.h"
#include "message_jni.h"
#include "offline_push_info_jni.h"
#include "send_callback_impl.h"
#include "receive_message_opt_info_jni.h"
#include "group_message_read_member_list_jni.h"
#include "message_list_get_option_jni.h"
#include "message_search_result_item_jni.h"
#include "message_search_result_jni.h"
#include "message_search_param_jni.h"
#include "message_receipt_jni.h"
#include "message_extension_jni.h"
#include "message_extension_result_jni.h"
#include "message_reaction_jni.h"
#include "message_reaction_result_jni.h"
#include "message_reaction_user_result_jni.h"
#include "TIMCloud.h"
#include "LogUtil.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<tim::jni::AdvancedMsgListenerJni> advance_listener_jni;

DEFINE_NATIVE_FUNC(void, NativeAddAdvancedMsgListener, jobject listener, jstring listenerPath) {
    if (!advance_listener_jni) {
        advance_listener_jni = std::make_shared<tim::jni::AdvancedMsgListenerJni>();
    }

    advance_listener_jni->AddListener(env, listener, listenerPath);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveAdvancedMsgListener, jstring listenerPath) {
    if (advance_listener_jni) {
        advance_listener_jni->RemoveListener(env, listenerPath);
        if (advance_listener_jni->empty()) {
            advance_listener_jni.reset();
        }
    }
}

DEFINE_NATIVE_FUNC(jstring, NativeSendMessage, jobject message, jstring receiver, jstring group_id, jint priority, jboolean onlineUserOnly,
                   jobject offlinePushInfo, jobject callback) {
    std::string conv_id;
    TIMConvType conv_type;
    std::string receiverStr = tim::jni::StringJni::Jstring2Cstring(env, receiver);
    if (!receiverStr.empty()) {
        conv_id = receiverStr;
        conv_type = TIMConvType::kTIMConv_C2C;
    } else {
        conv_id = tim::jni::StringJni::Jstring2Cstring(env, group_id);
        conv_type = TIMConvType::kTIMConv_Group;
    }
    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::SendMessageConvert2CoreObject(message,priority,onlineUserOnly,offlinePushInfo);
    std::string message_json_str = json::Serialize(*message_json);

    const size_t kMessageIDLength = 128;
    char message_id_buffer[kMessageIDLength] = {0};

    tim::TIMEngine::GetInstance()->SendMessage(conv_id.c_str(), conv_type, message_json_str.c_str(), message_id_buffer,
                                               new tim::SendTIMCallbackIMpl(callback));
    return tim::jni::StringJni::Cstring2Jstring(env, message_id_buffer);
}

DEFINE_NATIVE_FUNC(void, NativeSetMsgLocalCustomData,jobject message) {
    std::unique_ptr<json::Object> json_msg = tim::jni::MessageJni::Convert2CoreObject(message);
    tim::TIMEngine::GetInstance()->SetMsgLocalCustomData(json::Serialize(*json_msg).c_str());
}

DEFINE_NATIVE_FUNC(void, NativeSetC2CReceiveMessageOpt, jobject userIdList, jint opt, jobject callback) {
    json::Array userIdArray;
    int size = tim::jni::ArrayListJni::Size(userIdList);
    for (int i = 0; i < size; ++i) {
        auto abstractJStr = (jstring) tim::jni::ArrayListJni::Get(userIdList, i);
        std::string abstract = tim::jni::StringJni::Jstring2Cstring(env, abstractJStr);
        userIdArray.push_back(abstract);
        env->DeleteLocalRef(abstractJStr);
    }
    std::string userIdArrayStr = json::Serialize(userIdArray);
    auto messageOpt = TIMReceiveMessageOpt(opt);

    tim::TIMEngine::GetInstance()->SetC2CReceiveMessageOpt(userIdArrayStr.c_str(), messageOpt, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetC2CReceiveMessageOpt, jobject userIdList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array userId_array;
    int size = tim::jni::ArrayListJni::Size(userIdList);
    for (int i = 0; i < size; ++i) {
        auto abstractJStr = (jstring) tim::jni::ArrayListJni::Get(userIdList, i);
        std::string abstract = tim::jni::StringJni::Jstring2Cstring(env, abstractJStr);
        userId_array.push_back(abstract);
        env->DeleteLocalRef(abstractJStr);
    }
    std::string userId_arrayStr = json::Serialize(userId_array);

    tim::TIMEngine::GetInstance()->GetC2CReceiveMessageOpt(userId_arrayStr.c_str(),
                                                           [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                               tim::jni::ScopedJEnv scopedJEnv;
                                                               auto _env = scopedJEnv.GetEnv();
                                                               auto _callback = (jobject) user_data;

                                                               if (TIMErrCode::ERR_SUCC == code) {
                                                                   json::Array opt_array = json::Deserialize(json_params);
                                                                   jobject optInfoObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                   for (const auto &i: opt_array) {
                                                                       jobject optInfoObj = tim::jni::ReceiveMessageOptInfoJni::Convert2JObject(i);
                                                                       tim::jni::ArrayListJni::Add(optInfoObjList, optInfoObj);
                                                                       _env->DeleteLocalRef(optInfoObj);
                                                                   }
                                                                   tim::jni::IMCallbackJNI::Success(_callback, optInfoObjList);
                                                                   _env->DeleteLocalRef(optInfoObjList);

                                                               } else {
                                                                   tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                               }
                                                               _env->DeleteGlobalRef(_callback);
                                                           }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetGroupReceiveMessageOpt, jstring groupId, jint opt, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, groupId);
    auto messageOpt = TIMReceiveMessageOpt(opt);
    tim::TIMEngine::GetInstance()->SetGroupReceiveMessageOpt(groupIDStr.c_str(), messageOpt, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSetAllReceiveMessageOpt, jint opt, jint start_hour, jint start_minute, jint start_second, jlong duration, jobject
        callback) {
    tim::TIMEngine::GetInstance()->SetAllReceiveMessageOpt(TIMReceiveMessageOpt(opt), start_hour, start_minute, start_second, duration,
                                                           new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSetAllReceiveMessageOpt2Time, jint opt, jlong start_time_stamp, jlong duration, jobject callback) {
    tim::TIMEngine::GetInstance()->SetAllReceiveMessageOpt2(TIMReceiveMessageOpt(opt), start_time_stamp, duration, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetAllReceiveMessageOpt, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetAllReceiveMessageOpt([](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto *_env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array opt_array = json::Deserialize(json_params);
            jobject messageObjList = tim::jni::ArrayListJni::NewArrayList();
            for (const auto &item: opt_array) {
                jobject optObj = tim::jni::ReceiveMessageOptInfoJni::Convert2JObject_AllRecvMsg(item);
                tim::jni::ArrayListJni::Add(messageObjList, optObj);
                _env->DeleteLocalRef(optObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, messageObjList);
            _env->DeleteLocalRef(messageObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetC2CHistoryMessageList, jstring userId, jint count, jobject lastMsg, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string conv_id = tim::jni::StringJni::Jstring2Cstring(env, userId);
    TIMConvType conv_type = TIMConvType::kTIMConv_C2C;

    json::Object json_get_msg_param;
    if (lastMsg) {
        std::unique_ptr<json::Object> json_msg = tim::jni::MessageJni::Convert2CoreObject(lastMsg);
        json_get_msg_param[kTIMMsgGetMsgListParamLastMsg] = *json_msg;
    }
    json_get_msg_param[kTIMMsgGetMsgListParamIsRamble] = true;
    json_get_msg_param[kTIMMsgGetMsgListParamIsForward] = false;
    json_get_msg_param[kTIMMsgGetMsgListParamCount] = count;
    std::string paramStr = json::Serialize(json_get_msg_param);

    tim::TIMEngine::GetInstance()->GetHistoryMessageList(conv_id.c_str(), conv_type, paramStr.c_str(),
                                                         [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                             tim::jni::ScopedJEnv scopedJEnv;
                                                             auto *_env = scopedJEnv.GetEnv();
                                                             auto _callback = (jobject) user_data;

                                                             if (TIMErrCode::ERR_SUCC == code) {
                                                                 json::Array message_array = json::Deserialize(json_params);
                                                                 jobject messageObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                 for (const auto &i: message_array) {
                                                                     jobject messageObj = tim::jni::MessageJni::Convert2JObject(i);
                                                                     tim::jni::ArrayListJni::Add(messageObjList, messageObj);
                                                                     _env->DeleteLocalRef(messageObj);
                                                                 }
                                                                 tim::jni::IMCallbackJNI::Success(_callback, messageObjList);
                                                                 _env->DeleteLocalRef(messageObjList);
                                                             } else {
                                                                 tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                             }
                                                             _env->DeleteGlobalRef(_callback);
                                                         }, jni_callback);
}


DEFINE_NATIVE_FUNC(void, NativeGetGroupHistoryMessageList, jstring groupId, jint count, jobject lastMsg, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string conv_id = tim::jni::StringJni::Jstring2Cstring(env, groupId);
    TIMConvType conv_type = TIMConvType::kTIMConv_Group;

    json::Object json_get_msg_param;
    if (lastMsg) {
        std::unique_ptr<json::Object> json_msg = tim::jni::MessageJni::Convert2CoreObject(lastMsg);
        json_get_msg_param[kTIMMsgGetMsgListParamLastMsg] = *json_msg;
    }
    json_get_msg_param[kTIMMsgGetMsgListParamIsRamble] = true;
    json_get_msg_param[kTIMMsgGetMsgListParamIsForward] = false;
    json_get_msg_param[kTIMMsgGetMsgListParamCount] = count;
    std::string paramStr = json::Serialize(json_get_msg_param);
    tim::TIMEngine::GetInstance()->GetHistoryMessageList(conv_id.c_str(), conv_type, paramStr.c_str(),
                                                         [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                             tim::jni::ScopedJEnv scopedJEnv;
                                                             auto _env = scopedJEnv.GetEnv();
                                                             auto _callback = (jobject) user_data;

                                                             if (TIMErrCode::ERR_SUCC == code) {
                                                                 json::Array message_array = json::Deserialize(json_params);
                                                                 jobject messageObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                 for (const auto &i: message_array) {
                                                                     jobject messageObj = tim::jni::MessageJni::Convert2JObject(i);
                                                                     tim::jni::ArrayListJni::Add(messageObjList, messageObj);
                                                                     _env->DeleteLocalRef(messageObj);
                                                                 }
                                                                 tim::jni::IMCallbackJNI::Success(_callback, messageObjList);
                                                                 _env->DeleteLocalRef(messageObjList);

                                                             } else {
                                                                 tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                             }
                                                             _env->DeleteGlobalRef(_callback);
                                                         }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetHistoryMessageList, jobject option, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_get_msg_param;
    tim::jni::MessageListGetOptionJni::Convert2CoreObject(option, json_get_msg_param);
    auto conv_type = TIMConvType(json_get_msg_param["convType"].ToInt());
    std::string conv_id;
    if (json_get_msg_param.HasKey("convID")) {
        conv_id = json_get_msg_param["convID"].ToString();
    }
    std::string paramStr = json::Serialize(json_get_msg_param);

    tim::TIMEngine::GetInstance()->GetHistoryMessageList(conv_id.c_str(), conv_type, paramStr.c_str(),
                                                         [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                             tim::jni::ScopedJEnv scopedJEnv;
                                                             auto _env = scopedJEnv.GetEnv();
                                                             auto _callback = (jobject) user_data;

                                                             if (TIMErrCode::ERR_SUCC == code) {
                                                                 json::Array message_array = json::Deserialize(json_params);
                                                                 jobject messageObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                 for (const auto &i: message_array) {
                                                                     jobject messageObj = tim::jni::MessageJni::Convert2JObject(i);
                                                                     tim::jni::ArrayListJni::Add(messageObjList, messageObj);
                                                                     _env->DeleteLocalRef(messageObj);
                                                                 }
                                                                 tim::jni::IMCallbackJNI::Success(_callback, messageObjList);
                                                                 _env->DeleteLocalRef(messageObjList);

                                                             } else {
                                                                 tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                             }
                                                             _env->DeleteGlobalRef(_callback);
                                                         }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeRevokeMessage, jobject msg, jobject callback) {
    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(msg);
    std::string conv_id = (*message_json)[kTIMMsgConvId];
    auto conv_type = TIMConvType((*message_json)[kTIMMsgConvType].ToInt());
    std::string messageStr = json::Serialize(*message_json);
    tim::TIMEngine::GetInstance()->RevokeMessage(conv_id.c_str(), conv_type, messageStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeModifyMessage, jobject msg, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(msg);
    std::string messageStr = json::Serialize(*message_json);
    tim::TIMEngine::GetInstance()->ModifyMessage(messageStr.c_str(),
                                                 [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                     tim::jni::ScopedJEnv scopedJEnv;
                                                     auto _env = scopedJEnv.GetEnv();
                                                     auto _callback = (jobject) user_data;

                                                     jobject messageObj = tim::jni::MessageJni::Convert2JObject(json::Deserialize(json_params));
                                                     tim::jni::IMCallbackJNI::FailOrComplete(_callback, code, desc, messageObj);

                                                     _env->DeleteGlobalRef(_callback);
                                                 }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteMessageFromLocalStorage, jobject msg, jobject callback) {
    json::Object json_value_msgdelete;
    std::unique_ptr<json::Object> json_value_msg = tim::jni::MessageJni::Convert2CoreObject(msg);
    std::string conv_id = (*json_value_msg)[kTIMMsgConvId];
    auto conv_type = TIMConvType((*json_value_msg)[kTIMMsgConvType].ToInt());
    json_value_msgdelete[kTIMMsgDeleteParamMsg] = *json_value_msg;
    json_value_msgdelete[kTIMMsgDeleteParamIsRamble] = false;//仅删除本地
    std::string msgDeleteStr = json::Serialize(json_value_msgdelete);
    tim::TIMEngine::GetInstance()->DeleteMessages(conv_id.c_str(), conv_type, msgDeleteStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDeleteMessages, jobject messages, jobject callback) {
    json::Array message_array;
    int size;
    size = tim::jni::ArrayListJni::Size(messages);
    for (int i = 0; i < size; ++i) {
        jobject messageObj = tim::jni::ArrayListJni::Get(messages, i);
        std::unique_ptr<json::Object> message = tim::jni::MessageJni::Convert2CoreObject(messageObj);
        message_array.push_back(*message);
        env->DeleteLocalRef(messageObj);
    }
    std::string message_arrayStr = json::Serialize(message_array);

    std::string conv_id = message_array[0][kTIMMsgConvId];
    auto conv_type = TIMConvType(message_array[0][kTIMMsgConvType].ToInt());

    tim::TIMEngine::GetInstance()->DeleteMsgList(conv_id.c_str(), conv_type, message_arrayStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeClearC2CHistoryMessage, jstring userId, jobject callback) {
    std::string userIDStr = tim::jni::StringJni::Jstring2Cstring(env, userId);
    tim::TIMEngine::GetInstance()->ClearHistoryMessage(userIDStr.c_str(), TIMConvType::kTIMConv_C2C, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeClearGroupHistoryMessage, jstring groupId, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, groupId);
    tim::TIMEngine::GetInstance()->ClearHistoryMessage(groupIDStr.c_str(), TIMConvType::kTIMConv_Group, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(jstring, NativeInsertGroupMessageToLocalStorage, jobject msg, jstring groupId, jstring sender, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> json_value_msg = tim::jni::MessageJni::Convert2CoreObject(msg);
    std::string conv_id = tim::jni::StringJni::Jstring2Cstring(env, groupId);
    auto conv_type = TIMConvType::kTIMConv_Group;

    json::Object json_msg;
    json_msg[kTIMMsgSender] = tim::jni::StringJni::Jstring2Cstring(env, sender);
    json_msg[kTIMMsgClientTime] = (long long) TIMGetServerTime();
    json_msg[kTIMMsgServerTime] = (long long) TIMGetServerTime();
    json_msg[kTIMMsgElemArray] = (*json_value_msg)[kTIMMsgElemArray];
    std::string json_msgStr = json::Serialize(json_msg);

    tim::TIMEngine::GetInstance()->InsertMessageToLocalStorage(conv_id.c_str(), conv_type, json_msgStr.c_str(),
                                                               [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                   tim::jni::ScopedJEnv scopedJEnv;
                                                                   auto _env = scopedJEnv.GetEnv();
                                                                   auto _callback = (jobject) user_data;

                                                                   if (TIMErrCode::ERR_SUCC == code) {
                                                                       json::Object msg_json = json::Deserialize(json_params);
                                                                       jobject messageObj = tim::jni::MessageJni::Convert2JObject(msg_json);
                                                                       tim::jni::IMCallbackJNI::Success(_callback, messageObj);
                                                                       _env->DeleteLocalRef(messageObj);
                                                                   } else {
                                                                       tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                                   }
                                                                   _env->DeleteGlobalRef(_callback);
                                                               }, jni_callback);
    return tim::jni::StringJni::Cstring2Jstring(env, "");
}

DEFINE_NATIVE_FUNC(jstring, NativeInsertC2CMessageToLocalStorage, jobject msg, jstring userId, jstring sender, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> json_value_msg = tim::jni::MessageJni::Convert2CoreObject(msg);
    std::string conv_id = tim::jni::StringJni::Jstring2Cstring(env, userId);
    auto conv_type = TIMConvType::kTIMConv_C2C;

    json::Object json_msg;
    json_msg[kTIMMsgSender] = tim::jni::StringJni::Jstring2Cstring(env, sender);
    json_msg[kTIMMsgClientTime] = (long long) TIMGetServerTime();
    json_msg[kTIMMsgServerTime] = (long long) TIMGetServerTime();
    json_msg[kTIMMsgElemArray] = (*json_value_msg)[kTIMMsgElemArray];
    std::string json_msgStr = json::Serialize(json_msg);

    tim::TIMEngine::GetInstance()->InsertMessageToLocalStorage(conv_id.c_str(), conv_type, json_msgStr.c_str(),
                                                               [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                   tim::jni::ScopedJEnv scopedJEnv;
                                                                   auto _env = scopedJEnv.GetEnv();
                                                                   auto _callback = (jobject) user_data;

                                                                   if (TIMErrCode::ERR_SUCC == code) {
                                                                       json::Object msg_json = json::Deserialize(json_params);
                                                                       jobject messageObj = tim::jni::MessageJni::Convert2JObject(msg_json);
                                                                       tim::jni::IMCallbackJNI::Success(_callback, messageObj);
                                                                       _env->DeleteLocalRef(messageObj);
                                                                   } else {
                                                                       tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                                   }
                                                                   _env->DeleteGlobalRef(_callback);
                                                               }, jni_callback);
    return tim::jni::StringJni::Cstring2Jstring(env, "");
}

DEFINE_NATIVE_FUNC(void, NativeFindMessages, jobject messageIdList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array messageID_array;
    int size = tim::jni::ArrayListJni::Size(messageIdList);
    for (int i = 0; i < size; ++i) {
        auto abstractJStr = (jstring) tim::jni::ArrayListJni::Get(messageIdList, i);
        std::string abstract = tim::jni::StringJni::Jstring2Cstring(env, abstractJStr);
        messageID_array.push_back(abstract);
        env->DeleteLocalRef(abstractJStr);
    }
    std::string paramStr = json::Serialize(messageID_array);

    tim::TIMEngine::GetInstance()->FindMessages(paramStr.c_str(), [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array message_array = json::Deserialize(json_params);
            jobject messageObjList = tim::jni::ArrayListJni::NewArrayList();
            for (const auto &i: message_array) {
                jobject messageObj = tim::jni::MessageJni::Convert2JObject(i);
                tim::jni::ArrayListJni::Add(messageObjList, messageObj);
                _env->DeleteLocalRef(messageObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, messageObjList);
            _env->DeleteLocalRef(messageObjList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSearchLocalMessages, jobject searchParam, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object search_param_json;
    tim::jni::MessageSearchParamJni::Convert2CoreObject(searchParam, search_param_json);
    std::string paramStr = json::Serialize(search_param_json);

    tim::TIMEngine::GetInstance()->SearchLocalMessages(paramStr.c_str(),
                                                       [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                           tim::jni::ScopedJEnv scopedJEnv;
                                                           auto _env = scopedJEnv.GetEnv();
                                                           auto _callback = (jobject) user_data;

                                                           if (TIMErrCode::ERR_SUCC == code) {
                                                               json::Object result_json = json::Deserialize(json_params);
                                                               jobject searchResultObj = tim::jni::MessageSearchResultJni::Convert2JObject(
                                                                       result_json);
                                                               tim::jni::IMCallbackJNI::Success(_callback, searchResultObj);
                                                               _env->DeleteLocalRef(searchResultObj);
                                                           } else {
                                                               tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                           }
                                                           _env->DeleteGlobalRef(_callback);
                                                       }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSearchCloudMessages, jobject search_param, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object search_param_json;
    tim::jni::MessageSearchParamJni::Convert2CoreObject(search_param, search_param_json);
    std::string paramStr = json::Serialize(search_param_json);

    tim::TIMEngine::GetInstance()->SearchCloudMessages(paramStr.c_str(),
                                                       [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                           tim::jni::ScopedJEnv scopedJEnv;
                                                           auto _env = scopedJEnv.GetEnv();
                                                           auto _callback = (jobject) user_data;

                                                           if (TIMErrCode::ERR_SUCC == code) {
                                                               json::Object result_json = json::Deserialize(json_params);
                                                               jobject searchResultObj = tim::jni::MessageSearchResultJni::Convert2JObject(
                                                                       result_json);
                                                               tim::jni::IMCallbackJNI::Success(_callback, searchResultObj);
                                                               _env->DeleteLocalRef(searchResultObj);
                                                           } else {
                                                               tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                           }
                                                           _env->DeleteGlobalRef(_callback);
                                                       }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSendMessageReadReceipts, jobject messageList, jobject callback) {
    json::Array message_array;
    int size;
    size = tim::jni::ArrayListJni::Size(messageList);
    for (int i = 0; i < size; ++i) {
        jobject messageObj = tim::jni::ArrayListJni::Get(messageList, i);
        std::unique_ptr<json::Object> message = tim::jni::MessageJni::Convert2CoreObject(messageObj);
        message_array.push_back(*message);
        env->DeleteLocalRef(messageObj);
    }
    std::string paramStr = json::Serialize(message_array);

    tim::TIMEngine::GetInstance()->SendMessageReadReceipts(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetMessageReadReceipts, jobject messageList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array message_array;
    int size;
    size = tim::jni::ArrayListJni::Size(messageList);
    for (int i = 0; i < size; ++i) {
        jobject messageObj = tim::jni::ArrayListJni::Get(messageList, i);
        if (messageObj) {
            std::unique_ptr<json::Object> message = tim::jni::MessageJni::Convert2CoreObject(messageObj);
            if (message) {
                message_array.push_back(*message);
            }
            env->DeleteLocalRef(messageObj);
        }
    }
    std::string paramStr = json::Serialize(message_array);

    tim::TIMEngine::GetInstance()->GetMessageReadReceipts(paramStr.c_str(),
                                                          [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                              tim::jni::ScopedJEnv scopedJEnv;
                                                              auto _env = scopedJEnv.GetEnv();
                                                              auto _callback = (jobject) user_data;

                                                              if (TIMErrCode::ERR_SUCC == code) {
                                                                  json::Array receipt_array = json::Deserialize(json_params);
                                                                  jobject messageReceiptObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                  for (int i = 0; i < receipt_array.size(); ++i) {
                                                                      jobject messageReceiptObj = tim::jni::MessageReceiptJni::Convert2JObject(
                                                                              receipt_array[i]);
                                                                      tim::jni::ArrayListJni::Add(messageReceiptObjList, messageReceiptObj);
                                                                      _env->DeleteLocalRef(messageReceiptObj);
                                                                  }
                                                                  tim::jni::IMCallbackJNI::Success(_callback, messageReceiptObjList);
                                                                  _env->DeleteLocalRef(messageReceiptObjList);
                                                              } else {
                                                                  tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                              }
                                                              _env->DeleteGlobalRef(_callback);
                                                          }, jni_callback);
}
DEFINE_NATIVE_FUNC(void, NativeGetGroupMessageReadMemberList, jobject message, jint filter, jlong nextSeq, jint count, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string msgStr = json::Serialize(*message_json);
    auto readMembersFilter = TIMGroupMessageReadMembersFilter(filter);

    tim::TIMEngine::GetInstance()->GetGroupMessageReadMemberList(msgStr.c_str(), readMembersFilter, nextSeq, count,
                                                                 [](const char *json_group_member_array, uint64_t next_seq, bool is_finished,
                                                                    const void *user_data) {
                                                                     tim::jni::ScopedJEnv scopedJEnv;
                                                                     auto _env = scopedJEnv.GetEnv();
                                                                     auto _callback = (jobject) user_data;

                                                                     if (json_group_member_array != nullptr && strlen(json_group_member_array) > 0) {
                                                                         json::Array member_array = json::Deserialize(json_group_member_array);
                                                                         jobject readMemberListObj = tim::jni::GroupMessageReadMemberListJni::Convert2JObject(
                                                                                 member_array, next_seq, is_finished);
                                                                         tim::jni::IMCallbackJNI::Success(_callback, readMemberListObj);
                                                                         _env->DeleteLocalRef(readMemberListObj);
                                                                     } else {
                                                                         tim::jni::IMCallbackJNI::Fail(_callback, 0,
                                                                                                       "get group message read member list is fail");
                                                                     }
                                                                     _env->DeleteGlobalRef(_callback);
                                                                 }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetMessageExtensions, jobject message, jobject extensions, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string messageStr = json::Serialize(*message_json);

    json::Array json_extension_array;
    int size = tim::jni::ArrayListJni::Size(extensions);
    for (int i = 0; i < size; ++i){
        json::Object extension_json;
        jobject extension_obj = tim::jni::ArrayListJni::Get(extensions,i);
        bool flag = tim::jni::MessageExtensionJni::Convert2CoreObject(extension_obj,extension_json);
        if (flag){
            json_extension_array.push_back(extension_json);
        }
    }
    std::string paramStr = json::Serialize(json_extension_array);

    tim::TIMEngine::GetInstance()->SetMessageExtensions(messageStr.c_str(),paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array extensionResult_array = json::Deserialize(json_params);
            jobject extensionResultObjList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < extensionResult_array.size(); ++i) {
                jobject extensionResultObj = tim::jni::MessageExtensionResultJni::Convert2JObject(extensionResult_array[i]);
                tim::jni::ArrayListJni::Add(extensionResultObjList, extensionResultObj);
                _env->DeleteLocalRef(extensionResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, extensionResultObjList);
            _env->DeleteLocalRef(extensionResultObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetMessageExtensions, jobject message, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string messageStr = json::Serialize(*message_json);

    tim::TIMEngine::GetInstance()->GetMessageExtensions(messageStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array extensionResult_array = json::Deserialize(json_params);
            jobject extensionResultObjList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < extensionResult_array.size(); ++i) {
                jobject extensionResultObj = tim::jni::MessageExtensionJni::Convert2JObject(extensionResult_array[i]);
                tim::jni::ArrayListJni::Add(extensionResultObjList, extensionResultObj);
                _env->DeleteLocalRef(extensionResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, extensionResultObjList);
            _env->DeleteLocalRef(extensionResultObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteMessageExtensions, jobject message, jobject keys, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string messageStr = json::Serialize(*message_json);

    json::Array key_array;
    int size = tim::jni::ArrayListJni::Size(keys);
    for (int i = 0; i < size; ++i){
        json::Object extension_json;
        auto keyStr = (jstring)tim::jni::ArrayListJni::Get(keys,i);
        key_array.push_back(tim::jni::StringJni::Jstring2Cstring(env,keyStr));
    }
    std::string paramStr = json::Serialize(key_array);
    tim::TIMEngine::GetInstance()->DeleteMessageExtensions(messageStr.c_str(),paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array extensionResult_array = json::Deserialize(json_params);
            jobject extensionResultObjList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < extensionResult_array.size(); ++i) {
                jobject extensionResultObj = tim::jni::MessageExtensionResultJni::Convert2JObject(extensionResult_array[i]);
                tim::jni::ArrayListJni::Add(extensionResultObjList, extensionResultObj);
                _env->DeleteLocalRef(extensionResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, extensionResultObjList);
            _env->DeleteLocalRef(extensionResultObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeAddMessageReaction, jobject message, jstring reaction_id, jobject callback) {
    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string messageStr = json::Serialize(*message_json);
    std::string reactionIdStr = tim::jni::StringJni::Jstring2Cstring(env,reaction_id);
    tim::TIMEngine::GetInstance()->AddMessageReaction(messageStr.c_str(),reactionIdStr.c_str(),new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeRemoveMessageReaction, jobject message, jstring reaction_id, jobject callback) {
    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string messageStr = json::Serialize(*message_json);
    std::string reactionIdStr = tim::jni::StringJni::Jstring2Cstring(env,reaction_id);
    tim::TIMEngine::GetInstance()->RemoveMessageReaction(messageStr.c_str(),reactionIdStr.c_str(),new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeGetMessageReactions, jobject message_list, jint max_user_count_per_reaction, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array message_array;
    int size;
    size = tim::jni::ArrayListJni::Size(message_list);
    for (int i = 0; i < size; ++i) {
        jobject messageObj = tim::jni::ArrayListJni::Get(message_list, i);
        if (messageObj) {
            std::unique_ptr<json::Object> message = tim::jni::MessageJni::Convert2CoreObject(messageObj);
            if (message) {
                message_array.push_back(*message);
            }
            env->DeleteLocalRef(messageObj);
        }
    }
    std::string paramStr = json::Serialize(message_array);
    tim::TIMEngine::GetInstance()->GetMessageReactions(paramStr.c_str(),max_user_count_per_reaction,[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array reactionsResult_array = json::Deserialize(json_params);
            jobject reactionsResultObjList = tim::jni::ArrayListJni::NewArrayList();
            for (const auto & i : reactionsResult_array) {
                jobject reactionsResultObj = tim::jni::MessageReactionResultJni::Convert2JObject(i);
                tim::jni::ArrayListJni::Add(reactionsResultObjList, reactionsResultObj);
                _env->DeleteLocalRef(reactionsResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, reactionsResultObjList);
            _env->DeleteLocalRef(reactionsResultObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetAllUserListOfMessageReaction, jobject message, jstring reaction_id, jint next_seq, jint count, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::unique_ptr<json::Object> message_json = tim::jni::MessageJni::Convert2CoreObject(message);
    std::string messageStr = json::Serialize(*message_json);
    std::string reactionIdStr = tim::jni::StringJni::Jstring2Cstring(env,reaction_id);
    
    tim::TIMEngine::GetInstance()->GetAllUserListOfMessageReaction(messageStr.c_str(),reactionIdStr.c_str(),next_seq,count,[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array reactionUserResult_array = json::Deserialize(json_params);
            jobject reactionUserResultObjList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < reactionUserResult_array.size(); ++i) {
                jobject reactionUserResultObj = tim::jni::MessageReactionUserResultJni::Convert2JObject(reactionUserResult_array[i]);
                tim::jni::ArrayListJni::Add(reactionUserResultObjList, reactionUserResultObj);
                _env->DeleteLocalRef(reactionUserResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, reactionUserResultObjList);
            _env->DeleteLocalRef(reactionUserResultObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeTranslateText, jobject source_text_list, jstring source_language, jstring target_language, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array source_text_array;
    int size = tim::jni::ArrayListJni::Size(source_text_list);
    for (int i = 0; i < size; ++i){
        json::Object extension_json;
        auto keyStr = (jstring)tim::jni::ArrayListJni::Get(source_text_list,i);
        source_text_array.push_back(tim::jni::StringJni::Jstring2Cstring(env,keyStr));
    }
    std::string paramStr = json::Serialize(source_text_array);

    std::string souLan = tim::jni::StringJni::Jstring2Cstring(env,source_language);
    std::string tarLan = tim::jni::StringJni::Jstring2Cstring(env,target_language);

    tim::TIMEngine::GetInstance()->TranslateText(paramStr.c_str(),souLan.c_str(),tarLan.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array translate_array = json::Deserialize(json_params);
            jobject j_obj_translateHashMap = tim::jni::HashMapJni::NewHashMap();
            for (const auto &item: translate_array) {
                jstring sourceTextKey = tim::jni::StringJni::Cstring2Jstring(_env,item[kTIMMsgTranslateTextSourceText]);
                if (sourceTextKey) {
                    jstring targetTextValue = tim::jni::StringJni::Cstring2Jstring(_env,item[kTIMMsgTranslateTextTargetText]);
                    if (targetTextValue) {
                        tim::jni::HashMapJni::Put(j_obj_translateHashMap, sourceTextKey, targetTextValue);
                        _env->DeleteLocalRef(targetTextValue);
                    }
                    _env->DeleteLocalRef(sourceTextKey);
                }
            }
            tim::jni::IMCallbackJNI::Success(_callback, j_obj_translateHashMap);
            _env->DeleteLocalRef(j_obj_translateHashMap);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeMarkC2CMessageAsRead, jstring userId, jobject callback) {
    std::string userIDStr = tim::jni::StringJni::Jstring2Cstring(env, userId);
    tim::TIMEngine::GetInstance()->MarkMsgReportRead(userIDStr.c_str(), TIMConvType::kTIMConv_C2C, nullptr, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeMarkGroupMessageAsRead, jstring groupId, jobject callback) {
    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, groupId);
    tim::TIMEngine::GetInstance()->MarkMsgReportRead(groupIDStr.c_str(), TIMConvType::kTIMConv_C2C, nullptr, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeMarkAllMessageAsRead, jobject callback) {
    tim::TIMEngine::GetInstance()->MarkAllMessageAsRead(new tim::TIMCallbackIMpl(callback));
}

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeAddAdvancedMsgListener",          "(Lcom/tencent/imsdk/v2/V2TIMAdvancedMsgListener;Ljava/lang/String;)V",                                  (void *) NativeAddAdvancedMsgListener},
        {"nativeRemoveAdvancedMsgListener",       "(Ljava/lang/String;)V",                                                                                 (void *) NativeRemoveAdvancedMsgListener},
        {"nativeSendMessage",
                                                  "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/lang/String;Ljava/lang/String;"
                                                  "IZLcom/tencent/imsdk/v2/V2TIMOfflinePushInfo;Lcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;", (jstring *) NativeSendMessage},
        {"nativeSetMsgLocalCustomData",           "(Lcom/tencent/imsdk/v2/V2TIMMessage;)V",                                             (void *) NativeSetMsgLocalCustomData},
        {"nativeSetC2CReceiveMessageOpt",         "(Ljava/util/List;ILcom/tencent/imsdk/common/IMCallback;)V",                                             (void *) NativeSetC2CReceiveMessageOpt},
        {"nativeGetC2CReceiveMessageOpt",         "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                              (void *) NativeGetC2CReceiveMessageOpt},
        {"nativeSetGroupReceiveMessageOpt",       "(Ljava/lang/String;ILcom/tencent/imsdk/common/IMCallback;)V",                                           (void *) NativeSetGroupReceiveMessageOpt},
        {"nativeSetAllReceiveMessageOpt",         "(IIIIJLcom/tencent/imsdk/common/IMCallback;)V",                                                         (void *) NativeSetAllReceiveMessageOpt},
        {"nativeSetAllReceiveMessageOpt",         "(IJJLcom/tencent/imsdk/common/IMCallback;)V",                                                           (void *) NativeSetAllReceiveMessageOpt2Time},
        {"nativeGetAllReceiveMessageOpt",         "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                              (void *) NativeGetAllReceiveMessageOpt},
        {"nativeGetC2CHistoryMessageList",        "(Ljava/lang/String;ILcom/tencent/imsdk/v2/V2TIMMessage;Lcom/tencent/imsdk/common/IMCallback;)V",        (void *) NativeGetC2CHistoryMessageList},
        {"nativeGetGroupHistoryMessageList",      "(Ljava/lang/String;ILcom/tencent/imsdk/v2/V2TIMMessage;Lcom/tencent/imsdk/common/IMCallback;)V",        (void *) NativeGetGroupHistoryMessageList},
        {"nativeGetHistoryMessageList",           "(Lcom/tencent/imsdk/v2/V2TIMMessageListGetOption;Lcom/tencent/imsdk/common/IMCallback;)V",              (void *) NativeGetHistoryMessageList},
        {"nativeRevokeMessage",                   "(Lcom/tencent/imsdk/v2/V2TIMMessage;Lcom/tencent/imsdk/common/IMCallback;)V",                           (void *) NativeRevokeMessage},
        {"nativeModifyMessage",                   "(Lcom/tencent/imsdk/v2/V2TIMMessage;Lcom/tencent/imsdk/common/IMCallback;)V",                           (void *) NativeModifyMessage},
        {"nativeDeleteMessageFromLocalStorage",   "(Lcom/tencent/imsdk/v2/V2TIMMessage;Lcom/tencent/imsdk/common/IMCallback;)V",                           (void *) NativeDeleteMessageFromLocalStorage},
        {"nativeDeleteMessages",                  "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                              (void *) NativeDeleteMessages},
        {"nativeClearC2CHistoryMessage",          "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                            (void *) NativeClearC2CHistoryMessage},
        {"nativeClearGroupHistoryMessage",        "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                            (void *) NativeClearGroupHistoryMessage},
        {"nativeInsertGroupMessageToLocalStorage",
                                                  "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/lang/String;"
                                                  "Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;",                            (jstring *) NativeInsertGroupMessageToLocalStorage},
        {"nativeInsertC2CMessageToLocalStorage",
                                                  "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/lang/String;"
                                                  "Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;",                            (jstring *) NativeInsertC2CMessageToLocalStorage},
        {"nativeFindMessages",                    "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                              (void *) NativeFindMessages},
        {"nativeSearchLocalMessages",             "(Lcom/tencent/imsdk/v2/V2TIMMessageSearchParam;Lcom/tencent/imsdk/common/IMCallback;)V",                (void *) NativeSearchLocalMessages},
        {"nativeSearchCloudMessages",             "(Lcom/tencent/imsdk/v2/V2TIMMessageSearchParam;Lcom/tencent/imsdk/common/IMCallback;)V",                (void *) NativeSearchCloudMessages},
        {"nativeSendMessageReadReceipts",         "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                              (void *) NativeSendMessageReadReceipts},
        {"nativeGetMessageReadReceipts",          "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                              (void *) NativeGetMessageReadReceipts},
        {"nativeGetGroupMessageReadMemberList",   "(Lcom/tencent/imsdk/v2/V2TIMMessage;IJILcom/tencent/imsdk/common/IMCallback;)V",                        (void *) NativeGetGroupMessageReadMemberList},
        {"nativeSetMessageExtensions",            "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",           (void *) NativeSetMessageExtensions},
        {"nativeGetMessageExtensions",            "(Lcom/tencent/imsdk/v2/V2TIMMessage;Lcom/tencent/imsdk/common/IMCallback;)V",                           (void *) NativeGetMessageExtensions},
        {"nativeDeleteMessageExtensions",         "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",           (void *) NativeDeleteMessageExtensions},
        {"nativeAddMessageReaction",              "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",         (void *) NativeAddMessageReaction},
        {"nativeRemoveMessageReaction",           "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",         (void *) NativeRemoveMessageReaction},
        {"nativeGetMessageReactions",             "(Ljava/util/List;ILcom/tencent/imsdk/common/IMCallback;)V",                                             (void *) NativeGetMessageReactions},
        {"nativeGetAllUserListOfMessageReaction", "(Lcom/tencent/imsdk/v2/V2TIMMessage;Ljava/lang/String;IILcom/tencent/imsdk/common/IMCallback;)V",       (void *) NativeGetAllUserListOfMessageReaction},
        {"nativeTranslateText",                   "(Ljava/util/List;Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",          (void *) NativeTranslateText},
        {"nativeMarkC2CMessageAsRead",            "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                            (void *) NativeMarkC2CMessageAsRead},
        {"nativeMarkGroupMessageAsRead",          "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                            (void *) NativeMarkGroupMessageAsRead},
        {"nativeMarkAllMessageAsRead",            "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                              (void *) NativeMarkAllMessageAsRead},
};

//注册 native 方法
int RegisterNativeMethodsForV2TIMMessageManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMMessageManager", gMethods,
                                                 sizeof(gMethods) / sizeof(gMethods[0]));
}

#ifdef __cplusplus
}
#endif