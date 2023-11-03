//
// Created by EDY on 2022/10/8.
//

#ifndef IMCPLUSPLUSDEMO_MESSAGE_SEARCH_RESULT_ITEM_JNI_H
#define IMCPLUSPLUSDEMO_MESSAGE_SEARCH_RESULT_ITEM_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class MessageSearchResultItemJni{
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &searchResultItem_json);

        private:
            enum FieldID{
                FieldIDConversationID = 0,
                FieldIDMessageCount,
                FieldIDMessageList,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,
                MethodIDAddMessage,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_MESSAGE_SEARCH_RESULT_ITEM_JNI_H
