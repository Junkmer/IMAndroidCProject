//
// Created by EDY on 2023/11/2.
//

#ifndef IMCSDKDEMO_MESSAGE_REACTION_JNI_H
#define IMCSDKDEMO_MESSAGE_REACTION_JNI_H


class message_reaction_jni {

};

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class MessageReactionJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &messageReaction_json);
            static bool Convert2CoreObject(const jobject &j_obj_messageReaction, json::Object &messageReaction_json);

        private:
            enum FieldID{
                FieldIDReactionID = 0,
                FieldIDTotalUserCount,
                FieldIDPartialUserList,
                FieldIDReactedByMyself,

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


#endif //IMCSDKDEMO_MESSAGE_REACTION_JNI_H
