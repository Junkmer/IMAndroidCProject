//
// Created by EDY on 2023/11/2.
//

#ifndef IMCSDKDEMO_MESSAGE_EXTENSION_JNI_H
#define IMCSDKDEMO_MESSAGE_EXTENSION_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class MessageExtensionJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &messageExtension_json);
            static bool Convert2CoreObject(const jobject &j_obj_messageExtension, json::Object &messageExtension_json);

        private:
            enum FieldID{
                FieldIDExtensionKey = 0,
                FieldIDExtensionValue,

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


#endif //IMCSDKDEMO_MESSAGE_EXTENSION_JNI_H
