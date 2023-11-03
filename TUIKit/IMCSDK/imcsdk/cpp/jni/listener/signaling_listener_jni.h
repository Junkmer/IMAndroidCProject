//
// Created by EDY on 2022/9/27.
//

#ifndef IMCPLUSPLUSDEMO_SIGNALING_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_SIGNALING_LISTENER_JNI_H

#include <jni.h>
#include <memory>
#include <map>
#include "json.h"

namespace tim {
    namespace jni {
        class SignalingListenerJni final : public std::enable_shared_from_this<SignalingListenerJni> {
        public:
            SignalingListenerJni() = default;

            ~SignalingListenerJni() = default;

            void AddListener(JNIEnv *env, jobject listener_signaling, jstring listenerPath);

            void RemoveListener(JNIEnv *env, jstring listenerPath);

            static bool InitIDs(JNIEnv *env);

            bool empty();

        private:
            static void ImplTIMSignalingReceiveNewInvitationCallback(const char *invite_id, const char *inviter, const char *group_id,
                                                                     const char *json_invitee_list, const char *data, const void *user_data);

            static void ImplTIMSignalingInviteeAcceptedCallback(const char *invite_id, const char *invitee, const char *data, const void *user_data);

            static void ImplTIMSignalingInviteeRejectedCallback(const char *invite_id, const char *invitee, const char *data, const void *user_data);

            static void ImplTIMSignalingInvitationCancelledCallback(const char *invite_id, const char *inviter, const char *data, const void *user_data);

            static void ImplTIMSignalingInvitationTimeoutCallback(const char *invite_id, const char *json_invitee_list, const void *user_data);

            static void ImplTIMSignalingInvitationModifiedCallback(const char *invite_id, const char *data, const void *user_data);

        protected:
            // V2TIMSignalingListener
            static void
            OnReceiveNewInvitation(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &inviter, const std::string &groupID,
                                   const json::Array &inviteeList, const std::string &data);

            static void
            OnInviteeAccepted(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &invitee, const std::string &data);

            static void
            OnInviteeRejected(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &invitee, const std::string &data);

            static void
            OnInvitationCancelled(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &inviter, const std::string &data);

            static void OnInvitationTimeout(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const json::Array &inviteeList);

            static void onInvitationModified(const std::map<std::string, jobject> &_listener, const std::string &inviteID, const std::string &data);

        private:
            enum MethodID {
                MethodIDOnReceiveNewInvitation = 0,
                MethodIDOnInviteeAccepted,
                MethodIDOnInviteeRejected,
                MethodIDOnInvitationCancelled,
                MethodIDOnInvitationTimeout,
                MethodIDOnIInvitationModified,

                MethodIDMax,
            };

            std::map<std::string, jobject> listener_signaling_map;
            static jclass j_cls_;
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_SIGNALING_LISTENER_JNI_H
