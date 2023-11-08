//
// Created by EDY on 2022/9/21.
//

#include <LogUtil.h>
#include "elem_handler_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "image_jni.h"
#include "TIMCloudDef.h"
#include "message_jni.h"

namespace tim {
    namespace jni {
        IElemHandler::~IElemHandler() {
            if (j_cls_) {
                ScopedJEnv scopedJEnv;
                scopedJEnv.GetEnv()->DeleteGlobalRef(j_cls_);
                j_cls_ = nullptr;
            }
        }

//////////////////////////////////////////// V2TIMTextElem   ////////////////////////////////////////////

        std::string TextElemHandler::GetHandlerType() {
            return "TextElemHandler";
        }

        bool TextElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMTextElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "text", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[TextFieldText] = jfield;

            return true;
        }

        jobject TextElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Text) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (nullptr == jElemObj) {
                return nullptr;
            }

            size_t len = elem_json[kTIMTextElemContent].ToString().size();
            if (len > 0) {
                jstring textJStr = StringJni::Cstring2Jstring(env, elem_json[kTIMTextElemContent].ToString());
                env->SetObjectField(jElemObj, j_field_array_[TextFieldText], textJStr);
                env->DeleteLocalRef(textJStr);
            }
            return jElemObj;
        }

        std::unique_ptr<json::Object> TextElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Text || !jElemObj) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object textElem;
            textElem[kTIMElemType] = TIMElemType::kTIMElem_Text;

            auto jstr = (jstring) env->GetObjectField(jElemObj, j_field_array_[TextFieldText]);
            if (jstr) {
                textElem[kTIMTextElemContent] = StringJni::Jstring2Cstring(env, jstr).c_str();
                env->DeleteLocalRef(jstr);
            }

            return std::make_unique<json::Object>(textElem);
        }

//////////////////////////////////////////// V2TIMCustomElem ////////////////////////////////////////////

        std::string CustomElemHandler::GetHandlerType() {
            return "CustomElemHandler";
        }

        bool CustomElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMCustomElem");
            if (nullptr == cls) {
                return false;
            }

            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "data", "[B");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[CustomFieldData] = jfield;

            jfield = env->GetFieldID(j_cls_, "description", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[CustomFieldDescription] = jfield;

            jfield = env->GetFieldID(j_cls_, "extension", "[B");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[CustomFieldExtension] = jfield;

            return true;
        }

        jobject CustomElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Custom) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemOjb = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (!jElemOjb) {
                return nullptr;
            }

            size_t len = 0;

            len = elem_json[kTIMCustomElemData].ToString().size();
            if (len > 0) {
                jbyteArray byteArr = StringJni::Cstring2JbyteArray(env, elem_json[kTIMCustomElemData].ToString());
                env->SetObjectField(jElemOjb, j_field_array_[CustomFieldData], byteArr);
                env->DeleteLocalRef(byteArr);
            }

            len = elem_json[kTIMCustomElemDesc].ToString().size();
            if (len > 0) {
                jstring str = StringJni::Cstring2Jstring(env, elem_json[kTIMCustomElemDesc].ToString());
                env->SetObjectField(jElemOjb, j_field_array_[CustomFieldDescription], str);
                env->DeleteLocalRef(str);
            }

            len = elem_json[kTIMCustomElemExt].ToString().size();
            if (len > 0) {
                jbyteArray byteArr = StringJni::Cstring2JbyteArray(env, elem_json[kTIMCustomElemExt].ToString());
                env->SetObjectField(jElemOjb, j_field_array_[CustomFieldExtension], byteArr);
                env->DeleteLocalRef(byteArr);
            }

            return jElemOjb;
        }

        std::unique_ptr<json::Object> CustomElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Custom || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object customElem;
            customElem[kTIMElemType] = TIMElemType::kTIMElem_Custom;

            auto byteArr = (jbyteArray) env->GetObjectField(jElemObj, j_field_array_[CustomFieldData]);
            if (byteArr) {
                customElem[kTIMCustomElemData] = StringJni::JbyteArray2Cstring(env, byteArr);
                env->DeleteLocalRef(byteArr);
            }

            auto str = (jstring) env->GetObjectField(jElemObj, j_field_array_[CustomFieldDescription]);
            if (str) {
                customElem[kTIMCustomElemDesc] = StringJni::Jstring2Cstring(env, str).c_str();
                env->DeleteLocalRef(str);
            }

            byteArr = (jbyteArray) env->GetObjectField(jElemObj, j_field_array_[CustomFieldExtension]);
            if (byteArr) {
                customElem[kTIMCustomElemExt] = StringJni::JbyteArray2Cstring(env, byteArr).c_str();
                env->DeleteLocalRef(byteArr);
            }
            return std::make_unique<json::Object>(customElem);
        }

