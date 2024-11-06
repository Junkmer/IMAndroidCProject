//
// Created by EDY on 2022/9/20.
//

#include <LogUtil.h>
#include "message_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "elem_processor_jni.h"
#include "offline_push_info_jni.h"
#include "TIMMessageManager.h"
#include "TIMManager.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {

        jclass MessageJni::j_cls_;
        jfieldID MessageJni::j_filed_id_array[FieldIDMax];
        jmethodID MessageJni::j_method_id_array[MethodIDMax];

        bool MessageJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMessage");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array[MethodIDConstructor] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addElem", "(Lcom/tencent/imsdk/v2/V2TIMElem;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array[MethodIDAddMessageElem] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "msgID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDMsgID] = jfield;

            jfield = env->GetFieldID(j_cls_, "sender", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDSender] = jfield;

            jfield = env->GetFieldID(j_cls_, "nickName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDNickName] = jfield;

            jfield = env->GetFieldID(j_cls_, "friendRemark", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDFriendRemark] = jfield;

            jfield = env->GetFieldID(j_cls_, "faceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "nameCard", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDNameCard] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "status", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDStatus] = jfield;

            jfield = env->GetFieldID(j_cls_, "serverTime", "J" );
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDServerTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "clientTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDClientTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "senderTinyId", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDSenderTinyId] = jfield;

            jfield = env->GetFieldID(j_cls_, "receiverTinyId", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDReceiverTinyId] = jfield;

            jfield = env->GetFieldID(j_cls_, "elemList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDElemList] = jfield;

            jfield = env->GetFieldID(j_cls_, "localCustomInt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDLocalCustomInt] = jfield;

            jfield = env->GetFieldID(j_cls_, "localCustomData", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDLocalCustomData] = jfield;

            jfield = env->GetFieldID(j_cls_, "cloudCustomData", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDCloudCustomData] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSelf", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsSelf] = jfield;

            jfield = env->GetFieldID(j_cls_, "isRead", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsRead] = jfield;

            jfield = env->GetFieldID(j_cls_, "isPeerRead", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsPeerRead] = jfield;

            jfield = env->GetFieldID(j_cls_, "needReadReceipt", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDNeedReadReceipt] = jfield;

            jfield = env->GetFieldID(j_cls_, "isBroadcastMessage", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsBroadcastMessage] = jfield;

            jfield = env->GetFieldID(j_cls_, "priority", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDPriority] = jfield;

            jfield = env->GetFieldID(j_cls_, "offlinePushInfo", "Lcom/tencent/imsdk/v2/V2TIMOfflinePushInfo;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDOfflinePushInfo] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupAtUserList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDGroupAtUserList] = jfield;

            jfield = env->GetFieldID(j_cls_, "seq", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDSeq] = jfield;

            jfield = env->GetFieldID(j_cls_, "random", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDRandom] = jfield;

            jfield = env->GetFieldID(j_cls_, "isOnlineMsg", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsOnlineMsg] = jfield;

            jfield = env->GetFieldID(j_cls_, "isExcludedFromUnreadCount", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsExcludedFromUnreadCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "isExcludedFromLastMessage", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsExcludedFromLastMessage] = jfield;

            jfield = env->GetFieldID(j_cls_, "isSupportMessageExtension", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsSupportMessageExtension] = jfield;

            jfield = env->GetFieldID(j_cls_, "isForwardMessage", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDIsForwardMessage] = jfield;

            jfield = env->GetFieldID(j_cls_, "targetGroupMemberList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_filed_id_array[FieldIDTargetGroupMemberList] = jfield;

            return true;
        }

        jobject MessageJni::Convert2JObject(const json::Object &message_json_obj) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject j_obj_message = env->NewObject(j_cls_, j_method_id_array[MethodIDConstructor]);
            if (nullptr == j_obj_message) {
                return nullptr;
            }

            env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDMsgID],
                                StringJni::Cstring2Jstring(env, message_json_obj[kTIMMsgMsgId].ToString()));
            env->SetLongField(j_obj_message, j_filed_id_array[FieldIDServerTime], message_json_obj[kTIMMsgServerTime].ToInt64());
            env->SetLongField(j_obj_message, j_filed_id_array[FieldIDClientTime], message_json_obj[kTIMMsgClientTime].ToInt64());
            env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDSender],
                                StringJni::Cstring2Jstring(env, message_json_obj[kTIMMsgSender].ToString()));
            env->SetLongField(j_obj_message, j_filed_id_array[FieldIDSenderTinyId],message_json_obj[kTIMMsgSenderTinyId].ToInt64());

            if (message_json_obj.HasKey(kTIMMsgSenderProfile)) {
                json::Object sender_profile = message_json_obj[kTIMMsgSenderProfile];
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDNickName],
                                    StringJni::Cstring2Jstring(env, sender_profile[kTIMUserProfileNickName].ToString()));
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDFriendRemark],
                                    StringJni::Cstring2Jstring(env, sender_profile[kTIMUserProfileFriendRemark].ToString()));
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDFaceUrl],
                                    StringJni::Cstring2Jstring(env, sender_profile[kTIMUserProfileFaceUrl]));
            }

            if (message_json_obj.HasKey(kTIMMsgSenderGroupMemberInfo)) {
                json::Object sender_member_info = message_json_obj[kTIMMsgSenderGroupMemberInfo];
                if (sender_member_info.size() > 0) {
                    env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDNameCard],
                                        StringJni::Cstring2Jstring(env, sender_member_info[kTIMGroupMemberInfoNameCard].ToString()));
                }
            }

            int convType = message_json_obj[kTIMMsgConvType].ToInt();
            if (convType == TIMConvType::kTIMConv_C2C) {
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDGroupID], StringJni::Cstring2Jstring(env, ""));
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDUserID], StringJni::Cstring2Jstring(env, message_json_obj[kTIMMsgConvId]));
                env->SetLongField(j_obj_message, j_filed_id_array[FieldIDReceiverTinyId],message_json_obj[kTIMMsgReceiverTinyId].ToInt64());
            } else if (convType == TIMConvType::kTIMConv_Group) {
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDGroupID],
                                    StringJni::Cstring2Jstring(env, message_json_obj[kTIMMsgConvId]));
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDUserID], StringJni::Cstring2Jstring(env, ""));
                env->SetLongField(j_obj_message, j_filed_id_array[FieldIDReceiverTinyId],0);
            }

            env->SetIntField(j_obj_message, j_filed_id_array[FieldIDStatus], message_json_obj[kTIMMsgStatus]);

            env->SetIntField(j_obj_message, j_filed_id_array[FieldIDLocalCustomInt], message_json_obj[kTIMMsgCustomInt]);
            env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDLocalCustomData],
                                StringJni::Cstring2Jstring(env, message_json_obj[kTIMMsgCustomStr].ToString()));
            env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDCloudCustomData],
                                StringJni::Cstring2Jstring(env, message_json_obj[kTIMMsgCloudCustomStr].ToString()));
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsSelf], message_json_obj[kTIMMsgIsFormSelf].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsRead], message_json_obj[kTIMMsgIsRead].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsPeerRead], message_json_obj[kTIMMsgIsPeerRead].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDNeedReadReceipt], message_json_obj[kTIMMsgNeedReadReceipt].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsBroadcastMessage], message_json_obj[kTIMMsgIsBroadcastMessage].ToBool());
            env->SetIntField(j_obj_message, j_filed_id_array[FieldIDPriority], message_json_obj[kTIMMsgPriority]);

            if (message_json_obj.HasKey(kTIMMsgOfflinePushConfig)) {
                jobject offlinePushObj = OfflinePushInfoJni::Convert2JObject(message_json_obj[kTIMMsgOfflinePushConfig]);
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDOfflinePushInfo], offlinePushObj);
                env->DeleteLocalRef(offlinePushObj);
            }
            if (message_json_obj.HasKey(kTIMMsgElemArray)) {
                json::Array elem_array = message_json_obj[kTIMMsgElemArray];
                if (elem_array.size() > 0) {
                    for (int i = 0; i < elem_array.size(); ++i) {
                        auto *jElemObj = ElemProcessor::GetInstance().ParseElem(elem_array[i]);
                        if (jElemObj) {
                            env->CallVoidMethod(j_obj_message, j_method_id_array[MethodIDAddMessageElem], jElemObj);
                            env->DeleteLocalRef(jElemObj);
                        }
                    }
                }
            }

            if (message_json_obj.HasKey(kTIMMsgGroupAtUserArray)) {
                jobject arrayList = ArrayListJni::NewArrayList();
                json::Array groupAtList_json = message_json_obj[kTIMMsgGroupAtUserArray];
                for (const auto &i: groupAtList_json) {
                    jstring userIDObj = StringJni::Cstring2Jstring(env, i.ToString());
                    ArrayListJni::Add(arrayList, userIDObj);

                    env->DeleteLocalRef(userIDObj);
                }
                env->SetObjectField(j_obj_message, j_filed_id_array[FieldIDGroupAtUserList], arrayList);
                env->DeleteLocalRef(arrayList);
            }

            env->SetLongField(j_obj_message, j_filed_id_array[FieldIDSeq], message_json_obj[kTIMMsgSeq].ToInt64());
            env->SetLongField(j_obj_message, j_filed_id_array[FieldIDRandom], message_json_obj[kTIMMsgRand].ToInt64());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsOnlineMsg], message_json_obj[kTIMMsgIsOnlineMsg].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsExcludedFromUnreadCount], message_json_obj[kTIMMsgIsExcludedFromUnreadCount].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsExcludedFromLastMessage], message_json_obj[kTIMMsgExcludedFromLastMessage].ToBool());
            env->SetBooleanField(j_obj_message, j_filed_id_array[FieldIDIsSupportMessageExtension], message_json_obj[kTIMMsgSupportMessageExtension].ToBool());
            return j_obj_message;
        }

        std::unique_ptr<json::Object> MessageJni::Convert2CoreObject(const jobject &messageObj) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            json::Object message;

            auto msgIDStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDMsgID]);
            if (nullptr != msgIDStr) {
                message[kTIMMsgMsgId] = StringJni::Jstring2Cstring(env, msgIDStr).c_str();
                env->DeleteLocalRef(msgIDStr);
            }

            message[kTIMMsgServerTime] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDServerTime]);
            message[kTIMMsgClientTime] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDClientTime]);

            auto senderStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDSender]);
            if (nullptr != senderStr) {
                message[kTIMMsgSender] = StringJni::Jstring2Cstring(env, senderStr).c_str();
                env->DeleteLocalRef(senderStr);
            }

            message[kTIMMsgSenderTinyId] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDSenderTinyId]);
            message[kTIMMsgReceiverTinyId] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDReceiverTinyId]);

