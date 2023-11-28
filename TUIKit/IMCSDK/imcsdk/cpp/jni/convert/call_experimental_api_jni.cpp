//
// Created by EDY on 2023/10/19.
//

#include <jni_helper.h>
#include "call_experimental_api_jni.h"
#include "java_basic_jni.h"
#include "TIMExperimentalAPI.h"
#include "jni_util.h"
#include "jni_json_objarr_util.h"
#include "TIMManager.h"

namespace tim {
    namespace jni {

        std::string CallExperimentalAPIJni::ConvertToCoreObject(JNIEnv *env, jstring const &api, const jobject &param) {
            //TODO::实验性api的回调处理待完善
            std::string apiStr = tim::jni::StringJni::Jstring2Cstring(env, api);

            json::Object json_param;

            if (apiStr == "setCustomServerInfo") {
                setCustomServerInfo(param, json_param);
            } else if (apiStr == "setQuicChannelInfo") {
                setQuicChannelInfo(param, json_param);
            } else if (apiStr == "setProxyInfo") {
                setProxyInfo(param, json_param);
            } else if (apiStr == "initLocalStorage") {
                initLocalStorage(param, json_param);
            } else if (apiStr == "setTestEnvironment") {
                setTestEnvironment(param, json_param);
            } else if (apiStr == "setIPv6Prior") {
                setIPv6Prior(param, json_param);
            } else if (apiStr == "setCosSaveRegion") {
                setCosSaveRegion(param, json_param);
            } else if (apiStr == "setUIPlatform") {
                setUIPlatform(param, json_param);
            } else if (apiStr == "setDatabaseEncryptInfo") {
                setDatabaseEncryptInfo(param, json_param);
            } else if (apiStr == "isCommercialAbilityEnabled") {
                isCommercialAbilityEnabled(param, json_param);
            } else if (apiStr == "setPacketRetryInfo") {
                setPacketRetryInfo(param, json_param);
            } else if (apiStr == "setOfflinePushState") {
                setOfflinePushState(param, json_param);
            } else if (apiStr == "getOfflinePushState") {
                getOfflinePushState(json_param);
            } else if (apiStr == "getMessageRevoker") {
                getMessageRevoker(param, json_param);
            } else if (apiStr == "writeLog") {
                writeLog(param, json_param);
            } else if (apiStr == "sendTRTCCustomData") {
                sendTRTCCustomData(param, json_param);
            } else if (apiStr == "clearLocalHistoryMessage") {
                clearLocalHistoryMessage(param, json_param);
            } else if (apiStr == "reportTUIComponentUsage") {
                reportTUIComponentUsage(param, json_param);
            } else if (apiStr == "setApplicationID") {
                setApplicationID(param, json_param);
            }
            return json::Serialize(json_param);
        }