//////////////////////////////////////////// V2TIMImageElem  ////////////////////////////////////////////

        std::string ImageElemHandler::GetHandlerType() {
            return "ImageElemHandler";
        }

        bool ImageElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMImageElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[ImageMethodConstructor] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addImage", "(Lcom/tencent/imsdk/v2/V2TIMImageElem$V2TIMImage;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[ImageMethodAddImage] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "path", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[ImageFieldPath] = jfield;

            jfield = env->GetFieldID(j_cls_, "imageList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[ImageFieldImageList] = jfield;

            return true;
        }

        jobject ImageElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Image) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[ImageMethodConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            jstring str = StringJni::Cstring2Jstring(env, elem_json[kTIMImageElemOrigPath]);
            if (str) {
                env->SetObjectField(jElemObj, j_field_array_[ImageFieldPath], str);
                env->DeleteLocalRef(str);
            }

            json::Object orig_json;
            orig_json["uuid"] = elem_json[kTIMImageElemOrigId];
            orig_json["type"] = 0;
            orig_json["size"] = elem_json[kTIMImageElemOrigPicSize];
            orig_json["width"] = elem_json[kTIMImageElemOrigPicWidth];
            orig_json["height"] = elem_json[kTIMImageElemOrigPicHeight];
            orig_json["url"] = elem_json[kTIMImageElemOrigUrl];
            jobject orig_obj = ImageJni::Convert2JObject(env, orig_json);
            if (orig_obj) {
                env->CallVoidMethod(jElemObj, j_method_id_array_[ImageMethodAddImage], orig_obj);
                env->DeleteLocalRef(orig_obj);
            }

            json::Object thumb_json;
            thumb_json["uuid"] = elem_json[kTIMImageElemThumbId];
            thumb_json["type"] = 1;
            thumb_json["size"] = elem_json[kTIMImageElemThumbPicSize];
            thumb_json["width"] = elem_json[kTIMImageElemThumbPicWidth];
            thumb_json["height"] = elem_json[kTIMImageElemThumbPicHeight];
            thumb_json["url"] = elem_json[kTIMImageElemThumbUrl];
            jobject thumb_obj = ImageJni::Convert2JObject(env, thumb_json);
            if (thumb_obj) {
                env->CallVoidMethod(jElemObj, j_method_id_array_[ImageMethodAddImage], thumb_obj);
                env->DeleteLocalRef(thumb_obj);
            }

            json::Object large_json;
            large_json["uuid"] = elem_json[kTIMImageElemLargeId];
            large_json["type"] = 2;
            large_json["size"] = elem_json[kTIMImageElemLargePicSize];
            large_json["width"] = elem_json[kTIMImageElemLargePicWidth];
            large_json["height"] = elem_json[kTIMImageElemLargePicHeight];
            large_json["url"] = elem_json[kTIMImageElemLargeUrl];
            jobject large_obj = ImageJni::Convert2JObject(env, large_json);
            if (large_obj) {
                env->CallVoidMethod(jElemObj, j_method_id_array_[ImageMethodAddImage], large_obj);
                env->DeleteLocalRef(large_obj);
            }
            return jElemObj;
        }

        std::unique_ptr<json::Object> ImageElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Image || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object imageElem;
            imageElem[kTIMElemType] = TIMElemType::kTIMElem_Image;

            auto pathStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[ImageFieldPath]);
            if (pathStr) {
                imageElem[kTIMImageElemOrigPath] = StringJni::Jstring2Cstring(env, pathStr).c_str();
                env->DeleteLocalRef(pathStr);
            }
            imageElem[kTIMImageElemLevel] = TIMImageLevel::kTIMImageLevel_Orig;

            jobject imageList = env->GetObjectField(jElemObj, j_field_array_[ImageFieldImageList]);
            auto len = ArrayListJni::Size(imageList);

            for (int i = 0; i < len; ++i) {
                jobject item_obj = ArrayListJni::Get(imageList, i);
                if (nullptr == item_obj) {
                    continue;
                }
                ImageJni::Convert2CoreObject(item_obj, imageElem);
                env->DeleteLocalRef(item_obj);
            }
            env->DeleteLocalRef(imageList);

            return std::make_unique<json::Object>(imageElem);
        }

