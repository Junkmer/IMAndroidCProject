//
// Created by EDY on 2022/8/22.
//

#include <android/log.h>
#include <jni.h>

//日志打印
#include "../util/LogUtil.h"

// Java 基础数据类型
#include "java_basic_jni.h"

// C++ 回调 Java 通用 Callback
#include "basic/jni_helper.h"

// 初始化 SDKConfig
#include "convert/sdk_config_jni.h"

//监听相关
#include "advanced_msg_listener_jni.h"
//#include "base_listener_jni.h"
#include "conversation_listener_jni.h"
#include "friend_listener_jni.h"
#include "group_listener_jni.h"
#include "im_callbak_jni.h"
#include "signaling_listener_jni.h"
#include "simple_listener_jni.h"

//会话相关
#include "conversation_jni.h"
#include "conversation_list_filter_jni.h"
#include "conversation_operation_result_jni.h"
#include "conversation_result_jni.h"

//资料关系链相关
#include "friend_add_application_jni.h"
#include "friend_application_jni.h"
#include "friend_application_result_jni.h"
#include "friend_check_result_jni.h"
#include "friend_group_jni.h"
#include "friend_info_jni.h"
#include "friend_info_result_jni.h"
#include "friend_operation_result_jni.h"
#include "friend_search_param_jni.h"
#include "user_full_info_jni.h"
#include "user_info_jni.h"
#include "user_status_jni.h"

//群组相关
#include "create_group_member_info_jni.h"
#include "group_application_jni.h"
#include "group_application_result_jni.h"
#include "group_at_info_jni.h"
#include "group_change_info_jni.h"
#include "group_info_jni.h"
#include "group_info_result_jni.h"
#include "group_member_change_info_jni.h"
#include "group_member_full_info_jni.h"
#include "group_member_info_jni.h"
#include "group_member_info_result_jni.h"
#include "group_member_operation_result_jni.h"
#include "group_member_search_param_jni.h"
#include "group_message_read_member_list_jni.h"
#include "group_search_param.h"
#include "group_topic_info_jni.h"
#include "topic_info_result_jni.h"
#include "topic_operation_result_jni.h"

//推送相关
#include "offline_push_config_jni.h"
#include "offline_push_info_jni.h"

//消息相关
#include "elem_processor_jni.h"
#include "image_jni.h"
#include "message_extension_jni.h"
#include "message_extension_result_jni.h"
#include "message_jni.h"
#include "message_list_get_option_jni.h"
#include "message_reaction_jni.h"
#include "message_reaction_result_jni.h"
#include "message_reaction_user_result_jni.h"
#include "message_receipt_jni.h"
#include "message_search_param_jni.h"
#include "message_search_result_item_jni.h"
#include "message_search_result_jni.h"
#include "receive_message_opt_info_jni.h"

//信令相关
#include "signaling_info_jni.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHECK_INITIDS_RESULT(success, class_name) \
    if(false == (success)) {                      \
        LOGE("%s InitIDs failed",class_name);     \
    }

extern int RegisterNativeMethodsForV2TIMManager(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMConversationManager(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMFriendshipManager(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMGroupManager(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMMessageManager(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMOfflinePushManager(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMSignalingManager(JNIEnv *env);
//注册下载媒体消息文件
extern int RegisterNativeMethodsForV2TIMImageElem(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMSoundElem(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMVideoElem(JNIEnv *env);
extern int RegisterNativeMethodsForV2TIMFileElem(JNIEnv *env);
//注册下载合并消息
extern int RegisterNativeMethodsForV2TIMMergerElem(JNIEnv *env);

static void InitJNI(JNIEnv *env) {
//********************************************************************************************//
//*********************************** Java 基础数据类型 ***************************************//
//********************************************************************************************//
    // Java 基础数据类型
    bool success = tim::jni::IntegerJni::Init(env);
    CHECK_INITIDS_RESULT(success, "IntegerJni")

    success = tim::jni::LongJni::Init(env);
    CHECK_INITIDS_RESULT(success, "LongJni")

    success = tim::jni::StringJni::Init(env);
    CHECK_INITIDS_RESULT(success, "StringJni")

    success = tim::jni::ArrayListJni::Init(env);
    CHECK_INITIDS_RESULT(success, "ArrayListJni")

    success = tim::jni::HashMapJni::Init(env);
    CHECK_INITIDS_RESULT(success, "HashMapJni")

//********************************************************************************************//
//******************************** C 回调 Java 通用 IMCallback ********************************//
//********************************************************************************************//
    success = tim::jni::IMCallbackJNI::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "IMCallbackJNI")

    // 初始化 SDKConfig
    success = tim::jni::SDKConfigJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "SDKConfigJni")

//********************************************************************************************//
//***************************************** 监听相关 ******************************************//
//********************************************************************************************//
    success = tim::jni::AdvancedMsgListenerJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "AdvancedMsgListenerJni")

    success = tim::jni::ConversationListenerJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ConversationListenerJni")

    success = tim::jni::FriendListenerJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendListenerJni")

    success = tim::jni::GroupListenerJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupListenerJni")

    success = tim::jni::IMCallbackJNI::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "IMCallbackJNI")

    success = tim::jni::SignalingListenerJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "SignalingListenerJni")

    success = tim::jni::SimpleListenerJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "SimpleListenerJni")

//********************************************************************************************//
//***************************************** 会话相关 ******************************************//
//********************************************************************************************//
    success = tim::jni::ConversationJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ConversationJni")

    success = tim::jni::ConversationListFilterJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ConversationListFilterJni")

    success = tim::jni::ConversationOperationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ConversationOperationResultJni")

    success = tim::jni::ConversationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ConversationResultJni")

//********************************************************************************************//
//************************************** 资料关系链相关 ****************************************//
//********************************************************************************************//
    success = tim::jni::FriendAddApplicationJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendAddApplicationJni")

    success = tim::jni::FriendApplicationJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendApplicationJni")

    success = tim::jni::FriendApplicationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendApplicationResultJni")

    success = tim::jni::FriendCheckResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendCheckResultJni")

    success = tim::jni::FriendGroupJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendGroupJni")

    success = tim::jni::FriendInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendInfoJni")

    success = tim::jni::FriendInfoResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendInfoResultJni")

    success = tim::jni::FriendOperationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendOperationResultJni")

    success = tim::jni::FriendSearchParamJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "FriendSearchParamJni")

    success = tim::jni::UserFullInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "UserFullInfoJni")

    success = tim::jni::UserInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "UserInfoJni")

    success = tim::jni::UserStatusJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "UserStatusJni")