//------------------ 发送者用户资料 - start ------------------------//
            json::Object senderUserInfo_json;

            auto nickNameStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDNickName]);
            if (nullptr != nickNameStr) {
                senderUserInfo_json[kTIMUserProfileNickName] = StringJni::Jstring2Cstring(env, nickNameStr).c_str();
                env->DeleteLocalRef(nickNameStr);
            }

            auto friendRemarkStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDFriendRemark]);
            if (nullptr != friendRemarkStr) {
                senderUserInfo_json[kTIMUserProfileFriendRemark] = StringJni::Jstring2Cstring(env, friendRemarkStr).c_str();
                env->DeleteLocalRef(friendRemarkStr);
            }

            auto faceURLStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDFaceUrl]);
            if (nullptr != faceURLStr) {
                senderUserInfo_json[kTIMUserProfileFaceUrl] = StringJni::Jstring2Cstring(env, faceURLStr).c_str();
                env->DeleteLocalRef(faceURLStr);
            }

            message[kTIMMsgSenderProfile] = senderUserInfo_json;
//------------------ end ------------------------//
//------------------ 发送者群成员 - start ------------------------//

            json::Object memberInfo_json;

            auto nameCardStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDNameCard]);
            if (nullptr != nameCardStr) {
                memberInfo_json[kTIMGroupMemberInfoNameCard] = StringJni::Jstring2Cstring(env, nameCardStr).c_str();
                env->DeleteLocalRef(nameCardStr);
            }

            message[kTIMMsgSenderGroupMemberInfo] = memberInfo_json;
