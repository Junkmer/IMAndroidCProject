//
// Created by EDY on 2023/10/19.
//

#ifndef IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H
#define IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H

#include <jni.h>
#include <string>
#include "json.h"

namespace tim {
    namespace jni {

        class CallExperimentalAPIJni {
        public:
            static std::string ConvertToCoreObject(JNIEnv *env, const jstring &api, const jobject &param);

        private:
            static void setCustomServerInfo(const jobject &param, json::Object &json_param);

            static void setQuicChannelInfo(const jobject &param, json::Object &json_param);

            static void setProxyInfo(const jobject &param, json::Object &json_param);

            static void initLocalStorage(const jobject &param, json::Object &json_param);

            static void setTestEnvironment(const jobject &param, json::Object &json_param);

            static void setIPv6Prior(const jobject &param, json::Object &json_param);

            static void setCosSaveRegion(const jobject &param, json::Object &json_param);

            static void setUIPlatform(const jobject &param, json::Object &json_param);

            static void setBuildInfo(const jobject &param, json::Object &json_param);

            static void setDatabaseEncryptInfo(const jobject &param, json::Object &json_param);

            static void isCommercialAbilityEnabled(const jobject &param, json::Object &json_param);

            static void setPacketRetryInfo(const jobject &param, json::Object &json_param);

            static void setOfflinePushState(const jobject &param, json::Object &json_param);

            static void getOfflinePushState(json::Object &json_param);

            static void getMessageRevoker(const jobject &param, json::Object &json_param);

            static void writeLog(const jobject &param, json::Object &json_param);

            static void sendTRTCCustomData(const jobject &param, json::Object &json_param);

            static void clearLocalHistoryMessage(const jobject &param, json::Object &json_param);

            static void reportTUIComponentUsage(const jobject &param, json::Object &json_param);

            static void setApplicationID(const jobject &param, json::Object &json_param);

        };

    } // namespace tim
} // namespace jni


#endif //IMCSDKDEMO_CALL_EXPERIMENTAL_API_JNI_H
