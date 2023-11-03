//
// Created by EDY on 2022/9/7.
//

#ifndef IMCPLUSPLUSDEMO_V2_IM_ENGINE_H
#define IMCPLUSPLUSDEMO_V2_IM_ENGINE_H

#include <jni.h>
#include <functional>
#include "tim_common.h"
#include "tim_callback_impl.h"
#include "send_callback_impl.h"
#include "download_callback_impl.h"
#include <vector>
#include "json.h"

namespace tim {
    class TIMEngine {

    protected:
        TIMEngine() = default;

        ~TIMEngine() = default;

    public:
        static TIMEngine *GetInstance();

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMManager  ----------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
//        void AddSDKListener(V2TIMSDKListener *listener);
//
//        void RemoveSDKListener(V2TIMSDKListener *listener);

        bool InitSDK(uint32_t sdkAppID, const char *json_config);

        void UnInitSDK();

        const char *GetVersion();

        uint64_t GetServerTime();

        void Login(const char *userID, const char *userSig, TIMCallbackIMpl *callback);

        void Logout(TIMCallbackIMpl *callbackImpl);

        std::string GetLoginUser();

        TIMLoginStatus GetLoginStatus();

//        void AddGroupListener(V2TIMGroupListener *listener);
//
//        void RemoveGroupListener(V2TIMGroupListener *listener);

        void GetUsersInfo(const char *json_get_user_profile_list_param, TIMCommCallback cb, const jobject &callback);

        void SetSelfInfo(const char *json_modify_self_user_profile_param, TIMCallbackIMpl *callbackImpl);

        void SubscribeUserInfo(const char *json_user_id_list, TIMCallbackIMpl *callbackImpl);

        void UnsubscribeUserInfo(const char *json_user_id_list, TIMCallbackIMpl *callbackImpl);

        void GetUserStatus(const char *json_identifier_array, TIMCommCallback cb, const jobject &callback);

        void SetSelfStatus(const char *json_current_user_status, TIMCallbackIMpl *callbackImpl);

        void SubscribeUserStatus(const char *json_identifier_array, TIMCallbackIMpl *callbackImpl);

        void UnsubscribeUserStatus(const char *json_identifier_array, TIMCallbackIMpl *callbackImpl);

        void CallExperimentalAPI(const char *json_param, TIMCommCallback cb, const jobject &callback);

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMConversationManager  ----------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

//        void AddConversationListener(V2TIMConversationListener *listener);
//
//        void RemoveConversationListener(V2TIMConversationListener *listener);

        void GetConversationList(TIMCommCallback cb, const jobject &callback);

        void GetConversationListByID(const char *json_get_conv_list_param, TIMCommCallback cb, const jobject &callback);

        void GetConversationListByFilter(const char *filter, uint64_t next_seq, uint32_t count, TIMCommCallback cb, const jobject &callback);

        void DeleteConversation(const char *conv_id, enum TIMConvType conv_type, TIMCallbackIMpl *callback);

        void SetConversationDraft(const char *conv_id, enum TIMConvType conv_type, const char *json_draft_param, TIMCallbackIMpl *callback);

        void SetConversationCustomData(const char *conversation_id_array, const char *custom_data, TIMCommCallback cb, const jobject &callback);

        void PinConversation(const char *conv_id, TIMConvType conv_type, bool is_pinned, TIMCallbackIMpl *callbackImpl);

        void MarkConversation(const char *conversation_id_array, uint64_t mark_type, bool enable_mark, TIMCommCallback cb, const jobject &callback);

        void GetTotalUnreadMessageCount(TIMCommCallback cb, const jobject &callback);

        void GetUnreadMessageCountByFilter(const char* filter, TIMCommCallback cb, const jobject &callback);

        void SubscribeUnreadMessageCountByFilter(const char* filter);

        void UnsubscribeUnreadMessageCountByFilter(const char* filter);

        void CleanConversationUnreadMessageCount(const char* conversation_id, uint64_t clean_timestamp, uint64_t clean_sequence, TIMCallbackIMpl *callback);

        void CreateConversationGroup(const char *group_name, const char *conversation_id_array, TIMCommCallback cb, const jobject &callback);

        void GetConversationGroupList(TIMCommCallback cb, const jobject &callback);