//------------------ end ------------------------//

            auto groupIDStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDGroupID]);
            if (nullptr != groupIDStr) {
                std::string groupID = StringJni::Jstring2Cstring(env, groupIDStr);
                if (!groupID.empty()) {
                    message[kTIMMsgConvId] = groupID;
                    message[kTIMMsgConvType] = TIMConvType::kTIMConv_Group;
                }
                env->DeleteLocalRef(groupIDStr);
            }

            auto userIDStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDUserID]);
            if (nullptr != userIDStr) {
                std::string userID = StringJni::Jstring2Cstring(env, userIDStr);
                if (!userID.empty()) {
                    message[kTIMMsgConvId] = userID;
                    message[kTIMMsgConvType] = TIMConvType::kTIMConv_C2C;
                }
                env->DeleteLocalRef(userIDStr);
            }

            message[kTIMMsgStatus] = TIMMsgStatus(env->GetIntField(messageObj, j_filed_id_array[FieldIDStatus]));

            message[kTIMMsgCustomInt] = env->GetIntField(messageObj, j_filed_id_array[FieldIDLocalCustomInt]);
            auto localCustomDataStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDLocalCustomData]);
            if (nullptr != localCustomDataStr) {
                message[kTIMMsgCustomStr] = StringJni::Jstring2Cstring(env, localCustomDataStr);
                env->DeleteLocalRef(localCustomDataStr);
            }

            auto cloudCustomDataStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDCloudCustomData]);
            if (nullptr != cloudCustomDataStr) {
                message[kTIMMsgCloudCustomStr] = StringJni::Jstring2Cstring(env, cloudCustomDataStr);
                env->DeleteLocalRef(cloudCustomDataStr);
            }

            message[kTIMMsgIsFormSelf] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsSelf]);
            message[kTIMMsgIsRead] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsRead]);
            message[kTIMMsgIsPeerRead] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsPeerRead]);
            message[kTIMMsgNeedReadReceipt] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDNeedReadReceipt]);
            message[kTIMMsgIsBroadcastMessage] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsBroadcastMessage]);
            message[kTIMMsgPriority] = TIMMsgPriority(env->GetIntField(messageObj, j_filed_id_array[FieldIDPriority]));

            jobject offlinePushObj = env->GetObjectField(messageObj, j_filed_id_array[FieldIDOfflinePushInfo]);
            if (nullptr != offlinePushObj) {
                json::Object offline_push_config;
                bool flag = OfflinePushInfoJni::Convert2CoreObject(offlinePushObj, offline_push_config);
                if (flag) {
                    message[kTIMMsgOfflinePushConfig] = offline_push_config;
                }
                env->DeleteLocalRef(offlinePushObj);
            }

            auto *elemListObj = env->GetObjectField(messageObj, j_filed_id_array[FieldIDElemList]);
            int size;
            size = ArrayListJni::Size(elemListObj);
            if (size > 0) {
                json::Array elem_array;
                for (int i = 0; i < size; ++i) {
                    auto *jElemObj = ArrayListJni::Get(elemListObj, i);
                    if (nullptr == jElemObj) {
                        continue;
                    }
                    jclass cls = env->GetObjectClass(jElemObj);
                    jmethodID get_type = env->GetMethodID(cls, "getElementType", "()I");
                    if (!get_type) continue;

                    auto type = env->CallIntMethod(jElemObj, get_type);
                    std::unique_ptr<json::Object> elem = ElemProcessor::GetInstance().BuildElem(JElemType2CElemType(type), jElemObj);
                    if (elem) {
                        elem_array.push_back(*elem);
                    }
                }
                message[kTIMMsgElemArray] = elem_array;
            }

            jobject j_obj_atUserList = env->GetObjectField(messageObj, j_filed_id_array[FieldIDGroupAtUserList]);
            size = ArrayListJni::Size(j_obj_atUserList);
            if (size > 0) {
                json::Array at_user_array;
                for (int i = 0; i < size; ++i) {
                    auto atUserObj = (jstring) ArrayListJni::Get(j_obj_atUserList, i);
                    at_user_array.push_back(StringJni::Jstring2Cstring(env, atUserObj).c_str());
                }
                if (at_user_array.size() > 0) {
                    message[kTIMMsgGroupAtUserArray] = at_user_array;
                }
            }

            message[kTIMMsgSeq] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDSeq]);
            message[kTIMMsgRand] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDRandom]);
            message[kTIMMsgIsExcludedFromUnreadCount] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsExcludedFromUnreadCount]);
            message[kTIMMsgExcludedFromLastMessage] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsExcludedFromLastMessage]);
            message[kTIMMsgSupportMessageExtension] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsSupportMessageExtension]);

            //转发消息时需要设置
            message[kTIMMsgIsForwardMessage] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsForwardMessage]);

            //发送群定向消息时设置
            jobject j_obj_TargetGroupMemberList = env->GetObjectField(messageObj, j_filed_id_array[FieldIDTargetGroupMemberList]);
            if (j_obj_TargetGroupMemberList) {
                size = ArrayListJni::Size(j_obj_TargetGroupMemberList);
                if (size > 0) {
                    json::Array member_array;
                    for (int i = 0; i < size; ++i) {
                        auto memberId = (jstring) ArrayListJni::Get(j_obj_TargetGroupMemberList, i);
                        member_array.push_back(StringJni::Jstring2Cstring(env, memberId).c_str());
                    }
                    if (member_array.size() > 0) {
                        message[kTIMMsgTargetGroupMemberArray] = member_array;
                    }
                }
            }

            return std::make_unique<json::Object>(message);
        }


        std::unique_ptr<json::Object>
        MessageJni::SendMessageConvert2CoreObject(jobject &messageObj, const jint priority, const jboolean onlineUserOnly, const jobject &offlinePushInfo) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            //点击消息发送时，对原始消息体做最初始赋值
            env->SetIntField(messageObj, j_filed_id_array[FieldIDStatus], TIMMsgStatus::kTIMMsg_Sending);
            env->SetBooleanField(messageObj, j_filed_id_array[FieldIDIsSelf], true);
            env->SetBooleanField(messageObj, j_filed_id_array[FieldIDIsRead], true);

            json::Object message;

            message[kTIMMsgPriority] = priority;
            message[kTIMMsgIsOnlineMsg] = (bool) onlineUserOnly;
            if (offlinePushInfo) {
                json::Object offline_push_info_json;
                tim::jni::OfflinePushInfoJni::Convert2CoreObject(offlinePushInfo, offline_push_info_json);
                message[kTIMMsgOfflinePushConfig] = offlinePushInfo;
            }

            auto msgIDStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDMsgID]);
            if (nullptr != msgIDStr) {
                message[kTIMMsgMsgId] = StringJni::Jstring2Cstring(env, msgIDStr).c_str();
                env->DeleteLocalRef(msgIDStr);
            }

            message[kTIMMsgServerTime] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDServerTime]);
            message[kTIMMsgClientTime] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDClientTime]);

            auto senderStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDSender]);
            if (nullptr != senderStr) {
                message[kTIMMsgSender] = StringJni::Jstring2Cstring(env, senderStr).c_str();
                env->DeleteLocalRef(senderStr);
            }

            message[kTIMMsgSenderTinyId] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDSenderTinyId]);
            message[kTIMMsgReceiverTinyId] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDReceiverTinyId]);