        void CallExperimentalAPIJni::setCustomServerInfo(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetCustomServerInfo;

            json::Object custom_server_info_obj;

            jobject j_arr_long = JSONObjectJNI::optJson2JArr(env, param, "longconnectionAddressList");
            if (j_arr_long) {
                json::Array long_server_address_array;
                int lengthLong = JSONArrayJNI::jArr2Length(env, param);
                for (int i = 0; i < lengthLong; ++i) {
                    jobject j_obj_long = JSONArrayJNI::optJArr2JsonObj(env, j_arr_long, i);
                    if (j_obj_long) {
                        json::Object long_server_address;
                        jstring j_ip = JSONObjectJNI::optJsonString(env, j_obj_long, "ip");
                        int j_port = JSONObjectJNI::optJsonInt(env, j_obj_long, "port");
                        bool j_isIpv6 = JSONObjectJNI::optJsonBoolean(env, j_obj_long, "isIPv6");

                        long_server_address[kTIMServerAddressIp] = StringJni::Jstring2Cstring(env, j_ip);
                        long_server_address[kTIMServerAddressPort] = j_port;
                        long_server_address[kTIMServerAddressIsIPv6] = j_isIpv6;

                        env->DeleteLocalRef(j_ip);
                        env->DeleteLocalRef(j_obj_long);
                    }
                }
                env->DeleteLocalRef(j_arr_long);
                custom_server_info_obj[kTIMCustomServerInfoLongConnectionAddressArray] = long_server_address_array;// array [ServerAddress](), 只写(必填), 长连接服务器地址列表
            }

            jobject j_arr_short = JSONObjectJNI::optJson2JArr(env, param, "shortconnectionAddressList");
            if (j_arr_short) {
                json::Array short_server_address_array;
                int lengthShort = JSONArrayJNI::jArr2Length(env, param);
                for (int i = 0; i < lengthShort; ++i) {
                    jobject j_obj_short = JSONArrayJNI::optJArr2JsonObj(env, j_arr_short, i);
                    if (j_obj_short) {
                        json::Object short_server_address;
                        jstring j_ip = JSONObjectJNI::optJsonString(env, j_obj_short, "ip");
                        int j_port = JSONObjectJNI::optJsonInt(env, j_obj_short, "port");
                        bool j_isIpv6 = JSONObjectJNI::optJsonBoolean(env, j_obj_short, "isIPv6");

                        short_server_address[kTIMServerAddressIp] = StringJni::Jstring2Cstring(env, j_ip);
                        short_server_address[kTIMServerAddressPort] = j_port;
                        short_server_address[kTIMServerAddressIsIPv6] = j_isIpv6;
                        short_server_address_array.push_back(short_server_address);

                        env->DeleteLocalRef(j_ip);
                        env->DeleteLocalRef(j_obj_short);
                    }
                }
                env->DeleteLocalRef(j_arr_short);
                custom_server_info_obj[kTIMCustomServerInfoShortConnectionAddressArray] = short_server_address_array;// array [ServerAddress](), 只写(选填), 短连接服务器地址列表
            }

            custom_server_info_obj[kTIMCustomServerInfoServerPublicKey] = JSONObjectJNI::optJsonString(env, param, "serverPublicKey");// string, 只写(必填), 服务器公钥

            json_param[kTIMRequestSetCustomServerInfoParam] = custom_server_info_obj;// object [CustomServerInfo](), 只写(选填), 自定义服务器信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetCustomServerInfo 时需要设置
        }

        void CallExperimentalAPIJni::setQuicChannelInfo(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetQuicChannelInfo;
            bool j_isUseQuicChannel = JSONObjectJNI::optJsonBoolean(env, param, "forceUseQuicChannel");

            // bool, 只写(选填), true 表示设置 Quic 通道信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetQuicChannelInfo 时需要设置
            json_param[kTIMRequestSetQuicChannelInfoParam] = j_isUseQuicChannel;
        }

        void CallExperimentalAPIJni::setProxyInfo(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationUpdateProxyInfo;

            int j_type = JSONObjectJNI::optJsonInt(env, param, "proxyType");// 代理类型 proxyType: 0 无代理，1 Http 代理，2 Socks5 代理
            jstring j_host = JSONObjectJNI::optJsonString(env, param, "proxyHost");
            int j_port = JSONObjectJNI::optJsonInt(env, param, "proxyPort");
            jstring j_username = JSONObjectJNI::optJsonString(env, param, "proxyUsername");
            jstring j_password = JSONObjectJNI::optJsonString(env, param, "proxyPassword");
            if (j_type == 1) {
                json_param[kTIMHttpProxyInfoIp] = j_host;
                json_param[kTIMHttpProxyInfoPort] = j_port;
                json_param[kTIMHttpProxyInfoUserName] = j_username;
                json_param[kTIMHttpProxyInfoPassword] = j_password;
            } else if (j_type == 2) {
                json_param[kTIMSocks5ProxyInfoIp] = j_host;
                json_param[kTIMSocks5ProxyInfoPort] = j_port;
                json_param[kTIMSocks5ProxyInfoUserName] = j_username;
                json_param[kTIMSocks5ProxyInfoPassword] = j_password;
            }

            env->DeleteLocalRef(j_host);
            env->DeleteLocalRef(j_username);
            env->DeleteLocalRef(j_password);
        }

        void CallExperimentalAPIJni::initLocalStorage(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationInitLocalStorage;

            // string, 只写(选填), 初始化 Database 的用户 ID, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationInitLocalStorage 时需要设置
            json_param[kTIMRequestInitLocalStorageParam] = StringJni::Jstring2Cstring(env, (jstring) param);
        }

        void CallExperimentalAPIJni::setTestEnvironment(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetEnv;

            // bool, 只写(选填), true 表示设置当前环境为测试环境，false表示设置当前环境是正式环境，默认是正式环境, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetEnv 时需要设置
            json_param[kTIMRequestSetEnvParam] = (bool) param;
        }

