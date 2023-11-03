//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_SIGNALING_INFO_JNI_H
#define IMCPLUSPLUSDEMO_SIGNALING_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class SignalingInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &signalingInfo_json);
            static bool Convert2CoreObject(const jobject &j_obj_signalingInfo, json::Object &signalingInfo_json);

        private:
            enum FieldID {
                FieldIDInviteID = 0,
                FieldIDGroupID,
                FieldIDInviter,
                FieldIDInviteeList,
                FieldIDData,
                FieldIDTimeout,
                FieldIDActionType,

                FieldIDMax
            };

            enum MethodID {
                MethodIDConstruct = 0,
                MethodIDAddInvitee,

                MethodIDMax,
            };

            static jclass j_cls;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_SIGNALING_INFO_JNI_H
