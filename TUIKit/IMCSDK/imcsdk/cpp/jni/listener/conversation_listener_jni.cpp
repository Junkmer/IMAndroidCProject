//
// Created by EDY on 2022/9/27.
//

#include <LogUtil.h>
#include <tim_engine.h>
#include <jni_helper.h>
#include "conversation_listener_jni.h"
#include "conversation_jni.h"
#include "java_basic_jni.h"
#include "TIMCloud.h"
#include "conversation_list_filter_jni.h"

namespace tim {
    namespace jni {

        jclass ConversationListenerJni::j_cls_;
        jmethodID ConversationListenerJni::j_method_id_array_[MethodIDMax];

        void ConversationListenerJni::InitListener() {
            TIMSetConvEventCallback(ImplTIMConvEventCallback, &listener_conversation_map);
            TIMSetConvTotalUnreadMessageCountChangedCallback(ImplTIMConvTotalUnreadMessageCountChangedCallback, &listener_conversation_map);
            TIMSetConvUnreadMessageCountChangedByFilterCallback(ImplTIMConvTotalUnreadMessageCountChangedByFilterCallback,
                                                                &listener_conversation_map);
            TIMSetConvConversationGroupCreatedCallback(ImplTIMConvConversationGroupCreatedCallback, &listener_conversation_map);
            TIMSetConvConversationGroupDeletedCallback(ImplTIMConvConversationGroupDeletedCallback, &listener_conversation_map);
            TIMSetConvConversationGroupNameChangedCallback(ImplTIMConvConversationGroupNameChangedCallback, &listener_conversation_map);
            TIMSetConvConversationsAddedToGroupCallback(ImplTIMConvConversationsAddedToGroupCallback, &listener_conversation_map);
            TIMSetConvConversationsDeletedFromGroupCallback(ImplTIMConvConversationsDeletedFromGroupCallback, &listener_conversation_map);
        }

        void ConversationListenerJni::UnInitListener() {
            TIMSetConvEventCallback(nullptr, nullptr);
            TIMSetConvTotalUnreadMessageCountChangedCallback(nullptr, nullptr);
            TIMSetConvUnreadMessageCountChangedByFilterCallback(nullptr, nullptr);
            TIMSetConvConversationGroupCreatedCallback(nullptr, nullptr);
            TIMSetConvConversationGroupDeletedCallback(nullptr, nullptr);
            TIMSetConvConversationGroupNameChangedCallback(nullptr, nullptr);
            TIMSetConvConversationsAddedToGroupCallback(nullptr, nullptr);
            TIMSetConvConversationsDeletedFromGroupCallback(nullptr, nullptr);
        }

        void ConversationListenerJni::AddListener(JNIEnv *env, jobject listener_conversation, jstring listenerPath) {
            if (nullptr == listener_conversation) {
                LOGE("ConversationListenerJni | AddListener listener_simple is null");
                return;
            }
//            if (listener_conversation_map.empty()) {
//                TIMSetConvEventCallback(ImplTIMConvEventCallback, &listener_conversation_map);
//                TIMSetConvTotalUnreadMessageCountChangedCallback(ImplTIMConvTotalUnreadMessageCountChangedCallback, &listener_conversation_map);
//                TIMSetConvUnreadMessageCountChangedByFilterCallback(ImplTIMConvTotalUnreadMessageCountChangedByFilterCallback,
//                                                                    &listener_conversation_map);
//                TIMSetConvConversationGroupCreatedCallback(ImplTIMConvConversationGroupCreatedCallback, &listener_conversation_map);
//                TIMSetConvConversationGroupDeletedCallback(ImplTIMConvConversationGroupDeletedCallback, &listener_conversation_map);
//                TIMSetConvConversationGroupNameChangedCallback(ImplTIMConvConversationGroupNameChangedCallback, &listener_conversation_map);
//                TIMSetConvConversationsAddedToGroupCallback(ImplTIMConvConversationsAddedToGroupCallback, &listener_conversation_map);
//                TIMSetConvConversationsDeletedFromGroupCallback(ImplTIMConvConversationsDeletedFromGroupCallback, &listener_conversation_map);
//            }

            std::string path = StringJni::Jstring2Cstring(env, listenerPath);
            for (auto &item: listener_conversation_map) {
                if (path.empty() || path == item.first) {
                    return;
                }
            }

            jobject j_obj = env->NewGlobalRef(listener_conversation);
            listener_conversation_map.insert(std::make_pair(path, j_obj));
        }