        void DeleteConversationGroup(const char *group_name, TIMCallbackIMpl *callbackImpl);

        void RenameConversationGroup(const char *old_name, const char *new_name, TIMCallbackIMpl *callbackImpl);

        void AddConversationsToGroup(const char *group_name, const char *conversation_id_array, TIMCommCallback cb, const jobject &callback);

        void DeleteConversationsFromGroup(const char *group_name, const char *conversation_id_array, TIMCommCallback cb, const jobject &callback);

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMFriendshipManager  ------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

//        void AddFriendListener(V2TIMFriendshipListener *listener);
//
//        void RemoveFriendListener(V2TIMFriendshipListener *listener);

        void GetFriendList(TIMCommCallback cb, const jobject &obj);

        void AddFriend(const char *json_add_friend_param, TIMCommCallback cb, const jobject &callback);

        void SetFriendInfo(const char *json_modify_friend_info_param, TIMCallbackIMpl *callbackImpl);

        void DeleteFromFriendList(const char *json_delete_friend_param, TIMCommCallback cb, const jobject &callback);

        void CheckFriend(const char *json_check_friend_list_param, TIMCommCallback cb, const jobject &callback);

        void CreateFriendGroup(const char *json_create_friend_group_param, TIMCommCallback cb, const jobject &callback);

        void GetFriendGroups(const char *json_get_friend_group_list_param, TIMCommCallback cb, const jobject &callback);

        void RenameFriendGroup(const char *json_modify_friend_group_param, TIMCallbackIMpl *callbackImpl);

        void DeleteFriendGroup(const char *json_delete_friend_group_param, TIMCallbackIMpl *callbackImpl);

        void AddToBlackList(const char *json_add_to_blacklist_param, TIMCommCallback cb, const jobject &callback);

        void GetBlackList(TIMCommCallback cb, const jobject &callback);

        void DeleteFromBlackList(const char *json_delete_from_blacklist_param, TIMCommCallback cb, const jobject &callback);

        void GetFriendApplicationList(const char *json_get_pendency_list_param, TIMCommCallback cb, const jobject &callback);

        void ProcessFriendApplication(const char *json_handle_friend_add_param, TIMCommCallback cb, const jobject &callback);

        void DeleteFriendApplication(const char *json_delete_pendency_param, TIMCallbackIMpl *callbackImpl);

        void SetFriendApplicationRead(TIMCallbackIMpl *callbackImpl);

        void SearchFriends(const char *json_search_friends_param, TIMCommCallback cb, const jobject &callback);

        void GetFriendsInfo(const char *json_get_friends_info_param, TIMCommCallback cb, const jobject &callback);

        void AddFriendsToFriendGroup(const char *json_modify_friend_group_param, TIMCommCallback cb, const jobject &callback);

        void DeleteFriendsFromFriendGroup(const char *json_modify_friend_group_param, TIMCommCallback cb, const jobject &callback);

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMGroupManager  -----------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

        void CreateGroup(const char *json_group_create_param, const TIMCommCallback &cb, const jobject &callback);

        void DismissGroup(const char *group_id, TIMCallbackIMpl *callbackImpl);

        void JoinGroup(const char *group_id, const char *hello_msg, TIMCallbackIMpl *callbackImpl);

        void QuitGroup(const char *group_id, TIMCallbackIMpl *callbackImpl);

        void InviteUserToGroup(const char *json_group_invite_param, const TIMCommCallback &cb, const jobject &callback);

        void KickGroupMember(const char *json_group_delete_param,  const TIMCommCallback &cb, const jobject &callback);

        void GetJoinedGroupList(TIMCommCallback cb, const jobject &callback);

        void GetGroupsInfo(const char *json_group_getinfo_param, TIMCommCallback cb, const jobject &callback);

        void SetGroupInfo(const char *json_group_modifyinfo_param, TIMCallbackIMpl *callbackImpl);

        void GetGroupMemberList(const char *json_group_getmeminfos_param, TIMCommCallback cb, const jobject &callback);

        void SetGroupMemberInfo(const char *json_group_modifymeminfo_param, TIMCallbackIMpl *callbackImpl);

