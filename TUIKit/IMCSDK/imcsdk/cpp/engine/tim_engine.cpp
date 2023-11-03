
#include "tim_engine.h"

#include <utility>
#include <im_callbak_jni.h>

#include "LogUtil.h"
#include "jni_helper.h"
#include "jni.h"
#include "tim_common.h"
#include "value_callback_impl.h"
#include "message_jni.h"
#include "TIMCloud.h"
#include "tim_callback_impl.h"
#include "download_callback_impl.h"
#include "json.h"

namespace tim {

    TIMEngine *TIMEngine::GetInstance() {
        static auto *instance = new TIMEngine();
        return instance;
    }

    void ImplTIMCommCallback(int32_t code, const char *desc, const char *json_params, const void *user_data) {
        auto _callbackImpl_ = (TIMCallbackIMpl *) user_data;
        _callbackImpl_->defaultCallBack(code, desc, json_params, user_data);
    }

    void ErrorCallBack(int ret, TIMCallbackIMpl *callbackImpl) {
        if (TIM_SUCC != ret) {
            callbackImpl->onError(ret);
        }
    }

    void ErrorCallBack(int ret, const jobject &callback) {
        if (TIM_SUCC != ret) {
            tim::jni::IMCallbackJNI::Fail(callback, ret);
            tim::jni::ScopedJEnv scopedJEnv;
            scopedJEnv.GetEnv()->DeleteGlobalRef(callback);
        }
    }

