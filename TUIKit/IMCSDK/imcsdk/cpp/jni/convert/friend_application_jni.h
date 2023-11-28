//
// Created by EDY on 2022/9/28.
//

#ifndef IMCPLUSPLUSDEMO_FRIEND_APPLICATION_JNI_H
#define IMCPLUSPLUSDEMO_FRIEND_APPLICATION_JNI_H

#include <jni.h>
#include "json.h"
#include "TIMFriendshipManager.h"

namespace tim {
    namespace jni {
        class FriendApplicationJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &application_json);
            static bool Convert2CoreObject_Request(const jobject &j_obj_application,json::Object &application_json);
            static bool Convert2CoreObject_Delete(const jobject &j_obj_application,std::string &userid);
            static int CPendencyType2JApplicationType(TIMFriendPendencyType cPendencyType);

        private:
            enum FieldID {
                FieldIDUserID = 0,
                FieldIDNickname,
                FieldIDFaceUrl,
                FieldIDAddTime,
                FieldIDAddSource,
                FieldIDAddWording,
                FieldIDType,

                FieldIDMax,
            };

            enum MethodID {
                MethodIDConstruct = 0,

                MethodIDMax,
            };

            enum JavaApplicationType {
                V2TIM_FRIEND_APPLICATION_COME_IN = 1,
                V2TIM_FRIEND_APPLICATION_SEND_OUT = 2,
                V2TIM_FRIEND_APPLICATION_BOTH = 3,
            };

            static jclass j_cls_;
            static jfieldID j_field_id_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_FRIEND_APPLICATION_JNI_H