        void MarkGroupMemberList(const char *group_id, const char *member_array, uint32_t mark_type, bool enable_mark, TIMCallbackIMpl *callbackImpl);

        void GetGroupApplicationList(const char *json_group_getpendence_list_param, TIMCommCallback cb, const jobject &callback);

        void SetGroupApplicationRead(TIMCallbackIMpl *callbackImpl);

        void ProcessGroupApplication(const char *json_group_handle_pendency_param, TIMCallbackIMpl *callbackImpl);

        void GetGroupOnlineMemberCount(const char *groupid, TIMCommCallback cb, const jobject &callback);

        void GroupSetGroupCounters(const char* group_id, const char* json_group_counter_array, TIMCommCallback cb, const jobject &callback);

        void GroupGetGroupCounters(const char* group_id, const char* json_group_counter_key_array, TIMCommCallback cb, const jobject &callback);

        void GroupIncreaseGroupCounter(const char* group_id, const char* group_counter_key, int64_t group_counter_value, TIMCommCallback cb, const jobject &callback);

        void GroupDecreaseGroupCounter(const char* group_id, const char* group_counter_key, int64_t group_counter_value, TIMCommCallback cb, const jobject &callback);

        void SearchGroups(const char *json_group_search_groups_param, TIMCommCallback cb, const jobject &callback);

        void SearchGroupMembers(const char *json_group_search_group_members_param, TIMCommCallback cb, const jobject &callback);

        void InitGroupAttributes(const char *group_id, const char *json_group_atrributes, TIMCallbackIMpl *callbackImpl);

        void SetGroupAttributes(const char *group_id, const char *json_group_atrributes, TIMCallbackIMpl *callbackImpl);

        void DeleteGroupAttributes(const char *group_id, const char *json_keys, TIMCallbackIMpl *callbackImpl);

        void GetGroupAttributes(const char *group_id, const char *json_keys, TIMCommCallback cb, const jobject &callback);

        void GetJoinedCommunityList(TIMCommCallback cb, const jobject &callback);

        void CreateTopicInCommunity(const char *group_id, const char *json_topic_info, TIMCommCallback cb, const jobject &callback);

        void DeleteTopicFromCommunity(const char *group_id, const char *json_topic_id_array, TIMCommCallback cb, const jobject &callback);

        void SetTopicInfo(const char *json_topic_info, TIMCallbackIMpl *callbackImpl);

        void GetTopicInfoList(const char *group_id, const char *json_topic_id_array, TIMCommCallback cb, jobject &callback);

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMMessageManager  ---------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

//        void AddAdvancedMsgListener(V2TIMAdvancedMsgListener *listener);
//
//        void RemoveAdvancedMsgListener(V2TIMAdvancedMsgListener *listener);

        void SendMessage(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, char *message_id_buffer,
                         SendTIMCallbackIMpl *sendTimCallbackIMpl);

        void FindMessages(const char *json_message_id_array, TIMCommCallback cb,const jobject &callback);

        void MarkMsgReportRead(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, TIMCallbackIMpl *callbackImpl);

        void MarkAllMessageAsRead(TIMCallbackIMpl *callbackImpl);

        void RevokeMessage(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, TIMCallbackIMpl *callbackImpl);

        void ModifyMessage(const char *json_msg_param, TIMCommCallback cb, const jobject &callback);

        void InsertMessageToLocalStorage(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, TIMCommCallback cb, const jobject &callback);

        void GetHistoryMessageList(const char *conv_id, enum TIMConvType conv_type, const char *json_get_msg_param, TIMCommCallback cb,
                                   const jobject &callback);

        void
        DeleteMessages(const char *conv_id, enum TIMConvType conv_type, const char *json_msgdel_param, TIMCallbackIMpl *callbackImpl);

        void DeleteMsgList(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_array, TIMCallbackIMpl *callbackImpl);

        void ClearHistoryMessage(const char *conv_id, enum TIMConvType conv_type, TIMCallbackIMpl *callbackImpl);

        void SetC2CReceiveMessageOpt(const char *json_identifier_array, TIMReceiveMessageOpt opt, TIMCallbackIMpl *callbackImpl);

        void GetC2CReceiveMessageOpt(const char *json_identifier_array, TIMCommCallback cb, const jobject &callback);

