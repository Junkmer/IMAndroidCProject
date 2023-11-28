//
// Created by EDY on 2022/9/27.
//

#ifndef IMCPLUSPLUSDEMO_ADVANCED_MSG_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_ADVANCED_MSG_LISTENER_JNI_H

#include <jni.h>
#include <memory>
#include <map>
#include "json.h"
#include "observer_init.h"

namespace tim {
    namespace jni {
    class AdvancedMsgListenerJni final : public std::enable_shared_from_this<AdvancedMsgListenerJni>,Observer {
    private:
        //实现Observer 纯虚函数
        void initListener() override;

        void unInitListener() override;

        public:
            AdvancedMsgListenerJni() = default;

            ~AdvancedMsgListenerJni() = default;

            void AddListener(JNIEnv *env, jobject listener_advanced_msg, jstring listenerPath);

            void RemoveListener(JNIEnv *env, jstring listenerPath);

            static bool InitIDs(JNIEnv *env);

            bool empty();

        private:
            static void ImplTIMRecvNewMsgCallback(const char *json_msg_array, const void *user_data);

            static void ImplTIMMsgReadedReceiptCallback(const char *json_msg_readed_receipt_array, const void *user_data);

            static void ImplTIMMsgRevokeCallback(const char *json_msg_revoke_info_array, const void *user_data);

            static void ImplTIMMsgUpdateCallback(const char *json_msg_array, const void *user_data);

        protected:
            // V2TIMAdvancedMsgListener
            static void OnRecvNewMessage(const std::map<std::string, jobject> &_listener_,const json::Object &message_json);

            static void OnRecvC2CReadReceipt(const std::map<std::string, jobject> &_listener_,const json::Array &receiptList_json);

            static void OnRecvMessageReadReceipts(const std::map<std::string, jobject> &_listener_,const json::Array &receiptList_json);

            static void OnRecvMessageRevoked(const std::map<std::string, jobject> &_listener_,const std::string &messageID_json);

            static void OnRecvMessageModified(const std::map<std::string, jobject> &_listener_,const json::Object &message_json);

        private:
            enum MethodID {
                MethodIDOnRecvNewMessage = 0,
                MethodIDOnRecvMessageReadReceipts,
                MethodIDOnRecvC2CReadReceipt,
                MethodIDOnRecvMessageRevoked,
                MethodIDOnRecvMessageModified,

                MethodIDMax,
            };

            std::map<std::string, jobject> listener_advanced_msg_map;
            static jclass j_cls_;
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_ADVANCED_MSG_LISTENER_JNI_H
