//
// Created by EDY on 2023/10/19.
//

#include <jni_helper.h>
#include "call_experimental_api_jni.h"
#include "java_basic_jni.h"
#include "TIMExperimentalAPI.h"
#include "jni_util.h"

namespace tim {
    namespace jni {

        std::string CallExperimentalAPIJni::ConvertToCoreObject(JNIEnv *env, jstring const &api, const jobject &param) {
            //TODO::实验性api的回调处理待完善
            std::string apiStr = tim::jni::StringJni::Jstring2Cstring(env, api);

            json::Object json_param;
            
            if (apiStr == "setCustomServerInfo") {
                setCustomServerInfo(param,json_param);
            } else if(apiStr == "setQuicChannelInfo"){
                setQuicChannelInfo(param,json_param);
            } else if(apiStr == "setProxyInfo"){
                setProxyInfo(param,json_param);
            } else if(apiStr == "initLocalStorage"){
                initLocalStorage(param,json_param);
            } else if(apiStr == "setTestEnvironment"){
                setTestEnvironment(param,json_param);
            } else if(apiStr == "setIPv6Prior"){
                setIPv6Prior(param,json_param);
            } else if(apiStr == "setCosSaveRegion"){
                setCosSaveRegion(param,json_param);
            } else if(apiStr == "setUIPlatform"){
                setUIPlatform(param,json_param);
//            } else if(apiStr == kTIMInternalOperationSetBuildInfo){
//            暂时没有设置设备信息隐藏接口
            } else if(apiStr == "setDatabaseEncryptInfo"){
                setDatabaseEncryptInfo(param,json_param);
            } else if(apiStr == "isCommercialAbilityEnabled"){
                isCommercialAbilityEnabled(param,json_param);
            } else if(apiStr == "setPacketRetryInfo"){
                setPacketRetryInfo(param,json_param);
            } else if(apiStr == "setOfflinePushState"){
                setOfflinePushState(param,json_param);
            } else if(apiStr == "getOfflinePushState"){
                getOfflinePushState(param,json_param);
            } else if(apiStr == "getMessageRevoker"){
                getMessageRevoker(param,json_param);
            } else if(apiStr == "writeLog"){
                writeLog(param,json_param);
            } else if(apiStr == "sendTRTCCustomData"){
                sendTRTCCustomData(param,json_param);
            } else if(apiStr == "clearLocalHistoryMessage"){
                clearLocalHistoryMessage(param,json_param);
            } else if(apiStr == "reportTUIComponentUsage"){
                reportTUIComponentUsage(param,json_param);
            } else if(apiStr == "setApplicationID"){
                setApplicationID(param,json_param);
            }
            return json::Serialize(json_param);
        }

        void CallExperimentalAPIJni::setCustomServerInfo(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetCustomServerInfo;
            
            json::Object custom_server_info_obj;

            json::Object long_server_address;
            long_server_address[kTIMServerAddressIp] = "";
            long_server_address[kTIMServerAddressPort] = 0;
            long_server_address[kTIMServerAddressIsIPv6] = false;
            long_server_address[kTIMServerAddressIsQuic] = false;
            custom_server_info_obj[kTIMCustomServerInfoLongConnectionAddressArray] = long_server_address;// array [ServerAddress](), 只写(必填), 长连接服务器地址列表

            json::Object short_server_address;
            short_server_address[kTIMServerAddressIp] = "";
            short_server_address[kTIMServerAddressPort] = 0;
            short_server_address[kTIMServerAddressIsIPv6] = false;
            short_server_address[kTIMServerAddressIsQuic] = false;
            custom_server_info_obj[kTIMCustomServerInfoShortConnectionAddressArray] = short_server_address;// array [ServerAddress](), 只写(选填), 短连接服务器地址列表

            custom_server_info_obj[kTIMCustomServerInfoServerPublicKey] = "";// string, 只写(必填), 服务器公钥

            json_param[kTIMRequestSetCustomServerInfoParam] = custom_server_info_obj;// object [CustomServerInfo](), 只写(选填), 自定义服务器信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetCustomServerInfo 时需要设置
        }

        void CallExperimentalAPIJni::setQuicChannelInfo(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetQuicChannelInfo;

            json_param[kTIMRequestSetQuicChannelInfoParam] = (bool) param;// bool, 只写(选填), true 表示设置 Quic 通道信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetQuicChannelInfo 时需要设置

        }

        void CallExperimentalAPIJni::setProxyInfo(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationUpdateProxyInfo;
        }