//------------------ 发送者用户资料 - start ------------------------//
            json::Object senderUserInfo_json;

            auto nickNameStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDNickName]);
            if (nullptr != nickNameStr) {
                senderUserInfo_json[kTIMUserProfileNickName] = StringJni::Jstring2Cstring(env, nickNameStr).c_str();
                env->DeleteLocalRef(nickNameStr);
            }

            auto friendRemarkStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDFriendRemark]);
            if (nullptr != friendRemarkStr) {
                senderUserInfo_json[kTIMUserProfileFriendRemark] = StringJni::Jstring2Cstring(env, friendRemarkStr).c_str();
                env->DeleteLocalRef(friendRemarkStr);
            }

            auto faceURLStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDFaceUrl]);
            if (nullptr != faceURLStr) {
                senderUserInfo_json[kTIMUserProfileFaceUrl] = StringJni::Jstring2Cstring(env, faceURLStr).c_str();
                env->DeleteLocalRef(faceURLStr);
            }

            message[kTIMMsgSenderProfile] = senderUserInfo_json;
//------------------ end ------------------------//
//------------------ 发送者群成员 - start ------------------------//

            json::Object memberInfo_json;

            auto nameCardStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDNameCard]);
            if (nullptr != nameCardStr) {
                memberInfo_json[kTIMGroupMemberInfoNameCard] = StringJni::Jstring2Cstring(env, nameCardStr).c_str();
                env->DeleteLocalRef(nameCardStr);
            }

            message[kTIMMsgSenderGroupMemberInfo] = memberInfo_json;
