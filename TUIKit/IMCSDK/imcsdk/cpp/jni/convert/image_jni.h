//
// Created by EDY on 2022/9/21.
//

#ifndef IMCPLUSPLUSDEMO_IMAGE_JNI_H
#define IMCPLUSPLUSDEMO_IMAGE_JNI_H

#include <jni.h>
#include <memory>
#include "json.h"

namespace tim {
    namespace jni {
        class ImageJni {
        public:

            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(JNIEnv *env,const json::Object &image_json);
            static bool Convert2CoreObject(const jobject &jElemObj,json::Object &image_json);
        public:
            enum FieldID{
                FieldIDUuid = 0,
                FieldIDType,
                FieldIDSize,
                FieldIDWidth,
                FieldIDHeight,
                FieldIDUrl,
                FieldDownloadParam,

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
    }
}
#endif //IMCPLUSPLUSDEMO_IMAGE_JNI_H
