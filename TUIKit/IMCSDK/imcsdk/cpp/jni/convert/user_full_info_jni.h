//
// Created by EDY on 2022/9/16.
//

#ifndef IMCPLUSPLUSDEMO_USER_FULL_INFO_JNI_H
#define IMCPLUSPLUSDEMO_USER_FULL_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class UserFullInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &userFullInfo_json_obj);
            static jobject Convert2JObject_UpdateCallback(const json::Object &friend_info_item_json);
            static bool Convert2CoreObject(const jobject &j_obj_userFullInfo,json::Object &userFullInfo_json);
            static bool Convert2CoreObject_SetSelfInfo(const jobject &j_obj_userFullInfo,json::Object &userFullInfo_json);

        private:
            enum FieldID {
                FieldIDUserID = 0,
                FieldIDNickname,
                FieldIDFaceUrl,
                FieldIDSelfSignature,
                FieldIDGender,
                FieldIDRole,
                FieldIDLevel,
                FieldIDBirthday,
                FieldIDAllowType,
                FieldIDCustomHashMap,

                FieldIDMax
            };

            enum MethodID {
                MethodIDConstruct = 0,

                MethodIDMax,
            };

            static jclass j_cls;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }
}

#endif //IMCPLUSPLUSDEMO_USER_FULL_INFO_JNI_H
