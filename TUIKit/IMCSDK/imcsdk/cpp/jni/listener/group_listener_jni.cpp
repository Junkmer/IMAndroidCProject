//
// Created by EDY on 2022/9/23.
//

#include <LogUtil.h>
#include <jni_helper.h>
#include <group_member_info_jni.h>
#include "group_listener_jni.h"
#include "tim_engine.h"
#include "java_basic_jni.h"
#include "group_member_change_info_jni.h"
#include "group_change_info_jni.h"
#include "group_topic_info_jni.h"
#include "TIMCloud.h"
#include "LogUtil.h"
#include "message_jni.h"

namespace tim {
    namespace jni {
        jclass GroupListenerJni::j_cls_;
        jmethodID GroupListenerJni::j_method_id_array_[MethodIDMax];

        void GroupListenerJni::AddListener(JNIEnv *env, jobject listener_simple, jstring listenerPath) {
            if (nullptr == listener_simple) {
                LOGE("GroupListenerJni | AddListener listener_simple is null");
                return;
            }
            if (listener_group_map.empty()) {
                TIMAddRecvNewMsgCallback(ImplTIMRecvNewMsgCallback, &listener_group_map);
                TIMSetGroupTipsEventCallback(ImplTIMGroupTipsEventCallback, &listener_group_map);
                TIMSetGroupAttributeChangedCallback(ImplTIMGroupAttributeChangedCallback, &listener_group_map);
                TIMSetGroupCounterChangedCallback(ImplTIMGroupCounterChangedCallback, &listener_group_map);
                TIMSetGroupTopicCreatedCallback(ImplTIMGroupTopicCreatedCallback, &listener_group_map);
                TIMSetGroupTopicDeletedCallback(ImplTIMGroupTopicDeletedCallback, &listener_group_map);
                TIMSetGroupTopicChangedCallback(ImplTIMGroupTopicChangedCallback, &listener_group_map);
            }

            std::string path = StringJni::Jstring2Cstring(env, listenerPath);
            for (auto &item: listener_group_map) {
                if (path.empty() || path == item.first) {
                    return;
                }
            }

            jobject j_obj = env->NewGlobalRef(listener_simple);
            listener_group_map.insert(std::make_pair(path, j_obj));
        }

        void GroupListenerJni::RemoveListener(JNIEnv *env, jstring listenerPath) {
            if (nullptr == listenerPath) {
                LOGE("GroupListenerJni | RemoveListener listener is null");
                return;
            }
            listener_group_map.erase(StringJni::Jstring2Cstring(env, listenerPath));
            if (listener_group_map.empty()) {
                TIMAddRecvNewMsgCallback(ImplTIMRecvNewMsgCallback, nullptr);
                TIMSetGroupTipsEventCallback(ImplTIMGroupTipsEventCallback, nullptr);
                TIMSetGroupAttributeChangedCallback(ImplTIMGroupAttributeChangedCallback, nullptr);
                TIMSetGroupCounterChangedCallback(ImplTIMGroupCounterChangedCallback, nullptr);
                TIMSetGroupTopicCreatedCallback(ImplTIMGroupTopicCreatedCallback, nullptr);
                TIMSetGroupTopicDeletedCallback(ImplTIMGroupTopicDeletedCallback, nullptr);
                TIMSetGroupTopicChangedCallback(ImplTIMGroupTopicChangedCallback, nullptr);
            }
        }