//------------------ end ------------------------//

            auto groupIDStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDGroupID]);
            if (nullptr != groupIDStr) {
                std::string groupID = StringJni::Jstring2Cstring(env, groupIDStr);
                if (!groupID.empty()) {
                    message[kTIMMsgConvId] = groupID;
                    message[kTIMMsgConvType] = TIMConvType::kTIMConv_Group;
                }
                env->DeleteLocalRef(groupIDStr);
            }

            auto userIDStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDUserID]);
            if (nullptr != userIDStr) {
                std::string userID = StringJni::Jstring2Cstring(env, userIDStr);
                if (!userID.empty()) {
                    message[kTIMMsgConvId] = userID;
                    message[kTIMMsgConvType] = TIMConvType::kTIMConv_C2C;
                }
                env->DeleteLocalRef(userIDStr);
            }

            message[kTIMMsgStatus] = TIMMsgStatus(env->GetIntField(messageObj, j_filed_id_array[FieldIDStatus]));

            message[kTIMMsgCustomInt] = env->GetIntField(messageObj, j_filed_id_array[FieldIDLocalCustomInt]);
            auto localCustomDataStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDLocalCustomData]);
            if (nullptr != localCustomDataStr) {
                message[kTIMMsgCustomStr] = StringJni::Jstring2Cstring(env, localCustomDataStr);
                env->DeleteLocalRef(localCustomDataStr);
            }

            auto cloudCustomDataStr = (jstring) env->GetObjectField(messageObj, j_filed_id_array[FieldIDCloudCustomData]);
            if (nullptr != cloudCustomDataStr) {
                message[kTIMMsgCloudCustomStr] = StringJni::Jstring2Cstring(env, cloudCustomDataStr);
                env->DeleteLocalRef(cloudCustomDataStr);
            }

            message[kTIMMsgIsFormSelf] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsSelf]);
            message[kTIMMsgIsRead] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsRead]);
            message[kTIMMsgIsPeerRead] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsPeerRead]);
            message[kTIMMsgNeedReadReceipt] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDNeedReadReceipt]);
            message[kTIMMsgIsBroadcastMessage] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsBroadcastMessage]);
            message[kTIMMsgPriority] = TIMMsgPriority(env->GetIntField(messageObj, j_filed_id_array[FieldIDPriority]));

            auto *elemListObj = env->GetObjectField(messageObj, j_filed_id_array[FieldIDElemList]);
            int size;
            size = ArrayListJni::Size(elemListObj);
            if (size > 0) {
                json::Array elem_array;
                for (int i = 0; i < size; ++i) {
                    auto *jElemObj = ArrayListJni::Get(elemListObj, i);
                    if (nullptr == jElemObj) {
                        continue;
                    }
                    jclass cls = env->GetObjectClass(jElemObj);
                    jmethodID get_type = env->GetMethodID(cls, "getElementType", "()I");
                    if (!get_type) continue;

                    auto type = env->CallIntMethod(jElemObj, get_type);
                    std::unique_ptr<json::Object> elem = ElemProcessor::GetInstance().BuildElem(JElemType2CElemType(type), jElemObj);
                    if (elem) {
                        elem_array.push_back(*elem);
                    }
                }
                message[kTIMMsgElemArray] = elem_array;
            }

            jobject j_obj_atUserList = env->GetObjectField(messageObj, j_filed_id_array[FieldIDGroupAtUserList]);
            size = ArrayListJni::Size(j_obj_atUserList);
            if (size > 0) {
                json::Array at_user_array;
                for (int i = 0; i < size; ++i) {
                    auto atUserObj = (jstring) ArrayListJni::Get(j_obj_atUserList, i);
                    at_user_array.push_back(StringJni::Jstring2Cstring(env, atUserObj).c_str());
                }
                if (at_user_array.size() > 0) {
                    message[kTIMMsgGroupAtUserArray] = at_user_array;
                }
            }

            message[kTIMMsgSeq] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDSeq]);
            message[kTIMMsgRand] = (long long) env->GetLongField(messageObj, j_filed_id_array[FieldIDRandom]);
            message[kTIMMsgIsExcludedFromUnreadCount] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsExcludedFromUnreadCount]);
            message[kTIMMsgExcludedFromLastMessage] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsExcludedFromLastMessage]);
            message[kTIMMsgSupportMessageExtension] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsSupportMessageExtension]);

            //转发消息时需要设置
            message[kTIMMsgIsForwardMessage] = (bool) env->GetBooleanField(messageObj, j_filed_id_array[FieldIDIsForwardMessage]);

            //发送群定向消息时设置
            jobject j_obj_TargetGroupMemberList = env->GetObjectField(messageObj, j_filed_id_array[FieldIDTargetGroupMemberList]);
            if (j_obj_TargetGroupMemberList) {
                size = ArrayListJni::Size(j_obj_TargetGroupMemberList);
                if (size > 0) {
                    json::Array member_array;
                    for (int i = 0; i < size; ++i) {
                        auto memberId = (jstring) ArrayListJni::Get(j_obj_TargetGroupMemberList, i);
                        member_array.push_back(StringJni::Jstring2Cstring(env, memberId).c_str());
                    }
                    if (member_array.size() > 0) {
                        message[kTIMMsgTargetGroupMemberArray] = member_array;
                    }
                }
            }

            return std::make_unique<json::Object>(message);
        }

        TIMElemType MessageJni::JElemType2CElemType(int jElemType) {
            //im native sdk 中的消息类型 转换 im c sdk中的消息类型
            switch (jElemType) {
                case JavaElemType::V2TIM_ELEM_TYPE_TEXT:// 文本消息
                    return kTIMElem_Text;
                case JavaElemType::V2TIM_ELEM_TYPE_IMAGE:// 图片消息
                    return kTIMElem_Image;
                case JavaElemType::V2TIM_ELEM_TYPE_SOUND:// 声音消息
                    return kTIMElem_Sound;
                case JavaElemType::V2TIM_ELEM_TYPE_CUSTOM:// 自定义消息
                    return kTIMElem_Custom;
                case JavaElemType::V2TIM_ELEM_TYPE_FILE:// 文件消息
                    return kTIMElem_File;
                case JavaElemType::V2TIM_ELEM_TYPE_GROUP_TIPS:// 群组tip消息
                    return kTIMElem_GroupTips;
                case JavaElemType::V2TIM_ELEM_TYPE_FACE:// 表情消息
                    return kTIMElem_Face;
                case JavaElemType::V2TIM_ELEM_TYPE_LOCATION:// 位置消息
                    return kTIMElem_Location;
                case JavaElemType::V2TIM_ELEM_TYPE_VIDEO:// 视频消息
                    return kTIMElem_Video;
                case JavaElemType::V2TIM_ELEM_TYPE_MERGER:// 合并消息消息
                    return kTIMElem_Merge;
                default:
                    return kTIMElem_Invalid;
            }
        }

    }
}

#include "message_jni.h"
