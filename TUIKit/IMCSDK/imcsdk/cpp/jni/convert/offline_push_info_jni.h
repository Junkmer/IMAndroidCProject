//
// Created by EDY on 2022/9/23.
//

#ifndef IMCPLUSPLUSDEMO_OFFLINE_PUSH_INFO_JNI_H
#define IMCPLUSPLUSDEMO_OFFLINE_PUSH_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class OfflinePushInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &offlinePushInfo_json);
            static bool Convert2CoreObject(const jobject &object, json::Object &offlinePushInfo_json);

        private:
            enum FieldID {
                FieldIDTitle = 0,
                FieldIDDesc,
                FieldIDExt,
                FieldIDDisable,
                FieldIDIOSSound,
                FieldIDAndroidSound,
                FieldIDIgnoreIOSBadge,
                FieldIDOppoChannelID,
                FieldIDFcmChannelID,
                FieldIDXiaomiChannelID,
                FieldIDVivoClassification,
                FieldIDVivoCategory,
                FieldIDIOSPushType,
                FieldIDHuaweiCategory,

                FieldIDMax
            };

            enum MethodID {
                MethodIDConstructor = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim


#endif //IMCPLUSPLUSDEMO_OFFLINE_PUSH_INFO_JNI_H
