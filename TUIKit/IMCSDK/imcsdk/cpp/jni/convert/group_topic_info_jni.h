//
// Created by Junker on 2022/9/23.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_TOPIC_INFO_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_TOPIC_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupTopicInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &topicInfo_json);
            static bool Convert2CoreObject(const jobject &object,json::Object &topicInfo_json);

        private:
            enum FieldID {
                FieldIDTopicID = 0,
                FieldIDTopicName,
                FieldIDTopicFaceUrl,
                FieldIDIntroduction,
                FieldIDNotification,
                FieldIDAllMute,
                FieldIDSelfMuteTime,
                FieldIDCustomString,
                FieldIDRecvOpt,
                FieldIDDraftText,
                FieldIDUnreadCount,
                FieldIDLastMessage,
                FieldIDGroupAtInfoList,

                FieldIDMax,
            };

            enum MethodID {
                MethodIDConstruct = 0,
                MethodIDAddGroupAtInfo,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_id_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_GROUP_TOPIC_INFO_JNI_H
