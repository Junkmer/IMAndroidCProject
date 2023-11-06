//
// Created by EDY on 2022/8/22.
//

#include <jni.h>
#include "jni_util.h"
#include "java_basic_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "signaling_listener_jni.h"
#include "tim_callback_impl.h"
#include "value_callback_impl.h"
#include "im_callbak_jni.h"
#include "offline_push_info_jni.h"
#include "message_jni.h"
#include "signaling_info_jni.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<tim::jni::SignalingListenerJni> signaling_listener_jni;

DEFINE_NATIVE_FUNC(void, NativeAddSignalingListener, jobject signal_listener, jstring listenerPath) {
    if (!signaling_listener_jni) {
        signaling_listener_jni = std::make_shared<tim::jni::SignalingListenerJni>();
    }

    signaling_listener_jni->AddListener(env, signal_listener, listenerPath);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveSignalingListener, jstring listenerPath) {
    if (signaling_listener_jni) {
        signaling_listener_jni->RemoveListener(env, listenerPath);
        if (signaling_listener_jni->empty()) {
            signaling_listener_jni.reset();
        }
    }
}

DEFINE_NATIVE_FUNC(jstring, NativeInvite, jstring invitee, jstring data, jboolean online_user_only, jobject offline_push_info, jint timeout,
                   jobject callback) {

    std::string inviteeStr = tim::jni::StringJni::Jstring2Cstring(env, invitee);
    std::string dataStr = tim::jni::StringJni::Jstring2Cstring(env, data);
    json::Object pushInfo;
    if (offline_push_info) {
        tim::jni::OfflinePushInfoJni::Convert2CoreObject(offline_push_info, pushInfo);
    }
    std::string pushInfoStr = json::Serialize(pushInfo);

    char *inviteIDStr = nullptr;
    tim::TIMEngine::GetInstance()->Invite(inviteeStr.c_str(), dataStr.c_str(), online_user_only, pushInfoStr.c_str(), timeout, inviteIDStr,
                                          new tim::TIMCallbackIMpl(callback));
    return tim::jni::StringJni::charToJstring(env, inviteIDStr);
}

DEFINE_NATIVE_FUNC(jstring, NativeInviteInGroup, jstring group_id, jobject invitee_list, jstring data, jboolean online_user_only, jint timeout,
                   jobject callback) {

    std::string groupIDStr = tim::jni::StringJni::Jstring2Cstring(env, group_id);

    json::Array inviteeList;
    int size = tim::jni::ArrayListJni::Size(invitee_list);
    for (int i = 0; i < size; ++i) {
        auto inviteeJStr = (jstring) tim::jni::ArrayListJni::Get(invitee_list, i);
        inviteeList.push_back(tim::jni::StringJni::Jstring2Cstring(env, inviteeJStr));
        env->DeleteLocalRef(inviteeJStr);
    }
    std::string inviteeListStr = json::Serialize(inviteeList);
    std::string dataStr = tim::jni::StringJni::Jstring2Cstring(env, data);

    char *inviteIDStr = nullptr;
    tim::TIMEngine::GetInstance()->InviteInGroup(groupIDStr.c_str(), inviteeListStr.c_str(), dataStr.c_str(), online_user_only, timeout,
                                                 inviteIDStr, new tim::TIMCallbackIMpl(callback));
    return tim::jni::StringJni::charToJstring(env, inviteIDStr);
}