        bool GroupListenerJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupListener");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "onMemberEnter", "(Ljava/lang/String;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnMemberEnter] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onMemberLeave",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnMemberLeave] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onMemberInvited",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnMemberInvited] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onMemberKicked",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnMemberKicked] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onMemberInfoChanged", "(Ljava/lang/String;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnMemberInfoChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onAllGroupMembersMuted", "(Ljava/lang/String;Z)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnAllGroupMembersMuted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onMemberMarkChanged", "(Ljava/lang/String;Ljava/util/List;IZ)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnMemberMarkChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onGroupCreated", "(Ljava/lang/String;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGroupCreated] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onGroupDismissed",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGroupDismissed] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onGroupRecycled",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGroupRecycled] = jmethod;

            jmethod =
                    env->GetMethodID(j_cls_, "onGroupInfoChanged", "(Ljava/lang/String;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGroupInfoChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onReceiveJoinApplication",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;Ljava/lang/String;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnReceiveJoinRequest] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onApplicationProcessed",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;ZLjava/lang/String;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnReceiveJoinResponse] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onGrantAdministrator",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGrantAdministrator] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onRevokeAdministrator",
                                       "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnRevokeAdministrator] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onQuitFromGroup", "(Ljava/lang/String;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnQuitFromGroup] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onReceiveRESTCustomData", "(Ljava/lang/String;[B)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnReceiveCustomData] = jmethod;

            jmethod =
                    env->GetMethodID(j_cls_, "onGroupAttributeChanged", "(Ljava/lang/String;Ljava/util/Map;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGroupAttributeChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onTopicCreated", "(Ljava/lang/String;Ljava/lang/String;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnTopicCreated] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onTopicDeleted", "(Ljava/lang/String;Ljava/util/List;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnTopicDeleted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onTopicInfoChanged", "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMTopicInfo;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnTopicChanged] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onGroupCounterChanged", "(Ljava/lang/String;Ljava/lang/String;J)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDOnGroupCounterChanged] = jmethod;

            return true;
        }

        bool GroupListenerJni::empty() {
            return listener_group_map.empty();
        }

        void GroupListenerJni::ImplTIMRecvNewMsgCallback(const char *json_msg_array, const void *user_data) {
            LOGE("ImplTIMRecvNewMsgCallback — json_msg_array = %s",json_msg_array);
            json::Array msg_array = json::Deserialize(json_msg_array);
            for (const auto &item: msg_array) {
                json::Array elem_array = item[kTIMMsgElemArray];
                if (item.HasKey(kTIMMsgElemArray) && elem_array.size() == 1){
                    json::Object elem_json = elem_array[0];
                    int elem_type = elem_json[kTIMElemType];

                    if (elem_type != TIMElemType::kTIMElem_GroupTips && elem_type != TIMElemType::kTIMElem_GroupReport){
                        return;
                    }

                    auto _listener = (std::map<std::string, jobject> *) user_data;

                    if (elem_type == TIMElemType::kTIMElem_GroupReport){
                        //*******处理群system消息*******
                        std::string groupID = elem_json[kTIMGroupReportElemGroupId];
                        json::Object op_user_json;

                        int reportType = elem_json[kTIMGroupReportElemReportType];
                        if (reportType == TIMGroupReportType::kTIMGroupReport_Create){//创建群
                            OnGroupCreated(*_listener,groupID);
                        }else if(reportType == TIMGroupReportType::kTIMGroupReport_Delete){//群被解散
                            op_user_json = elem_json[kTIMGroupReportElemOpGroupMemberInfo];
                            OnGroupDismissed(*_listener,groupID,op_user_json);
                        }else if(reportType == TIMGroupReportType::kTIMGroupReport_GroupRecycle){//群已被回收
                            op_user_json = elem_json[kTIMGroupReportElemOpGroupMemberInfo];
                            OnGroupRecycled(*_listener,groupID,op_user_json);
                        }else if(reportType == TIMGroupReportType::kTIMGroupReport_AddRequest){//主动申请加群请求(只有管理员会接收到)
                            json::Object member_json = elem_json[kTIMGroupReportElemOpGroupMemberInfo];
                            std::string opReason = elem_json[kTIMGroupReportElemMsg];
                            OnReceiveJoinApplication(*_listener,groupID,member_json,opReason);
                        }else if(reportType == TIMGroupReportType::kTIMGroupReport_AddAccept
                                    || reportType == TIMGroupReportType::kTIMGroupReport_AddRefuse){//加群请求已经被群主或管理员处理了（只有申请人能够收到）
                            op_user_json = elem_json[kTIMGroupReportElemOpGroupMemberInfo];
                            bool isAgreeJoin = reportType == TIMGroupReportType::kTIMGroupReport_AddAccept;
                            std::string opReason = elem_json[kTIMGroupReportElemMsg];
                            OnApplicationProcessed(*_listener,groupID,op_user_json,isAgreeJoin,opReason);
                        }else if(reportType == TIMGroupReportType::kTIMGroupReport_Quit){//主动退出群组（主要用于多端同步）
                            OnQuitFromGroup(*_listener,groupID);
                        }else if(reportType == TIMGroupReportType::kTIMGroupReport_UserDefine){//收到 RESTAPI 下发的自定义系统消息
                            std::string customData = elem_json[kTIMGroupReportElemOpUserInfo];
                            OnReceiveRESTCustomData(*_listener,groupID,customData);
                        }
                    }else if (elem_type == TIMElemType::kTIMElem_GroupTips){
                        //*******处理群tip消息*******

                    }
                }
            }
        }

        void GroupListenerJni::ImplTIMGroupTipsEventCallback(const char *json_group_tip, const void *user_data) {
            LOGE("ImplTIMGroupTipsEventCallback — json_group_tip = %s",json_group_tip);

            auto *_listener = (std::map<std::string, jobject> *) user_data;

            json::Object tip_json = json::Deserialize(json_group_tip);
            std::string groupID = tip_json[kTIMGroupTipsElemGroupId];
            json::Object op_user_json;
            json::Array member_array;

            auto tipType = TIMGroupTipType(tip_json[kTIMGroupTipsElemTipType].ToInt());
            if (tipType == TIMGroupTipType::kTIMGroupTip_Invite) {// 邀请加入提示
                /**
                 * TODO::注：由于目前此回调暂时无法区分进群成员具体是被邀请进群还是主动申请进群，暂通过新消息回调实现。
                 */
                member_array = tip_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                OnMemberEnter(*_listener, groupID, member_array);
//
//                op_user_json = tip_json[kTIMGroupTipsElemOpGroupMemberInfo];
//                OnMemberInvited(*_listener, groupID, op_user_json, member_array);
            } else if (tipType == TIMGroupTipType::kTIMGroupTip_Quit) {// 退群提示
                member_array = tip_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                for (const auto &item : member_array){
                    OnMemberLeave(*_listener, groupID, item);
                }
            } else if (tipType == TIMGroupTipType::kTIMGroupTip_Kick) {// 踢人提示
                op_user_json = tip_json[kTIMGroupTipsElemOpGroupMemberInfo];
                member_array = tip_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                OnMemberKicked(*_listener, groupID, op_user_json, member_array);
            } else if (tipType == TIMGroupTipType::kTIMGroupTip_SetAdmin) {// 设置管理员提示
                op_user_json = tip_json[kTIMGroupTipsElemOpGroupMemberInfo];
                member_array = tip_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                OnGrantAdministrator(*_listener, groupID, op_user_json, member_array);
            } else if (tipType == TIMGroupTipType::kTIMGroupTip_CancelAdmin) {// 取消管理员提示
                op_user_json = tip_json[kTIMGroupTipsElemOpGroupMemberInfo];
                member_array = tip_json[kTIMGroupTipsElemChangedGroupMemberInfoArray];
                OnRevokeAdministrator(*_listener, groupID, op_user_json, member_array);
            } else if (tipType == TIMGroupTipType::kTIMGroupTip_GroupInfoChange) {// 群信息修改提示
                json::Array group_change_info_array = tip_json[kTIMGroupTipsElemGroupChangeInfoArray];
                OnGroupInfoChanged(*_listener, groupID, group_change_info_array);

                for (const auto &item: group_change_info_array){
                    auto flag = TIMGroupTipGroupChangeFlag(item[kTIMGroupTipGroupChangeInfoFlag].ToInt());
                    if (flag == TIMGroupTipGroupChangeFlag::kTIMGroupTipChangeFlag_ShutupAll){
                        bool isAllMute = item[kTIMGroupTipGroupChangeInfoBoolValue];
                        onAllGroupMembersMuted(*_listener,groupID,isAllMute);
                    }
                }

            } else if (tipType == TIMGroupTipType::kTIMGroupTip_MemberInfoChange) {// 群成员信息修改提示
                json::Array member_change_info_array = tip_json[kTIMGroupTipsElemMemberChangeInfoArray];
                OnMemberInfoChanged(*_listener, groupID, member_change_info_array);
            } else if(tipType == TIMGroupTipType::kTIMGroupTip_MemberMarkChange){// 群成员标记修改提示
                json::Array member_mark_array = tip_json[kTIMGroupTipsElemMemberMarkInfoArray];
                for (const auto &item: member_mark_array){
                    json::Array member_id_array = item[kTIMGroupTipMemberMarkChangeInfoUserIDList];
                    int markType = item[kTIMGroupTipMemberMarkChangeInfoMarkType];
                    bool enableMark = item[kTIMGroupTipMemberChangeInfoEnableMark];
                    onMemberMarkChanged(*_listener,groupID,member_array,markType,enableMark);
                }
            }
        }

        void GroupListenerJni::ImplTIMGroupAttributeChangedCallback(const char *group_id, const char *json_group_attribute_array, const void *user_data) {
            OnGroupAttributeChanged(*(std::map<std::string, jobject> *) user_data, group_id, json::Deserialize(json_group_attribute_array));
        }

        void GroupListenerJni::ImplTIMGroupCounterChangedCallback(const char *group_id, const char *group_counter_key, int64_t group_counter_new_value,
                                                                  const void *user_data) {
            onGroupCounterChanged(*(std::map<std::string, jobject> *) user_data, group_id, group_counter_key, group_counter_new_value);
        }

        void GroupListenerJni::ImplTIMGroupTopicCreatedCallback(const char *group_id, const char *topic_id, const void *user_data) {
            OnTopicCreated(*(std::map<std::string, jobject> *) user_data, group_id, topic_id);
        }

        void GroupListenerJni::ImplTIMGroupTopicDeletedCallback(const char *group_id, const char *topic_id_array, const void *user_data) {
            OnTopicDeleted(*(std::map<std::string, jobject> *) user_data, group_id, json::Deserialize(topic_id_array));
        }

        void GroupListenerJni::ImplTIMGroupTopicChangedCallback(const char *group_id, const char *topic_info, const void *user_data) {
            OnTopicChanged(*(std::map<std::string, jobject> *) user_data, group_id, json::Deserialize(topic_info));
        }

        void GroupListenerJni::OnMemberEnter(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &memberList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_memberList = ArrayListJni::NewArrayList();
            for (const auto & i : memberList) {
                jobject memberObj = GroupMemberInfoJni::Convert2JObject(i);
                ArrayListJni::Add(j_memberList, memberObj);
                env->DeleteLocalRef(memberObj);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberEnter], groupIDStr, j_memberList);
            }

            env->DeleteLocalRef(j_memberList);
        }

        void GroupListenerJni::OnMemberLeave(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &member) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_obj_member = GroupMemberInfoJni::Convert2JObject(member);
            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberLeave], groupIDStr, j_obj_member);
            }
            env->DeleteLocalRef(j_obj_member);
        }

        void GroupListenerJni::OnMemberInvited(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser,
                                               const json::Array &memberList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_opUser = GroupMemberInfoJni::Convert2JObject(opUser);

            jobject j_obj_memberList = ArrayListJni::NewArrayList();

            for (const auto & i : memberList) {
                jobject j_member = GroupMemberInfoJni::Convert2JObject(i);
                ArrayListJni::Add(j_obj_memberList, j_member);
                env->DeleteLocalRef(j_member);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberInvited], groupIDStr, j_opUser, j_obj_memberList);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_opUser);
            env->DeleteLocalRef(j_obj_memberList);
        }

        void GroupListenerJni::OnMemberKicked(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser,
                                              const json::Array &memberList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_opUser = GroupMemberInfoJni::Convert2JObject(opUser);

            jobject j_obj_memberList = ArrayListJni::NewArrayList();
            for (const auto & i : memberList) {
                jobject j_obj_member = GroupMemberInfoJni::Convert2JObject(i);
                ArrayListJni::Add(j_obj_memberList, j_obj_member);
                env->DeleteLocalRef(j_obj_member);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberKicked], groupIDStr, j_opUser, j_obj_memberList);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_opUser);
            env->DeleteLocalRef(j_obj_memberList);
        }

        void GroupListenerJni::OnMemberInfoChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID,
                                                   const json::Array &v2TIMGroupMemberChangeInfoList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_obj_groupMemberChangeInfoList = ArrayListJni::NewArrayList();
            for (const auto & i : v2TIMGroupMemberChangeInfoList) {
                jobject j_obj_groupMemberChangeInfo = GroupMemberChangeInfoJni::Convert2JObject(i);
                ArrayListJni::Add(j_obj_groupMemberChangeInfoList, j_obj_groupMemberChangeInfo);
                env->DeleteLocalRef(j_obj_groupMemberChangeInfo);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberInfoChanged], groupIDStr, j_obj_groupMemberChangeInfoList);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_groupMemberChangeInfoList);
        }

        void GroupListenerJni::onAllGroupMembersMuted(const std::map<std::string, jobject> &_listener_,const std::string &groupID, bool isMute){
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            for (const auto & item_listener : _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnAllGroupMembersMuted], groupIDStr, isMute);
            }

            env->DeleteLocalRef(groupIDStr);
        }

        void GroupListenerJni::onMemberMarkChanged(const std::map<std::string, jobject> &_listener_,const std::string &groupID, const json::Array &memberIDList, int markType, bool enableMark){
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_obj_memberIDList = ArrayListJni::NewArrayList();
            for (const auto &item: memberIDList){
                jobject j_obj_memberID = StringJni::Cstring2Jstring(env,item);
                ArrayListJni::Add(j_obj_memberIDList, j_obj_memberID);
                env->DeleteLocalRef(j_obj_memberID);
            }

            for (const auto & item_listener : _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberMarkChanged], groupIDStr,j_obj_memberIDList,markType,enableMark);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_memberIDList);
        }

        void GroupListenerJni::OnGroupCreated(const std::map<std::string, jobject> &_listener_, const std::string &groupID) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            for (const auto & item_listener : _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnGroupCreated], groupIDStr);
            }

            env->DeleteLocalRef(groupIDStr);
        }

        void GroupListenerJni::OnGroupDismissed(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_obj_opUser = GroupMemberInfoJni::Convert2JObject(opUser);

            for (const auto & item_listener : _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberKicked], groupIDStr, j_obj_opUser);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_opUser);
        }

        void GroupListenerJni::OnGroupRecycled(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_obj_opUser = GroupMemberInfoJni::Convert2JObject(opUser);

            for (const auto & item_listener : _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnGroupRecycled], groupIDStr, j_obj_opUser);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_opUser);
        }

        void GroupListenerJni::OnGroupInfoChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &changeInfos) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_obj_changeInfoList = ArrayListJni::NewArrayList();
            for (const auto & changeInfo : changeInfos) {
                jobject j_obj_changeInfo = GroupChangeInfoJni::Convert2JObject(changeInfo);
                ArrayListJni::Add(j_obj_changeInfoList, j_obj_changeInfo);
                env->DeleteLocalRef(j_obj_changeInfo);
            }

            for (const auto & item_listener : _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnGroupInfoChanged], groupIDStr, j_obj_changeInfoList);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_changeInfoList);
        }

        void
        GroupListenerJni::OnGroupAttributeChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &groupAttributeMap) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject map_obj = HashMapJni::NewHashMap();
            for (const auto &item: groupAttributeMap) {
                jstring key = StringJni::Cstring2Jstring(env, item[TIMGroupAttributeKey]);
                jstring value = StringJni::Cstring2Jstring(env, item[TIMGroupAttributeValue]);
                HashMapJni::Put(map_obj, key, value);

                env->DeleteLocalRef(key);
                env->DeleteLocalRef(value);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnMemberKicked], groupIDStr, map_obj);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(map_obj);
        }

        void GroupListenerJni::OnReceiveJoinApplication(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &member,
                                                        const std::string &opReason) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_obj_member = GroupMemberInfoJni::Convert2JObject(member);
            jstring j_str_opReason = StringJni::Cstring2Jstring(env, opReason);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnReceiveJoinRequest], groupIDStr, j_obj_member, j_str_opReason);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_member);
            env->DeleteLocalRef(j_str_opReason);
        }

        void GroupListenerJni::OnApplicationProcessed(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser,
                                                      bool isAgreeJoin,
                                                      const std::string &opReason) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_obj_opUser = GroupMemberInfoJni::Convert2JObject(opUser);
            jstring j_str_opReason = StringJni::Cstring2Jstring(env, groupID);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnReceiveJoinResponse], groupIDStr, j_obj_opUser, isAgreeJoin, j_str_opReason);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_opUser);
            env->DeleteLocalRef(j_str_opReason);
        }

        void GroupListenerJni::OnGrantAdministrator(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser,
                                                    const json::Array &memberList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_obj_opUser = GroupMemberInfoJni::Convert2JObject(opUser);
            jobject j_obj_memberList = ArrayListJni::NewArrayList();

            for (const auto & i : memberList) {
                jobject j_obj_member = GroupMemberInfoJni::Convert2JObject(i);
                ArrayListJni::Add(j_obj_memberList, j_obj_member);
                env->DeleteLocalRef(j_obj_member);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnGrantAdministrator], groupIDStr, j_obj_opUser, j_obj_memberList);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_opUser);
            env->DeleteLocalRef(j_obj_memberList);
        }

        void GroupListenerJni::OnRevokeAdministrator(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &opUser,
                                                     const json::Array &memberList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_obj_opUser = GroupMemberInfoJni::Convert2JObject(opUser);
            jobject j_obj_memberList = ArrayListJni::NewArrayList();
            for (const auto & i : memberList) {
                jobject j_obj_member = GroupMemberInfoJni::Convert2JObject(i);
                ArrayListJni::Add(j_obj_memberList, j_obj_member);
                env->DeleteLocalRef(j_obj_member);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnRevokeAdministrator], groupIDStr, j_obj_opUser, j_obj_memberList);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_obj_opUser);
            env->DeleteLocalRef(j_obj_memberList);
        }

        void GroupListenerJni::OnQuitFromGroup(const std::map<std::string, jobject> &_listener_, const std::string &groupID) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnQuitFromGroup], groupIDStr);
            }

            env->DeleteLocalRef(groupIDStr);
        }

        void GroupListenerJni::OnReceiveRESTCustomData(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const std::string &customData) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_str_customData = StringJni::Cstring2Jstring(env, customData);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnReceiveCustomData], groupIDStr, j_str_customData);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(j_str_customData);
        }

        void GroupListenerJni::OnTopicCreated(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const std::string &topicID) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring communityIDStr = StringJni::Cstring2Jstring(env, groupID);
            jstring j_str_topicID = StringJni::Cstring2Jstring(env, topicID);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnTopicCreated], communityIDStr, j_str_topicID);
            }

            env->DeleteLocalRef(communityIDStr);
            env->DeleteLocalRef(j_str_topicID);
        }

        void GroupListenerJni::OnTopicDeleted(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Array &topicIDList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring communityIDStr = StringJni::Cstring2Jstring(env, groupID);

            jobject j_obj_topicIDList = ArrayListJni::NewArrayList();
            for (const auto & i : topicIDList) {
                jstring j_obj_topicID = StringJni::Cstring2Jstring(env, i);
                ArrayListJni::Add(j_obj_topicIDList, j_obj_topicID);
                env->DeleteLocalRef(j_obj_topicID);
            }

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnTopicDeleted], communityIDStr, j_obj_topicIDList);
            }

            env->DeleteLocalRef(communityIDStr);
            env->DeleteLocalRef(j_obj_topicIDList);
        }

        void GroupListenerJni::OnTopicChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const json::Object &topicInfo) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring communityIDStr = StringJni::Cstring2Jstring(env, groupID);
            jobject j_obj_topicInfo = GroupTopicInfoJni::Convert2JObject(topicInfo);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnTopicChanged], communityIDStr, j_obj_topicInfo);
            }

            env->DeleteLocalRef(communityIDStr);
            env->DeleteLocalRef(j_obj_topicInfo);
        }

        void
        GroupListenerJni::onGroupCounterChanged(const std::map<std::string, jobject> &_listener_, const std::string &groupID, const std::string &key, long newValue) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring groupIDStr = StringJni::Cstring2Jstring(env, groupID);
            jstring keyStr = StringJni::Cstring2Jstring(env, key);

            for (auto &item_listener: _listener_) {
                env->CallVoidMethod(item_listener.second, j_method_id_array_[MethodIDOnGroupCounterChanged], groupIDStr, keyStr, newValue);
            }

            env->DeleteLocalRef(groupIDStr);
            env->DeleteLocalRef(keyStr);
        }

    }// namespace jni
}// namespace tim