//
// Created by EDY on 2022/9/23.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_LISTENER_JNI_H

#include <jni.h>
#include <memory>
#include <map>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupListenerJni final : public std::enable_shared_from_this<GroupListenerJni> {
        public:
            GroupListenerJni() = default;

            ~GroupListenerJni() = default;

            void AddListener(JNIEnv *env, jobject listener_simple, jstring listenerPath);

            void RemoveListener(JNIEnv *env, jstring listenerPath);

            static bool InitIDs(JNIEnv *env);

            bool empty();

        private:
            static void ImplTIMRecvNewMsgCallback(const char *json_msg_array, const void *user_data);

            static void ImplTIMGroupTipsEventCallback(const char *json_group_tip, const void *user_data);

            static void ImplTIMGroupAttributeChangedCallback(const char *group_id, const char *json_group_attribute_array, const void *user_data);

            static void ImplTIMGroupCounterChangedCallback(const char *group_id, const char *group_counter_key, int64_t group_counter_new_value, const void *user_data);

            static void ImplTIMGroupTopicCreatedCallback(const char *group_id, const char *topic_id, const void *user_data);

            static void ImplTIMGroupTopicDeletedCallback(const char *group_id, const char *topic_id_array, const void *user_data);

            static void ImplTIMGroupTopicChangedCallback(const char *group_id, const char *topic_info, const void *user_data);

        protected:
            //V2TIMGroupListener
            static void OnMemberEnter(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &memberList);

            static void OnMemberLeave(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &member);

            static void
            OnMemberInvited(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser, const json::Array &memberList);

            static void
            OnMemberKicked(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser, const json::Array &memberList);

            static void
            OnMemberInfoChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &v2TIMGroupMemberChangeInfoList);

            static void OnGroupCreated(const std::map<std::string, jobject> &_listener_, const std::string &groupID);

            static void OnGroupDismissed(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser);

            static void OnGroupRecycled(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser);

            static void OnGroupInfoChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &changeInfos);

            static void OnGroupAttributeChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &groupAttributeMap);

            static void OnReceiveJoinApplication(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &member,
                                                 const std::string &opReason);

            static void OnApplicationProcessed(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser, bool isAgreeJoin,
                                               const std::string &opReason);

            static void
            OnGrantAdministrator(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser, const json::Array &memberList);

            static void OnRevokeAdministrator(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser,
                                              const json::Array &memberList);

            static void OnQuitFromGroup(const std::map<std::string, jobject> &_listener_, const std::string &groupID);

            static void OnReceiveRESTCustomData(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const std::string &customData);

            static void OnTopicCreated(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const std::string &topicID);

            static void OnTopicDeleted(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &topicIDList);

            static void OnTopicChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &topicInfo);

            static void onGroupCounterChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const std::string &key, long newValue);

        private:
            enum MethodID {
                MethodIDOnMemberEnter = 0,
                MethodIDOnMemberLeave,
                MethodIDOnMemberInvited,
                MethodIDOnMemberKicked,
                MethodIDOnMemberInfoChanged,
                MethodIDOnGroupCreated,
                MethodIDOnGroupDismissed,
                MethodIDOnGroupRecycled,
                MethodIDOnGroupInfoChanged,
                MethodIDOnReceiveJoinRequest,
                MethodIDOnReceiveJoinResponse,
                MethodIDOnGrantAdministrator,
                MethodIDOnRevokeAdministrator,
                MethodIDOnQuitFromGroup,
                MethodIDOnReceiveCustomData,
                MethodIDOnGroupAttributeChanged,
                MethodIDOnTopicCreated,
                MethodIDOnTopicDeleted,
                MethodIDOnTopicChanged,
                MethodIDOnGroupCounterChanged,

                MethodIDMax,
            };

            std::map<std::string, jobject> listener_group_map;
            static jclass j_cls_;
            static jmethodID j_method_id_array_[MethodIDMax];
        };

    }// namespace jni
}// namespace tim


#endif //IMCPLUSPLUSDEMO_GROUP_LISTENER_JNI_H
