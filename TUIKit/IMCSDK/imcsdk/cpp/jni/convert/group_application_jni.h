//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_APPLICATION_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_APPLICATION_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupApplicationJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &groupApplication_json);
            static bool Convert2CoreObject(const jobject &j_obj_groupApplication,json::Object &groupApplication_json);

        private:
            enum FieldID{
                FieldIDGroupID = 0,
                FieldIDFromUser,
                FieldIDFromUserNickName,
                FieldIDFromUserFaceUrl,
                FieldIDToUser,
                FieldIDAddTime,
                FieldIDRequestMsg,
                FieldIDHandledMsg,
                FieldIDApplicationType,
                FieldIDHandleStatus,
                FieldIDHandleResult,
                FieldIDAuthentication,

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


#endif //IMCPLUSPLUSDEMO_GROUP_APPLICATION_JNI_H