        void ConversationListenerJni::RemoveListener(JNIEnv *env, jstring listenerPath) {
            if (nullptr == listenerPath) {
                LOGE("ConversationListenerJni | RemoveListener listener is null");
                return;
            }
            listener_conversation_map.erase(StringJni::Jstring2Cstring(env, listenerPath));
//            if (listener_conversation_map.empty()) {
//                TIMSetConvEventCallback(nullptr, nullptr);
//                TIMSetConvTotalUnreadMessageCountChangedCallback(nullptr, nullptr);
//                TIMSetConvUnreadMessageCountChangedByFilterCallback(nullptr, nullptr);
//                TIMSetConvConversationGroupCreatedCallback(nullptr, nullptr);
//                TIMSetConvConversationGroupDeletedCallback(nullptr, nullptr);
//                TIMSetConvConversationGroupNameChangedCallback(nullptr, nullptr);
//                TIMSetConvConversationsAddedToGroupCallback(nullptr, nullptr);
//                TIMSetConvConversationsDeletedFromGroupCallback(nullptr, nullptr);
//            }
        }

        bool ConversationListenerJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMConversationListener");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "onSyncServerStart", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnSyncServerStart] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onSyncServerFinish", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnSyncServerFinish] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onSyncServerFailed", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnSyncServerFailed] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onNewConversation", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnNewConversation] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationChanged", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationDeleted", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationDeleted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onTotalUnreadMessageCountChanged", "(J)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnTotalUnreadMessageCountChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onUnreadMessageCountChangedByFilter", "(Lcom/tencent/imsdk/v2/V2TIMConversationListFilter;J)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnUnreadMessageCountChangedByFilter] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationGroupCreated", "(Ljava/lang/String;Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationGroupCreated] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationGroupDeleted", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationGroupDeleted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationGroupNameChanged", "(Ljava/lang/String;Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationGroupNameChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationsAddedToGroup", "(Ljava/lang/String;Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationsAddedToGroup] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onConversationsDeletedFromGroup", "(Ljava/lang/String;Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnConversationsDeletedFromGroup] = jmethod;

            return true;
        }

        bool ConversationListenerJni::empty() {
            return listener_conversation_map.empty();
        }

        void ConversationListenerJni::ImplTIMConvEventCallback(enum TIMConvEvent conv_event, const char *json_conv_array, const void *user_data) {
            /**
             * >> 会话新增
             * >> 会话删除
             * >> 会话更新
             * >> 会话开始
             * >> 会话结束
             */
             auto listener = *(std::map<std::string, jobject> *) user_data;
             json::Array conv_array_json = json::Deserialize(json_conv_array);
            if (conv_event == TIMConvEvent::kTIMConvEvent_Add || conv_event == TIMConvEvent::kTIMConvEvent_Update) {
                if (conv_event == TIMConvEvent::kTIMConvEvent_Update) {
                    OnConversationChanged(listener,conv_array_json);
                } else {
                    OnNewConversation(listener,conv_array_json);
                }
            } else {
                switch (conv_event) {
                    case TIMConvEvent::kTIMConvEvent_Del:
                        onConversationDeleted(listener,conv_array_json);
                        break;
                    case TIMConvEvent::kTIMConvEvent_Start:
                        OnSyncServerStart(listener);
                        break;
                    case TIMConvEvent::kTIMConvEvent_Finish:
                        OnSyncServerFinish(listener);
                        break;
                    default:
                        OnSyncServerFailed(listener);
                        break;
                }
            }

        }

        void ConversationListenerJni::ImplTIMConvTotalUnreadMessageCountChangedCallback(int total_unread_count, const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            OnTotalUnreadMessageCountChanged(listener,total_unread_count);
        }

        void ConversationListenerJni::ImplTIMConvTotalUnreadMessageCountChangedByFilterCallback(const char *filter, int total_unread_count,
                                                                                                const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            json::Object filter_json = json::Deserialize(filter);
            onUnreadMessageCountChangedByFilter(listener,filter_json,total_unread_count);
        }

        void ConversationListenerJni::ImplTIMConvConversationGroupCreatedCallback(const char *group_name, const char *conversation_array,
                                                                                  const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            OnConversationGroupCreated(listener,group_name,json::Deserialize(conversation_array));
        }

        void ConversationListenerJni::ImplTIMConvConversationGroupDeletedCallback(const char *group_name, const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            OnConversationGroupDeleted(listener,group_name);
        }

        void
        ConversationListenerJni::ImplTIMConvConversationGroupNameChangedCallback(const char *old_name, const char *new_name, const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            OnConversationGroupNameChanged(listener,old_name,new_name);
        }

        void ConversationListenerJni::ImplTIMConvConversationsAddedToGroupCallback(const char *group_name, const char *conversation_array,
                                                                                   const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            OnConversationsAddedToGroup(listener,group_name,json::Deserialize(conversation_array));
        }

        void ConversationListenerJni::ImplTIMConvConversationsDeletedFromGroupCallback(const char *group_name, const char *conversation_array,
                                                                                       const void *user_data) {
            auto listener = *(std::map<std::string, jobject> *) user_data;
            OnConversationsDeletedFromGroup(listener,group_name,json::Deserialize(conversation_array));
        }

        void ConversationListenerJni::OnSyncServerStart(const std::map<std::string, jobject> &_listener_) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnSyncServerStart]);
            }
        }

        void ConversationListenerJni::OnSyncServerFinish(const std::map<std::string, jobject> &_listener_) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnSyncServerFinish]);
            }
        }

        void ConversationListenerJni::OnSyncServerFailed(const std::map<std::string, jobject> &_listener_) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnSyncServerFailed]);
            }
        }

        void ConversationListenerJni::OnNewConversation(const std::map<std::string, jobject> &_listener_, const json::Array &conversationList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject conversationObjList = ArrayListJni::NewArrayList();
            for (const auto &item: conversationList) {
                jobject conversationObj = ConversationJni::Convert2JObject(item);
                if (conversationObj) {
                    ArrayListJni::Add(conversationObjList, conversationObj);
                    env->DeleteLocalRef(conversationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnNewConversation], conversationObjList);
            }

            env->DeleteLocalRef(conversationObjList);
        }

        void ConversationListenerJni::OnConversationChanged(const std::map<std::string, jobject> &_listener_, const json::Array &conversationList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject conversationObjList = ArrayListJni::NewArrayList();
            for (const auto &item: conversationList) {
                jobject conversationObj = ConversationJni::Convert2JObject(item);
                if (conversationObj) {
                    ArrayListJni::Add(conversationObjList, conversationObj);
                    env->DeleteLocalRef(conversationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationChanged], conversationObjList);
            }

            env->DeleteLocalRef(conversationObjList);
        }

        void ConversationListenerJni::onConversationDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &conversationIDList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject conversationObjList = ArrayListJni::NewArrayList();
            for (const auto &item: conversationIDList) {
                jobject conversationObj = StringJni::Cstring2Jstring(env,item);
                if (conversationObj) {
                    ArrayListJni::Add(conversationObjList, conversationObj);
                    env->DeleteLocalRef(conversationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationDeleted], conversationObjList);
            }

            env->DeleteLocalRef(conversationObjList);
        }

        void ConversationListenerJni::OnTotalUnreadMessageCountChanged(const std::map<std::string, jobject> &_listener_, long totalUnreadCount) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnTotalUnreadMessageCountChanged], totalUnreadCount);
            }
        }

        void ConversationListenerJni::onUnreadMessageCountChangedByFilter(const std::map<std::string, jobject> &_listener_, json::Object &filter_json,
                                                                          long totalUnreadCount) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject filter_obj = ConversationListFilterJni::Convert2JObject(filter_json);
            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnUnreadMessageCountChangedByFilter], filter_obj,
                                    (jlong) totalUnreadCount);
            }
            env->DeleteLocalRef(filter_obj);
        }

        void ConversationListenerJni::OnConversationGroupCreated(const std::map<std::string, jobject> &_listener_, const std::string &groupName,
                                                                 const json::Array &conversationList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupNameStr = StringJni::Cstring2Jstring(env, groupName);

            jobject conversationObjList = ArrayListJni::NewArrayList();
            for (const auto &item: conversationList) {
                jobject conversationObj = ConversationJni::Convert2JObject(item);
                if (conversationObj) {
                    ArrayListJni::Add(conversationObjList, conversationObj);
                    env->DeleteLocalRef(conversationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationGroupCreated], groupNameStr, conversationObjList);
            }

            env->DeleteLocalRef(groupNameStr);
            env->DeleteLocalRef(conversationObjList);
        }

        void ConversationListenerJni::OnConversationGroupDeleted(const std::map<std::string, jobject> &_listener_, const std::string &groupName) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupNameStr = StringJni::Cstring2Jstring(env, groupName);

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationGroupDeleted], groupNameStr);
            }

            env->DeleteLocalRef(groupNameStr);
        }

        void ConversationListenerJni::OnConversationGroupNameChanged(const std::map<std::string, jobject> &_listener_, const std::string &oldName,
                                                                     const std::string &newName) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring oldNameStr = StringJni::Cstring2Jstring(env, oldName);
            jstring newNameStr = StringJni::Cstring2Jstring(env, newName);

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationGroupNameChanged], oldNameStr, newNameStr);
            }

            env->DeleteLocalRef(oldNameStr);
            env->DeleteLocalRef(newNameStr);
        }

        void ConversationListenerJni::OnConversationsAddedToGroup(const std::map<std::string, jobject> &_listener_, const std::string &groupName,
                                                                  const json::Array &conversationList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupNameStr = StringJni::Cstring2Jstring(env, groupName);

            jobject conversationObjList = ArrayListJni::NewArrayList();
            for (const auto &item: conversationList) {
                jobject conversationObj = ConversationJni::Convert2JObject(item);
                if (conversationObj) {
                    ArrayListJni::Add(conversationObjList, conversationObj);
                    env->DeleteLocalRef(conversationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationsAddedToGroup], groupNameStr, conversationObjList);
            }

            env->DeleteLocalRef(groupNameStr);
            env->DeleteLocalRef(conversationObjList);
        }

        void ConversationListenerJni::OnConversationsDeletedFromGroup(const std::map<std::string, jobject> &_listener_, const std::string &groupName,
                                                                      const json::Array &conversationList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupNameStr = StringJni::Cstring2Jstring(env, groupName);

            jobject conversationObjList = ArrayListJni::NewArrayList();
            for (const auto &item: conversationList) {
                jobject conversationObj = ConversationJni::Convert2JObject(item);
                if (conversationObj) {
                    ArrayListJni::Add(conversationObjList, conversationObj);
                    env->DeleteLocalRef(conversationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnConversationsDeletedFromGroup], groupNameStr, conversationObjList);
            }

            env->DeleteLocalRef(groupNameStr);
            env->DeleteLocalRef(conversationObjList);
        }

    }// namespace jni
}// namespace tim