    void ErrorCallBack(int ret, DownloadTIMCallbackIMpl *callbackImpl) {
        if (TIM_SUCC != ret) {
            callbackImpl->OnError(ret);
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              BaseManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool TIMEngine::InitSDK(uint32_t sdkAppID, const char *json_config) {
        return TIMInit(sdkAppID, json_config);
    }

    void TIMEngine::UnInitSDK() {
        TIMUninit();
    }

    const char *TIMEngine::GetVersion() {
        return TIMGetSDKVersion();
    }

    uint64_t TIMEngine::GetServerTime() {
        return TIMGetServerTime();
    }

    void TIMEngine::Login(const char *userID, const char *userSig, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMLogin(userID, userSig, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::Logout(TIMCallbackIMpl *callbackImpl) {
        int ret = TIMLogout(ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    std::string TIMEngine::GetLoginUser() {
        const size_t kUserIDLength = 128;
        char user_id_buffer[kUserIDLength] = {0};
        int ret = TIMGetLoginUserID(user_id_buffer);
        if (TIM_SUCC != ret) {
            return "";
        }
        return user_id_buffer;
    }

    TIMLoginStatus TIMEngine::GetLoginStatus() {
        return TIMGetLoginStatus();
    }

    void TIMEngine::GetUsersInfo(const char *json_get_user_profile_list_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMProfileGetUserProfileList(json_get_user_profile_list_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetSelfInfo(const char *json_modify_self_user_profile_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMProfileModifySelfUserProfile(json_modify_self_user_profile_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetUserStatus(const char *json_identifier_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGetUserStatus(json_identifier_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetSelfStatus(const char *json_current_user_status, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSetSelfStatus(json_current_user_status, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SubscribeUserStatus(const char *json_identifier_array, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSubscribeUserStatus(json_identifier_array, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::UnsubscribeUserStatus(const char *json_identifier_array, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMUnsubscribeUserStatus(json_identifier_array, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::CallExperimentalAPI(const char *json_param, TIMCommCallback cb, const jobject &callback) {
        int ret = callExperimentalAPI(json_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              ConversationManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


    void TIMEngine::GetConversationList(TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvGetConvList(cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetConversationListByID(const char *json_get_conv_list_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvGetConvInfo(json_get_conv_list_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetConversationListByFilter(const char *filter, uint64_t next_seq, uint32_t count, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvGetConversationListByFilter(filter, next_seq, count, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteConversation(const char *conv_id, enum TIMConvType conv_type, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMConvDelete(conv_id, conv_type, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void
    TIMEngine::SetConversationDraft(const char *conv_id, enum TIMConvType conv_type, const char *json_draft_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMConvSetDraft(conv_id, conv_type, json_draft_param);
        if (ret == TIMErrCode::ERR_SUCC) {
            callbackImpl->defaultCallBack(ret, nullptr, nullptr, nullptr);
        } else {
            callbackImpl->onError(ret);
        }
    }

    void
    TIMEngine::SetConversationCustomData(const char *conversation_id_array, const char *custom_data, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvSetConversationCustomData(conversation_id_array, conversation_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::PinConversation(const char *conv_id, TIMConvType conv_type, bool is_pinned, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMConvPinConversation(conv_id, conv_type, is_pinned, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::MarkConversation(const char *conversation_id_array, uint64_t mark_type, bool enable_mark, TIMCommCallback cb,
                                     const jobject &callback) {
        int ret = TIMConvMarkConversation(conversation_id_array, mark_type, enable_mark, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetTotalUnreadMessageCount(TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvGetTotalUnreadMessageCount(cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::CreateConversationGroup(const char *group_name, const char *conversation_id_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvCreateConversationGroup(group_name, conversation_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetConversationGroupList(TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvGetConversationGroupList(cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteConversationGroup(const char *group_name, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMConvDeleteConversationGroup(group_name, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::RenameConversationGroup(const char *old_name, const char *new_name, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMConvRenameConversationGroup(old_name, new_name, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::AddConversationsToGroup(const char *group_name, const char *conversation_id_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvAddConversationsToGroup(group_name, conversation_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void
    TIMEngine::DeleteConversationsFromGroup(const char *group_name, const char *conversation_id_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMConvDeleteConversationsFromGroup(group_name, conversation_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              FriendshipManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


    void TIMEngine::GetFriendList(TIMCommCallback cb, const jobject &obj) {
        int ret = TIMFriendshipGetFriendProfileList(cb, obj);
        if (ret != TIM_SUCC) {
            tim::jni::IMCallbackJNI::Fail(obj, ret);
            tim::jni::ScopedJEnv scopedJEnv;
            scopedJEnv.GetEnv()->DeleteGlobalRef(obj);
        }
    }

    void TIMEngine::AddFriend(const char *json_add_friend_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipAddFriend(json_add_friend_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetFriendInfo(const char *json_modify_friend_info_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMFriendshipModifyFriendProfile(json_modify_friend_info_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::DeleteFromFriendList(const char *json_delete_friend_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipDeleteFriend(json_delete_friend_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::CheckFriend(const char *json_check_friend_list_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipCheckFriendType(json_check_friend_list_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::CreateFriendGroup(const char *json_create_friend_group_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipCreateFriendGroup(json_create_friend_group_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetFriendGroups(const char *json_get_friend_group_list_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipGetFriendGroupList(json_get_friend_group_list_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::RenameFriendGroup(const char *json_modify_friend_group_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMFriendshipModifyFriendGroup(json_modify_friend_group_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::DeleteFriendGroup(const char *json_delete_friend_group_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMFriendshipDeleteFriendGroup(json_delete_friend_group_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::AddToBlackList(const char *json_add_to_blacklist_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipAddToBlackList(json_add_to_blacklist_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetBlackList(TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipGetBlackList(cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteFromBlackList(const char *json_delete_from_blacklist_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipDeleteFromBlackList(json_delete_from_blacklist_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetFriendApplicationList(const char *json_get_pendency_list_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipGetPendencyList(json_get_pendency_list_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::ProcessFriendApplication(const char *json_handle_friend_add_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipHandleFriendAddRequest(json_handle_friend_add_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteFriendApplication(const char *json_delete_pendency_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMFriendshipDeletePendency(json_delete_pendency_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SetFriendApplicationRead(TIMCallbackIMpl *callbackImpl) {
        int ret = TIMFriendshipReportPendencyReaded(TIMGetServerTime(), ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SearchFriends(const char *json_search_friends_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipSearchFriends(json_search_friends_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetFriendsInfo(const char *json_get_friends_info_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipGetFriendsInfo(json_get_friends_info_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::AddFriendsToFriendGroup(const char *json_modify_friend_group_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipModifyFriendGroup(json_modify_friend_group_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteFriendsFromFriendGroup(const char *json_modify_friend_group_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMFriendshipModifyFriendGroup(json_modify_friend_group_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              GroupManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void TIMEngine::CreateGroup(const char *json_group_create_param, const TIMCommCallback &cb, const jobject &callback) {

        int ret = TIMGroupCreate(json_group_create_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DismissGroup(const char *group_id, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupDelete(group_id, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::JoinGroup(const char *groupID, const char *message, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupJoin(groupID, message, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::QuitGroup(const char *groupID, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupQuit(groupID, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::InviteUserToGroup(const char *json_group_invite_param, const TIMCommCallback &cb, const jobject &callback) {
        int ret = TIMGroupInviteMember(json_group_invite_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::KickGroupMember(const char *json_group_delete_param, const TIMCommCallback &cb, const jobject &callback) {
        int ret = TIMGroupDeleteMember(json_group_delete_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetJoinedGroupList(TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetJoinedGroupList(cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetGroupsInfo(const char *json_group_getinfo_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetGroupInfoList(json_group_getinfo_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetGroupInfo(const char *json_group_modifyinfo_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupModifyGroupInfo(json_group_modifyinfo_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetGroupMemberList(const char *json_group_getmeminfos_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetMemberInfoList(json_group_getmeminfos_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetGroupMemberInfo(const char *json_group_modifymeminfo_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupModifyMemberInfo(json_group_modifymeminfo_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::MarkGroupMemberList(const char *group_id, const char *member_array, uint32_t mark_type, bool enable_mark,
                                        TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupMarkGroupMemberList(group_id, member_array, mark_type, enable_mark, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetGroupApplicationList(const char *json_group_getpendence_list_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetPendencyList(json_group_getpendence_list_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetGroupApplicationRead(TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupReportPendencyReaded(TIMGetServerTime(), ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::ProcessGroupApplication(const char *json_group_handle_pendency_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupHandlePendency(json_group_handle_pendency_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetGroupOnlineMemberCount(const char *groupid, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetOnlineMemberCount(groupid, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SearchGroups(const char *json_group_search_groups_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupSearchGroups(json_group_search_groups_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SearchGroupMembers(const char *json_group_search_group_members_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupSearchGroupMembers(json_group_search_group_members_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::InitGroupAttributes(const char *group_id, const char *json_group_atrributes, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupInitGroupAttributes(group_id, json_group_atrributes, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SetGroupAttributes(const char *group_id, const char *json_group_atrributes, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupSetGroupAttributes(group_id, json_group_atrributes, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::DeleteGroupAttributes(const char *group_id, const char *json_keys, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupDeleteGroupAttributes(group_id, json_keys, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetGroupAttributes(const char *group_id, const char *json_keys, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetGroupAttributes(group_id, json_keys, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetJoinedCommunityList(TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupGetJoinedCommunityList(cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::CreateTopicInCommunity(const char *group_id, const char *json_topic_info, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupCreateTopicInCommunity(group_id, json_topic_info, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteTopicFromCommunity(const char *group_id, const char *json_topic_id_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMGroupDeleteTopicFromCommunity(group_id, json_topic_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetTopicInfo(const char *json_topic_info, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMGroupSetTopicInfo(json_topic_info, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetTopicInfoList(const char *group_id, const char *json_topic_id_array, TIMCommCallback cb, jobject &callback) {
        int ret = TIMGroupGetTopicInfoList(group_id, json_topic_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              MessageBaseManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void TIMEngine::AddAdvancedMsgListener(V2TIMAdvancedMsgListener *listener) {
//    GetMessageManager()->AddAdvancedMsgListener(listener);
//}
//
//void TIMEngine::RemoveAdvancedMsgListener(V2TIMAdvancedMsgListener *listener) {
//    GetMessageManager()->RemoveAdvancedMsgListener(listener);
//}

    void TIMEngine::SendMessage(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, char *message_id_buffer,
                                SendTIMCallbackIMpl *sendTimCallbackIMpl) {
        int ret = TIMMsgSendMessage(conv_id, conv_type, json_msg_param, message_id_buffer,
                                    tim::SendTIMCallbackIMpl::TIMCommCallbackImpl, sendTimCallbackIMpl);
        if (TIM_SUCC != ret) {
            sendTimCallbackIMpl->OnError(ret);
        }
    }

    void TIMEngine::FindMessages(const char *json_message_id_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgFindMessages(json_message_id_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::MarkMsgReportRead(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgReportReaded(conv_id, conv_type, json_msg_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::MarkAllMessageAsRead(TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgMarkAllMessageAsRead(ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::RevokeMessage(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgRevoke(conv_id, conv_type, json_msg_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::ModifyMessage(const char *json_msg_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgModifyMessage(json_msg_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void
    TIMEngine::InsertMessageToLocalStorage(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_param, TIMCommCallback cb,
                                           const jobject &callback) {
        int ret = TIMMsgSaveMsg(conv_id, conv_type, json_msg_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetHistoryMessageList(const char *conv_id, enum TIMConvType conv_type, const char *json_get_msg_param, TIMCommCallback cb,
                                          const jobject &callback) {
        int ret = TIMMsgGetMsgList(conv_id, conv_type, json_get_msg_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::DeleteMessages(const char *conv_id, enum TIMConvType conv_type, const char *json_msgdel_param, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgDelete(conv_id, conv_type, json_msgdel_param, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void
    TIMEngine::DeleteMsgList(const char *conv_id, enum TIMConvType conv_type, const char *json_msg_array, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgListDelete(conv_id, conv_type, json_msg_array, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::ClearHistoryMessage(const char *conv_id, enum TIMConvType conv_type, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgClearHistoryMessage(conv_id, conv_type, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SetC2CReceiveMessageOpt(const char *json_identifier_array, TIMReceiveMessageOpt opt, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgSetC2CReceiveMessageOpt(json_identifier_array, opt, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetC2CReceiveMessageOpt(const char *json_identifier_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgGetC2CReceiveMessageOpt(json_identifier_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetGroupReceiveMessageOpt(const char *group_id, TIMReceiveMessageOpt opt, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgSetGroupReceiveMessageOpt(group_id, opt, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SearchLocalMessages(const char *json_search_message_param, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgSearchLocalMessages(json_search_message_param, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SendMessageReadReceipts(const char *json_msg_array, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgSendMessageReadReceipts(json_msg_array, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetMessageReadReceipts(const char *json_msg_array, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgGetMessageReadReceipts(json_msg_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetGroupMessageReadMemberList(const char *json_msg, TIMGroupMessageReadMembersFilter filter, uint64_t next_seq, uint32_t count,
                                                  TIMMsgGroupMessageReadMemberListCallback cb, const jobject &callback) {
        int ret = TIMMsgGetGroupMessageReadMemberList(json_msg, filter, next_seq, count, cb, callback);
        ErrorCallBack(ret, callback);
    }

/** 下载富媒体消息 **/

    void ImplDownloadCallback(int32_t code, const char *desc, const char *json_params, const void *user_data) {
        auto _callbackImpl_ = (DownloadTIMCallbackIMpl *) user_data;
        _callbackImpl_->defaultCallBack(code, desc, json_params, user_data);
    }

    void TIMEngine::DownloadElemToPath(const char *json_download_elem_param, const char *path, DownloadTIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgDownloadElemToPath(json_download_elem_param, path, ImplDownloadCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::DownloadMergerMessage(const char *json_single_msg, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgDownloadMergerMessage(json_single_msg, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::ConvertVoiceToText(const char *url, const char *language, TIMCommCallback cb, const jobject &callback) {
        int ret = TIMMsgConvertVoiceToText(url,language,cb,callback);
        ErrorCallBack(ret, callback);
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              OfflinePushManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void TIMEngine::SetOfflinePushConfig(const char *json_token, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgSetOfflinePushToken(json_token, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::DoBackground(uint32_t unread_count, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgDoBackground(unread_count, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::DoForeground(TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgDoForeground(ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              SignalingManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void TIMEngine::Invite(const char *invitee, const char *data, bool online_user_only, const char *json_offline_push_info, int timeout,
                           char *invite_id_buffer, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSignalingInvite(invitee, data, online_user_only, json_offline_push_info, timeout, invite_id_buffer, ImplTIMCommCallback,
                                     callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::InviteInGroup(const char *group_id, const char *json_invitee_array, const char *data, bool online_user_only, int timeout,
                                  char *invite_id_buffer, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSignalingInviteInGroup(group_id, json_invitee_array, data, online_user_only, timeout, invite_id_buffer, ImplTIMCommCallback,
                                            callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::Cancel(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSignalingCancel(invite_id, data, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::Accept(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSignalingAccept(invite_id, data, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::Reject(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSignalingReject(invite_id, data, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetSignalingInfo(const char *json_msg, TIMCommCallback json_signaling_info_cb, const jobject &signalingInfo) {
        TIMGetSignalingInfo(json_msg, json_signaling_info_cb, signalingInfo);
    }

    void TIMEngine::SubscribeUserInfo(const char *json_user_id_list, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSubscribeUserInfo(json_user_id_list, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::UnsubscribeUserInfo(const char *json_user_id_list, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMUnsubscribeUserInfo(json_user_id_list, ImplTIMCommCallback, callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetUnreadMessageCountByFilter(const char *filter, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMConvGetUnreadMessageCountByFilter(filter, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SubscribeUnreadMessageCountByFilter(const char *filter) {
        TIMConvSubscribeUnreadMessageCountByFilter(filter);
    }

    void TIMEngine::UnsubscribeUnreadMessageCountByFilter(const char *filter) {
        TIMConvUnsubscribeUnreadMessageCountByFilter(filter);
    }

    void TIMEngine::CleanConversationUnreadMessageCount(const char *conversation_id, uint64_t clean_timestamp, uint64_t clean_sequence,
                                                        TIMCallbackIMpl *callback) {
        int ret = TIMConvCleanConversationUnreadMessageCount(conversation_id, clean_timestamp, clean_sequence, ImplTIMCommCallback, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GroupSetGroupCounters(const char *group_id, const char *json_group_counter_array, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMGroupSetGroupCounters(group_id, json_group_counter_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void
    TIMEngine::GroupGetGroupCounters(const char *group_id, const char *json_group_counter_key_array, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMGroupGetGroupCounters(group_id, json_group_counter_key_array, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GroupIncreaseGroupCounter(const char *group_id, const char *group_counter_key, int64_t group_counter_value, TIMCommCallback cb,
                                              jobject const &callback) {
        int ret = TIMGroupIncreaseGroupCounter(group_id, group_counter_key, group_counter_value, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GroupDecreaseGroupCounter(const char *group_id, const char *group_counter_key, int64_t group_counter_value, TIMCommCallback cb,
                                              jobject const &callback) {
        int ret = TIMGroupDecreaseGroupCounter(group_id, group_counter_key, group_counter_value, cb, callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetAllReceiveMessageOpt(enum TIMReceiveMessageOpt opt, int32_t start_hour, int32_t start_minute, int32_t start_second,
                                               uint32_t duration, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgSetAllReceiveMessageOpt(opt,start_hour,start_minute,start_second,duration,ImplTIMCommCallback,callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::SetAllReceiveMessageOpt2(enum TIMReceiveMessageOpt opt, uint32_t start_time_stamp, uint32_t duration,
                                                TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgSetAllReceiveMessageOpt2(opt,start_time_stamp,duration,ImplTIMCommCallback,callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetAllReceiveMessageOpt(TIMCommCallback cb, jobject const &callback) {
        int ret = TIMMsgGetAllReceiveMessageOpt(cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SearchCloudMessages(const char *json_search_message_param, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMMsgSearchCloudMessages(json_search_message_param,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::SetMessageExtensions(const char *json_msg, const char *json_extension_array, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMMsgSetMessageExtensions(json_msg,json_extension_array,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetMessageExtensions(const char *json_msg, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMMsgGetMessageExtensions(json_msg,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void
    TIMEngine::DeleteMessageExtensions(const char *json_msg, const char *json_extension_key_array, TIMCommCallback cb, jobject const &callback) {
        int ret = TIMMsgDeleteMessageExtensions(json_msg,json_extension_key_array,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::AddMessageReaction(const char *json_msg, const char *reaction_id, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgAddMessageReaction(json_msg,reaction_id,ImplTIMCommCallback,callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::RemoveMessageReaction(const char *json_msg, const char *reaction_id, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMMsgRemoveMessageReaction(json_msg,reaction_id,ImplTIMCommCallback,callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

    void TIMEngine::GetMessageReactions(const char *json_msg_array, uint32_t max_user_count_per_reaction, TIMCommCallback cb, jobject const
    &callback) {
        int ret = TIMMsgGetMessageReactions(json_msg_array,max_user_count_per_reaction,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::GetAllUserListOfMessageReaction(const char *json_msg, const char *reaction_id, uint32_t next_seq, uint32_t count,
                                                       TIMCommCallback cb, jobject const &callback) {
        int ret = TIMMsgGetAllUserListOfMessageReaction(json_msg,reaction_id,next_seq,count,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::TranslateText(const char *json_source_text_array, const char *source_language, const char *target_language, TIMCommCallback cb,
                                     jobject const &callback) {
        int ret = TIMMsgTranslateText(json_source_text_array,source_language,target_language,cb,callback);
        ErrorCallBack(ret, callback);
    }

    void TIMEngine::ModifyInvitation(const char *invite_id, const char *data, TIMCallbackIMpl *callbackImpl) {
        int ret = TIMSignalingModifyInvitation(invite_id,data,ImplTIMCommCallback,callbackImpl);
        ErrorCallBack(ret, callbackImpl);
    }

}