//////////////////////////////////////////// V2TIMSoundElem  ////////////////////////////////////////////

        std::string SoundElemHandler::GetHandlerType() {
            return "SoundElemHandler";
        }

        bool SoundElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMSoundElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "path", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[SoundFieldPath] = jfield;

            jfield = env->GetFieldID(j_cls_, "UUID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[SoundFieldUUID] = jfield;

            jfield = env->GetFieldID(j_cls_, "dataSize", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[SoundFieldDataSize] = jfield;

            jfield = env->GetFieldID(j_cls_, "duration", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[SoundFieldDuration] = jfield;

            jfield = env->GetFieldID(j_cls_, "fileId", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[SoundFieldFileId] = jfield;

            jfield = env->GetFieldID(j_cls_, "url", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[SoundFieldUrl] = jfield;

            return true;
        }

        jobject SoundElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Sound) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetObjectField(jElemObj, j_field_array_[SoundFieldPath],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMSoundElemFilePath].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[SoundFieldUUID],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMSoundElemBusinessId].ToString()));
            env->SetIntField(jElemObj, j_field_array_[SoundFieldDataSize], elem_json[kTIMSoundElemFileSize].ToInt());
            env->SetIntField(jElemObj, j_field_array_[SoundFieldDuration], elem_json[kTIMSoundElemFileTime].ToInt());
            env->SetObjectField(jElemObj, j_field_array_[SoundFieldPath],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMSoundElemFilePath].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[SoundFieldFileId],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMSoundElemFileId].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[SoundFieldUrl],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMSoundElemUrl].ToString()));

            return jElemObj;
        }

        std::unique_ptr<json::Object> SoundElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Sound || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object soundElem;
            soundElem[kTIMElemType] = TIMElemType::kTIMElem_Sound;

            jstring jStr = nullptr;
            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[SoundFieldPath]);
            if (jStr) {
                soundElem[kTIMSoundElemFilePath] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[SoundFieldUUID]);
            if (jStr) {
                soundElem[kTIMSoundElemBusinessId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            soundElem[kTIMSoundElemFileSize] = env->GetIntField(jElemObj, j_field_array_[SoundFieldDataSize]);
            soundElem[kTIMSoundElemFileTime] = env->GetIntField(jElemObj, j_field_array_[SoundFieldDuration]);

            return std::make_unique<json::Object>(soundElem);
        }

//////////////////////////////////////////// V2TIMVideoElem  ////////////////////////////////////////////

        std::string VideoElemHandler::GetHandlerType() {
            return "VideoElemHandler";
        }

        bool VideoElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMVideoElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;


            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "videoPath", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldVideoPath] = jfield;

            jfield = env->GetFieldID(j_cls_, "videoType", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldVideoType] = jfield;

            jfield = env->GetFieldID(j_cls_, "videoUUID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldVideoUUID] = jfield;

            jfield = env->GetFieldID(j_cls_, "videoSize", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldVideoSize] = jfield;

            jfield = env->GetFieldID(j_cls_, "duration", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldDuration] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotPath", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotPath] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotUUID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotUUID] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotSize", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotSize] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotWidth", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotWidth] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotHeight", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotHeight] = jfield;

            jfield = env->GetFieldID(j_cls_, "videoFileId", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldVideoFileId] = jfield;

            jfield = env->GetFieldID(j_cls_, "videoUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldVideoUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotFileId", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotFileId] = jfield;

            jfield = env->GetFieldID(j_cls_, "snapshotUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[VideoFieldSnapshotUrl] = jfield;

            return true;
        }

        jobject VideoElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Video) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetObjectField(jElemObj, j_field_array_[VideoFieldVideoPath],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemVideoPath].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[VideoFieldVideoUUID],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemBusinessId].ToString()));
            env->SetIntField(jElemObj, j_field_array_[VideoFieldVideoSize], elem_json[kTIMVideoElemVideoSize]);
            env->SetIntField(jElemObj, j_field_array_[VideoFieldDuration], elem_json[kTIMVideoElemVideoDuration]);

            env->SetObjectField(jElemObj, j_field_array_[VideoFieldSnapshotPath],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemImagePath].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[VideoFieldSnapshotUUID],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemImageId].ToString()));
            env->SetIntField(jElemObj, j_field_array_[VideoFieldSnapshotSize], elem_json[kTIMVideoElemImageSize]);
            env->SetIntField(jElemObj, j_field_array_[VideoFieldSnapshotWidth], elem_json[kTIMVideoElemImageWidth]);
            env->SetIntField(jElemObj, j_field_array_[VideoFieldSnapshotHeight], elem_json[kTIMVideoElemImageHeight]);
            env->SetObjectField(jElemObj, j_field_array_[VideoFieldVideoFileId],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemVideoId].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[VideoFieldVideoUrl],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemVideoUrl].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[VideoFieldSnapshotFileId],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemImageId].ToString()));
            env->SetObjectField(jElemObj, j_field_array_[VideoFieldSnapshotUrl],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMVideoElemImageUrl].ToString()));
            return jElemObj;
        }

        std::unique_ptr<json::Object> VideoElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Video || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object videoElem;
            videoElem[kTIMElemType] = TIMElemType::kTIMElem_Video;

            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[VideoFieldVideoPath]);
            if (jStr) {
                videoElem[kTIMVideoElemVideoPath] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[VideoFieldVideoType]);
            if (jStr) {
                videoElem[kTIMVideoElemVideoType] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[VideoFieldVideoUUID]);
            if (jStr) {
                videoElem[kTIMVideoElemBusinessId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            videoElem[kTIMVideoElemVideoSize] = env->GetIntField(jElemObj, j_field_array_[VideoFieldVideoSize]);
            videoElem[kTIMVideoElemVideoDuration] = env->GetIntField(jElemObj, j_field_array_[VideoFieldDuration]);

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[VideoFieldSnapshotPath]);
            if (jStr) {
                videoElem[kTIMVideoElemImagePath] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[VideoFieldSnapshotUUID]);
            if (jStr) {
                videoElem[kTIMVideoElemImageId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            videoElem[kTIMVideoElemImageSize] = env->GetIntField(jElemObj, j_field_array_[VideoFieldSnapshotSize]);
            videoElem[kTIMVideoElemImageWidth] = env->GetIntField(jElemObj, j_field_array_[VideoFieldSnapshotWidth]);
            videoElem[kTIMVideoElemImageHeight] = env->GetIntField(jElemObj, j_field_array_[VideoFieldSnapshotHeight]);

            return std::make_unique<json::Object>(videoElem);
        }

//////////////////////////////////////////// V2TIMFileElem   ////////////////////////////////////////////

        std::string FileElemHandler::GetHandlerType() {
            return "FileElemHandler";
        }

        bool FileElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFileElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "path", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FileFieldPath] = jfield;

            jfield = env->GetFieldID(j_cls_, "fileName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FileFieldFileName] = jfield;

            jfield = env->GetFieldID(j_cls_, "UUID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FileFieldUUID] = jfield;

            jfield = env->GetFieldID(j_cls_, "fileSize", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FileFieldFileSize] = jfield;

            jfield = env->GetFieldID(j_cls_, "fileId", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FileFieldFileId] = jfield;

            jfield = env->GetFieldID(j_cls_, "url", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FileFieldUrl] = jfield;

            return true;
        }

        jobject FileElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_File) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetObjectField(jElemObj, j_field_array_[FileFieldPath], StringJni::Cstring2Jstring(env, elem_json[kTIMFileElemFilePath]));
            env->SetObjectField(jElemObj, j_field_array_[FileFieldFileName], StringJni::Cstring2Jstring(env, elem_json[kTIMFileElemFileName]));
            env->SetObjectField(jElemObj, j_field_array_[FileFieldUUID], StringJni::Cstring2Jstring(env, elem_json[kTIMFileElemBusinessId]));
            env->SetIntField(jElemObj, j_field_array_[FileFieldFileSize], elem_json[kTIMFileElemFileSize]);
            env->SetObjectField(jElemObj, j_field_array_[FileFieldFileId], StringJni::Cstring2Jstring(env, elem_json[kTIMFileElemFileId]));
            env->SetObjectField(jElemObj, j_field_array_[FileFieldUrl], StringJni::Cstring2Jstring(env, elem_json[kTIMFileElemUrl]));
            return jElemObj;
        }

        std::unique_ptr<json::Object> FileElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_File || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object fileElem;
            fileElem[kTIMElemType] = TIMElemType::kTIMElem_File;

            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[FileFieldPath]);
            if (jStr) {
                fileElem[kTIMFileElemFilePath] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[FileFieldFileName]);
            if (jStr) {
                fileElem[kTIMFileElemFileName] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[FileFieldUUID]);
            if (jStr) {
                fileElem[kTIMFileElemBusinessId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            fileElem[kTIMFileElemFileSize] = env->GetIntField(jElemObj, j_field_array_[FileFieldFileSize]);

            return std::make_unique<json::Object>(fileElem);
        }

//////////////////////////////////////////// V2TIMLocationElem ////////////////////////////////////////////

        std::string LocationElemHandler::GetHandlerType() {
            return "LocationElemHandler";
        }

        bool LocationElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMLocationElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "desc", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[LocationFieldDesc] = jfield;

            jfield = env->GetFieldID(j_cls_, "longitude", "D");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[LocationFieldLongitude] = jfield;

            jfield = env->GetFieldID(j_cls_, "latitude", "D");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[LocationFieldLatitude] = jfield;

            return true;
        }

        jobject LocationElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Location) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetObjectField(jElemObj, j_field_array_[LocationFieldDesc],
                                StringJni::Cstring2Jstring(env, elem_json[kTIMLocationElemDesc].ToString()));
            env->SetDoubleField(jElemObj, j_field_array_[LocationFieldLongitude], elem_json[kTIMLocationElemLongitude].ToDouble());
            env->SetDoubleField(jElemObj, j_field_array_[LocationFieldLatitude], elem_json[kTIMLocationElemLatitude]);

            return jElemObj;
        }

        std::unique_ptr<json::Object> LocationElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Location || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object locationElem;
            locationElem[kTIMElemType] = TIMElemType::kTIMElem_Location;

            jstring jStr;
            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[LocationFieldDesc]);
            if (jStr) {
                locationElem[kTIMLocationElemDesc] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            locationElem[kTIMLocationElemLongitude] = env->GetDoubleField(jElemObj, j_field_array_[LocationFieldLongitude]);
            locationElem[kTIMLocationElemLatitude] = env->GetDoubleField(jElemObj, j_field_array_[LocationFieldLatitude]);

            return std::make_unique<json::Object>(locationElem);
        }

