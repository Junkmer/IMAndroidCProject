//
// Created by EDY on 2022/9/27.
//

#include <LogUtil.h>
#include <tim_engine.h>
#include <jni_helper.h>
#include "signaling_listener_jni.h"
#include "java_basic_jni.h"
#include "TIMCloud.h"

namespace tim {
    namespace jni {

        jclass SignalingListenerJni::j_cls_;
        jmethodID SignalingListenerJni::j_method_id_array_[MethodIDMax];

        void SignalingListenerJni::AddListener(JNIEnv *env, jobject listener_signaling, jstring listenerPath) {
            if (nullptr == listener_signaling) {
                LOGE("GroupListenerJni | AddListener listener_simple is null");
                return;
            }
            if (listener_signaling_map.empty()) {
                TIMSetSignalingReceiveNewInvitationCallback(ImplTIMSignalingReceiveNewInvitationCallback, &listener_signaling_map);
                TIMSetSignalingInviteeAcceptedCallback(ImplTIMSignalingInviteeAcceptedCallback, &listener_signaling_map);
                TIMSetSignalingInviteeRejectedCallback(ImplTIMSignalingInviteeRejectedCallback, &listener_signaling_map);
                TIMSetSignalingInvitationCancelledCallback(ImplTIMSignalingInvitationCancelledCallback, &listener_signaling_map);
                TIMSetSignalingInvitationTimeoutCallback(ImplTIMSignalingInvitationTimeoutCallback, &listener_signaling_map);
                TIMSetSignalingInvitationModifiedCallback(ImplTIMSignalingInvitationModifiedCallback, &listener_signaling_map);
            }

            std::string path = StringJni::Jstring2Cstring(env, listenerPath);
            for (auto &item: listener_signaling_map) {
                if (path.empty() || path == item.first) {
                    return;
                }
            }

            jobject j_obj = env->NewGlobalRef(listener_signaling);
            listener_signaling_map.insert(std::make_pair(path, j_obj));
        }

        void SignalingListenerJni::RemoveListener(JNIEnv *env, jstring listenerPath) {
            if (nullptr == listenerPath) {
                LOGE("SimpleListenerJni | RemoveListener listener_simple is null");
                return;
            }
            listener_signaling_map.erase(StringJni::Jstring2Cstring(env, listenerPath));
            if (listener_signaling_map.empty()) {
                TIMSetSignalingReceiveNewInvitationCallback(ImplTIMSignalingReceiveNewInvitationCallback, nullptr);
                TIMSetSignalingInviteeAcceptedCallback(ImplTIMSignalingInviteeAcceptedCallback, nullptr);
                TIMSetSignalingInviteeRejectedCallback(ImplTIMSignalingInviteeRejectedCallback, nullptr);
                TIMSetSignalingInvitationCancelledCallback(ImplTIMSignalingInvitationCancelledCallback, nullptr);
                TIMSetSignalingInvitationTimeoutCallback(ImplTIMSignalingInvitationTimeoutCallback, nullptr);
                TIMSetSignalingInvitationModifiedCallback(ImplTIMSignalingInvitationModifiedCallback, nullptr);
            }
        }

