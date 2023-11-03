//
// Created by Junker on 2022/10/17.
//

#include <jni.h>
#include "jni_util.h"
#include "java_basic_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "tim_callback_impl.h"
#include "value_callback_impl.h"
#include "im_callbak_jni.h"
#include "download_callback_impl.h"
#include "elem_processor_jni.h"
#include "message_jni.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_NATIVE_FUNC(void, NativeDownloadImage, jstring uuid,jstring url, int type,jstring path, jobject callback) {
    json::Object download_param;
    download_param[kTIMMsgDownloadElemParamBusinessId] = tim::jni::StringJni::Jstring2Cstring(env,uuid);
    download_param[kTIMMsgDownloadElemParamUrl] = tim::jni::StringJni::Jstring2Cstring(env,url);
    std::string paramStr = json::Serialize(download_param);

    std::string pathStr = tim::jni::StringJni::Jstring2Cstring(env,path);

    tim::TIMEngine::GetInstance()->DownloadElemToPath(pathStr.c_str(),pathStr.c_str(),new tim::DownloadTIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDownloadSound, jstring file_id, jstring uuid, jstring url, jstring path, jobject callback) {
    json::Object download_param;
    download_param[kTIMMsgDownloadElemParamType] = TIMDownloadType::kTIMDownload_Sound;
    download_param[kTIMMsgDownloadElemParamId] = tim::jni::StringJni::Jstring2Cstring(env,file_id);
    download_param[kTIMMsgDownloadElemParamBusinessId] = tim::jni::StringJni::Jstring2Cstring(env,uuid);
    download_param[kTIMMsgDownloadElemParamUrl] = tim::jni::StringJni::Jstring2Cstring(env,url);
    std::string paramStr = json::Serialize(download_param);

    std::string pathStr = tim::jni::StringJni::Jstring2Cstring(env,path);

    tim::TIMEngine::GetInstance()->DownloadElemToPath(pathStr.c_str(),pathStr.c_str(),new tim::DownloadTIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDownloadVideo, jstring file_id, jstring uuid, jstring url, jstring path, jobject callback) {
    json::Object download_param;
    download_param[kTIMMsgDownloadElemParamType] = TIMDownloadType::kTIMDownload_Video;
    download_param[kTIMMsgDownloadElemParamId] = tim::jni::StringJni::Jstring2Cstring(env,file_id);
    download_param[kTIMMsgDownloadElemParamBusinessId] = tim::jni::StringJni::Jstring2Cstring(env,uuid);
    download_param[kTIMMsgDownloadElemParamUrl] = tim::jni::StringJni::Jstring2Cstring(env,url);
    std::string paramStr = json::Serialize(download_param);

    std::string pathStr = tim::jni::StringJni::Jstring2Cstring(env,path);

    tim::TIMEngine::GetInstance()->DownloadElemToPath(pathStr.c_str(),pathStr.c_str(),new tim::DownloadTIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDownloadSnapshot, jstring file_id, jstring uuid, jstring url, jstring path, jobject callback) {
    json::Object download_param;
    download_param[kTIMMsgDownloadElemParamType] = TIMDownloadType::kTIMDownload_VideoThumb;
    download_param[kTIMMsgDownloadElemParamId] = tim::jni::StringJni::Jstring2Cstring(env,file_id);
    download_param[kTIMMsgDownloadElemParamBusinessId] = tim::jni::StringJni::Jstring2Cstring(env,uuid);
    download_param[kTIMMsgDownloadElemParamUrl] = tim::jni::StringJni::Jstring2Cstring(env,url);
    std::string paramStr = json::Serialize(download_param);

    std::string pathStr = tim::jni::StringJni::Jstring2Cstring(env,path);

    tim::TIMEngine::GetInstance()->DownloadElemToPath(pathStr.c_str(),pathStr.c_str(),new tim::DownloadTIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDownloadFile, jstring file_id, jstring uuid, jstring url, jstring path, jobject callback) {
    json::Object download_param;
    download_param[kTIMMsgDownloadElemParamType] = TIMDownloadType::kTIMDownload_File;
    download_param[kTIMMsgDownloadElemParamId] = tim::jni::StringJni::Jstring2Cstring(env,file_id);
    download_param[kTIMMsgDownloadElemParamBusinessId] = tim::jni::StringJni::Jstring2Cstring(env,uuid);
    download_param[kTIMMsgDownloadElemParamUrl] = tim::jni::StringJni::Jstring2Cstring(env,url);
    std::string paramStr = json::Serialize(download_param);

    std::string pathStr = tim::jni::StringJni::Jstring2Cstring(env,path);

    tim::TIMEngine::GetInstance()->DownloadElemToPath(pathStr.c_str(),pathStr.c_str(),new tim::DownloadTIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeDownloadMergerMessage, jobject elem, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);
    std::unique_ptr<json::Object> elem_json = tim::jni::ElemProcessor::GetInstance().BuildElem(TIMElemType::kTIMElem_Merge, elem);
    json::Object message_json;
    message_json[kTIMMsgElemArray].ToArray().push_back(*elem_json);
    std::string messageStr = json::Serialize(message_json);

    tim::TIMEngine::GetInstance()->DownloadMergerMessage(messageStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto *_env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array message_array = json::Deserialize(json_params);
            jobject messageObjList = tim::jni::ArrayListJni::NewArrayList();
            for (const auto & i : message_array) {
                jobject messageObj = tim::jni::MessageJni::Convert2JObject(i);
                tim::jni::ArrayListJni::Add(messageObjList, messageObj);
                _env->DeleteLocalRef(messageObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, messageObjList);
            _env->DeleteLocalRef(messageObjList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, MativeConvertVoiceToText, jstring url, jstring language, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string urlStr = tim::jni::StringJni::Jstring2Cstring(env,url);
    std::string languageStr = tim::jni::StringJni::Jstring2Cstring(env,language);

    tim::TIMEngine::GetInstance()->ConvertVoiceToText(urlStr.c_str(),languageStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto *_env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            jstring content = tim::jni::StringJni::charToJstring(_env,json_params);
            tim::jni::IMCallbackJNI::Success(_callback, content);
            _env->DeleteLocalRef(content);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

// 图片消息
static JNINativeMethod gImageMethods[] = {
        {"nativeDownloadImage", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeDownloadImage},
};

// 语音消息
static JNINativeMethod gSoundMethods[] = {
        {"nativeDownloadSound", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeDownloadSound},
        {"nativeConvertVoiceToText", "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) MativeConvertVoiceToText},
};

// 视频消息
static JNINativeMethod gVideoMethods[] = {
        {"nativeDownloadVideo",    "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeDownloadVideo},
        {"nativeDownloadSnapshot", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeDownloadSnapshot},
};

// 文件消息
static JNINativeMethod gFileMethods[] = {
        {"nativeDownloadFile", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeDownloadFile},
};

// 合并消息
static JNINativeMethod gMergerMethods[] = {
        {"nativeDownloadMergerMessage", "(Lcom/tencent/imsdk/v2/V2TIMMergerElem;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeDownloadMergerMessage},
};

//注册 V2TIMImageElem native 方法
int RegisterNativeMethodsForV2TIMImageElem(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMImageElem$V2TIMImage", gImageMethods,
                                                  sizeof(gImageMethods) / sizeof(gImageMethods[0]));
}

//注册 V2TIMSoundElem native 方法
int RegisterNativeMethodsForV2TIMSoundElem(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMSoundElem", gSoundMethods,
                                                  sizeof(gSoundMethods) / sizeof(gSoundMethods[0]));
}

//注册 V2TIMVideoElem native 方法
int RegisterNativeMethodsForV2TIMVideoElem(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMVideoElem", gVideoMethods,
                                                  sizeof(gVideoMethods) / sizeof(gVideoMethods[0]));
}

//注册 V2TIMFileElem native 方法
int RegisterNativeMethodsForV2TIMFileElem(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMFileElem", gFileMethods,
                                                  sizeof(gFileMethods) / sizeof(gFileMethods[0]));
}

//注册 V2TIMMergerElem native 方法
int RegisterNativeMethodsForV2TIMMergerElem(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMMergerElem", gMergerMethods,
                                                 sizeof(gMergerMethods) / sizeof(gMergerMethods[0]));
}

#ifdef __cplusplus
}
#endif