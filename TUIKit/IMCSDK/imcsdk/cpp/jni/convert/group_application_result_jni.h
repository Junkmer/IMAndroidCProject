//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_APPLICATION_RESULT_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_APPLICATION_RESULT_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupApplicationResultJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &applicationResult_json);

        private:
            enum FieldID{
                FieldIDUnreadCount = 0,
                FieldIDGroupApplicationList,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,
                MethodIDAddGroupApplication,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }//namespace jni
}//namespace tim


#endif //IMCPLUSPLUSDEMO_GROUP_APPLICATION_RESULT_JNI_H