//////////////////////////////////////////// V2TIMFaceElem   ////////////////////////////////////////////

        std::string FaceElemHandler::GetHandlerType() {
            return "FaceElemHandler";
        }

        bool FaceElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFaceElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[BaseElemConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "index", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FaceFieldIndex] = jfield;

            jfield = env->GetFieldID(j_cls_, "data", "[B");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FaceFieldData] = jfield;

            return true;
        }

        jobject FaceElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Face) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[BaseElemConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetIntField(jElemObj, j_field_array_[FaceFieldIndex], elem_json[kTIMFaceElemIndex]);

            jbyteArray byteArr = StringJni::Cstring2JbyteArray(env, elem_json[kTIMFaceElemBuf].ToString());
            if (byteArr) {
                env->SetObjectField(jElemObj, j_field_array_[FaceFieldData], byteArr);
                env->DeleteLocalRef(byteArr);
            }

            return jElemObj;
        }

        std::unique_ptr<json::Object> FaceElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Face || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object faceElem;
            faceElem[kTIMElemType] = TIMElemType::kTIMElem_Face;

            faceElem[kTIMFaceElemIndex] = env->GetIntField(jElemObj, j_field_array_[FaceFieldIndex]);
            auto byteArr = (jbyteArray) env->GetObjectField(jElemObj, j_field_array_[FaceFieldData]);
            if (byteArr){
                faceElem[kTIMFaceElemBuf] = StringJni::JbyteArray2Cstring(env,byteArr);
                env->DeleteLocalRef(byteArr);
            }

            return std::make_unique<json::Object>(faceElem);
        }

