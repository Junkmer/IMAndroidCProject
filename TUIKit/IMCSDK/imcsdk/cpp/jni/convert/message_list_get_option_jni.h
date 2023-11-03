//
// Created by EDY on 2022/10/8.
//

#ifndef IMCPLUSPLUSDEMO_MESSAGE_LIST_GET_OPTION_JNI_H
#define IMCPLUSPLUSDEMO_MESSAGE_LIST_GET_OPTION_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class MessageListGetOptionJni{
        public:
            static bool InitIDs(JNIEnv *env);
            static bool Convert2CoreObject(const jobject &j_obj_getOption,json::Object &get_msg_param_json);

        private:
            enum FieldID{
                FieldIDGetType = 0,
                FieldIDUserID,
                FieldIDGroupID,
                FieldIDCount,
                FieldIDLastMsg,
                FieldIDLastMsgSeq,
                FieldIDGetTimeBegin,
                FieldIDGetTimePeriod,
                FieldIDMessageTypeList,

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
    }
}

#endif //IMCPLUSPLUSDEMO_MESSAGE_LIST_GET_OPTION_JNI_H
