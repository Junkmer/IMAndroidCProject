//
// Created by Junker on 2022/9/27.
//

#include "conversation_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "conversation_jni.h"
#include "message_jni.h"
#include "group_at_info_jni.h"
#include "TIMConversationManager.h"
#include "TIMGroupManager.h"
#include "json.h"

namespace tim {
    namespace jni {

        jclass ConversationJni::j_cls_;
        jfieldID ConversationJni::j_field_id_array_[FieldIDMax];
        jmethodID ConversationJni::j_method_id_array_[MethodIDMax];

        bool ConversationJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMConversation");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addGroupAtInfo", "(Lcom/tencent/imsdk/v2/V2TIMGroupAtInfo;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddGroupAtInfo] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addMark", "(J)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddMark] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addConversationGroup", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDAddConversationGroup] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "conversationID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDConversationID] = jfield;

            jfield = env->GetFieldID(j_cls_, "type", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDType] = jfield;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "showName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDShowName] = jfield;

            jfield = env->GetFieldID(j_cls_, "faceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "recvOpt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDRecvOpt] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupType", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDGroupType] = jfield;

            jfield = env->GetFieldID(j_cls_, "unreadCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDUnreadCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "lastMessage", "Lcom/tencent/imsdk/v2/V2TIMMessage;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDLastMessage] = jfield;

            jfield = env->GetFieldID(j_cls_, "draftText", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDDraftText] = jfield;