//////////////////////////////////////////// V2TIMMergerElem ////////////////////////////////////////////

        std::string MergerElemHandler::GetHandlerType() {
            return "MergerElemHandler";
        }

        bool MergerElemHandler::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMergerElem");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MergerMethodConstructor] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addAbstract", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MergerMethodAddAbstract] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "layersOverLimit", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldLayersOverLimit] = jfield;

            jfield = env->GetFieldID(j_cls_, "messageList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldMessageList] = jfield;

            jfield = env->GetFieldID(j_cls_, "title", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldTitle] = jfield;

            jfield = env->GetFieldID(j_cls_, "abstractList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldAbstractList] = jfield;

            jfield = env->GetFieldID(j_cls_, "compatibleText", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldCompatibleText] = jfield;

            jfield = env->GetFieldID(j_cls_, "relayPbKey", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldRelayPbKey] = jfield;

            jfield = env->GetFieldID(j_cls_, "relayJsonKey", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldRelayJsonKey] = jfield;

            jfield = env->GetFieldID(j_cls_, "relayBuffer", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[MergerFieldRelayBuffer] = jfield;

            return true;
        }

        jobject MergerElemHandler::Convert2JObject(const json::Object &elem_json) {
            if (elem_json.size() == 0 || elem_json[kTIMElemType].ToInt() != TIMElemType::kTIMElem_Merge) {
                return nullptr;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jElemObj = env->NewObject(j_cls_, j_method_id_array_[MergerMethodConstructor]);
            if (!jElemObj) {
                return nullptr;
            }

            env->SetBooleanField(jElemObj, j_field_array_[MergerFieldLayersOverLimit], elem_json[kTIMMergerElemLayersOverLimit].ToBool());
            if (elem_json.HasKey(kTIMMergerElemMsgArray)){
                json::Array message_array = elem_json[kTIMMergerElemMsgArray];
                jobject list_obj = ArrayListJni::NewArrayList();
                for (const auto &item : message_array){
                    ArrayListJni::Add(list_obj,tim::jni::MessageJni::Convert2JObject(item));
                }
                env->SetObjectField(jElemObj,j_field_array_[MergerFieldMessageList],list_obj);
                env->DeleteLocalRef(list_obj);
            }

            env->SetObjectField(jElemObj, j_field_array_[MergerFieldTitle], StringJni::Cstring2Jstring(env, elem_json[kTIMMergerElemTitle]));

            if (elem_json.HasKey(kTIMMergerElemAbstractArray)){
                json::Array abstractArray = elem_json[kTIMMergerElemAbstractArray];
                for (const auto & i : abstractArray) {
                    jstring abstractStr = StringJni::Cstring2Jstring(env, i.ToString());
                    env->CallVoidMethod(jElemObj, j_method_id_array_[MergerMethodAddAbstract], abstractStr);
                    env->DeleteLocalRef(abstractStr);
                }
            }

            jstring compatibleText_jStr = StringJni::Cstring2Jstring(env, elem_json[kTIMMergerElemCompatibleText]);
            if (compatibleText_jStr) {
                env->SetObjectField(jElemObj, j_field_array_[MergerFieldCompatibleText], compatibleText_jStr);
                env->DeleteLocalRef(compatibleText_jStr);
            }

            jstring relayPbKey_jStr = StringJni::Cstring2Jstring(env, elem_json[kTIMMergerElemRelayPbKey]);
            if (relayPbKey_jStr) {
                env->SetObjectField(jElemObj, j_field_array_[MergerFieldRelayPbKey], relayPbKey_jStr);
                env->DeleteLocalRef(relayPbKey_jStr);
            }

            jstring relayJsonKey_jStr = StringJni::Cstring2Jstring(env, elem_json[kTIMMergerElemRelayJsonKey]);
            if (relayJsonKey_jStr) {
                env->SetObjectField(jElemObj, j_field_array_[MergerFieldRelayJsonKey], relayJsonKey_jStr);
                env->DeleteLocalRef(relayJsonKey_jStr);
            }

            jstring relayBuffer_jStr = StringJni::Cstring2Jstring(env, elem_json[kTIMMergerElemRelayBuffer]);
            if (relayBuffer_jStr) {
                env->SetObjectField(jElemObj, j_field_array_[MergerFieldRelayBuffer], relayBuffer_jStr);
                env->DeleteLocalRef(relayBuffer_jStr);
            }

            return jElemObj;
        }

        std::unique_ptr<json::Object> MergerElemHandler::Convert2CoreObject(int elemType, jobject jElemObj) {
            if (TIMElemType(elemType) != TIMElemType::kTIMElem_Merge || !jElemObj) return nullptr;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object mergerElem;
            mergerElem[kTIMElemType] = TIMElemType::kTIMElem_Merge;

            mergerElem[kTIMMergerElemLayersOverLimit] = (bool) env->GetBooleanField(jElemObj, j_field_array_[MergerFieldLayersOverLimit]);
            jstring jStr;
            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[MergerFieldTitle]);
            if (jStr){
                mergerElem[kTIMMergerElemTitle] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jobject messageListObj = env->GetObjectField(jElemObj,j_field_array_[MergerFieldMessageList]);
            size_t msg_size = ArrayListJni::Size(messageListObj);
            if (msg_size > 0){
                json::Array msg_array;
                for (int i = 0; i < msg_size; ++i){
                    auto item = (jobject) ArrayListJni::Get(messageListObj, i);
                    std::unique_ptr<json::Object> timMessage = tim::jni::MessageJni::Convert2CoreObject(item);
                    msg_array.push_back(*timMessage);
                    env->DeleteLocalRef(item);
                }
                env->DeleteLocalRef(messageListObj);
                mergerElem[kTIMMergerElemMsgArray] = msg_array;
            }

            jobject abstractListObj = env->GetObjectField(jElemObj, j_field_array_[MergerFieldAbstractList]);
            size_t len = ArrayListJni::Size(abstractListObj);
            if (len > 0){
                json::Array abstract_array;
                for (int i = 0; i < len; ++i) {
                    auto item = (jstring) ArrayListJni::Get(abstractListObj, i);
                    abstract_array.push_back(StringJni::Jstring2Cstring(env, item).c_str());
                    env->DeleteLocalRef(item);
                }
                env->DeleteLocalRef(abstractListObj);
                mergerElem[kTIMMergerElemAbstractArray] = abstract_array;
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[MergerFieldCompatibleText]);
            if (jStr) {
                mergerElem[kTIMMergerElemCompatibleText] = StringJni::Jstring2Cstring(env, jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[MergerFieldRelayPbKey]);
            if (jStr) {
                mergerElem[kTIMMergerElemRelayPbKey] = StringJni::Jstring2Cstring(env, jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[MergerFieldRelayJsonKey]);
            if (jStr) {
                mergerElem[kTIMMergerElemRelayJsonKey] = StringJni::Jstring2Cstring(env, jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(jElemObj, j_field_array_[MergerFieldRelayBuffer]);
            if (jStr) {
                mergerElem[kTIMMergerElemRelayBuffer] = StringJni::Jstring2Cstring(env, jStr);
                env->DeleteLocalRef(jStr);
            }

            return std::make_unique<json::Object>(mergerElem);
        }
    }// namespace jni
}// namespace tim