        void CallExperimentalAPIJni::initLocalStorage(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationInitLocalStorage;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();
            json_param[kTIMRequestInitLocalStorageParam] = StringJni::Jstring2Cstring(env,(jstring) param);// string, 只写(选填), 初始化 Database 的用户 ID, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationInitLocalStorage 时需要设置
        }

        void CallExperimentalAPIJni::setTestEnvironment(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetEnv;

            json_param[kTIMRequestSetEnvParam] = (bool) param;// bool, 只写(选填), true 表示设置当前环境为测试环境，false表示设置当前环境是正式环境，默认是正式环境, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetEnv 时需要设置
        }

        void CallExperimentalAPIJni::setIPv6Prior(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetIPv6Prior;

            json_param[kTIMRequestSetIPv6PriorParam] = (bool) param;// bool, 只写(选填), 在 IPv6 双栈网络下，是否优先使用 IPv6 协议，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetIPv6Prior 时需要设置
        }

        void CallExperimentalAPIJni::setCosSaveRegion(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetCosSaveRegionForConversation;

            json::Object cos_save_region;
            cos_save_region[kTIMCosSaveRegionForConversationParamConversationID] = "";
            cos_save_region[kTIMCosSaveRegionForConversationParamCosSaveRegion] = "";
            json_param[kTIMRequestSetCosSaveRegionForConversationParam] = cos_save_region;// object [CosSaveRegionForConversationParam](), 只写(选填), 设置 cos 存储区域的参数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetCosSaveRegionForConversation 时需要设置
        }

        void CallExperimentalAPIJni::setUIPlatform(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetUIPlatform;

            json_param[kTIMRequestSetUIPlatformParam] = IntegerJni::IntValue(
                    param);// uint32, 只写(选填), 设置 UI 平台，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetUIPlatform 时需要设置
        }

        void CallExperimentalAPIJni::setBuildInfo(jobject const &param, json::Object &json_param) {
//            json_param[kTIMRequestInternalOperation] = apiStr;
        }

        void CallExperimentalAPIJni::setDatabaseEncryptInfo(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetDatabaseEncryptInfo;

            json::Object encrypt_info;
            encrypt_info[kTIMDatabaseEncryptInfoEncryptType] = 0;
            encrypt_info[kTIMDatabaseEncryptInfoEncryptKey] = "";
            json_param[kTIMRequestSetDatabaseEncryptInfoParam] = encrypt_info;// object [DatabaseEncryptInfo](), 只写(选填), 设置数据库加密信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetDatabaseEncryptInfo 时需要设置

        }

        void CallExperimentalAPIJni::isCommercialAbilityEnabled(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationIsCommercialAbilityEnabled;

            json_param[kTIMRequestIsCommercialAbilityEnabledParam] = (unsigned long long)LongJni::LongValue(param);// uint64, 只写(选填), 商业化能力项枚举的组合值, 当
            // kTIMRequestInternalOperation 为 kTIMInternalOperationIsCommercialAbilityEnabled 时需要设置
        }

        void CallExperimentalAPIJni::setPacketRetryInfo(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetMaxRetryCount;

            json_param[kTIMRequestSetMaxRetryCountParam] = IntegerJni::IntValue(
                    param);// uint32, 只写(选填), 设置登录、发消息请求的重试次数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetMaxRetryCount 时需要设置
        }

        void CallExperimentalAPIJni::setOfflinePushState(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetOfflinePushState;

            json_param[kTIMRequestSetOfflinePushStateParam] = IntegerJni::IntValue(
                    param);// uint32, 只写(选填), 设置推送状态：0 - 不设置 1 - 开启推送 2 - 关闭推送, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetOfflinePushState 时需要设置
        }

        void CallExperimentalAPIJni::getOfflinePushState(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationGetOfflinePushState;
        }

        void CallExperimentalAPIJni::getMessageRevoker(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationGetMessageRevoker;

            std::vector<std::string> messageIDVector = ArrayListJni::JStringListToCoreVector(param);
            json::Array msgID_array;
            for (const auto &item: messageIDVector) {
                msgID_array.push_back(item);
            }
            json_param[kTIMRequestMessageRevokerMessageIdArray] = msgID_array;// array string, 只写(选填), 撤回消息的 id, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationGetMessageRevoker 时需要设置
        }

