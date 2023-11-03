//
// Created by Junker on 2022/9/15.
//

#ifndef IMCPLUSPLUSDEMO_SIMPLE_LISTENER_JNI_H
#define IMCPLUSPLUSDEMO_SIMPLE_LISTENER_JNI_H

#include <jni.h>
#include <memory>
#include <map>
#include "json.h"

namespace tim {
    namespace jni {
        class SimpleListenerJni final : public std::enable_shared_from_this<SimpleListenerJni> {

        public:
            SimpleListenerJni() = default;

            ~SimpleListenerJni() = default;

            void AddListener(JNIEnv *env, jobject listener_simple);

            void RemoveListener(jobject listener_simple);

            static bool InitIDs(JNIEnv *env);

            bool empty();

        protected:
            static void TIMAddRecvNewMsgCallbackImpl(const char *json_msg_array, const void *user_data);

            static void OnRecvC2CTextMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId,const json::Object &sender,
                                             const std::string &text);

            static void
            OnRecvC2CCustomMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId, const json::Object &sender,
                                   const std::string &customData);

            static void
            OnRecvGroupTextMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId, const std::string &groupId,
                                   const json::Object &sender, const std::string &text);

            static void
            OnRecvGroupCustomMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId, const std::string &groupId,
                                     const json::Object &sender, const std::string &customData);

        private:

            enum MethodID {
                MethodIDOnRecvC2CTextMessage = 0,
                MethodIDOnRecvC2CCustomMessage,
                MethodIDOnRecvGroupTextMessage,
                MethodIDOnRecvGroupCustomMessage,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jmethodID j_method_id_array_[MethodIDMax];
            std::map<jobject, jobject> listener_simple_map;
        };
    }
}

#endif //IMCPLUSPLUSDEMO_SIMPLE_LISTENER_JNI_H
