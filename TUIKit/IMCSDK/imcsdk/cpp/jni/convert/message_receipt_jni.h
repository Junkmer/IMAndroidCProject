//
// Created by Junker on 2022/9/27.
//

#ifndef IMCPLUSPLUSDEMO_MESSAGE_RECEIPT_JNI_H
#define IMCPLUSPLUSDEMO_MESSAGE_RECEIPT_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class MessageReceiptJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &messageReceipt_json);

        private:
            enum FieldID{
                FieldIDMsgID = 0,
                FieldIDUserID,
                FieldIDPeerRead,
                FieldIDTimestamp,
                FieldIDGroupID,
                FieldIDReadCount,
                FieldIDUnreadCount,
                
                FieldIDMax,
            };
            
            enum MethodID{
                MethodIDConstructor = 0,
                
                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_MESSAGE_RECEIPT_JNI_H