            jfield = env->GetFieldID(j_cls_, "draftTimestamp", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDDraftTimestamp] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupAtInfoList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDGroupAtInfoList] = jfield;

            jfield = env->GetFieldID(j_cls_, "pinned", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDPinned] = jfield;

            jfield = env->GetFieldID(j_cls_, "orderKey", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDOrderKey] = jfield;

            jfield = env->GetFieldID(j_cls_, "markList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDMarkList] = jfield;

            jfield = env->GetFieldID(j_cls_, "customData", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDCustomData] = jfield;

            jfield = env->GetFieldID(j_cls_, "conversationGroupList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDConversationGroupList] = jfield;

            return true;
        }

        jobject ConversationJni::Convert2JObject(const json::Object &conv_obj_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject conversationObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);
            if (nullptr == conversationObj) {
                return nullptr;
            }

            jstring jStr = nullptr;

            env->SetIntField(conversationObj, j_field_id_array_[FieldIDType], conv_obj_json[kTIMConvType]);

            std::string convId = conv_obj_json[kTIMConvId].ToString();
            if (!convId.empty()) {
                std::string convTitle;
                if (conv_obj_json[kTIMConvType].ToInt() == TIMConvType::kTIMConv_C2C){
                    convTitle = "c2c_";
                } else if (conv_obj_json[kTIMConvType].ToInt() == TIMConvType::kTIMConv_Group){
                    convTitle = "group_";
                }
                jStr = StringJni::Cstring2Jstring(env, convTitle.append(convId));
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDConversationID], jStr);
                env->DeleteLocalRef(jStr);
            }

            std::string userID = conv_obj_json[kTIMConvType].ToInt() == TIMConvType::kTIMConv_C2C ? conv_obj_json[kTIMConvId] : "";
            std::string groupID = conv_obj_json[kTIMConvType].ToInt() == TIMConvType::kTIMConv_Group ? conv_obj_json[kTIMConvId] : "";

            jStr = StringJni::Cstring2Jstring(env, userID);
            if (jStr) {
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDUserID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, groupID);
            if (jStr) {
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDGroupID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, conv_obj_json[kTIMConvShowName]);
            if (jStr) {
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDShowName], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, conv_obj_json[kTIMConvFaceUrl]);
            if (jStr) {
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDFaceUrl], jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetIntField(conversationObj, j_field_id_array_[FieldIDRecvOpt], conv_obj_json[kTIMConvRecvOpt]);

            auto groupType = TIMGroupType(conv_obj_json[kTIMConvGroupType].ToInt());
            std::string groupTypeStr;
            switch (groupType) {
                case TIMGroupType::kTIMGroup_Private:
                    groupTypeStr = "Work";
                    break;
                case TIMGroupType::kTIMGroup_Public:
                    groupTypeStr = "Public";
                    break;
                case TIMGroupType::kTIMGroup_ChatRoom:
                    groupTypeStr = "Meeting";
                    break;
                case TIMGroupType::kTIMGroup_AVChatRoom:
                    groupTypeStr = "AVChatRoom";
                    break;
                case TIMGroupType::kTIMGroup_Community:
                    groupTypeStr = "Community";
                    break;
                default:
                    groupTypeStr = "invite group type";
                    break;
            }
            jStr = StringJni::Cstring2Jstring(env, groupTypeStr);
            if (jStr) {
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDGroupType], jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetIntField(conversationObj, j_field_id_array_[FieldIDUnreadCount], conv_obj_json[kTIMConvUnReadNum]);

            bool isHasLastMsgFlag = conv_obj_json[kTIMConvIsHasLastMsg];
            if (isHasLastMsgFlag && conv_obj_json.HasKey(kTIMConvLastMsg)) {
                jobject lastMessageObj = MessageJni::Convert2JObject(conv_obj_json[kTIMConvLastMsg]);
                if (lastMessageObj) {
                    env->SetObjectField(conversationObj, j_field_id_array_[FieldIDLastMessage], lastMessageObj);
                    env->DeleteLocalRef(lastMessageObj);
                }
            }

            if (conv_obj_json[kTIMConvIsHasDraft].ToBool()) {
                if (conv_obj_json.HasKey(kTIMConvDraft)){
                    json::Object draft_json = conv_obj_json[kTIMConvDraft];
                    json::Object msg_json = draft_json[kTIMDraftMsg];
                    json::Array elem_array = msg_json[kTIMMsgElemArray];
                    json::Object text_elem = elem_array[0];
                    if (text_elem.HasKey(kTIMTextElemContent)){
                        jStr = StringJni::Cstring2Jstring(env, text_elem[kTIMTextElemContent]);
                        if (jStr) {
                            env->SetObjectField(conversationObj, j_field_id_array_[FieldIDDraftText], jStr);
                            env->DeleteLocalRef(jStr);
                        }
                        env->SetLongField(conversationObj, j_field_id_array_[FieldIDDraftTimestamp], draft_json[kTIMDraftEditTime].ToInt());
                    }
                }
            }

            if (conv_obj_json.HasKey(kTIMConvGroupAtInfoArray)){
                json::Array group_at_array = conv_obj_json[kTIMConvGroupAtInfoArray];

                for (int i = 0; i < group_at_array.size(); ++i) {
                    jobject atInfoObj = GroupAtInfoJni::Convert2JObject(group_at_array[0]);
                    if (atInfoObj) {
                        env->CallVoidMethod(conversationObj, j_method_id_array_[MethodIDAddGroupAtInfo], atInfoObj);
                        env->DeleteLocalRef(atInfoObj);
                    }
                }
            }

            env->SetBooleanField(conversationObj, j_field_id_array_[FieldIDPinned], conv_obj_json[kTIMConvIsPinned].ToBool());
            env->SetLongField(conversationObj, j_field_id_array_[FieldIDOrderKey], conv_obj_json[kTIMConvActiveTime].ToInt64());

            if (conv_obj_json.HasKey(kTIMConvMarkArray)){
                json::Array mark_array = conv_obj_json[kTIMConvMarkArray];
                for (int i = 0; i < mark_array.size(); ++i) {
                    env->CallVoidMethod(conversationObj, j_method_id_array_[MethodIDAddMark], mark_array[0].ToInt64());
                }
            }

            jStr = StringJni::Cstring2Jstring(env, conv_obj_json[kTIMConvCustomData]);
            if (jStr) {
                env->SetObjectField(conversationObj, j_field_id_array_[FieldIDCustomData], jStr);
                env->DeleteLocalRef(jStr);
            }

            if (conv_obj_json.HasKey(kTIMConvConversationGroupArray)){
                json::Array conv_group_array = conv_obj_json[kTIMConvConversationGroupArray];
                for (int i = 0; i < conv_group_array.size(); ++i) {
                    jstring conversationGroupStr = StringJni::Cstring2Jstring(env, conv_group_array[0]);
                    env->CallVoidMethod(conversationObj, j_method_id_array_[MethodIDAddMark], conversationGroupStr);
                    env->DeleteLocalRef(conversationGroupStr);
                }
            }

            return conversationObj;
        }
    }// namespace tim
}// namespace jni