        void SetGroupReceiveMessageOpt(const char *group_id, TIMReceiveMessageOpt opt, TIMCallbackIMpl *callbackImpl);

        void SetAllReceiveMessageOpt(enum TIMReceiveMessageOpt opt, int32_t start_hour, int32_t start_minute, int32_t start_second, uint32_t duration, TIMCallbackIMpl *callbackImpl);

        void SetAllReceiveMessageOpt2(enum TIMReceiveMessageOpt opt, uint32_t start_time_stamp, uint32_t duration, TIMCallbackIMpl *callbackImpl);

        void GetAllReceiveMessageOpt(TIMCommCallback cb, const jobject &callback);

        void SearchLocalMessages(const char *json_search_message_param, TIMCommCallback cb, const jobject &callback);

        void SearchCloudMessages(const char* json_search_message_param, TIMCommCallback cb, const jobject &callback);

        void SendMessageReadReceipts(const char *json_msg_array, TIMCallbackIMpl *callbackImpl);

        void GetMessageReadReceipts(const char *json_msg_array, TIMCommCallback cb, const jobject &callback);

        void GetGroupMessageReadMemberList(const char *json_msg, TIMGroupMessageReadMembersFilter filter, uint64_t next_seq, uint32_t count,
                                           TIMMsgGroupMessageReadMemberListCallback cb, const jobject &callback);

        void SetMessageExtensions(const char *json_msg, const char *json_extension_array, TIMCommCallback cb, const jobject &callback);

        void GetMessageExtensions(const char *json_msg, TIMCommCallback cb, const jobject &callback);

        void DeleteMessageExtensions(const char *json_msg, const char *json_extension_key_array, TIMCommCallback cb, const jobject &callback);

        void AddMessageReaction(const char *json_msg, const char *reaction_id, TIMCallbackIMpl *callbackImpl);

        void RemoveMessageReaction(const char *json_msg, const char *reaction_id, TIMCallbackIMpl *callbackImpl);

        void GetMessageReactions(const char *json_msg_array, uint32_t max_user_count_per_reaction, TIMCommCallback cb, const jobject &callback);

        void GetAllUserListOfMessageReaction(const char *json_msg, const char *reaction_id, uint32_t next_seq,
                                              uint32_t count, TIMCommCallback cb, const jobject &callback);

        void TranslateText(const char *json_source_text_array, const char *source_language, const char *target_language, TIMCommCallback cb, const jobject &callback);

        /** 下载富媒体消息 **/

        //下载消息内元素到指定文件路径(图片、视频、音频、文件)
        void DownloadElemToPath(const char* json_download_elem_param, const char* path, DownloadTIMCallbackIMpl *callbackImpl);

        //下载合并消息
        void DownloadMergerMessage(const char* json_single_msg, TIMCommCallback cb, const jobject &callback);

        void ConvertVoiceToText(const char *url, const char *language, TIMCommCallback cb, const jobject &callback);

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMOfflinePushManager  -----------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

        void SetOfflinePushConfig(const char *json_token, TIMCallbackIMpl *callbackImpl);

        void DoBackground(uint32_t unread_count, TIMCallbackIMpl *callbackImpl);

        void DoForeground(TIMCallbackIMpl *callbackImpl);

// ------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------  V2TIMSignalingManager  -------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

//    void AddSignalingListener(V2TIMSignalingListener *listener);

//    void RemoveSignalingListener(V2TIMSignalingListener *listener);

        void Invite(const char *invitee, const char *data, bool online_user_only, const char *json_offline_push_info, int timeout,
                    char *invite_id_buffer, TIMCallbackIMpl *callbackImpl);

        void InviteInGroup(const char *group_id, const char *json_invitee_array, const char *data, bool online_user_only, int timeout,
                           char *invite_id_buffer, TIMCallbackIMpl *callbackImpl);

        void Cancel(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl);

        void Accept(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl);

        void Reject(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl);

        void GetSignalingInfo(const char *json_msg, TIMCommCallback json_signaling_info_cb, const jobject &signalingInfo);

        void ModifyInvitation(const char* invite_id, const char* data, TIMCallbackIMpl *callbackImpl);

    };
}

#endif //IMCPLUSPLUSDEMO_V2_IM_ENGINE_H