        void CallExperimentalAPIJni::writeLog(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationWriteLog;

            json_param[kTIMRequestWriteLogLogLevelParam] = "";// int, 只写(选填), 打印日志的级别, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
            json_param[kTIMRequestWriteLogLogContentParam] = "";// string, 只写(选填), 打印日志的内容, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
            json_param[kTIMRequestWriteLogFileNameParam] = "";// string, 只写(选填), 打印日志的文件名, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
            json_param[kTIMRequestWriteLogFuncNameParam] = "";// string, 只写(选填), 打印日志的函数名, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
            json_param[kTIMRequestWriteLogLineNumberParam] = "";// int, 只写(选填), 打印日志的所在行数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置

        }

        void CallExperimentalAPIJni::sendTRTCCustomData(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSendTRTCCustomData;

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();
            json_param[kTIMRequestSendTRTCCustomDataParam] = StringJni::Jstring2Cstring(env,(jstring) param);// string, 只写(必填)，长连接透传发送的数据，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSendTRTCCustomData 时需要设置
        }

        void CallExperimentalAPIJni::clearLocalHistoryMessage(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationClearLocalHistoryMessage;

            json_param[kTIMRequestClearLocalHistoryMessageConversationIDParam] = "";// string, 只写(选填), 设置要删除消息的会话 id，当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
            json_param[kTIMRequestClearLocalHistoryMessageBeginTimestampParam] = 0;// uint64, 只写(选填), 设置要删除消息时间范围的开始时间,当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
            json_param[kTIMRequestClearLocalHistoryMessageEndTimestampParam] = 0;// uint64, 只写(选填), 设置要删除消息时间范围的结束时间，删除消息时间范围为 [begin_time, end_time]，若结束时间设置为 0，则删除开始时间之后的所有消息，若开始和结束时间都被设置为 0，则清空指定会话的所有消息，当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
        }

        void CallExperimentalAPIJni::reportTUIComponentUsage(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationReportTUIComponentUsage;

            json_param[kTIMRequestReportTUIComponentUsageUIComponentTypeParam] = param;// uint32, 只写(选填), Tuikit 上报类型
            json_param[kTIMRequestReportTUIComponentUsageUIStyleTypeParam] = param;// uint32, 只写(选填), Tuikit 风格，经典版、简约版
        }

        void CallExperimentalAPIJni::setApplicationID(jobject const &param, json::Object &json_param) {
            json_param[kTIMRequestInternalOperation] = kTIMInternalOperationSetApplicationID;
        }

        jobject CallExperimentalAPIJni::optJObject(const jobject &obj, jstring const &key) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(obj);
            jmethodID jmethodId = env->GetMethodID(clazz, "optJSONObject", "(Ljava/lang/String;)Lorg/json/JSONObject;");
            return env->CallObjectMethod(obj, jmethodId, key);
        }

        int CallExperimentalAPIJni::optJsonInt(const jobject &obj, jstring const &key) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(obj);
            jmethodID jmethodId = env->GetMethodID(clazz, "optInt", "(Ljava/lang/String;)I");
            return env->CallIntMethod(obj, jmethodId, key);
        }

        long CallExperimentalAPIJni::optJsonLong(const jobject &obj, jstring const &key) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(obj);
            jmethodID jmethodId = env->GetMethodID(clazz, "optLong", "(Ljava/lang/String;)J");
            return env->CallLongMethod(obj, jmethodId, key);
        }

        double CallExperimentalAPIJni::optJsonDouble(const jobject &obj, jstring const &key) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(obj);
            jmethodID jmethodId = env->GetMethodID(clazz, "optDouble", "(Ljava/lang/String;)D");
            return env->CallDoubleMethod(obj, jmethodId, key);
        }

        std::string CallExperimentalAPIJni::optJsonString(const jobject &obj, jstring const &key) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(obj);
            jmethodID jmethodId = env->GetMethodID(clazz, "optString", "(Ljava/lang/String;)Ljava/lang/String;");
            auto value = (jstring) env->CallObjectMethod(obj, jmethodId, key);
            return StringJni::Jstring2Cstring(env, value);
        }

        jobject CallExperimentalAPIJni::optJson2JArr(jobject const &arr, jstring const &key) {
            return nullptr;
        }

        int CallExperimentalAPIJni::jArr2Length(jobject const &arr) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(arr);
            jmethodID jmethodId = env->GetMethodID(clazz,"length", "()I");
            return env->CallIntMethod(arr,jmethodId);
        }

        jobject CallExperimentalAPIJni::optJArr2JsonObj(jobject const &arr, const int index) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jclass clazz = env->GetObjectClass(arr);
            jmethodID jmethodId = env->GetMethodID(clazz,"optJSONObject", "(I)Lorg/json/JSONObject;");
            return env->CallObjectMethod(arr,jmethodId,index);
        }

    } // namespace jni
} // namespace tim