        bool SignalingListenerJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMSignalingListener");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "onReceiveNewInvitation",
                                       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/util/List;Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnReceiveNewInvitation] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onInviteeAccepted", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnInviteeAccepted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onInviteeRejected", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnInviteeRejected] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onInvitationCancelled", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnInvitationCancelled] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onInvitationTimeout", "(Ljava/lang/String;Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnInvitationTimeout] = jmethod;

            jmethod = env->GetMethodID(j_cls_,"onInvitationModified", "(Ljava/lang/String;Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnIInvitationModified] = jmethod;

            return true;
        }

        bool SignalingListenerJni::empty() {
            return listener_signaling_map.empty();
        }

        void SignalingListenerJni::ImplTIMSignalingReceiveNewInvitationCallback(const char *invite_id, const char *inviter, const char *group_id,
                                                                                const char *json_invitee_list, const char *data, const void *user_data) {
            OnReceiveNewInvitation(*(std::map<std::string, jobject> *) user_data, invite_id, inviter, group_id, json::Deserialize(json_invitee_list), data);
        }

        void SignalingListenerJni::ImplTIMSignalingInviteeAcceptedCallback(const char *invite_id, const char *invitee, const char *data, const void *user_data) {
            OnInviteeAccepted(*(std::map<std::string, jobject> *) user_data,invite_id, invitee, data);
        }

        void SignalingListenerJni::ImplTIMSignalingInviteeRejectedCallback(const char *invite_id, const char *invitee, const char *data, const void *user_data) {
            OnInviteeRejected(*(std::map<std::string, jobject> *) user_data,invite_id,invitee,data);
        }

        void SignalingListenerJni::ImplTIMSignalingInvitationCancelledCallback(const char *invite_id, const char *inviter, const char *data, const void *user_data) {
            OnInvitationCancelled(*(std::map<std::string, jobject> *) user_data,invite_id,inviter,data);
        }

        void
        SignalingListenerJni::ImplTIMSignalingInvitationTimeoutCallback(const char *invite_id, const char *json_invitee_list, const void *user_data) {
            OnInvitationTimeout(*(std::map<std::string, jobject> *) user_data,invite_id,json::Deserialize(json_invitee_list));
        }

        void SignalingListenerJni::ImplTIMSignalingInvitationModifiedCallback(const char *invite_id, const char *data, const void *user_data) {
            onInvitationModified(*(std::map<std::string, jobject> *) user_data,invite_id,data);
        }

        void SignalingListenerJni::OnReceiveNewInvitation(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &inviter,
                                                          const std::string &groupID,
                                                          const json::Array &inviteeList, const std::string &data) {
            if (_listener.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring jStr_inviteID = StringJni::Cstring2Jstring(env, inviteID);
            jstring jStr_inviter = StringJni::Cstring2Jstring(env, inviter);
            jstring jStr_groupID = StringJni::Cstring2Jstring(env, groupID);

            jobject inviteeListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < inviteeList.size(); ++i) {
                jstring inviteeObj = StringJni::Cstring2Jstring(env, inviteeList[i]);
                if (inviteeObj) {
                    ArrayListJni::Add(inviteeListObj, inviteeObj);
                    env->DeleteLocalRef(inviteeObj);
                }
            }

            jstring jStr_data = StringJni::Cstring2Jstring(env, data);

            for (auto &item: _listener) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnReceiveNewInvitation], jStr_inviteID, jStr_inviter,
                                    jStr_groupID, inviteeListObj, jStr_data);
            }

            env->DeleteLocalRef(jStr_inviteID);
            env->DeleteLocalRef(jStr_inviter);
            env->DeleteLocalRef(jStr_groupID);
            env->DeleteLocalRef(inviteeListObj);
            env->DeleteLocalRef(jStr_data);
        }

        void SignalingListenerJni::OnInviteeAccepted(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &invitee,
                                                     const std::string &data) {
            if (_listener.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring jStr_inviteID = StringJni::Cstring2Jstring(env, inviteID);
            jstring jStr_invitee = StringJni::Cstring2Jstring(env, invitee);
            jstring jStr_data = StringJni::Cstring2Jstring(env, data);

            for (auto &item: _listener) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnInviteeAccepted], jStr_inviteID, jStr_invitee, jStr_data);
            }

            env->DeleteLocalRef(jStr_inviteID);
            env->DeleteLocalRef(jStr_invitee);
            env->DeleteLocalRef(jStr_data);
        }

        void SignalingListenerJni::OnInviteeRejected(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &invitee,
                                                     const std::string &data) {
            if (_listener.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring jStr_inviteID = StringJni::Cstring2Jstring(env, inviteID);
            jstring jStr_invitee = StringJni::Cstring2Jstring(env, invitee);
            jstring jStr_data = StringJni::Cstring2Jstring(env, data);

            for (auto &item: _listener) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnInviteeRejected], jStr_inviteID, jStr_invitee, jStr_data);
            }

            env->DeleteLocalRef(jStr_inviteID);
            env->DeleteLocalRef(jStr_invitee);
            env->DeleteLocalRef(jStr_data);
        }

        void SignalingListenerJni::OnInvitationCancelled(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &inviter,
                                                         const std::string &data) {
            if (_listener.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring jStr_inviteID = StringJni::Cstring2Jstring(env, inviteID);
            jstring jStr_inviter = StringJni::Cstring2Jstring(env, inviter);
            jstring jStr_data = StringJni::Cstring2Jstring(env, data);

            for (auto &item: _listener) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnInvitationCancelled], jStr_inviteID, jStr_inviter, jStr_data);
            }

            env->DeleteLocalRef(jStr_inviteID);
            env->DeleteLocalRef(jStr_inviter);
            env->DeleteLocalRef(jStr_data);
        }

        void SignalingListenerJni::OnInvitationTimeout(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const json::Array &inviteeList) {
            if (_listener.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring jStr_inviteID = StringJni::Cstring2Jstring(env, inviteID);

            jobject inviteeListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < inviteeList.size(); ++i) {
                jstring inviteeObj = StringJni::Cstring2Jstring(env, inviteeList[i]);
                if (inviteeObj) {
                    ArrayListJni::Add(inviteeListObj, inviteeObj);
                    env->DeleteLocalRef(inviteeObj);
                }
            }

            for (auto &item: _listener) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnInvitationTimeout], jStr_inviteID, inviteeListObj);
            }

            env->DeleteLocalRef(jStr_inviteID);
            env->DeleteLocalRef(inviteeListObj);
        }

        void SignalingListenerJni::onInvitationModified(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &data) {
            if (_listener.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring jStr_inviteID = StringJni::Cstring2Jstring(env, inviteID);
            jstring jStr_data = StringJni::Cstring2Jstring(env, data);

            for (auto &item: _listener) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnIInvitationModified], jStr_inviteID, jStr_data);
            }

            env->DeleteLocalRef(jStr_inviteID);
            env->DeleteLocalRef(jStr_data);
        }

    }// namespace jni
}// namespace tim