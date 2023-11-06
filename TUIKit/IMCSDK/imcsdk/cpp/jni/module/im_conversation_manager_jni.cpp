//
// Created by EDY on 2022/8/22.
//

#include <jni.h>
#include "jni_util.h"
#include "value_callback_impl.h"
#include "conversation_listener_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "conversation_result_jni.h"
#include "im_callbak_jni.h"
#include "java_basic_jni.h"
#include "conversation_jni.h"
#include "conversation_list_filter_jni.h"
#include "tim_callback_impl.h"
#include "conversation_operation_result_jni.h"
#include "LogUtil.h"
#include "json.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<tim::jni::ConversationListenerJni> conversation_callback_jni;

DEFINE_NATIVE_FUNC(void, NativeAddConversationListener, jobject listener, jstring listenerPath) {
    if (!conversation_callback_jni) {
        conversation_callback_jni = std::make_shared<tim::jni::ConversationListenerJni>();
    }
    conversation_callback_jni->AddListener(env, listener, listenerPath);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveConversationListener, jstring listenerPath) {
    if (conversation_callback_jni) {
        conversation_callback_jni->RemoveListener(env, listenerPath);
        if (conversation_callback_jni->empty()) {
            conversation_callback_jni.reset();
        }
    }
}

void CGetConversationList(JNIEnv *env, const char *filter, uint64_t next_seq, uint32_t count, jobject &callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetConversationListByFilter(filter, next_seq, count,
                                                               [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                   tim::jni::ScopedJEnv scopedJEnv;
                                                                   auto _callback = (jobject) user_data;

                                                                   auto _env = scopedJEnv.GetEnv();

                                                                   if (TIMErrCode::ERR_SUCC == code) {
                                                                       LOGE("conversation = %s", json_params);
                                                                       jobject conversationResultObj = tim::jni::ConversationResultJni::Convert2JObject(
                                                                               json_params);
                                                                       if (conversationResultObj) {
                                                                           tim::jni::IMCallbackJNI::Success(_callback, conversationResultObj);
                                                                           _env->DeleteLocalRef(conversationResultObj);
                                                                       }
                                                                   } else {
                                                                       tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                                   }
                                                                   _env->DeleteGlobalRef(_callback);
                                                               }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetConversationList, jlong next_seq, jint count, jobject callback) {
    CGetConversationList(env, "{}", next_seq, count, callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetConversation, jstring conversation_id, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_obj;
    std::string conversationID_str = tim::jni::StringJni::Jstring2Cstring(env, conversation_id);
    if (conversationID_str.find("c2c_") != std::string::npos) {//单聊会话 c2c_
        json_obj[kTIMGetConversationListParamConvType] = kTIMConv_C2C;
        json_obj[kTIMGetConversationListParamConvId] = conversationID_str.substr(3);
    } else if (conversationID_str.find("group_") != std::string::npos) {//群聊会话 group_xxx
        json_obj[kTIMGetConversationListParamConvType] = kTIMConv_Group;
        json_obj[kTIMGetConversationListParamConvId] = conversationID_str.substr(5);
    } else {
        json_obj[kTIMGetConversationListParamConvType] = kTIMConv_C2C;
        json_obj[kTIMGetConversationListParamConvId] = conversationID_str;
    }

    json::Array json_arry;
    json_arry.push_back(json_obj);
    std::string convIDArrayStr = json::Serialize(json_arry);

    tim::TIMEngine::GetInstance()->GetConversationListByID(convIDArrayStr.c_str(),
                                                           [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                               tim::jni::ScopedJEnv scopedJEnv;
                                                               auto _env = scopedJEnv.GetEnv();
                                                               auto _callback = (jobject) user_data;
                                                               if (TIMErrCode::ERR_SUCC == code) {
                                                                   json::Object conversation_json = json::Deserialize(json_params);
                                                                   jobject conversationObj = tim::jni::ConversationJni::Convert2JObject(
                                                                           conversation_json);
                                                                   if (conversationObj) {
                                                                       tim::jni::IMCallbackJNI::Success(_callback, conversationObj);
                                                                       _env->DeleteLocalRef(conversationObj);
                                                                   }
                                                               } else {
                                                                   tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                               }
                                                               _env->DeleteGlobalRef(_callback);
                                                           }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetConversationListForID, jobject conversation_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array json_arry;
    int size = tim::jni::ArrayListJni::Size(conversation_idlist);
    for (int i = 0; i < size; ++i) {
        auto item = (jstring) tim::jni::ArrayListJni::Get(conversation_idlist, i);
        std::string conversation = tim::jni::StringJni::Jstring2Cstring(env, item);
        json::Object json_obj;
        if (conversation.find("c2c_") != std::string::npos) {//单聊会话 c2c_
            json_obj[kTIMGetConversationListParamConvType] = kTIMConv_C2C;
            json_obj[kTIMGetConversationListParamConvId] = conversation.substr(3);
        } else if (conversation.find("group_") != std::string::npos) {//群聊会话 group_xxx
            json_obj[kTIMGetConversationListParamConvType] = kTIMConv_C2C;
            json_obj[kTIMGetConversationListParamConvId] = conversation.substr(5);
        } else {
            json_obj[kTIMGetConversationListParamConvType] = kTIMConv_C2C;
            json_obj[kTIMGetConversationListParamConvId] = conversation;
        }
        json_arry.push_back(json_obj);
        env->DeleteLocalRef(item);
    }

    std::string convIDArrayStr = json::Serialize(json_arry);
    tim::TIMEngine::GetInstance()->GetConversationListByID(convIDArrayStr.c_str(),
                                                           [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                               tim::jni::ScopedJEnv scopedJEnv;
                                                               auto _env = scopedJEnv.GetEnv();
                                                               auto _callback = (jobject) user_data;

                                                               if (TIMErrCode::ERR_SUCC == code) {
                                                                   json::Array conv_array = json::Deserialize(json_params);
                                                                   jobject conversationObjList = tim::jni::ArrayListJni::NewArrayList();
                                                                   for (const auto &i: conv_array) {
                                                                       jobject conversationObj = tim::jni::ConversationJni::Convert2JObject(i);
                                                                       if (conversationObj) {
                                                                           tim::jni::ArrayListJni::Add(conversationObjList, conversationObj);
                                                                           _env->DeleteLocalRef(conversationObj);
                                                                       }
                                                                   }

                                                                   tim::jni::IMCallbackJNI::Success(_callback, conversationObjList);
                                                                   _env->DeleteLocalRef(conversationObjList);
                                                               } else {
                                                                   tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
                                                               }
                                                               _env->DeleteGlobalRef(_callback);
                                                           }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetConversationListByFilter, jobject filter, jlong next_seq, jint count, jobject callback) {

    json::Object filter_json_obj;
    tim::jni::ConversationListFilterJni::Convert2CoreObject(filter, filter_json_obj);
    std::string filterStr = json::Serialize(filter_json_obj);

    CGetConversationList(env, filterStr.c_str(), next_seq, count, callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteConversation, jstring conversation_id, jobject callback) {
    std::string conversationID_str = tim::jni::StringJni::Jstring2Cstring(env, conversation_id);
    std::string convID;
    TIMConvType convType;
    if (conversationID_str.find("c2c_") != std::string::npos) {//单聊会话 c2c_
        convType = kTIMConv_C2C;
        convID = conversationID_str.substr(3);
    } else {//群聊会话 group_xxx
        convType = kTIMConv_Group;
        convID = conversationID_str.substr(5);
    }
    tim::TIMEngine::GetInstance()->DeleteConversation(convID.c_str(), convType, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSetConversationDraft, jstring conversation_id, jstring draft_text, jobject callback) {
    std::string conversationID_str = tim::jni::StringJni::Jstring2Cstring(env, conversation_id);
    std::string convID;
    TIMConvType convType;
    if (conversationID_str.find("c2c_") != std::string::npos) {//单聊会话 c2c_
        convType = kTIMConv_C2C;
        convID = conversationID_str.substr(3);
    } else {//群聊会话 group_xxx
        convType = kTIMConv_Group;
        convID = conversationID_str.substr(5);
    }
    std::string draftText = tim::jni::StringJni::Jstring2Cstring(env, draft_text);

    tim::TIMEngine::GetInstance()->SetConversationDraft(convID.c_str(), convType, draftText.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSetConversationCustomData, jobject conversation_idlist, jstring custom_data, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array conversation_vector;
    int size = tim::jni::ArrayListJni::Size(conversation_idlist);
    for (int i = 0; i < size; ++i) {
        auto item = (jstring) tim::jni::ArrayListJni::Get(conversation_idlist, i);
        std::string conversation = tim::jni::StringJni::Jstring2Cstring(env, item);
        conversation_vector.push_back(conversation);
        env->DeleteLocalRef(item);
    }

    std::string convArrayStr = json::Serialize(conversation_vector);
    std::string customStr = tim::jni::StringJni::Jstring2Cstring(env, custom_data);

    tim::TIMEngine::GetInstance()->SetConversationCustomData(convArrayStr.c_str(), customStr.c_str(),
                                                             [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                 tim::jni::ScopedJEnv scopedJEnv;
                                                                 auto _env = scopedJEnv.GetEnv();
                                                                 auto _callback = (jobject) user_data;

                                                                 if (TIMErrCode::ERR_SUCC == code) {
                                                                     json::Array conv_array = json::Deserialize(json_params);
                                                                     jobject operationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                                     for (const auto &i: conv_array) {
                                                                         jobject operationResultObj = tim::jni::ConversationOperationResultJni::Convert2JObject(
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

DEFINE_NATIVE_FUNC(void, NativePinConversation, jstring conversation_id, jboolean is_pinned, jobject callback) {
    std::string conversationID_str = tim::jni::StringJni::Jstring2Cstring(env, conversation_id);
    std::string convID;
    TIMConvType convType;
    if (conversationID_str.find("c2c_") != std::string::npos) {//单聊会话 c2c_
        convType = kTIMConv_C2C;
        convID = conversationID_str.substr(3);
    } else {//群聊会话 group_xxx
        convType = kTIMConv_Group;
        convID = conversationID_str.substr(5);
    }
    tim::TIMEngine::GetInstance()->PinConversation(convID.c_str(), convType, (bool) is_pinned, new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeMarkConversation, jobject conversation_idlist, jlong mark_type, jboolean enable_mark, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array conversation_vector;
    int size = tim::jni::ArrayListJni::Size(conversation_idlist);
    for (int i = 0; i < size; ++i) {
        auto item = (jstring) tim::jni::ArrayListJni::Get(conversation_idlist, i);
        std::string conversation = tim::jni::StringJni::Jstring2Cstring(env, item);
        conversation_vector.push_back(conversation);
        env->DeleteLocalRef(item);
    }

    std::string convVectorStr = json::Serialize(conversation_vector);

    tim::TIMEngine::GetInstance()->MarkConversation(convVectorStr.c_str(), (uint64_t) mark_type, (bool) enable_mark,
                                                    [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                        tim::jni::ScopedJEnv scopedJEnv;
                                                        auto _env = scopedJEnv.GetEnv();
                                                        auto _callback = (jobject) user_data;

                                                        if (TIMErrCode::ERR_SUCC == code) {
                                                            json::Array conv_array = json::Deserialize(json_params);
                                                            jobject operationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                            for (const auto &i: conv_array) {
                                                                jobject operationResultObj = tim::jni::ConversationOperationResultJni::Convert2JObject(
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

DEFINE_NATIVE_FUNC(void, NativeGetTotalUnreadMessageCount, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetTotalUnreadMessageCount([](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Object unReadTotal_json = json::Deserialize(json_params);
            int total = unReadTotal_json[kTIMConvGetTotalUnreadMessageCountResultUnreadCount];
            tim::jni::IMCallbackJNI::Success(_callback, tim::jni::LongJni::NewLongObj(total));
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}


DEFINE_NATIVE_FUNC(void, NativeGetUnreadMessageCountByFilter, jobject filter, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object filter_json;
    tim::jni::ConversationListFilterJni::Convert2CoreObject(filter,filter_json);
    std::string paramStr = json::Serialize(filter_json);
    tim::TIMEngine::GetInstance()->GetUnreadMessageCountByFilter(paramStr.c_str(), [](int32_t code, const char *desc, const char *json_params, const
    void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Object unReadTotal_json = json::Deserialize(json_params);
            int total = unReadTotal_json[kTIMConvGetTotalUnreadMessageCountResultUnreadCount];
            tim::jni::IMCallbackJNI::Success(_callback, tim::jni::LongJni::NewLongObj(total));
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}


DEFINE_NATIVE_FUNC(void, NativeSubscribeUnreadMessageCountByFilter, jobject filter) {
    json::Object filter_json;
    tim::jni::ConversationListFilterJni::Convert2CoreObject(filter,filter_json);
    std::string paramStr = json::Serialize(filter_json);
    tim::TIMEngine::GetInstance()->SubscribeUnreadMessageCountByFilter(paramStr.c_str());
}


DEFINE_NATIVE_FUNC(void, NativeUnsubscribeUnreadMessageCountByFilter, jobject filter) {
    json::Object filter_json;
    tim::jni::ConversationListFilterJni::Convert2CoreObject(filter,filter_json);
    std::string paramStr = json::Serialize(filter_json);
    tim::TIMEngine::GetInstance()->UnsubscribeUnreadMessageCountByFilter(paramStr.c_str());
}


DEFINE_NATIVE_FUNC(void, NativeCleanConversationUnreadMessageCount, jstring conversation_id, jlong clean_timestamp, jlong clean_sequence,
                   jobject callback) {
    std::string conv_id = tim::jni::StringJni::Jstring2Cstring(env,conversation_id);
    tim::TIMEngine::GetInstance()->CleanConversationUnreadMessageCount(conv_id.c_str(),clean_timestamp,clean_sequence,new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeCreateConversationGroup, jstring group_name, jobject conversation_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupName_str = tim::jni::StringJni::Jstring2Cstring(env, group_name);

    json::Array conversation_vector;
    int size = tim::jni::ArrayListJni::Size(conversation_idlist);
    for (int i = 0; i < size; ++i) {
        auto item = (jstring) tim::jni::ArrayListJni::Get(conversation_idlist, i);
        std::string conversation = tim::jni::StringJni::Jstring2Cstring(env, item);
        conversation_vector.push_back(conversation);
        env->DeleteLocalRef(item);
    }
    std::string convVectorStr = json::Serialize(conversation_vector);

    tim::TIMEngine::GetInstance()->CreateConversationGroup(groupName_str.c_str(), convVectorStr.c_str(),
                                                           [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                               tim::jni::ScopedJEnv scopedJEnv;
                                                               auto _env = scopedJEnv.GetEnv();
                                                               auto _callback = (jobject) user_data;

                                                               if (TIMErrCode::ERR_SUCC == code) {
                                                                   json::Array conv_array = json::Deserialize(json_params);
                                                                   jobject operationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                                   for (const auto &i: conv_array) {
                                                                       jobject operationResultObj = tim::jni::ConversationOperationResultJni::Convert2JObject(
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

DEFINE_NATIVE_FUNC(void, NativeGetConversationGroupList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetConversationGroupList([](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array group_array = json::Deserialize(json_params);
            jobject groupObjList = tim::jni::ArrayListJni::NewArrayList();
            for (const auto &i: group_array) {
                jstring groupObj = tim::jni::StringJni::Cstring2Jstring(_env, i);
                tim::jni::ArrayListJni::Add(groupObjList, groupObj);
                _env->DeleteLocalRef(groupObj);
            }

            tim::jni::IMCallbackJNI::Success(_callback, groupObjList);
            _env->DeleteLocalRef(groupObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteConversationGroup, jstring group_name, jobject callback) {

    std::string groupName_str = tim::jni::StringJni::Jstring2Cstring(env, group_name);
    tim::TIMEngine::GetInstance()->DeleteConversationGroup(groupName_str.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeRenameConversationGroup, jstring old_name, jstring new_name, jobject callback) {
    std::string oldName_str = tim::jni::StringJni::Jstring2Cstring(env, old_name);
    std::string newName_str = tim::jni::StringJni::Jstring2Cstring(env, new_name);

    tim::TIMEngine::GetInstance()->RenameConversationGroup(oldName_str.c_str(), newName_str.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeAddConversationsToGroup, jstring group_name, jobject conversation_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupName_str = tim::jni::StringJni::Jstring2Cstring(env, group_name);

    json::Array conversation_vector;
    int size = tim::jni::ArrayListJni::Size(conversation_idlist);
    for (int i = 0; i < size; ++i) {
        auto item = (jstring) tim::jni::ArrayListJni::Get(conversation_idlist, i);
        std::string conversation = tim::jni::StringJni::Jstring2Cstring(env, item);
        conversation_vector.push_back(conversation);
        env->DeleteLocalRef(item);
    }
    std::string convVectorStr = json::Serialize(conversation_vector);

    tim::TIMEngine::GetInstance()->AddConversationsToGroup(groupName_str.c_str(), convVectorStr.c_str(),
                                                           [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                               tim::jni::ScopedJEnv scopedJEnv;
                                                               auto _env = scopedJEnv.GetEnv();
                                                               auto _callback = (jobject) user_data;

                                                               if (TIMErrCode::ERR_SUCC == code) {
                                                                   json::Array conv_result_array = json::Deserialize(json_params);
                                                                   jobject operationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                                   for (const auto &i: conv_result_array) {
                                                                       jobject operationResultObj = tim::jni::ConversationOperationResultJni::Convert2JObject(
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

DEFINE_NATIVE_FUNC(void, NativeDeleteConversationsFromGroup, jstring group_name, jobject conversation_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string groupName_str = tim::jni::StringJni::Jstring2Cstring(env, group_name);

    json::Array conversation_vector;
    int size = tim::jni::ArrayListJni::Size(conversation_idlist);
    for (int i = 0; i < size; ++i) {
        auto item = (jstring) tim::jni::ArrayListJni::Get(conversation_idlist, i);
        std::string conversation = tim::jni::StringJni::Jstring2Cstring(env, item);
        conversation_vector.push_back(conversation);
        env->DeleteLocalRef(item);
    }
    std::string convVectorStr = json::Serialize(conversation_vector);

    tim::TIMEngine::GetInstance()->DeleteConversationsFromGroup(groupName_str.c_str(), convVectorStr.c_str(),
                                                                [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                                    tim::jni::ScopedJEnv scopedJEnv;
                                                                    auto _env = scopedJEnv.GetEnv();
                                                                    auto _callback = (jobject) user_data;

                                                                    if (TIMErrCode::ERR_SUCC == code) {
                                                                        json::Array conv_result_array = json::Deserialize(json_params);
                                                                        jobject operationResultList = tim::jni::ArrayListJni::NewArrayList();
                                                                        for (const auto &i: conv_result_array) {
                                                                            jobject operationResultObj = tim::jni::ConversationOperationResultJni::Convert2JObject(
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

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeAddConversationListener",             "(Lcom/tencent/imsdk/v2/V2TIMConversationListener;Ljava/lang/String;)V",                        (void *) NativeAddConversationListener},
        {"nativeRemoveConversationListener",          "(Ljava/lang/String;)V",                                                                        (void *) NativeRemoveConversationListener},
        {"nativeGetConversationList",                 "(JILcom/tencent/imsdk/common/IMCallback;)V",                                                   (void *) NativeGetConversationList},
        {"nativeGetConversation",                     "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                   (void *) NativeGetConversation},
        {"nativeGetConversationList",                 "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                     (void *) NativeGetConversationListForID},
        {"nativeGetConversationListByFilter",         "(Lcom/tencent/imsdk/v2/V2TIMConversationListFilter;JILcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeGetConversationListByFilter},
        {"nativeDeleteConversation",                  "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                   (void *) NativeDeleteConversation},
        {"nativeSetConversationDraft",                "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                 (void *) NativeSetConversationDraft},
        {"nativeSetConversationCustomData",           "(Ljava/util/List;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeSetConversationCustomData},
        {"nativePinConversation",                     "(Ljava/lang/String;ZLcom/tencent/imsdk/common/IMCallback;)V",                                  (void *) NativePinConversation},
        {"nativeMarkConversation",                    "(Ljava/util/List;JZLcom/tencent/imsdk/common/IMCallback;)V",                                   (void *) NativeMarkConversation},
        {"nativeGetTotalUnreadMessageCount",          "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                     (void *) NativeGetTotalUnreadMessageCount},
        {"nativeGetUnreadMessageCountByFilter",       "(Lcom/tencent/imsdk/v2/V2TIMConversationListFilter;Lcom/tencent/imsdk/common/IMCallback;)V",   (void *) NativeGetUnreadMessageCountByFilter},
        {"nativeSubscribeUnreadMessageCountByFilter", "(Lcom/tencent/imsdk/v2/V2TIMConversationListFilter;)V",                                        (void *) NativeSubscribeUnreadMessageCountByFilter},
        {"nativeSubscribeUnreadMessageCountByFilter", "(Lcom/tencent/imsdk/v2/V2TIMConversationListFilter;)V",                                        (void *) NativeSubscribeUnreadMessageCountByFilter},
        {"nativeCleanConversationUnreadMessageCount", "(Ljava/lang/String;JJLcom/tencent/imsdk/common/IMCallback;)V",                                 (void *) NativeCleanConversationUnreadMessageCount},
        {"nativeCreateConversationGroup",             "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeCreateConversationGroup},
        {"nativeGetConversationGroupList",            "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                     (void *) NativeGetConversationGroupList},
        {"nativeDeleteConversationGroup",             "(Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                   (void *) NativeDeleteConversationGroup},
        {"nativeRenameConversationGroup",             "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                 (void *) NativeRenameConversationGroup},
        {"nativeAddConversationsToGroup",             "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeAddConversationsToGroup},
        {"nativeDeleteConversationsFromGroup",        "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                   (void *) NativeDeleteConversationsFromGroup},

};

//注册 native 方法
int RegisterNativeMethodsForV2TIMConversationManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMConversationManager", gMethods,
                                                 sizeof(gMethods) / sizeof(gMethods[0]));
}

#ifdef __cplusplus
}
#endif