//********************************************************************************************//
//**************************************** 群组相关 *******************************************//
//********************************************************************************************//
    success = tim::jni::CreateGroupMemberInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "CreateGroupMemberInfoJni")

    success = tim::jni::GroupApplicationJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupApplicationJni")

    success = tim::jni::GroupApplicationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupApplicationResultJni")

    success = tim::jni::GroupAtInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupAtInfoJni")

    success = tim::jni::GroupChangeInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupChangeInfoJni")

    success = tim::jni::GroupInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupInfoJni")

    success = tim::jni::GroupInfoResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupInfoResultJni")

    success = tim::jni::GroupMemberChangeInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMemberChangeInfoJni")

    success = tim::jni::GroupMemberFullInfoJNI::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMemberFullInfoJNI")

    success = tim::jni::GroupMemberInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMemberInfoJni")

    success = tim::jni::GroupMemberInfoResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMemberInfoResultJni")

    success = tim::jni::GroupMemberOperationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMemberOperationResultJni")

    success = tim::jni::GroupMemberSearchParamJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMemberSearchParamJni")

    success = tim::jni::GroupMessageReadMemberListJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupMessageReadMemberListJni")

    success = tim::jni::GroupSearchParam::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupSearchParam")

    success = tim::jni::GroupTopicInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "GroupTopicInfoJni")

    success = tim::jni::TopicInfoResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "TopicInfoResultJni")

    success = tim::jni::TopicOperationResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "TopicOperationResultJni")

//********************************************************************************************//
//**************************************** 推送相关 *******************************************//
//********************************************************************************************//
    success = tim::jni::OfflinePushConfigJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "OfflinePushConfigJni")

    success = tim::jni::OfflinePushInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "OfflinePushInfoJni")

//********************************************************************************************//
//**************************************** 消息相关 *******************************************//
//********************************************************************************************//

    success = tim::jni::ImageJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ImageJni")

    success = tim::jni::MessageExtensionJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageExtensionJni")

    success = tim::jni::MessageExtensionResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageExtensionResultJni")

    success = tim::jni::MessageJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageJni")

    success = tim::jni::MessageListGetOptionJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageListGetOptionJni")

    success = tim::jni::MessageReactionJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageReactionJni")

    success = tim::jni::MessageReactionResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageReactionResultJni")

    success = tim::jni::MessageReactionUserResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageReactionUserResultJni")

    success = tim::jni::MessageReceiptJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageReceiptJni")

    success = tim::jni::MessageSearchParamJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageSearchParamJni")

    success = tim::jni::MessageSearchResultItemJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageSearchResultItemJni")

    success = tim::jni::MessageSearchResultJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "MessageSearchResultJni")

    success = tim::jni::ReceiveMessageOptInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "ReceiveMessageOptInfoJni")

    tim::jni::ElemProcessor::GetInstance().RegisterElemHandler();

//********************************************************************************************//
//**************************************** 信令相关 *******************************************//
//********************************************************************************************//
    success = tim::jni::SignalingInfoJni::InitIDs(env);
    CHECK_INITIDS_RESULT(success, "SignalingInfoJni")
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    tim::jni::JniHelper::Init(vm);

    jint result = JNI_ERR;
    tim::jni::ScopedJEnv scopedJEnv;
    JNIEnv *env = scopedJEnv.GetEnv();
    if (NULL == env) {
        __android_log_print(ANDROID_LOG_ERROR, "Native-IMSDK",
                            " ############### invalid JNIEnv ############### ");
        return result;
    }
    LOGE("jvm_wrapper.cpp JNI_OnLoad | env = %p", env);
    InitJNI(env);

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMManager(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMConversationManager(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMFriendshipManager(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMGroupManager(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMMessageManager(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMOfflinePushManager(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMSignalingManager(env)) {
        return result;
    }

    //下载消息

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMImageElem(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMSoundElem(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMVideoElem(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMFileElem(env)) {
        return result;
    }

    if (JNI_TRUE != RegisterNativeMethodsForV2TIMMergerElem(env)){
        return result;
    }

#ifdef __arm__
    __android_log_print(ANDROID_LOG_INFO, "Native-IMSDK",
                        " ############### imsdk version arm32 ############### ");
#endif

#ifdef __aarch64__
    __android_log_print(ANDROID_LOG_INFO, "Native-IMSDK",
                        " ################ imsdk version arm64 ############### ");
#endif

    return JNI_VERSION_1_6;
}

//JNIEXPORT void JNI_OnUnload(JavaVM *aJvm, void *aReserved){}

#ifdef __cplusplus
}
#endif