        void CallExperimentalAPIJni::setIPv6Prior(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetIPv6Prior;

            // bool, 只写(选填), 在 IPv6 双栈网络下，是否优先使用 IPv6 协议，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetIPv6Prior 时需要设置
            json_param[kTIMRequestSetIPv6PriorParam] = (bool) param;
        }

        void CallExperimentalAPIJni::setCosSaveRegion(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetCosSaveRegionForConversation;

            jstring j_convId = JSONObjectJNI::optJsonString(env, param, "conversationID");
            jstring j_cosRegion = JSONObjectJNI::optJsonString(env, param, "cosSaveRegion");
            json::Object cos_save_region;
            cos_save_region[kTIMCosSaveRegionForConversationParamConversationID] = StringJni::Jstring2Cstring(env, j_convId);
            cos_save_region[kTIMCosSaveRegionForConversationParamCosSaveRegion] = StringJni::Jstring2Cstring(env, j_cosRegion);

            // object [CosSaveRegionForConversationParam](), 只写(选填), 设置 cos 存储区域的参数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetCosSaveRegionForConversation 时需要设置
            json_param[kTIMRequestSetCosSaveRegionForConversationParam] = cos_save_region;

            env->DeleteLocalRef(j_convId);
            env->DeleteLocalRef(j_cosRegion);
        }

        void CallExperimentalAPIJni::setUIPlatform(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetUIPlatform;

            /**
             *  UI 组件库类型:
             *  1：flutter 2:flutter_uikit 3:uni-app 4:uni-app_uikit 5:unity 6:unity_uikit
             *  7:electron 8:electron_uikit 9:ue 10:ue_uikit 11:mp 12:mp_uikit 13:web 14:web_uikit 15:tuikit
             */
            // uint32, 只写(选填), 设置 UI 平台，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetUIPlatform 时需要设置
            json_param[kTIMRequestSetUIPlatformParam] = IntegerJni::IntValue(param);
        }

        void CallExperimentalAPIJni::setDatabaseEncryptInfo(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetDatabaseEncryptInfo;

            int j_type = JSONObjectJNI::optJsonInt(env, param, "encryptType");
            jstring j_key = JSONObjectJNI::optJsonString(env, param, "encryptKey");

            json::Object encrypt_info;
            encrypt_info[kTIMDatabaseEncryptInfoEncryptType] = j_type;
            encrypt_info[kTIMDatabaseEncryptInfoEncryptKey] = StringJni::Jstring2Cstring(env, j_key);
            // object [DatabaseEncryptInfo](), 只写(选填), 设置数据库加密信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetDatabaseEncryptInfo 时需要设置
            json_param[kTIMRequestSetDatabaseEncryptInfoParam] = encrypt_info;

            env->DeleteLocalRef(j_key);
        }

        void CallExperimentalAPIJni::isCommercialAbilityEnabled(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationIsCommercialAbilityEnabled;

            // uint64, 只写(选填), 商业化能力项枚举的组合值, 当kTIMRequestInternalOperation 为 kTIMInternalOperationIsCommercialAbilityEnabled 时需要设置
            json_param[kTIMRequestIsCommercialAbilityEnabledParam] = (unsigned long long) LongJni::LongValue(param);
        }

        void CallExperimentalAPIJni::setPacketRetryInfo(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            int j_maxCount = JSONObjectJNI::optJsonInt(env, param, "maxRetryCount");
            int j_timeout = JSONObjectJNI::optJsonInt(env, param, "packetRequestTimeout");

            // uint32, 只写(选填), 设置登录、发消息请求的重试次数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetMaxRetryCount 时需要设置
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetMaxRetryCount;
            json_param[kTIMRequestSetMaxRetryCountParam] = j_maxCount;

            if (j_timeout > 0) {
                json::Object json_timeout;
                // int64, 只写(选填), 设置登录、发消息请求的超时时间, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetPacketRequestTimeout 时需要设置
                json_timeout[kTIMRequestInternalOperation] = kTIMInternalOperationSetPacketRequestTimeout;
                json_timeout[kTIMRequestSetPacketRequestTimeoutParam] = j_timeout;
                std::string paramStr = json::Serialize(json_timeout);
                callExperimentalAPI(paramStr.c_str(), nullptr, nullptr);
            }
        }

