//
// Created by EDY on 2022/9/27.
//

#ifndef IMCPLUSPLUSDEMO_CONVERSATION_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_CONVERSATION_LISTENER_JNI_H

#include <jni.h>
#include <memory>
#include <map>
#include "TIMCloud.h"
#include "json.h"

namespace tim {
    namespace jni {
        class ConversationListenerJni final : public std::enable_shared_from_this<ConversationListenerJni> {
        public:
            void InitListener();

            void UnInitListener();

        public:
            ConversationListenerJni() = default;

            ~ConversationListenerJni() = default;

            void AddListener(JNIEnv *env, jobject listener_conversation, jstring listenerPath);

            void RemoveListener(JNIEnv *env, jstring listenerPath);

            static bool InitIDs(JNIEnv *env);

            bool empty();

        private://实现 IM C SDK 接口回调
            static void ImplTIMConvEventCallback(enum TIMConvEvent conv_event, const char *json_conv_array, const void *user_data);

            static void ImplTIMConvTotalUnreadMessageCountChangedCallback(int total_unread_count, const void *user_data);

            static void ImplTIMConvTotalUnreadMessageCountChangedByFilterCallback(const char *filter, int total_unread_count, const void *user_data);

            static void ImplTIMConvConversationGroupCreatedCallback(const char *group_name, const char *conversation_array, const void *user_data);

            static void ImplTIMConvConversationGroupDeletedCallback(const char *group_name, const void *user_data);

            static void ImplTIMConvConversationGroupNameChangedCallback(const char *old_name, const char *new_name, const void *user_data);

            static void ImplTIMConvConversationsAddedToGroupCallback(const char *group_name, const char *conversation_array, const void *user_data);

            static void
            ImplTIMConvConversationsDeletedFromGroupCallback(const char *group_name, const char *conversation_array, const void *user_data);

        protected:
            // V2TIMConversationListener
            static void OnSyncServerStart(const std::map<std::string, jobject> &_listener_);

            static void OnSyncServerFinish(const std::map<std::string, jobject> &_listener_);

            static void OnSyncServerFailed(const std::map<std::string, jobject> &_listener_);

            static void OnNewConversation(const std::map<std::string, jobject> &_listener_, const json::Array &conversationList);

            static void OnConversationChanged(const std::map<std::string, jobject> &_listener_, const json::Array &conversationList);

            static void onConversationDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &conversationIDList);

            static void OnTotalUnreadMessageCountChanged(const std::map<std::string, jobject> &_listener_, long totalUnreadCount);

            static void
            onUnreadMessageCountChangedByFilter(const std::map<std::string, jobject> &_listener_, json::Object &filter_json, long totalUnreadCount);

            static void OnConversationGroupCreated(const std::map<std::string, jobject> &_listener_, const std::string &groupName,
                                                   const json::Array &conversationList);

            static void OnConversationGroupDeleted(const std::map<std::string, jobject> &_listener_, const std::string &groupName);

            static void
            OnConversationGroupNameChanged(const std::map<std::string, jobject> &_listener_, const std::string &oldName, const std::string &newName);

            static void OnConversationsAddedToGroup(const std::map<std::string, jobject> &_listener_, const std::string &groupName,
                                                    const json::Array &conversationList);

            static void OnConversationsDeletedFromGroup(const std::map<std::string, jobject> &_listener_, const std::string &groupName,
                                                        const json::Array &conversationList);

        private:
            enum MethodID {
                MethodIDOnSyncServerStart = 0,
                MethodIDOnSyncServerFinish,
                MethodIDOnSyncServerFailed,
                MethodIDOnNewConversation,
                MethodIDOnConversationChanged,
                MethodIDOnConversationDeleted,
                MethodIDOnTotalUnreadMessageCountChanged,
                MethodIDOnUnreadMessageCountChangedByFilter,
                MethodIDOnConversationGroupCreated,
                MethodIDOnConversationGroupDeleted,
                MethodIDOnConversationGroupNameChanged,
                MethodIDOnConversationsAddedToGroup,
                MethodIDOnConversationsDeletedFromGroup,

                MethodIDMax,
            };

            std::map<std::string, jobject> listener_conversation_map;
            static jclass j_cls_;
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_CONVERSATION_LISTENER_JNI_H
