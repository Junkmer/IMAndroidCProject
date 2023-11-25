//
// Created by EDY on 2022/9/27.
//

#ifndef IMCPLUSPLUSDEMO_FRIEND_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_FRIEND_LISTENER_JNI_H

#include <jni.h>
#include <memory>
#include <map>
#include "json.h"
#include "observer_init.h"

namespace tim {
    namespace jni {
        class FriendListenerJni final : public std::enable_shared_from_this<FriendListenerJni>,Observer {
        private:
            //实现Observer 纯虚函数
            void initListener() override;

            void unInitListener() override;

        public:
            FriendListenerJni() = default;

            ~FriendListenerJni() = default;

            void AddListener(JNIEnv *env, jobject listener_friend, jstring listenerPath);

            void RemoveListener(JNIEnv *env, jstring listenerPath);

            static bool InitIDs(JNIEnv *env);

            bool empty();

        private:
            static void ImplTIMOnAddFriendCallback(const char *json_identifier_array, const void *user_data);

            static void ImplTIMOnDeleteFriendCallback(const char *json_identifier_array, const void *user_data);

            static void ImplTIMUpdateFriendProfileCallback(const char *json_friend_profile_update_array, const void *user_data);

            static void ImplTIMFriendAddRequestCallback(const char *json_friend_add_request_pendency_array, const void *user_data);

            static void ImplTIMFriendApplicationListDeletedCallback(const char *json_identifier_array, const void *user_data);

            static void ImplTIMFriendApplicationListReadCallback(const void *user_data);

            static void ImplTIMFriendBlackListAddedCallback(const char *json_friend_black_added_array, const void *user_data);

            static void ImplTIMFriendBlackListDeletedCallback(const char *json_identifier_array, const void *user_data);


        protected:
            // V2TIMFriendshipListener
            static void OnFriendApplicationListAdded(const std::map<std::string, jobject> &_listener_, const json::Array &applicationList_json);

            static void OnFriendApplicationListDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &userIDList_json);

            static void OnFriendApplicationListRead(const std::map<std::string, jobject> &_listener_);

            static void OnFriendListAdded(const std::map<std::string, jobject> &_listener_, const json::Array &friendInfoList_json);

            static void OnFriendListDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &userIDList_json);

            static void OnBlackListAdded(const std::map<std::string, jobject> &_listener_, const json::Array &friendInfoList_json);

            static void OnBlackListDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &userIDList_json);

            static void OnFriendInfoChanged(const std::map<std::string, jobject> &_listener_, const json::Array &friendInfoList_json);

        private:
            enum MethodID {
                MethodIDOnFriendApplicationListAdded = 0,
                MethodIDOnFriendApplicationListDeleted,
                MethodIDOnFriendApplicationListRead,
                MethodIDOnFriendListAdded,
                MethodIDOnFriendListDeleted,
                MethodIDOnBlackListAdd,
                MethodIDOnBlackListDeleted,
                MethodIDOnFriendInfoChanged,

                MethodIDMax,
            };
            std::map<std::string, jobject> listener_friend_map;
            static jclass j_cls_;
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_FRIEND_LISTENER_JNI_H