        void CallExperimentalAPIJni::setOfflinePushState(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetOfflinePushState;

            // uint32, 只写(选填), 设置推送状态：0 - 不设置 1 - 开启推送 2 - 关闭推送, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetOfflinePushState 时需要设置
            json_param[kTIMRequestSetOfflinePushStateParam] = IntegerJni::IntValue(param);
        }

        void CallExperimentalAPIJni::getOfflinePushState(json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationGetOfflinePushState;
        }

        void CallExperimentalAPIJni::getMessageRevoker(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationGetMessageRevoker;

            std::vector<std::string> messageIDVector = ArrayListJni::JStringListToCoreVector(param);
            json::Array msgID_array;
            for (const auto &item: messageIDVector) {
                msgID_array.push_back(item);
            }
            // array string, 只写(选填), 撤回消息的 id, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationGetMessageRevoker 时需要设置
            json_param[kTIMRequestMessageRevokerMessageIdArray] = msgID_array;
        }

        void CallExperimentalAPIJni::writeLog(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationWriteLog;

            int j_level = JSONObjectJNI::optJsonInt(env, param, "");
            jstring j_fileName = JSONObjectJNI::optJsonString(env, param, "");
            jstring j_funcName = JSONObjectJNI::optJsonString(env, param, "");
            int j_lineNumber = JSONObjectJNI::optJsonInt(env, param, "");
            jstring j_logContent = JSONObjectJNI::optJsonString(env, param, "");

            // 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
            json_param[kTIMRequestWriteLogLogLevelParam] = j_level;
            json_param[kTIMRequestWriteLogLogContentParam] = StringJni::Jstring2Cstring(env, j_fileName);
            json_param[kTIMRequestWriteLogFileNameParam] = StringJni::Jstring2Cstring(env, j_funcName);
            json_param[kTIMRequestWriteLogFuncNameParam] = j_lineNumber;
            json_param[kTIMRequestWriteLogLineNumberParam] = StringJni::Jstring2Cstring(env, j_logContent);

            env->DeleteLocalRef(j_fileName);
            env->DeleteLocalRef(j_funcName);
            env->DeleteLocalRef(j_logContent);
        }

        void CallExperimentalAPIJni::sendTRTCCustomData(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSendTRTCCustomData;

            // string, 只写(必填)，长连接透传发送的数据，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSendTRTCCustomData 时需要设置
            json_param[kTIMRequestSendTRTCCustomDataParam] = StringJni::JbyteArray2Cstring(env, (jbyteArray) param);
        }

        void CallExperimentalAPIJni::clearLocalHistoryMessage(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationClearLocalHistoryMessage;

            jstring j_convId = JSONObjectJNI::optJsonString(env, param, "conversationID");
            long j_beginTime = JSONObjectJNI::optJsonLong(env, param, "beginTimestamp");
            long j_endTime = JSONObjectJNI::optJsonLong(env, param, "endTimestamp");

            // 当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
            json_param[kTIMRequestClearLocalHistoryMessageConversationIDParam] = StringJni::Jstring2Cstring(env, j_convId);
            json_param[kTIMRequestClearLocalHistoryMessageBeginTimestampParam] = (long long) j_beginTime;
            json_param[kTIMRequestClearLocalHistoryMessageEndTimestampParam] = (long long) j_endTime;

            env->DeleteLocalRef(j_convId);
        }

        void CallExperimentalAPIJni::reportTUIComponentUsage(jobject const &param, json::Object &json_param) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationReportTUIComponentUsage;

            int j_componentType = JSONObjectJNI::optJsonInt(env,param,"UIComponentType");
            int j_styleType = JSONObjectJNI::optJsonInt(env,param,"UIStyleType");

            json_param[kTIMRequestReportTUIComponentUsageUIComponentTypeParam] = j_componentType;// uint32, 只写(选填), Tuikit 上报类型
            json_param[kTIMRequestReportTUIComponentUsageUIStyleTypeParam] = j_styleType;// uint32, 只写(选填), Tuikit 风格，经典版、简约版
        }

        void CallExperimentalAPIJni::setApplicationID(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetApplicationID;

            json_param[kTIMRequestSetApplicationIDParam] = IntegerJni::IntValue(param);
        }


    } // namespace jni
} // namespace tim
