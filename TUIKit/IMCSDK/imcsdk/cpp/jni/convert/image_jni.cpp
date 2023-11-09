//
// Created by EDY on 2022/9/21.
//

#include "image_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMMessageManager.h"

namespace tim {
    namespace jni {

        jclass ImageJni::j_cls_;
        jfieldID ImageJni::j_field_array_[FieldIDMax];
        jmethodID ImageJni::j_method_id_array_[MethodIDMax];

        bool ImageJni::InitIDs(JNIEnv *env) {
            if (j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMImageElem$V2TIMImage");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "uuid", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUuid] = jfield;

            jfield = env->GetFieldID(j_cls_, "type", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDType] = jfield;

            jfield = env->GetFieldID(j_cls_, "size", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSize] = jfield;

            jfield = env->GetFieldID(j_cls_, "width", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDWidth] = jfield;

            jfield = env->GetFieldID(j_cls_, "height", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDHeight] = jfield;

            jfield = env->GetFieldID(j_cls_, "url", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUrl] = jfield;

            return true;
        }

        jobject ImageJni::Convert2JObject(JNIEnv *env,const json::Object &image_json) {

            jobject jImage = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!jImage) {
                return nullptr;
            }
            env->SetObjectField(jImage, j_field_array_[FieldIDUuid], StringJni::Cstring2Jstring(env, image_json["uuid"]));
            env->SetIntField(jImage, j_field_array_[FieldIDType], image_json["type"].ToInt());
            env->SetIntField(jImage, j_field_array_[FieldIDSize], image_json["size"].ToInt());
            env->SetIntField(jImage, j_field_array_[FieldIDWidth], image_json["width"].ToInt());
            env->SetIntField(jImage, j_field_array_[FieldIDHeight], image_json["height"].ToInt());
            env->SetObjectField(jImage, j_field_array_[FieldIDUrl], StringJni::Cstring2Jstring(env, image_json["url"]));
            return jImage;
        }

        bool ImageJni::Convert2CoreObject(const jobject &jElemObj, json::Object &image_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            auto type = env->GetIntField(jElemObj, j_field_array_[FieldIDType]);
            auto uuidStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[FieldIDUuid]);
            auto urlStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[FieldIDUrl]);
            if (type == 0){//orig
                if (uuidStr){
                    image_json[kTIMImageElemOrigId] = StringJni::Jstring2Cstring(env, uuidStr);
                    env->DeleteLocalRef(uuidStr);
                }
                image_json[kTIMImageElemOrigPicSize] = env->GetIntField(jElemObj, j_field_array_[FieldIDSize]);
                image_json[kTIMImageElemOrigPicWidth] = env->GetIntField(jElemObj, j_field_array_[FieldIDWidth]);
                image_json[kTIMImageElemOrigPicHeight] = env->GetIntField(jElemObj, j_field_array_[FieldIDHeight]);
                if (urlStr){
                    image_json[kTIMImageElemOrigUrl] = StringJni::Jstring2Cstring(env, urlStr).c_str();
                    env->DeleteLocalRef(urlStr);
                }
            } else if (type == 1){//thumb
                if (uuidStr){
                    image_json[kTIMImageElemThumbId] = StringJni::Jstring2Cstring(env, uuidStr);
                    env->DeleteLocalRef(uuidStr);
                }
                image_json[kTIMImageElemThumbPicSize] = env->GetIntField(jElemObj, j_field_array_[FieldIDSize]);
                image_json[kTIMImageElemThumbPicWidth] = env->GetIntField(jElemObj, j_field_array_[FieldIDWidth]);
                image_json[kTIMImageElemThumbPicHeight] = env->GetIntField(jElemObj, j_field_array_[FieldIDHeight]);
                if (urlStr){
                    image_json[kTIMImageElemThumbUrl] = StringJni::Jstring2Cstring(env, urlStr).c_str();
                    env->DeleteLocalRef(urlStr);
                }
            } else if (type ==2){//large
                if (uuidStr){
                    image_json[kTIMImageElemLargeId] = StringJni::Jstring2Cstring(env, uuidStr);
                    env->DeleteLocalRef(uuidStr);
                }
                image_json[kTIMImageElemLargePicSize] = env->GetIntField(jElemObj, j_field_array_[FieldIDSize]);
                image_json[kTIMImageElemLargePicWidth] = env->GetIntField(jElemObj, j_field_array_[FieldIDWidth]);
                image_json[kTIMImageElemLargePicHeight] = env->GetIntField(jElemObj, j_field_array_[FieldIDHeight]);
                if (urlStr){
                    image_json[kTIMImageElemLargeUrl] = StringJni::Jstring2Cstring(env, urlStr).c_str();
                    env->DeleteLocalRef(urlStr);
                }
            }
            return true;
        }

    }
}