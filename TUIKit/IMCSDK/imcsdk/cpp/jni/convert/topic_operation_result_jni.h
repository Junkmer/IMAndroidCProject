//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_TOPIC_OPERATION_RESULT_JNI_H
#define IMCPLUSPLUSDEMO_TOPIC_OPERATION_RESULT_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class TopicOperationResultJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &operationResult_json);

        private:
            enum FieldID{
                FieldIDErrorCode = 0,
                FieldIDErrorMessage,
                FieldIDTopicID,

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
    }//namespace jni
}//namespace tim

#endif //IMCPLUSPLUSDEMO_TOPIC_OPERATION_RESULT_JNI_H
