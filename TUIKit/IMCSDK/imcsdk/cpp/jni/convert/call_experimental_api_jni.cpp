//
// Created by EDY on 2023/10/19.
//

#include "call_experimental_api_jni.h"
#include "java_basic_jni.h"
#include "TIMExperimentalAPI.h"
#include "jni_util.h"

namespace tim {
    namespace jni {

        std::string CallExperimentalAPIJni::ConvertToCoreObject(JNIEnv *env, jstring const &api, jobject const &param) {
            //TODO::实验性api的回调处理待完善
            std::string apiStr = tim::jni::StringJni::Jstring2Cstring(env, api);

            json::Object json_param;
            json_param[kTIMRequestInternalOperation] = apiStr;

            if (apiStr == kTIMInternalOperationSSOData) {
                json::Object sso_data_param;
                sso_data_param[kTIMSSODataParamCmd] = "";
                sso_data_param[kTIMSSODataParamBody] = "";
                sso_data_param[kTIMSSODataParamTimeout] = 0;
                json_param[kTIMRequestSSODataParam] = sso_data_param;// object [SSODataParam](), 只写(选填), sso发包请求, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSSOData 时需要设置
            } else if (apiStr == kTIMInternalOperationUserId2TinyId) {
                std::vector<std::string> userIDVector = ArrayListJni::JStringListToCoreVector(param);
                json::Array userID_array;
                for (const auto &item : userIDVector){
                    userID_array.push_back(item);
                }
                json_param[kTIMRequestUserId2TinyIdParam] = userID_array;// array string, 只写(选填), 请求需要转换成tinyid的userid列表, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationUserId2TinyId 时需要设置
            } else if (apiStr == kTIMInternalOperationTinyId2UserId) {
                std::vector<std::string> tinyIDVector = ArrayListJni::JStringListToCoreVector(param);
                json::Array tinyID_array;
                for (const auto &item : tinyIDVector){
                    tinyID_array.push_back(std::stoull(item, nullptr));
                }
                json_param[kTIMRequestTinyId2UserIdParam] = tinyID_array;// array uint64, 只写(选填), 请求需要转换成userid的tinyid列表, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationTinyId2UserId 时需要设置
            } else if (apiStr == kTIMInternalOperationSetEnv) {
                json_param[kTIMRequestSetEnvParam] = (bool)param;// bool, 只写(选填), true 表示设置当前环境为测试环境，false表示设置当前环境是正式环境，默认是正式环境, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetEnv 时需要设置
            } else if (apiStr == kTIMInternalOperationSetIPv6Prior) {
                json_param[kTIMRequestSetIPv6PriorParam] = (bool)param;// bool, 只写(选填), 在 IPv6 双栈网络下，是否优先使用 IPv6 协议，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetIPv6Prior 时需要设置
            } else if (apiStr == kTIMInternalOperationSetMaxRetryCount) {
                json_param[kTIMRequestSetMaxRetryCountParam] = IntegerJni::IntValue(param);// uint32, 只写(选填), 设置登录、发消息请求的重试次数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetMaxRetryCount 时需要设置
            } else if (apiStr == kTIMInternalOperationSetPacketRequestTimeout) {
                json_param[kTIMRequestSetPacketRequestTimeoutParam] = (long long )LongJni::LongValue(param);// int64, 只写(选填), 设置登录、发消息请求的超时时间, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetPacketRequestTimeout 时需要设置
            } else if (apiStr == kTIMInternalOperationSetCustomServerInfo) {
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
            } else if (apiStr == kTIMInternalOperationSetQuicChannelInfo) {
                json_param[kTIMRequestSetQuicChannelInfoParam] = (bool)param;// bool, 只写(选填), true 表示设置 Quic 通道信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetQuicChannelInfo 时需要设置
            } else if (apiStr == kTIMInternalOperationSetSM4GCMCallback) {
                json::Object SM4GCM_Callback;
                SM4GCM_Callback[kTIMSM4GCMCallbackParamEncrypt] = "";
                SM4GCM_Callback[kTIMSM4GCMCallbackParamDecrypt] = "";
                json_param[kTIMRequestSetSM4GCMCallbackParam] = SM4GCM_Callback;// object [SM4GCMCallbackParam](), 只写(选填), 国密 SM4 GCM 回调函数地址的参数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetSM4GCMCallback 时需要设置
            } else if (apiStr == kTIMInternalOperationInitLocalStorage) {
                json_param[kTIMRequestInitLocalStorageParam] = StringJni::Jstring2Cstring(env,(jstring)param);// string, 只写(选填), 初始化 Database 的用户 ID, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationInitLocalStorage 时需要设置
            } else if (apiStr == kTIMInternalOperationSetCosSaveRegionForConversation) {
                json::Object cos_save_region;
                cos_save_region[kTIMCosSaveRegionForConversationParamConversationID] = "";
                cos_save_region[kTIMCosSaveRegionForConversationParamCosSaveRegion] = "";
                json_param[kTIMRequestSetCosSaveRegionForConversationParam] = cos_save_region;// object [CosSaveRegionForConversationParam](), 只写(选填), 设置 cos 存储区域的参数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetCosSaveRegionForConversation 时需要设置
            } else if (apiStr == kTIMInternalOperationSetUIPlatform) {
                json_param[kTIMRequestSetUIPlatformParam] = IntegerJni::IntValue(param);// uint32, 只写(选填), 设置 UI 平台，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetUIPlatform 时需要设置
            } else if (apiStr == kTIMInternalOperationSetDatabaseEncryptInfo) {
                json::Object encrypt_info;
                encrypt_info[kTIMDatabaseEncryptInfoEncryptType] = 0;
                encrypt_info[kTIMDatabaseEncryptInfoEncryptKey] = "";
                json_param[kTIMRequestSetDatabaseEncryptInfoParam] = encrypt_info;// object [DatabaseEncryptInfo](), 只写(选填), 设置数据库加密信息, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetDatabaseEncryptInfo 时需要设置
            } else if (apiStr == kTIMInternalOperationIsCommercialAbilityEnabled) {
                json_param[kTIMRequestIsCommercialAbilityEnabledParam] = IntegerJni::IntValue(param);// uint64, 只写(选填), 商业化能力项枚举的组合值, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationIsCommercialAbilityEnabled 时需要设置
            } else if (apiStr == kTIMInternalOperationSetOfflinePushState) {
                json_param[kTIMRequestSetOfflinePushStateParam] = IntegerJni::IntValue(param);// uint32, 只写(选填), 设置推送状态：0 - 不设置 1 - 开启推送 2 - 关闭推送, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationSetOfflinePushState 时需要设置
            } else if (apiStr == kTIMInternalOperationGetMessageRevoker) {
                std::vector<std::string> messageIDVector = ArrayListJni::JStringListToCoreVector(param);
                json::Array msgID_array;
                for (const auto &item : messageIDVector){
                    msgID_array.push_back(item);
                }
                json_param[kTIMRequestMessageRevokerMessageIdArray] = msgID_array;// array string, 只写(选填), 撤回消息的 id, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationGetMessageRevoker 时需要设置
            } else if (apiStr == kTIMInternalOperationWriteLog) {
                json_param[kTIMRequestWriteLogLogLevelParam] = "";// int, 只写(选填), 打印日志的级别, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
                json_param[kTIMRequestWriteLogLogContentParam] = "";// string, 只写(选填), 打印日志的内容, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
                json_param[kTIMRequestWriteLogFileNameParam] = "";// string, 只写(选填), 打印日志的文件名, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
                json_param[kTIMRequestWriteLogFuncNameParam] = "";// string, 只写(选填), 打印日志的函数名, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
                json_param[kTIMRequestWriteLogLineNumberParam] = "";// int, 只写(选填), 打印日志的所在行数, 当 kTIMRequestInternalOperation 为 kTIMInternalOperationWriteLog 时需要设置
            } else if (apiStr == kTIMInternalOperationClearLocalHistoryMessage) {
                json_param[kTIMRequestClearLocalHistoryMessageConversationIDParam] = "";// string, 只写(选填), 设置要删除消息的会话 id，当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
                json_param[kTIMRequestClearLocalHistoryMessageBeginTimestampParam] = 0;// uint64, 只写(选填), 设置要删除消息时间范围的开始时间,当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
                json_param[kTIMRequestClearLocalHistoryMessageEndTimestampParam] = 0;// uint64, 只写(选填), 设置要删除消息时间范围的结束时间，删除消息时间范围为 [begin_time, end_time]，若结束时间设置为 0，则删除开始时间之后的所有消息，若开始和结束时间都被设置为 0，则清空指定会话的所有消息，当 kTIMRequestInternalOperation 为 kTIMInternalOperationClearLocalHistoryMessage 时需要设置
            } else if (apiStr == kTIMInternalOperationReportTUIComponentUsage) {
                json_param[kTIMRequestReportTUIComponentUsageUIComponentTypeParam] = param;// uint32, 只写(选填), Tuikit 上报类型
                json_param[kTIMRequestReportTUIComponentUsageUIStyleTypeParam] = param;// uint32, 只写(选填), Tuikit 风格，经典版、简约版
            } else if (apiStr == kTIMInternalOperationSendTRTCCustomData) {
                json_param[kTIMRequestSendTRTCCustomDataParam] = StringJni::Jstring2Cstring(env,(jstring)param);// string, 只写(必填)，长连接透传发送的数据，当 kTIMRequestInternalOperation 为 kTIMInternalOperationSendTRTCCustomData 时需要设置
            }
            return json::Serialize(json_param);
        }
    } // namespace jni
} // namespace tim