DEFINE_NATIVE_FUNC(void, NativeCancel, jstring invite_id, jstring data, jobject callback) {

    std::string inviteIDStr = tim::jni::StringJni::Jstring2Cstring(env, invite_id);
    std::string dataStr = tim::jni::StringJni::Jstring2Cstring(env, data);

    tim::TIMEngine::GetInstance()->Cancel(inviteIDStr.c_str(), dataStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeAccept, jstring invite_id, jstring data, jobject callback) {

    std::string inviteIDStr = tim::jni::StringJni::Jstring2Cstring(env, invite_id);
    std::string dataStr = tim::jni::StringJni::Jstring2Cstring(env, data);

    tim::TIMEngine::GetInstance()->Accept(inviteIDStr.c_str(), dataStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeReject, jstring invite_id, jstring data, jobject callback) {

    std::string inviteIDStr = tim::jni::StringJni::Jstring2Cstring(env, invite_id);
    std::string dataStr = tim::jni::StringJni::Jstring2Cstring(env, data);

    tim::TIMEngine::GetInstance()->Reject(inviteIDStr.c_str(), dataStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

//TODO::该实现存在问题，待优化
DEFINE_NATIVE_FUNC(jobject, NativeGetSignalingInfo, jobject msg) {
    std::unique_ptr<json::Object> timMessage = tim::jni::MessageJni::Convert2CoreObject(msg);
    std::string timMessageStr = json::Serialize(*timMessage);
    jobject signalingInfoObj = nullptr;
    tim::TIMEngine::GetInstance()->GetSignalingInfo(timMessageStr.c_str(),
                                                    [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                                                        auto signalObj = (jobject) user_data;
                                                        signalObj = tim::jni::SignalingInfoJni::Convert2JObject(json::Deserialize(json_params));
                                                    }, signalingInfoObj);
    return signalingInfoObj;
}

DEFINE_NATIVE_FUNC(void, NativeAddInvitedSignaling, jobject info, jobject callback) {

    //暂不支持此项能力
//    json::Object signalingInfo;
//    bool flag = tim::jni::SignalingInfoJni::Convert2CoreObject(info, signalingInfo);
//    if (flag) {
//        tim::TIMEngine::GetInstance()->(signalingInfo, new tim::TIMCallbackIMpl(callback));
//    }

}

DEFINE_NATIVE_FUNC(void, NativeModifyInvitation, jstring invite_id, jstring data, jobject callback) {
    std::string inviteIdStr = tim::jni::StringJni::Jstring2Cstring(env,invite_id);
    std::string dataStr = tim::jni::StringJni::Jstring2Cstring(env,data);
    tim::TIMEngine::GetInstance()->ModifyInvitation(inviteIdStr.c_str(),dataStr.c_str(),new tim::TIMCallbackIMpl(callback));
}

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeAddSignalingListener",    "(Lcom/tencent/imsdk/v2/V2TIMSignalingListener;Ljava/lang/String;)V",                                              (void *) NativeAddSignalingListener},
        {"nativeRemoveSignalingListener", "(Ljava/lang/String;)V",                                                                                           (void *) NativeRemoveSignalingListener},
        {"nativeInvite",                  "(Ljava/lang/String;Ljava/lang/String;ZLcom/tencent/imsdk/v2/V2TIMOfflinePushInfo;"
                                          "ILcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;",                                                       (jstring *) NativeInvite},
        {"nativeInviteInGroup",           "(Ljava/lang/String;Ljava/util/List;Ljava/lang/String;ZILcom/tencent/imsdk/common/IMCallback;)Ljava/lang/String;", (jstring *) NativeInviteInGroup},
        {"nativeCancel",                  "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                    (void *) NativeCancel},
        {"nativeAccept",                  "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                    (void *) NativeAccept},
        {"nativeReject",                  "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                    (void *) NativeReject},
        {"nativeGetSignalingInfo",        "(Lcom/tencent/imsdk/v2/V2TIMMessage;)Lcom/tencent/imsdk/v2/V2TIMSignalingInfo;",                                  (jobject *) NativeGetSignalingInfo},
        {"nativeAddInvitedSignaling",     "(Lcom/tencent/imsdk/v2/V2TIMSignalingInfo;Lcom/tencent/imsdk/common/IMCallback;)V",                               (void *) NativeAddInvitedSignaling},
        {"nativeModifyInvitation",        "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",                                    (void *) NativeModifyInvitation},
};

//注册 native 方法
int RegisterNativeMethodsForV2TIMSignalingManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMSignalingManager", gMethods,
                                                 sizeof(gMethods) / sizeof(gMethods[0]));
}


#ifdef __cplusplus
}
#endif