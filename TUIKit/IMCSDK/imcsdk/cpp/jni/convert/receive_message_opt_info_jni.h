//
// Created by EDY on 2022/10/8.
//

#ifndef IMCPLUSPLUSDEMO_RECEIVE_MESSAGE_OPT_INFO_JNI_H
#define IMCPLUSPLUSDEMO_RECEIVE_MESSAGE_OPT_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class ReceiveMessageOptInfoJni{
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &messageOptInfo_json);
            static jobject Convert2JObject_AllRecvMsg(const json::Object &messageOptInfo_json);

        private:
            enum FieldID{
                FieldIDStartHour = 0,
                FieldIDStartMinute,
                FieldIDStartSecond,
                FieldIDStartTimeStamp,
                FieldIDDuration,
                FieldIDAllReceiveMessageOpt,
                FieldIDC2CReceiveMessageOpt,
                FieldIDUserID,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_RECEIVE_MESSAGE_OPT_INFO_JNI_H
